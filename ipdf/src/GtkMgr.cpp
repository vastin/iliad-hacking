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
 
#include "GtkMgr.h"
#include "goo/gmem.h"
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <liberdm/display.h>
#include <gdk/gdkx.h>
#include <string.h>
#include "config.h"
#define defaultRGBCube  5


GtkMgr::GtkMgr(void)
: window(NULL)
, gc(NULL)
, pixmap(NULL)
, visual(NULL)
, bkColor(-1)
, fb_mem(NULL)
, fb(0)
{
    fb = open ("/dev/fb0", O_RDWR);
    fb_mem = (unsigned char *)mmap(NULL, SCREEN_WIDTH * CLIENT_AREA, PROT_READ|PROT_WRITE,MAP_SHARED,fb,0);
}

GtkMgr::~GtkMgr(void)
{
    munmap(fb_mem, SCREEN_WIDTH * CLIENT_AREA);
    close(fb);
}

void GtkMgr::setupWindow()
{
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), PACKAGE " " VERSION );
    gtk_window_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(window), 0);
    gtk_widget_set_size_request(GTK_WIDGET(window), SCREEN_WIDTH, SCREEN_HEIGHT - TOOLBAR_HEIGTH - PAGEBAR_HEIGHT);
    gtk_window_set_modal(GTK_WINDOW(window), FALSE);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    gtk_widget_set_events (window,
        GDK_EXPOSURE_MASK | GDK_BUTTON_MOTION_MASK |
        GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK |
        GDK_KEY_PRESS_MASK | GDK_KEY_RELEASE_MASK);

    // disable double buffering
    GTK_WIDGET_UNSET_FLAGS(window, GTK_DOUBLE_BUFFERED);        
    
    gtk_widget_show(window);

    pixmap = gdk_pixmap_new (window->window,
        window->allocation.width,
        window->allocation.height, -1);
    gc = gdk_gc_new (pixmap);
    lineColor.pixel = gdk_rgb_xpixel_from_rgb (0x0);
    gdk_gc_set_foreground(gc, &lineColor);
    
    // init
    display = gdk_x11_display_get_xdisplay(gdk_display_get_default());
    screenNum = DefaultScreen(display);
    visual = DefaultVisual(display, screenNum);

    // hide cursor
    Pixmap bm_no;
    Colormap cmap;
    Cursor no_ptr;
    XColor black, dummy;
    static char bm_no_data[] = {0, 0, 0, 0, 0, 0, 0, 0};

    cmap = DefaultColormap(display, DefaultScreen(display));
    XAllocNamedColor(display, cmap, "black", &black, &dummy);
    bm_no = XCreateBitmapFromData(display, GDK_WINDOW_XWINDOW(window->window), bm_no_data, 8, 8);
    no_ptr = XCreatePixmapCursor(display, bm_no, bm_no, &black, &black, 0, 0);

    XDefineCursor(display, GDK_WINDOW_XWINDOW(window->window), no_ptr);
    XFreeCursor(display, no_ptr);
    if (bm_no != None)
    {
        XFreePixmap(display, bm_no);
    }        
    XFreeColors(display, cmap, &black.pixel, 1, 0);
}

void GtkMgr::dfb_copyImage(unsigned char *mem, SplashBitmap * bitmap, 
                              int xSrc , int ySrc , 
                              int xDest , int yDest , 
                              int width , int height )
{
    // PV_LOGPRINTF("xSrc %d ySrc %d xDest %d yDest %d width %d height %d\n", xSrc, ySrc, 
    //    xDest, yDest, width, height);
    if (width < 0) 
    {
        width = bitmap->getWidth();
    }	    
    if (height < 0) 
    {
        height = bitmap->getHeight();
    }	    

    if (xDest < 0) 
    {
        xSrc -= xDest; width += xDest; xDest = 0; 
    }
    else if (xDest > SCREEN_WIDTH) 
    {
        return;
    }
    
    if (yDest < 0) 
    { 
        ySrc -= yDest; height += yDest; yDest = 0; 
    }
    else if (yDest > CLIENT_AREA) 
    {
        return;
    }

    if (xDest + width >= SCREEN_WIDTH) { width = SCREEN_WIDTH - xDest; }
    if (yDest + height >= CLIENT_AREA) { height = CLIENT_AREA - yDest; }

    unsigned char *p = (unsigned char *)(bitmap->getDataPtr() + ySrc * bitmap->getRowSize() + xSrc);
    mem = mem + yDest * SCREEN_WIDTH + xDest;

    // PV_LOGPRINTF("xSrc %d ySrc %d xDest %d yDest %d width %d height %d\n", xSrc, ySrc, 
    //    xDest, yDest, width, height);

    for(int i = 0; i < height; ++i)
    {
        memcpy(mem, p, width);
        p += bitmap->getRowSize();
        mem += SCREEN_WIDTH;
    }
}

void GtkMgr::clearBkground()
{
    memset(fb_mem, bkColor, SCREEN_WIDTH * CLIENT_AREA);
}

void GtkMgr::drawImage(SplashBitmap * bitmap, 
                          int xSrc, int ySrc, 
                          int xDest, int yDest, 
                          int width , int height, 
                          GBool bDrawBorder)
{
    if (bitmap) 
    {
        if (width < 0) width = bitmap->getWidth();
        if (height < 0) height = bitmap->getHeight();

        dfb_copyImage(fb_mem, bitmap, xSrc, ySrc, xDest, yDest, width, height);

        if (bDrawBorder)
        {
            // line 
            XSetForeground(display, GDK_GC_XGC(gc), 0);
            XSetLineAttributes(display, GDK_GC_XGC(gc), 1, LineSolid, CapProjecting, JoinMiter);
            XDrawRectangle(display, GDK_WINDOW_XWINDOW(window->window), GDK_GC_XGC(gc), 
                xDest, yDest, width, height);
        }
    } 
}

void GtkMgr::setLineColor(const int color)
{
    XSetForeground(display, GDK_GC_XGC(gc), color);
}

void GtkMgr::setLineAttributes(const int width, const int style)
{
    XSetLineAttributes(display, GDK_GC_XGC(gc), width, style, CapProjecting, JoinMiter);
}

void GtkMgr::drawLine(const int x1, const int y1, const int x2, const int y2)
{
    XDrawLine(display, GDK_WINDOW_XWINDOW(window->window), GDK_GC_XGC(gc), x1, y1, x2, y2);
}

