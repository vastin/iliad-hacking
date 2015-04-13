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

#ifndef PDFPORTRAITVIEW_H_
#define PDFPORTRAITVIEW_H_

#include "PageInfo.h"
#include "LayoutMgr.h"
#include "DisplayList.h"
#include "Settings.h"
#include <gtk/gtk.h>
//////////////////////////////////////////////////////////////////////////
// CPDFPortraitView
// In Portrait View mode, we display page with rotate angle = 0
// The Portrait view could have page mode and continouse mode
//////////////////////////////////////////////////////////////////////////
class CPDFView;
class CPDFPortraitView
{
private:
    CPDFView *      view;
    CLayoutMgr      layout;
    CDisplayList    list;
    int             nStart, nEnd;   // display pages range [nStart,nEnd]
    int             nOffset;        // in continous mode, pagebar_offset
    GBool           bReady;         // ready to receive user input

public:
    CPDFPortraitView(void);
    ~CPDFPortraitView(void);

private:
    void onPageRendered(const int pageNumber);
    void updateToolbar(CPageInfo *page);
    void updatePagebar();
    void zoomFromRect(int left, int top, int right, int bottom);
    void zoomToRect(int left, int top, int right, int bottom);
    void switchMode();
    void drawScribble(const int x, const int y, const int pn);
    void getPagesRange(int & start, int & end, const int pageNumber, const ContinousModeScroll scroll);
    void validatePosition();  
    void setReady(const GBool b = gTrue) { bReady = b; }
    void getPageScreenPosition(const int pn, int & x, int & y);
    void zoomBack();    
    void backward();
    void forward();

    /////////////////////////// for page mode ////////////////////////////
    void zoomFromRectPageMode(rectangle & rect);
    void adjustPositionPageMode(const rectangle & oldRect, double ratio);
    void zoomFitPageMode();
    GBool doLinkPageMode(const int x, const int y);

    ///////////////////////// for continous mode /////////////////////////
    void zoomFromRectContinousMode(rectangle & rect);
    void adjustPositionContinousMode(const rectangle & oldRect, double ratio);
    void zoomFitContinousMode();
    GBool doLinkContinousMode(const int x, const int y);

    ////////////////////////// page display /////////////////////////////////
    // displayPage -> displayPages -> drawPages -> drawPageImage
    void displayPages(const int start, const int end);
    void drawPages(const int start, const int end);
    void drawPageImage(CPageInfo *page);

public:
    void setView(CPDFView *p) { view = p; }
    void init();
    void clear();
    void displayPage(const int pageNumber, const ContinousModeScroll mode = ScrollToPage);
    
    void ipcCbk(gint * fd,    GdkInputCondition id);
    int  hitTest(const int x, const int y, int & ox, int & oy);
    GBool isReady() const { return bReady; }

    ////////////////////// gtk event handler /////////////////////////////
    gboolean buttonPressCbk(GdkEventButton *);
    gboolean buttonReleaseCbk(GdkEventButton *);
    gboolean motionNotifyCbk(GdkEventMotion *);
    gboolean keyPressCbk(GdkEventKey *);
    gboolean deleteCbk(GdkEvent *);

};


#endif
