/* 
 * Copyright (C) 2006, iRex Technologies B.V.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
 
#include "PDFCore.h"
#include "PDFViewerLog.h"
#include "controller.h"
#include <math.h>

CPDFCore::CPDFCore(SplashColorMode colorModeA, int bitmapRowPadA,
	         GBool reverseVideoA, SplashColorPtr paperColorA)
{
    outputDev = NULL;//new SplashOutputDev(colorModeA, bitmapRowPadA, reverseVideoA, paperColorA);
    textOut = NULL;//new TextOutputDev(NULL, gTrue, gFalse, gFalse);
    pdfdoc = NULL;    
    ctrl   = NULL;
}

CPDFCore::~CPDFCore(void)
{
    if (outputDev) { delete outputDev; outputDev = NULL; }
    if (pdfdoc) { delete pdfdoc; pdfdoc = NULL; }
    ctrl = NULL;
}

// we can reuse this core for multiple documents
int CPDFCore::open(GooString *fileNameA, GooString *ownerPassword,
         GooString *userPassword , void *guiDataA )
{
    int ret = errNone;
    // already open 
    if (pdfdoc)
    {
        PV_LOGPRINTF("Delete old doc %p\n", pdfdoc);
        delete pdfdoc;
    }

    // construct a new one
    pdfdoc = new PDFDoc(fileNameA, ownerPassword, userPassword, guiDataA);
    
    // check result
    if (!pdfdoc->isOk())
    {
        ret = pdfdoc->getErrorCode();
        delete pdfdoc;
        PV_ERRORPRINTF("Open PDF document %s failed with error code %d", 
            fileNameA->getCString(), ret);                
        return errOpenFile; 
    }
    
    // done
    if (NULL == outputDev)
    {
        SplashColor     white;
        white[0] = 255;
        white[1] = 255;
        white[2] = 255;
        outputDev = new SplashOutputDev(splashModeMono8, 4, gFalse, white);
    }
    outputDev->startDoc(pdfdoc->getXRef());
    return ret;
}

int CPDFCore::open(BaseStream *stream, GooString *ownerPassword ,
         GooString *userPassword , void *guiDataA)
{
    return errNone;
}

// only close the document
void CPDFCore::close()
{
    if (outputDev) {  delete outputDev; outputDev = NULL; }
    if (pdfdoc) { delete pdfdoc; pdfdoc = NULL; }
}

void CPDFCore::getPageBasicInfo(CPageInfo & info)
{
    info.pageDx = pdfdoc->getPageCropWidth(info.pageNumber);
    info.pageDy = pdfdoc->getPageCropHeight(info.pageNumber);
    info.pageRotate = pdfdoc->getPageRotate(info.pageNumber);
}

double CPDFCore::getPageWidth(const int pageNumber)
{
    return pdfdoc->getPageCropWidth(pageNumber);
}

double CPDFCore::getPageHeight(const int pageNumber)
{
    return pdfdoc->getPageCropHeight(pageNumber);
}

int CPDFCore::getPageRotate(const int pageNumber)
{
    return pdfdoc->getPageRotate(pageNumber);
}

int CPDFCore::getNumPages()
{
    if (NULL == pdfdoc) return -1;
    return pdfdoc->getNumPages();
}

RenderRet CPDFCore::renderPage(CPageInfo & info, GBool useMediaBox, 
                     GBool crop, GBool doLinks, GBool (*abortCheckCbk)(void *data) ,
                     void *abortCheckCbkData )
{
    if (NULL == pdfdoc)
    {
        PV_ERRORPRINTF("PDF document has not been opened!");    
        return Render_Error;
    }

    if (0 >= info.pageNumber || info.pageNumber > pdfdoc->getNumPages())
    {
        PV_ERRORPRINTF("PageNumber %d is invalid![1, %d]", 
            info.pageNumber,  pdfdoc->getNumPages());
        return Render_Error;
    }

    // get page basic information
    getPageBasicInfo(info);

    // calcuate layout
    calculateLayout(info);

    // render it now
#if (PV_PROFILE_ON)
    int t1 = ctrl->getHighTimeStamp();
#endif 

    RenderRet ret = Render_Error;
    
    ret = pdfdoc->displayPage(outputDev, info.pageNumber, 
        info.hDPI, info.vDPI, info.rotate, useMediaBox, crop, doLinks,
        abortCheckCbk, abortCheckCbkData);
    
    // check result
    if (ret == Render_Error || ret == Render_Invalid)
    {
        PV_ERRORPRINTF("Render page error!");
        return ret;
    }

    // bitmap has been allocated
    info.bitmap = outputDev->takeBitmap();
    if (ret == Render_Done)
    {
#if (PV_PROFILE_ON)
        int t2 = ctrl->getHighTimeStamp();
        PV_DUMP("PDFDoc::displayPage %d uses %d\n", info.pageNumber, t2 - t1);
#endif
	    
        // copy ctm and ictm
        memcpy(info.ctm, outputDev->getDefCTM(), 6 * sizeof(double));
        memcpy(info.ictm, outputDev->getDefICTM(), 6 * sizeof(double));        
                
        // take links
        info.links  = pdfdoc->takeLinks();
        
        // take text
        if (textOut)
        {
            pdfdoc->displayPage(textOut, info.pageNumber, info.hDPI, info.vDPI, info.rotate,
                gFalse, gTrue, gFalse);
            info.text = textOut->takeText();                
        }
        return Render_Done;
    }
    else if (ret == Render_Abort)
    {
        delete info.bitmap; 
        info.bitmap = NULL;
        info.links  = NULL;
    }
    return Render_Abort;
}

// calculate the layout according to zoom and rotation
void CPDFCore::calculateLayout(CPageInfo & info)
{
    PV_LOGPRINTF("info.pageZoom %lg\n", info.pageZoom);
    info.hDPI = info.pageZoom * 0.01 * DefDPI;
    info.vDPI = info.pageZoom * 0.01 * DefDPI;
}

// http://www.paperonweb.com/size.htm
// use lookup table
struct PageSpec
{
    int     hISO; 
    int     wISO;
    double  zoomMax;
};

static const PageSpec PageSpecTable [] =
{
    {3371, 2384, A0ZoomMax},
    {2384, 1686, A1ZoomMax},
    {1686, 1192, A2ZoomMax},
    {1192, 843,  A3ZoomMax},
    {843,  596,  A4ZoomMax},
    {612,  792,  A4ZoomMax}, 
    {596,  422,  A5ZoomMax},
    {422,  298,  A6ZoomMax},
    {298,  211,  A7ZoomMax},
    {211,  149,  A8ZoomMax},
    {149,  106,  A9ZoomMax},
};
static const int PageSpecSize = sizeof(PageSpecTable)/ sizeof(PageSpecTable[0]);

int CPDFCore::getPageType(const int pageNumber)
{
    if (NULL == pdfdoc) 
    {
        PV_ERRORPRINTF("Try to access NULL pdfdoc pointer!");
        return -1;
    }
    if (pageNumber < 1 || pageNumber > pdfdoc->getNumPages()) 
    {
        PV_ERRORPRINTF("Invalid page number %d!", pageNumber);
        return -1;
    }
    int pageMx = (int)pdfdoc->getPageMediaWidth(pageNumber);
    int pageMy = (int)pdfdoc->getPageMediaHeight(pageNumber);
    for(int i = 0 ; i < PageSpecSize; ++i)
    {
        if ((pageMx >= PageSpecTable[i].hISO && 
             pageMy >= PageSpecTable[i].wISO) ||
             (pageMy >= PageSpecTable[i].hISO && 
             pageMx >= PageSpecTable[i].wISO))
        {
            return i;
        }
    }
    return -1;
}

double CPDFCore::getZoomMax(const int pageNumber)
{
    /*
    if (NULL == pdfdoc) 
    {
        PV_ERRORPRINTF("Try to access NULL pdfdoc pointer!");
        return -1;
    }
    if (pageNumber < 1 || pageNumber > pdfdoc->getNumPages()) 
    {
        PV_ERRORPRINTF("Invalid page number %d!", pageNumber);
        return -1;
    }
    */
    int pageMx = (int)pdfdoc->getPageMediaWidth(pageNumber);
    int pageMy = (int)pdfdoc->getPageMediaHeight(pageNumber);
    for(int i = 0 ; i < PageSpecSize; ++i)
    {
        if ((pageMx >= PageSpecTable[i].hISO && 
             pageMy >= PageSpecTable[i].wISO) ||
             (pageMy >= PageSpecTable[i].hISO && 
             pageMx >= PageSpecTable[i].wISO))
        {
            // PV_LOGPRINTF("Page type %d\n", i);
            return PageSpecTable[i].zoomMax;
        }
    }
    return ZoomMax;
}

//////////////////////////////////////////////////////////////////////////
// verifyZoomMax compares zoom value with max value the page can be displayed
//////////////////////////////////////////////////////////////////////////
double CPDFCore::verifyZoomMax(double zoom,const int pageNumber)
{
    double tmp = getZoomMax(pageNumber);
    if (zoom > tmp)
    {
        return tmp;
    }
    return zoom;
}
