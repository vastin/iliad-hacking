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
 
#ifndef PDFVIEW_H_
#define PDFVIEW_H_

#include <liberdm/erdm.h>
#include "poppler/Link.h"
#include "PageInfo.h"
#include "Settings.h"
#include <gtk/gtk.h>

struct rectangle;
struct Controller;
class CPageInfo;

//////////////////////////////////////////////////////////////////////////
// PDF view class
// 1. serves as a proxy, it will send command to proper class for execution
// 2. maintain settings
// 3. provide common methods for other views
//
// CPDFView ---> CPDFLandscapeView
//         |
//         |---> CPDFPortraitView
//         | 
//         |---> CPDFNewView (add other views if needed)
//////////////////////////////////////////////////////////////////////////
class CPDFView
{
public:
    Controller *    ctrl;
private:
    GtkWidget *     drawArea;
    
    ///////////////////////// password //////////////////////////////
private:
    GBool           bAuth;
    GooString *     password;
    int             dialogDone;

public:
    GooString *     getPassword();
    GtkWidget *     getWindow() { return drawArea; }

    ////////////////////////// zoom and pan /////////////////////////
public:
    rectangle   zoomRect;   // used by zoom in/out 
    point       panPt;      // by pan
    void        initPan(const int x, const int y);
    void        initZoomRect(const int x, const int y);
    void        adjustZoomRect(const int x, const int y);
    GBool       isSmallOffset(const int x, const int y);
    double      calRatio(rectangle & rect, GBool bZoomin = gTrue);
    int         lessThreshold(const rectangle & rect);

    /////////////////////////// hyper link ///////////////////////////
public:
    GBool       doLink(const int pn, int offsetX, int offsetY);
    GBool       doAction(LinkAction * action);
    void        cvtDevToUser(CPageInfo *page, const int offsetX, const int offsetY, double & ux, double & uy);
    int         getLinkPage(LinkAction *action);

    //////////////// basic pre-render issue function /////////////////
    // the other views should call the following function to 
    // issue new request. 
public:
    void        issueNewRequests();
    void        issueItem(const int pn, GBool bHead = gTrue, const int time = 0);
    void        issuePrevNextPages(const int offset, const int from = 0, const int time = 0);
    void        issueLinks(CPageInfo *page);
    void        issueLink(Link * p);
    
public:
    ////////////////////// toolbar states /////////////////////////////
    GBool       isPanEnable();
    GBool       isZoomInEnable();
    GBool       isZoomOutEnable();
    GBool       isScribbleEnable();
    GBool       isEraseEnable();

    ///////////////////////// view & mode ////////////////////////////
    GBool       isPageMode(); 
    GBool       isContinousMode();
    void        setPageMode();
    void        setContinousMode();
    GBool       isPortraitView();
    GBool       isLandscapeView();

public:
    CPDFView(GtkWidget * win, Controller * c );
    ~CPDFView(void);

    /////////////////////// install event handler //////////////////////
    void installEventHandler(GtkWidget * win);

    /////////////////// by other views ////////////////////////////////
    void initializeViews();
    void rotateContinousMode(const int angle);    // clock wise + 
    void rotatePageMode(const int angle);
    void refreshDeviceScreen(eDmQuality quality, GBool bOff = gTrue);
    void displayPage(const int pageNumber, const ContinousModeScroll mode = ScrollToPage);     // check and draw
    void onClose();
    void quit();

    /////////////////// callback functions /////////////////////////////
    static void ipcCbk(gpointer client_data, gint * file_descriptor,
                      GdkInputCondition id);

    ////////////////////// gtk event handler ////////////////////////////
    static gboolean exposeCbk(GtkWidget *, GdkEventExpose *, gpointer);
    static gboolean buttonPressCbk(GtkWidget *, GdkEventButton *, gpointer);
    static gboolean buttonReleaseCbk(GtkWidget *, GdkEventButton *, gpointer);
    static gboolean motionNotifyCbk(GtkWidget *, GdkEventMotion *, gpointer);
    static gboolean keyPressCbk(GtkWidget *, GdkEventKey *, gpointer);
    static gboolean deleteCbk(GtkWidget *, GdkEvent *, gpointer);
};

#endif

