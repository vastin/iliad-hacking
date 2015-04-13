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
 
#ifndef PDFCORE_H_
#define PDFCORE_H_

#include "poppler/GlobalParams.h"
#include "poppler/PDFDoc.h"
#include "poppler/SplashOutputDev.h"
#include "poppler/ErrorCodes.h"
#include "poppler/Gfx.h"
#include "splash/SplashBitmap.h"
#include "splash/SplashTypes.h"
#include "goo/GooString.h"
#include "PageInfo.h"
#include "poppler/SecurityHandler.h"

struct Controller;
//////////////////////////////////////////////////////////////////////////
// PDFCore :
// 1. open document
// 2. render page
// 3. retrieve page information
//////////////////////////////////////////////////////////////////////////
class CPDFCore
{
private:
    PDFDoc * pdfdoc;
    SplashOutputDev * outputDev;
    TextOutputDev * textOut;
    Controller * ctrl;

protected:
    void calculateLayout(CPageInfo & info);
    void getPageBasicInfo(CPageInfo & info);

public:
    CPDFCore(SplashColorMode colorModeA, int bitmapRowPadA,
	         GBool reverseVideoA, SplashColorPtr paperColorA);
    ~CPDFCore(void);

    void    setController(struct Controller * c){ctrl = c; }
    int     open(GooString *fileNameA, GooString *ownerPassword = NULL,
	            GooString *userPassword = NULL, void *guiDataA = NULL);
    int     open(BaseStream *stream, GooString *ownerPassword = NULL,
                GooString *userPassword = NULL, void *guiDataA = NULL);
    void    close();                
    RenderRet renderPage(CPageInfo & info, GBool useMediaBox, 
                GBool crop, GBool doLinks, GBool (*abortCheckCbk)(void *data) = NULL,
		        void *abortCheckCbkData = NULL);
    PDFDoc * getPDFDoc() { return pdfdoc; }
    int     getNumPages();

    // page zoom value
    double  getZoomMax(const int pageNumber);
    int     getPageType(const int pageNumber);
    double  verifyZoomMax(double zoom, const int pageNumber);

    // basic page info
    double  getPageWidth(const int pageNumber);
    double  getPageHeight(const int pageNumber);
    int     getPageRotate(const int pageNumber);
};

#endif
