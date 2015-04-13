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
 
#ifndef PAGEINFO_H_
#define PAGEINFO_H_

#include <X11/Xlib.h>
#include <gdk/gdkx.h>

#include "splash/SplashBitmap.h"
#include "poppler/Object.h"
#include "poppler/SplashOutputDev.h"
#include "goo/GooList.h"
#include "poppler/Link.h"
#include "poppler/TextOutputDev.h"
#include "poppler/Gfx.h"
#include <signal.h>
#include "utils.h"

//////////////////////////////////////////////////////////////////////////
// pageinfo stores all information used to 
// 1. render a pdf page
// 2. display a pdf page
// 3. calculat a pdf page (A4,..., A0)
// 4. something related to state
//////////////////////////////////////////////////////////////////////////
class CPageInfo
{
public:
    CPageInfo(void);
    ~CPageInfo(void);

public:
    double          pageDx;         // getPageCropWidth
    double          pageDy;         // getPageCropHeight
    int             pageRotate;     // getPageRotate
    double          pageZoom;       // always >0, real zoom
    double          hDPI;           // out
    double          vDPI;           // out
    int             rotate;         // rotate, global setting
    int             pageNumber;
    int             timestamp;      // pre-rendering
    int             attributes;     // used to protect page

    // render result
    SplashBitmap *  bitmap;         // by SplashOutputDev
    Links *         links;
    TextPage *      text;	
    double          ctm[6];         // for hyperlink
    double          ictm[6];        // for hyperlink

public:
    // methods
    void    initialize();
    void    clear();
    GBool   isSame(const int pn, const double zoom, const int rotate);
    double  calZoom();
};
typedef CPageInfo * PageInfoPtr;

//////////////////////////////////////////////////////////////////////////
// page list management. It should be thread-safe
//////////////////////////////////////////////////////////////////////////
class TodoItem;
class PageList
{
private:
    pthread_mutex_t     mutex;  // protect the list
    PageInfoPtr *       list;
    int                 length;
    
    void    initialize();

public:
    PageList();
    ~PageList();
    
    void        reset(const int size);
    void        clear();
    void        addPage(CPageInfo * info);    
    void        removeOldestOne(const int center);
    CPageInfo * getPage(const int number);
    CPageInfo * checkPage(const CPageInfo & info);
    CPageInfo * checkPage(const TodoItem * pItem);
    void        dump();     // for debug
};

//////////////////////////////////////////////////////////////////////////
// todo list management. It should be thread-safe
//////////////////////////////////////////////////////////////////////////
class TodoItem
{
public:
    TodoItem();
    ~TodoItem();
    TodoItem(const TodoItem & right);
    
public:
    double  zoom;
    int     rotate;
    int     pageNumber;
    int     timeStamp;          // time stamp when request is issued
};

class TodoList
{
private:
    pthread_mutex_t     mutex;  // protect the list
    GooList *           list;

    void    initialize();
    void    removeDup(TodoItem * item); // remove duplicated items by time stamp

public:
    TodoList();
    ~TodoList();
    
    void        clear();
    void        pushItem(TodoItem * item, GBool bHead = gFalse, GBool bRemoveDup = gTrue);    
    TodoItem *  popItem();
};

#endif
