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

/*
 * Portions of this code are taken from xpdf and are
 * Copyright 1996-2003 Glyph & Cog, LLC
 */

#include "PDFView.h"
#include "PDFViewerLog.h"
#include "goo/gtypes.h"
#include "controller.h"
#include "PageInfo.h"
#include <X11/keysym.h>
#include "liberdm/erdm.h"
#include "PDFApp.h"
#include <liberipc/eripcviewer.h>
#include <math.h>
#include "PDFPortraitView.h"
#include "PDFLandscapeView.h"
#include "GtkAuthDialog.h"

static CPDFPortraitView portraitView;
static CPDFLandscapeView landscapeView;

CPDFView::CPDFView(GtkWidget * win, Controller * c)
{
    drawArea = win; ctrl = c;
    
    // install IPC callback message handler
    int nRet = 0, fd = -1;
    erServerChannel_t channel;
    nRet = erIpcOpenServerChannel(ER_PDF_VIEWER_CHANNEL, &channel);
    ctrl->channels.erIpcChannel = channel;
    fd = erIpcGetServerFd(channel);

    gdk_input_add (fd, 
        GDK_INPUT_READ, 
        (GdkInputFunction)ipcCbk,
        (gpointer) this);

    // password dialog
    bAuth = gFalse;
    password = NULL;

    // others
    panPt.x = panPt.y = 0;
    zoomRect.left = zoomRect.top = zoomRect.right = zoomRect.bottom = 0;
    portraitView.setView(this);
    landscapeView.setView(this);
    
    installEventHandler(win);
}

void CPDFView::initializeViews()
{
    // close dialog 
    if (bAuth)
    {
        bAuth = gFalse;
        gtk_window_unfullscreen(GTK_WINDOW(drawArea));
    }

    // init the views
    if (isLandscapeView())
    {
        landscapeView.init();
    }
    else
    {
        portraitView.init();
    }
}

CPDFView::~CPDFView(void)
{
}

void CPDFView::installEventHandler(GtkWidget * window)
{
    drawArea = window;

    gtk_signal_connect (GTK_OBJECT (window), "expose_event",
        GTK_SIGNAL_FUNC (exposeCbk), this);

    gtk_signal_connect (GTK_OBJECT (window), "button_press_event",
        GTK_SIGNAL_FUNC (buttonPressCbk), this);

    gtk_signal_connect (GTK_OBJECT (window), "button_release_event",
        GTK_SIGNAL_FUNC (buttonReleaseCbk), this);

    gtk_signal_connect (GTK_OBJECT (window), "motion_notify_event",
        GTK_SIGNAL_FUNC (motionNotifyCbk), this);

    gtk_signal_connect (GTK_OBJECT (window), "key_press_event",
        GTK_SIGNAL_FUNC (keyPressCbk), this);

    gtk_signal_connect (GTK_OBJECT (window), "delete_event",
        GTK_SIGNAL_FUNC (deleteCbk), this);
}

void CPDFView::onClose()
{
    portraitView.clear();
    landscapeView.clear();
    ctrl->scbMgr.close();
}

void CPDFView::initPan(const int x, const int y)
{
    panPt.x = x; panPt.y = y;
}

void CPDFView::initZoomRect(const int x, const int y)
{
    zoomRect.left = zoomRect.right = x;
    zoomRect.top = zoomRect.bottom = y;
}

void CPDFView::adjustZoomRect(const int x, const int y)
{
    if (zoomRect.left > x)
    {
        zoomRect.left = x;
    }
    if (zoomRect.top > y)
    {
        zoomRect.top = y;
    }
    if (zoomRect.right < x)
    {
        zoomRect.right = x;
    }
    if (zoomRect.bottom < y)
    {
        zoomRect.bottom = y;
    }
}

GBool CPDFView::isSmallOffset(const int x, const int y)
{
    if (isPanEnable())
    {
        return (abs(panPt.x - x) < SmallOffset &&
                abs(panPt.y - y) < SmallOffset);
    }
    if (isZoomInEnable() ||
        isZoomOutEnable())
    {
        adjustZoomRect(x, y);
        return (abs(zoomRect.left - zoomRect.right) < SmallOffset &&
                abs(zoomRect.top - zoomRect.bottom) < SmallOffset);
    }
    return gTrue;
}

int CPDFView::lessThreshold(const rectangle & rect)
{
    if (rect.right - rect.left < RectThreshold)
    {
        return SmallWidth;
    }

    if (rect.bottom - rect.top < RectThreshold)
    {
        return SmallHeight;
    }
    return SmallNone;
}

GBool CPDFView::isPanEnable()
{
    return ctrl->channels.isPanEnable();
}

GBool CPDFView::isZoomInEnable()
{
    return ctrl->channels.isZoomInEnable();
}

GBool CPDFView::isZoomOutEnable()
{
    return ctrl->channels.isZoomOutEnable();
}

GBool CPDFView::isScribbleEnable()
{
    return ctrl->channels.isScribbleEnable();
}

GBool CPDFView::isEraseEnable()
{
    return ctrl->channels.isEraseEnable();
}

GBool CPDFView::isPageMode()
{
    return (ctrl->settings.getMode() & ModePage);
}

GBool CPDFView::isContinousMode()
{
    return (ctrl->settings.getMode() & ModeContinous);
}

void CPDFView::setPageMode()
{
    ctrl->settings.setMode(ModePage);
}

void CPDFView::setContinousMode()
{
    ctrl->settings.setMode(ModeContinous);
}

GBool CPDFView::isPortraitView()
{
    return (ctrl->settings.getRotate() == 0 ||
            ctrl->settings.getRotate() == 180);
}

GBool CPDFView::isLandscapeView()
{
    return (ctrl->settings.getRotate() == 90 ||
            ctrl->settings.getRotate() == 270);
}

//////////////////////////////////////////////////////////////////////////
// event handler. 
gboolean CPDFView::exposeCbk(GtkWidget * widget, GdkEventExpose * event, gpointer ptr)
{
    CPDFView * view = (CPDFView *)ptr;
    if (view->isPortraitView())
    {
        if (!portraitView.isReady()) return TRUE;
    }
    else if (view->isLandscapeView())
    {
        if (!landscapeView.isReady()) return TRUE;
    }

    // draw now
    if (view->ctrl->settings.getCurrentPage() > 0)
    {
        view->displayPage(view->ctrl->settings.getCurrentPage(), ScrollRedraw);
    }
    return TRUE;
}

gboolean CPDFView::buttonPressCbk(GtkWidget * widget, GdkEventButton * event, gpointer user_data)
{
    CPDFView *view = (CPDFView *)user_data;
    if (view->isPortraitView())
    {
        return portraitView.buttonPressCbk(event);
    }
    else if (view->isLandscapeView())
    {
        return landscapeView.buttonPressCbk(event);
    }
    return TRUE;
}

gboolean CPDFView::buttonReleaseCbk(GtkWidget * widget, GdkEventButton * event, gpointer user_data)
{
    CPDFView *view = (CPDFView *)user_data;
    if (view->isPortraitView())
    {
        return portraitView.buttonReleaseCbk(event);
    }
    else if (view->isLandscapeView())
    {
        return landscapeView.buttonReleaseCbk(event);
    }
    return TRUE;
}

gboolean CPDFView::motionNotifyCbk(GtkWidget * widget, GdkEventMotion * event, gpointer user_data)
{
    CPDFView *view = (CPDFView *)user_data;
    if (view->isPortraitView())
    {
        return portraitView.motionNotifyCbk(event);
    }
    else if (view->isLandscapeView())
    {
        return landscapeView.motionNotifyCbk(event);
    }
    return TRUE;
}

gboolean CPDFView::keyPressCbk(GtkWidget * widget, GdkEventKey * event, gpointer user_data)
{
    CPDFView *view = (CPDFView *)user_data;
    if (view->isPortraitView())
    {
        return portraitView.keyPressCbk(event);
    }
    else if (view->isLandscapeView())
    {
        return landscapeView.keyPressCbk(event);
    }
    return FALSE;
}

gboolean CPDFView::deleteCbk(GtkWidget * widget, GdkEvent * event, gpointer user_data)
{
    return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// calculate the ratio,
// change it if zoom in/out does not work correctly
double CPDFView::calRatio(rectangle & rect, GBool bZoomin)
{
    double ratio = 1.0 , tmp = 1.0;
    int width = abs(rect.right - rect.left) + 1;    // avoid / 0
    int height = abs(rect.bottom - rect.top) + 1;   // avoid / 0

    if (bZoomin)
    {
        ratio = (double)ctrl->settings.getScreenWidth() / (double)width;
        tmp = (double)ctrl->settings.getScreenHeight() / (double)height;
    }
    else
    {
        ratio = (double)width / (double)ctrl->settings.getScreenWidth();
        tmp = (double)height / (double)ctrl->settings.getScreenHeight();
    }
    if (ratio > tmp) return tmp;
    return ratio;
}

//////////////////////////////////////////////////////////////////////////
// Should check zoom max value when
// 1. issues a new request 
// 2. receives a message from render thread
// 3. query a page in page list
//////////////////////////////////////////////////////////////////////////
void CPDFView::displayPage(const int pageNumber, const ContinousModeScroll mode)
{
    // which view should be used
    if (isPortraitView())
    {
        portraitView.displayPage(pageNumber, mode);
    }
    else if (isLandscapeView())
    {
        landscapeView.displayPage(pageNumber, mode);
    }
}

// checkpoint, issue a new request
void CPDFView::issueItem(const int pn, GBool bHead, const int time)
{
    if (pn < 1 || pn > ctrl->core->getNumPages())
    {
        PV_LOGPRINTF("Do not issue page %d", pn);
        return;
    }
    PV_LOGPRINTF("IssueItem %d\n", pn);
    TodoItem * item = new TodoItem;
    item->pageNumber = pn;
    item->rotate     = ctrl->settings.getRotate();
    item->zoom       = ctrl->calZoom(pn);
    item->timeStamp  = ctrl->getTimeStamp() + time;
    ctrl->thread.addTodoItem(item, bHead);
}

void CPDFView::issuePrevNextPages(const int width, const int from, const int timeOffset)
{
    if (ctrl->settings.getRenderDir() == RenderPrev)
    {
        for(int i = width; i >= 1; --i)
        {
            issueItem(ctrl->settings.getCurrentPage() - from - i, gTrue, timeOffset);
        }            
    }   
    else if (ctrl->settings.getRenderDir() == RenderNext)
    {
        for(int i = width; i >= 1; --i)
        {
            issueItem(ctrl->settings.getCurrentPage() + from + i, gTrue, timeOffset);
        }     
    }
}

void CPDFView::issueLinks(CPageInfo *page)
{
    if (NULL == page->links) return;
    int count = page->links->getNumLinks();
    PV_LOGPRINTF("Link count %d in page %d", count, page->pageNumber);
    for(int i = 0; i < count; ++i)
    {
        issueLink(page->links->getLink(i));
    }
}

void CPDFView::issueLink(Link * p)
{
    if (NULL == p) return;
    int pn = getLinkPage(p->getAction());
    if (pn >= 1)
    {
        PV_LOGPRINTF("Issue page %d", pn);
        issueItem(pn);
        return;
    }
    PV_LOGPRINTF("Invalid link page number %d!", pn);
}

int CPDFView::getLinkPage(LinkAction *action)
{
    if (NULL == action) return -1;
    LinkActionKind kind;
    LinkDest *dest = NULL;
    UGooString *namedDest = NULL;
  
    switch (kind = action->getKind()) 
    {
    case actionGoTo:
        dest = ((LinkGoTo *)action)->getDest();
        namedDest = ((LinkGoTo *)action)->getNamedDest();
        if (dest && dest->isPageRef()) 
        {
            Ref pageRef = dest->getPageRef();
            return ctrl->core->getPDFDoc()->findPage(pageRef.num, pageRef.gen);
        } 
        else if (namedDest)
        {
            dest = ctrl->core->getPDFDoc()->findDest(namedDest);
            if (dest)
            {
                Ref pageRef = dest->getPageRef();
                return ctrl->core->getPDFDoc()->findPage(pageRef.num, pageRef.gen);
            }                
        }
        break;
    default:
        break;        
    }
    return -1;
}

//////////////////////////////////////////////////////////////////////////
// issue new requests
// it's usually called when page is displayed
void CPDFView::issueNewRequests()
{
    CPageInfo * page = ctrl->pageList.getPage(ctrl->settings.getCurrentPage());
    PV_LOGPRINTF("Current page %d %p\n", ctrl->settings.getCurrentPage(), page);
    ctrl->thread.clearTodoList();
    if (page)
    {
        if (page->links)
        {
            int count = page->links->getNumLinks();
            if (count <= 0)
            {
                issuePrevNextPages(RenderWidth);
            }
            else
            {
                // the new pre-render scheme:
                // * pre-render the next/prev 1		    
                // * pre-render the hyperlink
                // * pre-render the left next/prev 5
                issuePrevNextPages(RenderWidth - RenderFrom, RenderFrom);
                issueLinks(page);
                issuePrevNextPages(RenderFrom, 0, TimeOffset);      
            }
        }
        else
        {
            issuePrevNextPages(RenderWidth);
        }
    }
    else
    {
        issuePrevNextPages(RenderWidth);
    }
    ctrl->thread.signal();
}


void CPDFView::ipcCbk(gpointer client_data, gint * fd,
                      GdkInputCondition id)
{
    CPDFView *viewer = (CPDFView *)client_data;
    if (viewer->bAuth) return; // in password dialog

    if (viewer->isPortraitView())
    {
        portraitView.ipcCbk(fd, id);
    }
    else if (viewer->isLandscapeView())
    {
        landscapeView.ipcCbk(fd, id);
    }
}

//////////////////////////////////////////////////////////////////////////
// rotate current page
// if zoom factor is not zoomPage, Should adjust current screenX and screenY
// steps:
// 1. get center point of intersection with screen
// 2. calculate the new screenx and screeny from center point and angle
// input parameter angle is a clock wise based incremental value
// it may change state 
void CPDFView::rotatePageMode(const int angle)
{
    // get page info
    CPageInfo * page = ctrl->pageList.getPage(ctrl->settings.getCurrentPage());

    // calculate intersection
    rectangle rect; rect.left = 0; rect.top = 0; 
    rect.right = ctrl->settings.getScreenWidth();
    rect.bottom = ctrl->settings.getScreenHeight();
    if (ctrl->settings.getScreenX() > 0) rect.left = ctrl->settings.getScreenX();
    if (ctrl->settings.getScreenY() > 0) rect.top = ctrl->settings.getScreenY();
    if (rect.right > ctrl->settings.getScreenX() + page->bitmap->getWidth())
    {
        rect.right = ctrl->settings.getScreenX() + page->bitmap->getWidth();
    }
    if (rect.bottom > ctrl->settings.getScreenY() + page->bitmap->getHeight())
    {
        rect.bottom = ctrl->settings.getScreenY() + page->bitmap->getHeight();
    }
    point center;
    center.x = ((rect.left + rect.right) >> 1);
    center.y = ((rect.top + rect.bottom) >> 1);
    int distX = 0, distY = 0;
    
    // calculate the new position 
    if (angle == 90)
    {
        distX = center.x - ctrl->settings.getScreenX();
        distY = page->bitmap->getHeight() - center.y + ctrl->settings.getScreenY();
        ctrl->settings.setScreenX(center.x - distY);
        ctrl->settings.setScreenY(center.y - distX);
    }
    else if (angle == -90)
    {
        distX = page->bitmap->getWidth() - center.x + ctrl->settings.getScreenX();
        distY = center.y - ctrl->settings.getScreenY();
        ctrl->settings.setScreenX(center.x - distY);
        ctrl->settings.setScreenY(center.y - distX);
    }
    int rot = angle + ctrl->settings.getRotate();
    if (rot < 0) rot += 360;
    else if (rot >= 360) rot -= 360;
    
    ctrl->settings.setRotate(rot);
    displayPage(ctrl->settings.getCurrentPage());
}

void CPDFView::rotateContinousMode(const int angle)
{
    int rot = ctrl->settings.getRotate();
    rot += angle;
    if (rot < 0) rot += 360;
    else if (rot >= 360) rot -= 360;
    ctrl->settings.setRotate(rot);
    
    // switch mode now. not enough to only switch x & y
    if (ctrl->settings.getRotate() == 270 || ctrl->settings.getRotate() == 90)
    {
        int x = ctrl->settings.getScreenX();
        ctrl->settings.setScreenX(ctrl->settings.getScreenY());
        ctrl->settings.setScreenY(x);
    }
    else 
    {
        int x = ctrl->settings.getScreenX();
        ctrl->settings.setScreenX(ctrl->settings.getScreenY());
        ctrl->settings.setScreenY(x);
    }

    // when rotate, disable zoomback
    ctrl->settings.disableZoomBack();

    initializeViews();
    displayPage(ctrl->settings.getCurrentPage(), ScrollToPosition);
}


void CPDFView::refreshDeviceScreen(eDmQuality quality, GBool bOff)
{
    dmDisplay(dmCmdPriorUrgent, quality);
    if (bOff)
    {
        ctrl->channels.busy_off();
    }
}

void CPDFView::cvtDevToUser(CPageInfo *page, const int offsetX, const int offsetY, double & ux, double & uy)
{
    ux = page->ictm[0] * (offsetX) + 
         page->ictm[2] * (offsetY) + page->ictm[4];
    uy = page->ictm[1] * (offsetX) + 
         page->ictm[3] * (offsetY) + page->ictm[5];    
}

GBool CPDFView::doLink(const int pn, int offsetX, int offsetY)
{
    CPageInfo * page = ctrl->pageList.getPage(pn);
    if (NULL == page || NULL == page->links) return gFalse;
    double ux = 0.0, uy = 0.0;
    cvtDevToUser(page, offsetX, offsetY, ux, uy);
    LinkAction * action = page->links->find(ux, uy);
    if (NULL == action) return gFalse;
    return doAction(action);
}

GBool CPDFView::doAction(LinkAction * action)
{
    PV_LOGPRINTF("Do some action!");
    LinkActionKind kind;
    LinkDest *dest = NULL;
    UGooString *namedDest = NULL;
  
    switch (kind = action->getKind()) 
    {
    case actionGoTo:
        ctrl->channels.busy_blink();
        PV_LOGPRINTF("actionGoTo");
        dest = ((LinkGoTo *)action)->getDest();
        namedDest = ((LinkGoTo *)action)->getNamedDest();
        if (dest && dest->isPageRef()) 
        {
            Ref pageRef = dest->getPageRef();
            int pageNumber = ctrl->core->getPDFDoc()->findPage(pageRef.num, pageRef.gen);
            PV_LOGPRINTF("Goto Page %d", pageNumber);
            displayPage(pageNumber);
            return gTrue;
        } 
        else if (namedDest)
        {
            dest = ctrl->core->getPDFDoc()->findDest(namedDest);
            if (dest)
            {
                Ref pageRef = dest->getPageRef();
                int pageNumber = ctrl->core->getPDFDoc()->findPage(pageRef.num, pageRef.gen);
                PV_LOGPRINTF("Goto Page %d", pageNumber);
                displayPage(pageNumber);
                return gTrue;
            }                
        }
        break;
    case actionLaunch:
        PV_LOGPRINTF("actionLaunch!");
        break;
                
    case  actionNamed:
        PV_LOGPRINTF("actionNamed!");     
        break;
    case  actionURI:
        PV_LOGPRINTF("actionURI!");     
        break;
    case actionMovie:
        PV_LOGPRINTF("actionMovie!");     
        break;
    default:
        PV_LOGPRINTF("unknown!");        
        break;
    }
    return gFalse;
}

//////////////////////////////////////////////////////////////////////////
// password authentication
GooString *CPDFView::getPassword() 
{
    // printf("CPDFView::getPassword\n");
    dialogDone = -1;
    bAuth = gTrue;
    GtkAuthDialog dlg;
    gtk_window_fullscreen(GTK_WINDOW(drawArea));
    if (!dlg.popupAuthDialog(this))
    {
        if (password)
        {
            delete password; 
            password = NULL;
        }            
    }
    else
    {
        password = new GooString(dlg.getPassword());
    }        
    return password;
}

void CPDFView::quit()
{
    // close
    onClose();

    // turn on busy indicator
    ctrl->channels.busy_blink();
    ctrl->app->quit(!bAuth);
}


