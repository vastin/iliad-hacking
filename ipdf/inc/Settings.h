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

#ifndef SETTINGS_H_
#define SETTINGS_H_
#include "PageInfo.h"
#include "goo/gtypes.h"
#include <liberdm/display.h>


//////////////////////////////////////////////////////////////////////////
// Define all settings here
//////////////////////////////////////////////////////////////////////////
#define ZoomPage        -1
#define ZoomWidth       -2
#define DefZoom         100
#define ZoomMax         350     // should be larger or equal to all ZoomMax
#define ZoomMin         25
#define InvalidPage     -1
#define InvalidZoom     -100
#define DefDPI          72
#define RenderNext      1
#define RenderPrev      -1
#define RenderWidth     5
#define RenderFrom      1       // used by new pre-render scheme
#define TimeOffset      2       // used to protect next/prev pages
#define JumpWidth       5       // long press
#define RemovePages     2       // if memory is not enough, how many pages should be removed
#define SmallOffset     5
#define RectThreshold   25
#define ProtectWidth    5

#define PageBreak       1
#define PageRepeat      20
#define Margin          -5

#define SmallWidth      1
#define SmallHeight     2
#define SmallNone       0

// define max zoom for these pages, memory memory memory memory
#define A0ZoomMax       100
#define A1ZoomMax       100
#define A2ZoomMax       100
#define A3ZoomMax       150
#define A4ZoomMax       200
#define A5ZoomMax       300
#define A6ZoomMax       300
#define A7ZoomMax       300
#define A8ZoomMax       350
#define A9ZoomMax       350

// mode
#define ModeContinous   1
#define ModePage        2
#define ModeNewspaper   4       // this two decide pre-render algorithm
#define ModeA4          8       // for A4 pages

// IPC buffer
#define IPC_BUF_MAX     1024

// Maximum amount of memory to use, set with setrlimit(2)
// Since the render thread has no nice way to limit its memory usage,
// we need to provide this safety net. Otherwise the OOM killer might
// kill some processes we cannot avoid to loose.
#define MAXIMUM_MEMORY_LIMIT    (50 * 1024 * 1024)

// Debug macro
#define HISTORY_DEBUG   0

// The maximum length of history list
#define HISTORY_MAX_LENGTH  15

enum ContinousModeScroll
{
    ScrollToPage        = 0,    // show page at top
    ScrollToNextPage    = 1,    // scroll next
    ScrollToPrevPage    = 2,
    ScrollToNextPages   = 3,    // 5 screen
    ScrollToPrevPages   = 4,    // 5 screen
    ScrollToPosition    = 5,    // scroll to specific position
    ScrollRedraw        = 6,    // do not scroll, only redraw
};

// It's only necessary to maintain the position and page number.
// If the zoom factor or rotation is changed, ipdf will return
// to the old page with new zoom factor and new rotation.
struct HistoryItem
{
    int     x;
    int     y;
    int     pageNumber;
    double  zoom;
    HistoryItem();
    ~HistoryItem();    
#if (HISTORY_DEBUG)
    static int count;
#endif    
};
typedef HistoryItem * HistoryItemPtr;

//////////////////////////////////////////////////////////////////////////
// CSettings store all settings used by application
// Change attributes from public to private, so that all caller should
// explicitly call these methods, because according to requirement, we need 
// record settings change
//////////////////////////////////////////////////////////////////////////
class CSettings
{
private:
    double      zoom;
    int         rotate;
    int         screenX;        // depends on view mode
    int         screenY;        // depends on view mode
    int         drawAreaWidth;  // usually it's a constant value
    int         drawAreaHeight; // could be change in run time, for full screen
    int         currentPage;
    int         renderDir;      // pre-render direction
    int         mode;           // could be page-page or continous
    int         zbState;        // zoom back state is defined by user clicks count
    GBool       enableScb;      // is scribble enabled
    GooList     backward;       // backward history list.
    GooList     forward;        // forward history list.
    HistoryItemPtr
                currentItem;    // current page item.
    HistoryItem record;         // for zoomback

private:
    void        clearHistory();

public:
    CSettings(void);
    ~CSettings(void);

    // zoom back    
    void    setNewZoom(const double z);
    GBool   zoomBack(); 
    GBool   canZoomBack();
    int     getZoomBackState() const { return zbState; }
    void    disableZoomBack();
    double  getZoom() const { return zoom; }
    void    setZoom(const double z) { zoom = z; }

    // persistence
    void load(const char * manifest);
    void save(const char *  manifest);

    // rotate
    int getRotate() const { return rotate; }
    void setRotate(const int r) { rotate = r; }

    // position & size
    void getScreenRect(rectangle & rect);
    int  getScreenX() const { return screenX; }
    int  getScreenY() const { return screenY; }
    int  getScreenWidth() const { return drawAreaWidth; }
    int  getScreenHeight() const { return drawAreaHeight; }
    void setScreenX(const int x) { screenX = x; }
    void setScreenY(const int y) { screenY = y; }
    void moveScreenX(const int d) { screenX += d; }
    void moveScreenY(const int d) { screenY += d; }
    void setScreenPosition(const int , const int);
    void setScreenSize(const int, const int);
    void setScreenRect(const rectangle & rect);
    
    // page
    int getCurrentPage() const { return currentPage; }
    void setCurrentPage(const int p);

    // render direction
    int getRenderDir() const { return renderDir; }
    void setRenderDir(const int d) { renderDir =d ;  }

    // mode
    int getMode() const { return mode; }
    void setMode(const int m) { mode = m; }
    
    // is scribble enabled. Scribble can only be disabled by manifest.
    GBool isScribbleEnabled() const { return enableScb; }

    // history list
    void addNewItem(HistoryItemPtr ptr);
    void addNewItem(const int page);
    HistoryItemPtr goBackward();
    HistoryItemPtr goForward();
    GBool canGoBack();
    GBool canGoForward();

#if (HISTORY_DEBUG)
    void  dumpHistory();
#endif
};

#endif
