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
 
#include "PDFLandscapeView.h"
#include "PDFView.h"
#include "controller.h"
#include <gdk/gdkkeysyms.h>
#include <liberipc/eripcviewer.h>
#include "PDFViewerLog.h"

//////////////////////////////////////////////////////////////////////////
// It's better to use two classes, so that they can be changeg independently.
// In this view:
// screenX refers the scroll position
// screenY refers the page image start position
//////////////////////////////////////////////////////////////////////////

CPDFLandscapeView::CPDFLandscapeView(void)
: view(NULL)
, nStart(-1)
, nEnd(-1)
, nOffset(100)
, bReady(gFalse)
{
}

CPDFLandscapeView::~CPDFLandscapeView(void)
{
}

//////////////////////////////////////////////////////////////////////////
// clear display list
// calculate layout according to mode
void CPDFLandscapeView::init()
{
    list.clear();
    if (view->isPageMode())
    {
        // validatePosition();
    }
    else if (view->isContinousMode())
    {
        // update zoom, otherwise zoomPage and zoomWidth will cause
        // some page problems. 
        double z = layout.initPagesPosition(view->ctrl);
        view->ctrl->settings.setZoom(z);
    }
    setReady(gFalse);
}

void CPDFLandscapeView::clear()
{
    list.clear();
    layout.clear();
}

//////////////////////////////////////////////////////////////////////////
// event handler
gboolean CPDFLandscapeView::buttonPressCbk(GdkEventButton *event)
{
    if (!isReady())
    {
        PV_LOGPRINTF("Not allow to handle mouse event now!");
        return FALSE;
    }
    int x = (int)event->x;
    int y = (int)event->y;
    if (y >= view->ctrl->settings.getScreenHeight())
    {
        return FALSE;
    }
    if (view->isPanEnable())
    {
        // enable panning, record the position
        view->initPan(x, y);
    }
    else if (view->isZoomInEnable() ||
        view->isZoomOutEnable())
    {
        view->initZoomRect(x, y);
    }
    else if (view->isScribbleEnable())
    {
        int ox, oy;
        int px, py;
        int pn = hitTest(x, y, ox, oy);

        // get page x & y
        getPageScreenPosition(pn, px, py);
        view->ctrl->scbMgr.onScribblePress(pn, view->ctrl, px, py, 
            x, y, ox, oy);
        return FALSE;
    }
    else if (view->isEraseEnable())
    {
        int ox, oy; 
        int pn = hitTest(x, y, ox, oy);
        getPageScreenPosition(pn, x, y);
        view->ctrl->scbMgr.onErasePress(pn, view->ctrl, 
            x, y, ox, oy);
    }
    return FALSE;
}          


gboolean CPDFLandscapeView::buttonReleaseCbk(GdkEventButton *event)
{
    if (!isReady())
    {
        PV_LOGPRINTF("Not allow to handle mouse event now!");
        return FALSE;
    }
    
    int x = (int)event->x;
    int y = (int)event->y;
    if (y >= view->ctrl->settings.getScreenHeight()) return FALSE;

    if (view->isScribbleEnable())
    {
        int ox, oy;
        int px, py;                
        int pn = hitTest(x, y, ox, oy);
        // get page x & y
        getPageScreenPosition(pn, px, py);     
        view->ctrl->scbMgr.onScribbleRelease(pn, view->ctrl, px, py, 
            x, y, ox, oy);
        return FALSE;                    
    }
    else if (view->isEraseEnable())
    {
        int ox, oy; 
        int pn = hitTest(x, y, ox, oy);
        getPageScreenPosition(pn, x, y);
        view->ctrl->scbMgr.onEraseRelease(pn, view->ctrl, 
            x, y, ox, oy);
        return FALSE;
    }
    // try to do hyper link at first. if not in scribble
    if (view->isSmallOffset(x, y))
    {
        PV_LOGPRINTF("Small Offset!");
        if (view->isPageMode())
        {
            if (doLinkPageMode(x, y))
                return FALSE;
        }
        else 
        {
            if (doLinkContinousMode(x, y))
                return FALSE;
        }
        PV_LOGPRINTF("Seems no link!");                                    
    }
    if (view->isPanEnable())
    {
        // pan should never out of range of current document
        int deltaX = view->panPt.x - x;
        int deltaY = view->panPt.y - y;

        if (view->isPageMode())
        {
            view->ctrl->settings.moveScreenX(-deltaX);
            view->ctrl->settings.moveScreenY(-deltaY);                    
        }
        else
        {                    
            view->ctrl->settings.moveScreenX(deltaX);
            view->ctrl->settings.moveScreenY(deltaY);                    
        }
        validatePosition();
        displayPage(view->ctrl->settings.getCurrentPage(), ScrollToPosition);
        return FALSE;
    }  
    else if (view->isZoomInEnable())
    {
        view->adjustZoomRect(x, y);
        zoomFromRect(view->zoomRect.left, 
            view->zoomRect.top,
            view->zoomRect.right,
            view->zoomRect.bottom);

    }	            
    else if (view->isZoomOutEnable())                    
    {
        view->adjustZoomRect(x, y);
        zoomToRect(view->zoomRect.left, 
            view->zoomRect.top,
            view->zoomRect.right,
            view->zoomRect.bottom);
    }
    return FALSE;
}

gboolean CPDFLandscapeView::motionNotifyCbk(GdkEventMotion *event)
{
    if (!isReady())
    {
        PV_LOGPRINTF("Not allow to handle mouse event now!");
        return FALSE;
    }
    int x = (int)event->x;
    int y = (int)event->y;
    if (y >= view->ctrl->settings.getScreenHeight())
    {
        PV_LOGPRINTF("Not allow to handle mouse event now!");
        return FALSE;
    }
    if (view->isZoomInEnable() || 
        view->isZoomOutEnable())
    {
        view->adjustZoomRect(x, y);        
    }   
    else if (view->isScribbleEnable())
    {
        int ox, oy; 
        int px, py;
        int pn = hitTest(x, y, ox, oy);
        
        // get page x & y
        getPageScreenPosition(pn, px, py);
        view->ctrl->scbMgr.onScribbleMove(pn, view->ctrl, px, py,
            x, y, ox, oy);
    }         
    else if (view->isEraseEnable())
    {
        int ox, oy; 
        int pn = hitTest(x, y, ox, oy);
        getPageScreenPosition(pn, x, y);
        view->ctrl->scbMgr.onEraseMove(pn, view->ctrl, 
            x, y, ox, oy);
    }
    return FALSE;
}

gboolean CPDFLandscapeView::keyPressCbk(GdkEventKey *event)
{
    if (event->time == 0)
    {
        onPageRendered(event->state);
        return FALSE;
    }

    switch (event->keyval)
    {
    case GDK_Home:
        displayPage(1);
        break;
    case GDK_F5:
        view->quit();
        break;  
    case GDK_Page_Up:
        view->ctrl->settings.setRenderDir(RenderPrev);
        if (view->ctrl->settings.getCurrentPage() <= 1)
        {
            displayPage(1, ScrollToPrevPage);    
        }
        else
        {
            displayPage(view->ctrl->settings.getCurrentPage() - 1, ScrollToPrevPage);
        }   
        break;
    case GDK_Page_Down:
        view->ctrl->settings.setRenderDir(RenderNext);
        if (view->ctrl->settings.getCurrentPage() >= view->ctrl->core->getNumPages())
        {
            displayPage(view->ctrl->settings.getCurrentPage(), ScrollToNextPage);    
        }
        else
        {
            displayPage(view->ctrl->settings.getCurrentPage() + 1, ScrollToNextPage);
        }   
        break;
    case GDK_F1: // iRex: long pageflip forward 
        view->ctrl->settings.setRenderDir(RenderNext);
        if (view->ctrl->settings.getCurrentPage() + JumpWidth > view->ctrl->core->getNumPages())
        {
            displayPage(view->ctrl->core->getNumPages());
        }
        else
        {
            displayPage(view->ctrl->settings.getCurrentPage() + JumpWidth, ScrollToNextPages);
        }            
        break;
    case GDK_F2: // iRex: long pageflip backward 
        view->ctrl->settings.setRenderDir(RenderPrev);
        if ((view->ctrl->settings.getCurrentPage() - JumpWidth) < 1)
        {
            displayPage(1);
        }
        else
        {
            displayPage(view->ctrl->settings.getCurrentPage() - JumpWidth, ScrollToPrevPages);
        }            
        break;
    default:
        view->ctrl->channels.busy_off();
        break; // no return: do not bypass keyPressCbk
    }
    return FALSE;
}

gboolean CPDFLandscapeView::deleteCbk(GdkEvent *event)
{
    return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// get page screen position
void CPDFLandscapeView::getPageScreenPosition(const int pn, int & x, int & y)
{
    if (view->isPageMode())
    {
        x = layout.getVirtualScreen().left;
        y = layout.getVirtualScreen().bottom;
    }
    else
    {
        rectangle rect;
        layout.getPageRect(pn, rect);
        x = rect.top - view->ctrl->settings.getScreenX();
        y = rect.left - view->ctrl->settings.getScreenY();
        y += (rect.right - rect.left);
    }

}

void CPDFLandscapeView::onPageRendered(const int pageNumber)
{
    // compare and draw, checkpoint
    if (list.remove(pageNumber))
    {
        if (list.isEmpty())
        {
            drawPages(nStart, nEnd);
        }
    }
}

void CPDFLandscapeView::ipcCbk( gint * fd,    GdkInputCondition id)
{
    char szBuffer[IPC_BUF_MAX]; 
    int nBuf = IPC_BUF_MAX;
    erIpcCmd_t command;
    erServerChannel_t channel = view->ctrl->channels.erIpcChannel;
    erIpcGetMessage(channel, szBuffer, &nBuf);
    if (vwrParseCommand(szBuffer, &command) >= 0) 
    {
        switch (command.cc) 
        {
        case ccVwrJumpToPage:
        {
            int pg = atoi(command.arg[1]);
            
            // should change render direction
            if (pg < view->ctrl->settings.getCurrentPage())
            {
                if (pg <= 1)
                {
                    view->ctrl->settings.setRenderDir(RenderNext);
                }
                else
                {
                    view->ctrl->settings.setRenderDir(RenderPrev);
                }
            }
            else if (pg > view->ctrl->settings.getCurrentPage())
            {
                if (pg >= view->ctrl->core->getNumPages())
                {
                    view->ctrl->settings.setRenderDir(RenderPrev);
                }
                else
                {
                    view->ctrl->settings.setRenderDir(RenderNext);
                }
            }
            
            displayPage(pg);
            break;
        }                
        case ccVwrToolbar:
        {
            // protect toolbar
            if (!isReady()) 
            {
                PV_TBPRINTF("Not allow to handle it during rendering!");
                return;
            }
            PV_TBPRINTF("Received toolbar message!");
            
            int iconID = atoi(command.arg[0]);
            int state  = atoi(command.arg[1]);
            if (view->ctrl->channels.onReceiveToolbarMsg(iconID, state))
            {
                view->ctrl->channels.setToolbarUpdateType();
                view->ctrl->channels.redrawToolbar();            
            }
            if (iconID == iconID_zoomfit)
            {
                if (view->isPageMode())
                {
                    zoomFitPageMode();
                }
                else if (view->isContinousMode())
                {
                    zoomFitContinousMode();
                }
            }
            else if (iconID == iconID_portrait_landscape ||
                     iconID == iconID_landscape_portrait )
            {
                view->ctrl->channels.setRotateState(RotateDirty);
                if (view->isPageMode())
                {                  
                    view->rotatePageMode(90);
                }
                else
                {
                    view->rotateContinousMode(90);
                }                    
            }
            else if (iconID == iconID_page_mode ||
                     iconID == iconID_continuous_mode)
            {
                switchMode();
            }
            else if (iconID == iconID_zoomback)
            {
                zoomBack();
            }
            else if (iconID == iconID_back)
            {
                backward();   
            }
            else if (iconID == iconID_forward)
            {
                forward();
            }
            else if (view->ctrl->scbMgr.isEraseDirty())
            {
                if (view->ctrl->channels.isScribbleIcon(iconID) ||
                    iconID == iconID_pan ||
                    iconID == iconID_zoomin)
                {
                    // need to redraw the page now
                    view->ctrl->scbMgr.setEraseDirty(gFalse);                    
                    view->ctrl->channels.setToolbarUpdateType(dmQFull);
                    if (iconID == iconID_zoomin)
                    {
                        // for zoom in, need to do something more
                        view->ctrl->channels.setZoomIn(3);                        
                    }
                    view->displayPage(view->ctrl->settings.getCurrentPage(), ScrollRedraw);
                }
            }
            break;
        }
        case ccVwrToolbarSync:
        {
            PV_TBPRINTF("Receive toolbar sync!");
            if (view->ctrl->channels.getToolbarUpdateType() == dmQTyping)
            {
                PV_TBPRINTF("Toolbar sync typing!");
                view->refreshDeviceScreen(dmQTyping, gFalse);
                view->ctrl->channels.setToolbarDirty(gFalse);
                view->ctrl->channels.setToolbarUpdateType(dmQUndefined);
            }
            // happens when a new page is displayed. 
            else if (view->ctrl->channels.getToolbarUpdateType() == dmQFull)
            {
                PV_TBPRINTF("Toolbar sync full update!");
                --view->ctrl->updateReq;
                if (view->ctrl->updateReq <= 0)
                {
                    view->refreshDeviceScreen(dmQFull);
                    view->ctrl->updateReq = 0;
                }
                view->ctrl->channels.setToolbarDirty(gFalse);
                view->ctrl->channels.setToolbarUpdateType(dmQUndefined);
            }
            break;
        }      
        case ccVwrPagebarSync:
        {
            --view->ctrl->updateReq;
            if (view->ctrl->updateReq <= 0)
            {
                view->refreshDeviceScreen(dmQFull);
                view->ctrl->updateReq = 0;
            }
            PV_TBPRINTF("Received pageBar sync!");
            break;
        }            
        default:
            break;
        }
    }
}

//////////////////////////////////////////////////////////////////////////
// 1. display specified page
// 2. change current page
// 3. issue new render request if page is availabe
void CPDFLandscapeView::displayPage(const int pageNumber, const ContinousModeScroll scroll)
{
    // check page number is valid or not
    if (pageNumber <= 0 ||
        pageNumber > view->ctrl->core->getNumPages())
    {
        PV_ERRORPRINTF("Invalid page number %d!", pageNumber);
        view->ctrl->channels.busy_off();
        return;
    }

    // set busy, not allow mouse input
    setReady(gFalse);

    if (view->isPageMode())
    {
        // change current page
        view->ctrl->settings.setCurrentPage(pageNumber);
        
        // display it
        displayPages(pageNumber, pageNumber);
    }
    else if (view->isContinousMode())
    {
        // calcuate pages should be displayed from pagenumber and scroll mode
        int start = pageNumber, end = pageNumber;
        getPagesRange(start, end, pageNumber, scroll);

        // debug
        PV_LOGPRINTF("Visible page range [%d, %d] current page %d", start, end, 
            view->ctrl->settings.getCurrentPage());

        // display them
        displayPages(start, end);
    }
}

//////////////////////////////////////////////////////////////////////////
// display range of pages: [start, end]
//////////////////////////////////////////////////////////////////////////
void CPDFLandscapeView::displayPages(const int start, const int end)
{
    // blink
    view->ctrl->channels.busy_blink();

    // record 
    nStart = start, nEnd = end;

    // check page number is valid or not
    GBool bRendering = gFalse;
    
    // clear all items
    list.clear();

    // check 
    for(int i = start; i <= end; ++i)
    {
        if (i >= 1 && i <= view->ctrl->core->getNumPages())
        {
            CPageInfo info; 
            info.pageNumber  = i;
            info.pageZoom    = view->ctrl->calZoom(i);
            info.rotate      = view->ctrl->settings.getRotate();
            CPageInfo * page = view->ctrl->pageList.checkPage(info);
            if (NULL == page)
            {
                TodoItem * item = new TodoItem;
                item->pageNumber = info.pageNumber;
                item->rotate     = info.rotate;
                item->zoom       = info.pageZoom;
                item->timeStamp  = view->ctrl->getTimeStamp();
                if (view->ctrl->thread.isInRendering(item))
                {
                    bRendering = gTrue;
                    PV_LOGPRINTF("page %d is in rendering!\n", i);
                    item->timeStamp = 0;    // set a flag
                }
                list.add(item);
            }
        }
    }

    // draw now if available 
    if (list.isEmpty())
    {
        drawPages(start, end);
    }
    else
    {
        if (bRendering)
        {
            list.issueItems(view);    
        }
        else
        {
            list.renderItems(view);  // abort and issue
        }
    }
}

void CPDFLandscapeView::zoomBack()
{
    if (!view->ctrl->settings.zoomBack())
    {
        return;
    }
    
    init();
    
    displayPage(view->ctrl->settings.getCurrentPage(), ScrollToPosition);
}

void CPDFLandscapeView::backward()
{
    HistoryItemPtr ptr = view->ctrl->settings.goBackward();
    if (ptr)
    {
        displayPage(view->ctrl->settings.getCurrentPage(), ScrollToPage);
    }
}

void CPDFLandscapeView::forward()
{
    HistoryItemPtr ptr = view->ctrl->settings.goForward();
    if (ptr)
    {
        displayPage(view->ctrl->settings.getCurrentPage(), ScrollToPage);
    }
}

void CPDFLandscapeView::drawPages(const int start, const int end)
{   
    // update pagebar
    updatePagebar();

    // update toolbar
    CPageInfo * page = NULL;
    page = view->ctrl->pageList.getPage(view->ctrl->settings.getCurrentPage());
    updateToolbar(page);

#if (PV_PROFILE_ON)
    int t1 = view->ctrl->getHighTimeStamp();
#endif
    
    // clear background
    view->ctrl->gtkMgr.clearBkground();

    // draw page image
    for(int i = start; i <= end; ++i)
    {
        page = view->ctrl->pageList.getPage(i);
        if (page)
        {
            drawPageImage(page);
        }
    }

    // sync 
    XSync(view->ctrl->gtkMgr.display, gTrue);	

#if (PV_PROFILE_ON)    
    int t2 = view->ctrl->getHighTimeStamp();
    PV_DUMP("\ndrawImage and XSync time %d\n", t2 - t1);
#endif    

    // check reference count
    if (view->ctrl->updateReq <= 0)
    {
        view->refreshDeviceScreen(dmQFull);
        view->ctrl->updateReq = 0;
    }

    // set ready. Now enable handle mouse event
    setReady();

    // issue new requests now
    view->issueNewRequests();
}

//////////////////////////////////////////////////////////////////////////
// called in continous mode
// 1. calcuate pages visible
// 2. calcuate which page is current page (max area)
void CPDFLandscapeView::getPagesRange(int & start, int & end, const int pageNumber, const ContinousModeScroll scroll)
{
    rectangle rect;
    if (scroll == ScrollToPage)
    {
        layout.getPageRect(pageNumber,rect);
        rect.bottom = rect.top + view->ctrl->settings.getScreenWidth();
        view->ctrl->settings.setScreenX(rect.top);
        view->ctrl->settings.setCurrentPage(layout.getPagesVisible(start, end, rect));
        return;
    }

    if (scroll == ScrollToNextPage)
    {
        view->ctrl->settings.moveScreenX(view->ctrl->settings.getScreenWidth() - PageRepeat);
        validatePosition();     
        view->ctrl->settings.getScreenRect(rect);
        view->ctrl->settings.setCurrentPage(layout.getPagesVisible(start, end, rect)); 
        return;
    }

    if (scroll == ScrollToPrevPage)
    {
        view->ctrl->settings.moveScreenX(PageRepeat - view->ctrl->settings.getScreenWidth());
        validatePosition();     
        view->ctrl->settings.getScreenRect(rect);
        view->ctrl->settings.setCurrentPage(layout.getPagesVisible(start, end, rect)); 
        return;
    }

    if (scroll == ScrollToPosition)
    {
        // position is ready, need check
        validatePosition();
        view->ctrl->settings.getScreenRect(rect);
        
        // when scroll, should pay attention to render direction
        int n = layout.getPagesVisible(start, end, rect);
        if (n < view->ctrl->settings.getCurrentPage())
        {
            view->ctrl->settings.setRenderDir(RenderPrev);
        }
        else
        {
            view->ctrl->settings.setRenderDir(RenderNext);
        }
        view->ctrl->settings.setCurrentPage(n);
        return;                
    }
    
    if (scroll == ScrollToNextPages)
    {
        view->ctrl->settings.moveScreenX(view->ctrl->settings.getScreenWidth() * JumpWidth);
        validatePosition();     
        view->ctrl->settings.getScreenRect(rect);
        view->ctrl->settings.setCurrentPage(layout.getPagesVisible(start, end, rect)); 
        return;
    }

    if (scroll == ScrollToPrevPages)
    {
        view->ctrl->settings.moveScreenX(- JumpWidth * view->ctrl->settings.getScreenWidth());
        validatePosition();     
        view->ctrl->settings.getScreenRect(rect);
        view->ctrl->settings.setCurrentPage(layout.getPagesVisible(start, end, rect)); 
        return;
    }
    
    
    if (scroll == ScrollRedraw)
    {
        start = nStart; end = nEnd;
    }
}

void CPDFLandscapeView::drawPageImage(CPageInfo *page)
{
    // update page time stamp
    page->timestamp = view->ctrl->getTimeStamp();

    // page position 
    int x = view->ctrl->settings.getScreenX();
    int y = view->ctrl->settings.getScreenY();

    // should adjust page position
    if (view->isPageMode())
    {
        if (page->bitmap->getHeight() <= view->ctrl->settings.getScreenHeight() ||
            view->ctrl->settings.getZoom() == ZoomPage)
        {
            y = ((view->ctrl->settings.getScreenHeight() - page->bitmap->getHeight()) >> 1);
            view->ctrl->settings.setScreenY(y);
        }
        layout.updatePageRect(x, y, page->bitmap->getWidth(), page->bitmap->getHeight());
    }
    else if (view->isContinousMode())
    {
        rectangle rect;
        layout.getPageRect(page->pageNumber, rect);
        x = rect.top - view->ctrl->settings.getScreenX();
        y = rect.left - view->ctrl->settings.getScreenY();        
    }

    // display image. draws border only in continous mode
    view->ctrl->gtkMgr.drawImage(page->bitmap,
        0, 0, 
        x, y,
        page->bitmap->getWidth(), page->bitmap->getHeight(),
        view->isContinousMode());

    // when drawing scribble, should use another y position
    y += page->bitmap->getHeight();
    drawScribble(x, y, page);
}

void CPDFLandscapeView::updateToolbar(CPageInfo *page)
{
    GBool bDirty = gFalse;
    
    // pan
    int w = page->bitmap->getWidth();
    int h = page->bitmap->getHeight();
    rectangle rect;
    layout.getVirtualScreen(rect);
    if ((view->isPageMode() &&
        w <= view->ctrl->settings.getScreenWidth() &&
        h <= view->ctrl->settings.getScreenHeight() &&
        view->ctrl->settings.getScreenX() >= 0 &&
        view->ctrl->settings.getScreenY() >= 0 &&
        view->ctrl->settings.getScreenY() + h <= view->ctrl->settings.getScreenHeight() && 
        view->ctrl->settings.getScreenX() + w <= view->ctrl->settings.getScreenWidth()))
    {
        if (view->ctrl->channels.getPanState() != iconState_grey)
        {
            bDirty = gTrue;
            view->ctrl->channels.setPanState(iconState_grey);
        }
    }
    else if (view->isContinousMode() &&
            rect.right - rect.left <= view->ctrl->settings.getScreenHeight() &&
            rect.bottom - rect.top <= view->ctrl->settings.getScreenWidth() &&
            view->ctrl->channels.getPanState() != iconState_grey)
    {
        bDirty = gTrue;
        view->ctrl->channels.setPanState(iconState_grey);
    }
    else
    {
        if (view->ctrl->channels.getPanState() == iconState_grey)
        {
            bDirty = gTrue;
            view->ctrl->channels.setPanState(iconState_normal);        
        }
    }

    // zoom
    double z  = page->calZoom();
    if (z >= view->ctrl->core->getZoomMax(page->pageNumber))
    {
        if (view->ctrl->channels.getZoomInState() != iconState_grey)
        {
            bDirty = gTrue;
            view->ctrl->channels.setZoomIn(iconState_grey);            
        }
    }    
    else
    {  
        if (view->ctrl->channels.getZoomInState() == iconState_grey ||
            view->ctrl->channels.getZoomInState() == iconState_selected)
        {
            bDirty = gTrue;
            view->ctrl->channels.setZoomIn(iconState_normal);
        }
        else if (view->ctrl->channels.getZoomInState() == 3)
        {
            view->ctrl->channels.setZoomIn(iconState_selected);
        }
    }
    
    // zoom back
    if (view->ctrl->channels.getZoombackState() != view->ctrl->settings.getZoomBackState())
    {
        bDirty = gTrue;
        view->ctrl->channels.setZoombackState(view->ctrl->settings.getZoomBackState());
    }

    /*
    if (z <= ZoomMin)
    {
        if (view->ctrl->channels.getZoomOutState() != iconState_grey)
        {
            bDirty = gTrue;
            view->ctrl->channels.setZoomOut(iconState_grey);        
        }
    }
    else
    {
        if (view->ctrl->channels.getZoomOutState() == iconState_grey ||
            view->ctrl->channels.getZoomOutState() == iconState_selected)
        {
            bDirty = gTrue;
            view->ctrl->channels.setZoomOut(iconState_normal);
        }
    }
    */
    
    // rotate
    if (view->ctrl->channels.getRotateState() != LandscapeToPortrait)
    {
        bDirty = gTrue;
        view->ctrl->channels.setRotateState(LandscapeToPortrait);
    }            
    
    // mode
    if (view->ctrl->channels.getContinousState() == iconState_normal &&
        view->isContinousMode())
    {
        bDirty = gTrue;
        view->ctrl->channels.setContinousState(3);
    }        
    else if (view->ctrl->channels.getContinousState() == 3 &&
             view->isPageMode())
    {
        bDirty = gTrue;
        view->ctrl->channels.setContinousState(iconState_normal);
    }

    // backward
    if (view->ctrl->channels.getBackwardState() == iconState_grey &&
        view->ctrl->settings.canGoBack())
    {
        bDirty = gTrue;
        view->ctrl->channels.setBackwardState(iconState_normal);
    }        
    else if (view->ctrl->channels.getBackwardState() == iconState_normal &&
        !view->ctrl->settings.canGoBack())
    {
        bDirty = gTrue;
        view->ctrl->channels.setBackwardState(iconState_grey);
    }

    // forward
    if (view->ctrl->channels.getForwardState() == iconState_grey &&
        view->ctrl->settings.canGoForward())
    {
        bDirty = gTrue;
        view->ctrl->channels.setForwardState(iconState_normal);
    }        
    else if (view->ctrl->channels.getForwardState() == iconState_normal &&
        !view->ctrl->settings.canGoForward())
    {
        bDirty = gTrue;
        view->ctrl->channels.setForwardState(iconState_grey);
    }

    // update
    if (bDirty)
    {
        view->ctrl->channels.setToolbarDirty();
        view->ctrl->channels.setToolbarUpdateType(dmQFull);
        view->ctrl->updateReq++;
        PV_LOGPRINTF("updateReq %d", view->ctrl->updateReq);        
        view->ctrl->channels.redrawToolbar();
    }
    else
    {
        view->ctrl->channels.setToolbarDirty(gFalse);
        view->ctrl->channels.setToolbarUpdateType(dmQUndefined);
    }
}

void CPDFLandscapeView::updatePagebar()
{
    // increase reference count
    view->ctrl->updateReq++;
    PV_LOGPRINTF("updateReq %d", view->ctrl->updateReq);
    view->ctrl->channels.pagebar_set_current_page(view->ctrl->settings.getCurrentPage());
    // offset. todo
}

//////////////////////////////////////////////////////////////////////////
// two kinds of zoom in: area and line
// steps:
// 1. check rectangle
// 2. calcuate ratio
// 3. calcuate the new position of rectangle
// 4. adjust screenX and screenY from rectangle position and ratio
void CPDFLandscapeView::zoomFromRect(int left, int top, int right, int bottom)
{
    rectangle rect; 
    rect.left = left; rect.top = top; rect.right = right; rect.bottom = bottom;
    rect.normalize();
    if (view->isPageMode())
    {
        zoomFromRectPageMode(rect);
    }
    else if (view->isContinousMode())
    {
        zoomFromRectContinousMode(rect);
    }
}

void CPDFLandscapeView::zoomFromRectPageMode(rectangle & rect)
{
    // tmp var
    double ratio = 1.0, max = 1.0, z = 1.0, tmp = 1.0;

    // get page info
    CPageInfo * page = view->ctrl->pageList.getPage(view->ctrl->settings.getCurrentPage());
    max = view->ctrl->core->getZoomMax(page->pageNumber);
    z   = page->calZoom();    
    ratio = view->calRatio(rect);

    // calcuate zoom factor from dpi: dpi = 0.01 * zoomA * 72
    // zoom and then calcuate the new position
    tmp = z * ratio;
    if (tmp >= max)
    {
        view->ctrl->settings.setNewZoom(max);
        ratio = max / z;
    }
    else if (tmp <= ZoomMin)
    {
        view->ctrl->settings.setNewZoom(ZoomMin);
        ratio = ZoomMin / z;
    }
    else
    {
        view->ctrl->settings.setNewZoom(tmp);
    }

    // center the rectangle
    adjustPositionPageMode(rect, ratio);

    // display it with new zoom factor
    displayPage(view->ctrl->settings.getCurrentPage());
}

//////////////////////////////////////////////////////////////////////////
// when in continous mode:
// 1. check first page that intersects with input rectangle
// 2. calculate offset
// 3. calculate ratio
void CPDFLandscapeView::zoomFromRectContinousMode(rectangle &rect)
{
    // tmp var
    double ratio = 1.0, max = 1.0, min = 99999.0, z = 1.0, tmp = 1.0;

    // convert from screen to virtual screen
    layout.screenToVirtualScreen(view->ctrl, rect);
    
    // get max and min zoom factor for all visible pages
    int pn = nStart;
    CPageInfo * page = NULL;
    while (pn <= nEnd)
    {
        tmp = view->ctrl->core->getZoomMax(pn);
        if (max < tmp) max = tmp;

        // actually, should check which page is selected. todo
        page = view->ctrl->pageList.getPage(pn);
        tmp  = page->calZoom();  
        if (min > tmp) min = tmp;
        ++pn;
    }
    z = min;
    ratio = view->calRatio(rect);

    // calcuate zoom factor from dpi: dpi = 0.01 * zoomA * 72
    // zoom and then calcuate the new position
    tmp = z * ratio;
    if (tmp >= max)
    {
        view->ctrl->settings.setNewZoom(max);
        ratio = max / z;
    }
    else if (tmp <= ZoomMin)
    {
        view->ctrl->settings.setNewZoom(ZoomMin);
        ratio = ZoomMin / z;
    }
    else
    {
        view->ctrl->settings.setNewZoom(tmp);
    }
    
    // reset 
    init();

    // center the rectangle for continous mode
    swap(rect.left, rect.top); swap(rect.right, rect.bottom);
    adjustPositionContinousMode(rect, ratio);

    // display it with new zoom factor
    displayPage(view->ctrl->settings.getCurrentPage(), ScrollToPosition);
}

void CPDFLandscapeView::zoomToRect(int left, int top, int right, int bottom)
{
    /*
    // check rectangle and normalize rectangle
    if (left > right) { int tmp = left; left = right; right = tmp; }
    if (top > bottom) { int tmp = top; top = bottom; bottom = tmp; }
    PV_LOGPRINTF("Zoom To rectangle (%d %d)-(%d %d)", left, top, right, bottom);

    double ratio = 1.0;
    CPageInfo * page = view->ctrl->pageList.getPage(view->ctrl->settings.getCurrentPage());
    rectangle rect;
    rect.left = left; rect.top = top; rect.right = right; rect.bottom = bottom;

    if (left == right || top == bottom) 
    {
        PV_LOGPRINTF("Rectangle is empty!");
        ratio = ZoomMin / page->calZoom();
        view->ctrl->settings.setNewZoom(ZoomMin);
        adjustPosition(rect, ratio);
    }
    else
    {        
        // calcuate zoom factor from dpi: dpi = 0.01 * zoomA * 72
        // zoom and then calcuate the new position
        ratio = view->calRatio(rect, gFalse);
        view->ctrl->settings.setZoom(page->calZoom() * ratio);
        if (view->ctrl->settings.getZoom() >= view->ctrl->core->getZoomMax(page->pageNumber))
        {
            view->ctrl->settings.setNewZoom(view->ctrl->core->getZoomMax(page->pageNumber));
            ratio = view->ctrl->core->getZoomMax(page->pageNumber) / page->calZoom();
        }
        else if (view->ctrl->settings.getZoom() <= ZoomMin)
        {
            view->ctrl->settings.setNewZoom(ZoomMin);
            ratio = ZoomMin / page->calZoom();
        }
        else
        {
            view->ctrl->settings.setNewZoom(view->ctrl->settings.getZoom());
        }
        adjustPosition( rect, ratio);
    }
    displayPage(view->ctrl->settings.getCurrentPage());
    */
}

//////////////////////////////////////////////////////////////////////////
// helper function
void CPDFLandscapeView::adjustPositionPageMode( const rectangle & oldRect, double ratio)
{
    // old offset
    int offsetX = 0, offsetY = 0;
    point oldCenter, newCenter;
    oldCenter.x = (oldRect.left + oldRect.right) / 2;
    oldCenter.y = (oldRect.top + oldRect.bottom) / 2;
    offsetX = oldCenter.x - view->ctrl->settings.getScreenX();
    offsetY = oldCenter.y - view->ctrl->settings.getScreenY();

    // calculate new rect
    rectangle newRect;   
    int width  = (int)((double)(oldRect.right - oldRect.left) * ratio);
    int height = (int)((double)(oldRect.bottom - oldRect.top) * ratio);
    newRect.top = (view->ctrl->settings.getScreenHeight() - height) / 2;
    newRect.bottom = newRect.top + height;
    newRect.left = (view->ctrl->settings.getScreenWidth() - width) /2;
    newRect.right = newRect.left + width;
    
    // offset   
    newCenter.y = (newRect.top + newRect.bottom) / 2;
    if (SmallNone != view->lessThreshold(oldRect))
    {
        newCenter.x =  oldCenter.x - (int)((double)offsetX * ratio);   
    }
    else
    {
        newCenter.x = (newRect.left + newRect.right) / 2;
        newCenter.x -= (int)((double)offsetX * ratio);
    }        
    
    view->ctrl->settings.setScreenX(newCenter.x );
    view->ctrl->settings.setScreenY(newCenter.y - (int)((double)offsetY * ratio));
}

//////////////////////////////////////////////////////////////////////////
// in continous mode, oldRect refers virtual screen
void CPDFLandscapeView::adjustPositionContinousMode(const rectangle & oldRect, double ratio)
{
    // calculate the center point & offset
    int x = 0, y = 0, offset = 0;
    point oldCenter, newCenter;
    oldCenter.x = (oldRect.left + oldRect.right) / 2;
    oldCenter.y = (oldRect.top + oldRect.bottom) / 2;
    offset = oldCenter.y - view->ctrl->settings.getScreenX();
    x = (int)((double)oldCenter.x * ratio);
    y = (int)((double)oldCenter.y * ratio);
    
    // new center point
    newCenter.x = x - view->ctrl->settings.getScreenHeight() / 2;
    if (SmallNone == view->lessThreshold(oldRect))
    {
        newCenter.y = y - view->ctrl->settings.getScreenWidth() / 2;
    }
    else
    {
        newCenter.y = y - offset;   // do not change offset
    }        
        
    // update screen position
    view->ctrl->settings.setScreenX(newCenter.y);
    view->ctrl->settings.setScreenY(newCenter.x);
}

//////////////////////////////////////////////////////////////////////////
// validatePosition checks screenx and screeny, correct them if they are 
// out of range.
void CPDFLandscapeView::validatePosition()
{
    rectangle rect; 
    int w = 0;
    layout.getVirtualScreen(rect);
    if (view->isContinousMode())
    {
        w = rect.right - rect.left;
        if (w > view->ctrl->settings.getScreenHeight())
        {
            if (view->ctrl->settings.getScreenY() < rect.left + Margin)
            {
                view->ctrl->settings.setScreenY(rect.left + Margin);
            }
            else if (view->ctrl->settings.getScreenY() > 
            rect.right - view->ctrl->settings.getScreenHeight() - Margin)
            {
                view->ctrl->settings.setScreenY(rect.right - view->ctrl->settings.getScreenHeight() - Margin);
            }
        }
        else
        {
            view->ctrl->settings.setScreenY((w - view->ctrl->settings.getScreenHeight()) / 2);
        }

        if (view->ctrl->settings.getScreenX() < rect.top + Margin)
        {
            view->ctrl->settings.setScreenX(rect.top + Margin);
        }
        else if (view->ctrl->settings.getScreenX() + view->ctrl->settings.getScreenWidth() > rect.bottom - Margin)
        {
            view->ctrl->settings.setScreenX(rect.bottom - view->ctrl->settings.getScreenWidth() - Margin);
        }
    }
    else if (view->isPageMode())
    {
        // here, rect is the page image rect
        w = rect.right - rect.left;
        int h = rect.bottom - rect.top;
        if ( h > view->ctrl->settings.getScreenHeight())
        {
            if (view->ctrl->settings.getScreenY() > -Margin)
            {
                view->ctrl->settings.setScreenY(-Margin);
            }
            else if (view->ctrl->settings.getScreenY() < 
                view->ctrl->settings.getScreenHeight() - h + Margin)
            {
                view->ctrl->settings.setScreenY(view->ctrl->settings.getScreenHeight() - h + Margin);
            }
        }
        else
        {
            view->ctrl->settings.setScreenY((view->ctrl->settings.getScreenHeight() - h) /2 );
        }
                
        if (w > view->ctrl->settings.getScreenWidth())
        {
            if (view->ctrl->settings.getScreenX() > -Margin)
            {
                view->ctrl->settings.setScreenX(-Margin);
            }
            else if (view->ctrl->settings.getScreenX() < view->ctrl->settings.getScreenWidth() - w + Margin)
            {
                view->ctrl->settings.setScreenX(view->ctrl->settings.getScreenWidth() - w + Margin);
            }
        }
        else
        {
            view->ctrl->settings.setScreenX((view->ctrl->settings.getScreenWidth() - w) / 2);
        }
    }
}

void CPDFLandscapeView::switchMode()
{
    // we should redraw when mode is changed
    if (view->isPageMode())
    {
        // from page mode to continous mode
        view->setContinousMode();

        // reset page position
        init();        
        // adjust position
        rectangle rect;
        layout.getPageRect(view->ctrl->settings.getCurrentPage(), rect);        
        int y = view->ctrl->settings.getScreenY();
        view->ctrl->settings.setScreenY(rect.left - y);
    }
    else if (view->isContinousMode())
    {
        // from continous mode to page mode
        view->setPageMode();
        
        // adjust position, in page mode, display page from top or from offset?
        rectangle rect;
        layout.getPageRect(view->ctrl->settings.getCurrentPage(), rect);
        view->ctrl->settings.setScreenX(0);
        int y = view->ctrl->settings.getScreenY();
        view->ctrl->settings.setScreenY(rect.left - y);        
    }

    // disable zoomback
    view->ctrl->settings.disableZoomBack();
    
    displayPage(view->ctrl->settings.getCurrentPage());
}

//////////////////////////////////////////////////////////////////////////
// zoom fit for page mode
void CPDFLandscapeView::zoomFitPageMode()
{
    // update settings, reset the screen position
    view->ctrl->settings.setNewZoom(ZoomPage);
    view->ctrl->settings.setScreenX(0);
    view->ctrl->settings.setScreenY(0);                
    displayPage(view->ctrl->settings.getCurrentPage());
}

//////////////////////////////////////////////////////////////////////////
// zoom fit for continous mode, should adjust position
void CPDFLandscapeView::zoomFitContinousMode()
{
    // calculate ratio
    double oldZoom = view->ctrl->calZoom(view->ctrl->settings.getCurrentPage());
    view->ctrl->settings.setZoom(ZoomPage);     // maybe use ZoomWidth
    double newZoom = view->ctrl->calZoom(view->ctrl->settings.getCurrentPage());
    view->ctrl->settings.setZoom(oldZoom);    
    view->ctrl->settings.setNewZoom(newZoom);
    double ratio = newZoom / oldZoom;
   
    // get current virtual screen 
    rectangle rect;
    rect.left = 0; rect.right = view->ctrl->settings.getScreenHeight();
    rect.top = view->ctrl->settings.getScreenX(); 
    rect.bottom = view->ctrl->settings.getScreenX();

    // reset page position list
    init();
    
    // adjust
    adjustPositionContinousMode(rect, ratio);

    // display 
    displayPage(view->ctrl->settings.getCurrentPage(), ScrollToPosition);
}

//////////////////////////////////////////////////////////////////////////
// by scribble, hitTest returns page number hits. 
// the output ox and oy is virtual offset value in the page
int CPDFLandscapeView::hitTest(const int x, const int y, int & ox, int & oy)
{
    if (view->isPageMode())
    {
        if (layout.hitTest(x, y))
        {
            // convert to page offset
            oy = x - view->ctrl->settings.getScreenX();
            ox = layout.getVirtualScreen().bottom - y;
            return view->ctrl->settings.getCurrentPage();
        }
    }
    else
    {
        if (nStart > 0 && nEnd > 0 && list.isEmpty())
        {
            // from screen to virtual screen
            oy = x + view->ctrl->settings.getScreenX();
            ox = y + view->ctrl->settings.getScreenY();
            int pn = layout.hitTest(ox, oy, nStart, nEnd);
            if (pn > 0)
            {
                rectangle rect;
                layout.getPageRect(pn, rect);
                oy -= rect.top; 
                ox = rect.right - ox;
                return pn;
            }
            return -1;
        }
    }
    return -1;
}

GBool CPDFLandscapeView::doLinkPageMode(const int x, const int y)
{
    return view->doLink(view->ctrl->settings.getCurrentPage(), 
        x - view->ctrl->settings.getScreenX(), 
        y - view->ctrl->settings.getScreenY());
}

// when do link, should use offset position in the page
GBool CPDFLandscapeView::doLinkContinousMode(const int x, const int y)
{
    point pt; pt.x = x; pt.y = y;
    layout.screenToVirtualScreen(view->ctrl, pt);
    int pn = layout.hitTest(pt.y, pt.x, nStart, nEnd);
    if (pn < 0) 
    {
        return gFalse;
    }        
    rectangle rect;
    layout.getPageRect(pn, rect);
    // pt.x = rect.top - view->ctrl->settings.getScreenX(); pt.x = x - pt.x;
    // pt.y = rect.left - view->ctrl->settings.getScreenY(); pt.y = y - pt.y;
    return view->doLink(pn, pt.x - rect.top, pt.y - rect.left);
}

//////////////////////////////////////////////////////////////////////////
// draw scribble lines
// x, y is page start position
void CPDFLandscapeView::drawScribble(const int x, const int y, CPageInfo *page)
{
    view->ctrl->scbMgr.drawScribblePage(page->pageNumber, 
        view->ctrl, x, y);
}
