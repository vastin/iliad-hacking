#ifndef GTKMGR_H_
#define GTKMGR_H_

#include <X11/Xlib.h>
#include "splash/SplashBitmap.h"
#include "splash/SplashTypes.h"
#include <gtk/gtk.h> 


class GtkMgr
{
public:
    GtkWidget *     window;
    GdkGC *         gc;
    GdkDrawable *   pixmap;
    Display *       display;
    Visual *        visual;
    int             screenNum;

    //////////////////////////////  color ////////////////////////////////    
    int             depth;     
    GdkColor        lineColor;
    int             bkColor;
    
    /////////////////////////// framebuffer //////////////////////////////
    unsigned char*  fb_mem;
    int             fb;
    void            dfb_copyImage(unsigned char *mem, SplashBitmap * bitmap, 
                        int xSrc = 0, int ySrc = 0, 
                        int xDest = 0, int yDest = 0, 
                        int width = -1, int height = -1);

public:
    GtkMgr(void);
    ~GtkMgr(void);

    void            setupWindow();
    void            clearBkground();
    void            drawImage(SplashBitmap * bitmap, 
                        int xSrc = 0, int ySrc = 0, 
                        int xDest = 0, int yDest = 0, 
                        int width = -1, int height = -1, 
                        GBool bDrawBorder = gTrue);
    void            setLineColor(const int color);
    void            setLineAttributes(const int width, const int style = LineSolid);
    void            drawLine(const int x1, const int y1, const int x2, const int y2);
};
#endif

