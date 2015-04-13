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
 
#include "ScribbleMgr.h"
#include "GtkMgr.h"
#include "controller.h"
#include "Settings.h"
#include "PDFViewerLog.h"
#include "utils.h"
#include <math.h>

static const double e = 0.001;

CScribbleMgr::CScribbleMgr(void)
: doc(NULL)
, page(NULL)
, stroke(NULL)
, pageNumber(-1)
, bErased(gFalse)
, bScbDirty(gFalse)
{
    scb_init();
}

CScribbleMgr::~CScribbleMgr(void)
{
    scb_uninit();
}

//////////////////////////////////////////////////////////////////////////
// open scribble data file
void CScribbleMgr::open(const char *dirName)
{
    close();
    
    // construct scribble file name
    if (NULL == dirName) return;
    strncpy(path.scbname, dirName, SCB_MAX_PATH);
    char *pos = strrchr(path.scbname, '/');
    strcpy(++pos, "scribble.irx");      
    pos += strlen("scribble.irx");
    *pos = 0;

    // open file
    doc = scb_doc_make_sure_exist(&path);
}

void CScribbleMgr::close(GBool bSave)
{
    if (doc && bSave && isScbDirty())
    {
        PV_SCBPRINTF("Save scribble now!");
        scb_doc_save(doc);
    }
    scb_doc_free(doc);
    doc = NULL;
    memset(&path, 0, sizeof(path));
    page = NULL;
    stroke = NULL;
    setScbDirty(gFalse);
}

void CScribbleMgr::setScbDirty(const GBool b)
{ 
    bScbDirty = b; 
}

void CScribbleMgr::save()
{
    if (doc && isScbDirty())
    {
        PV_SCBPRINTF("Save scribble now!");
        scb_doc_save(doc);
    }
    setScbDirty(gFalse);
}

void CScribbleMgr::generatePageId(ScbPageIdPtr pid, const int pn)
{
    snprintf(pid->id, SCB_MAX_PAGEID_LEN, "%d", pn);
}

void CScribbleMgr::beginStroke(const int pn, Controller * ctrl, 
                               const int px, const int py,
                               const int x, const int y,
                               const int ox, const int oy)
{
    PV_SCBPRINTF("Create new stroke!");

    // record current page number and page position
    pageNumber = pn;
    pagePoint.x = px; 
    pagePoint.y = py;

    // check page
    generatePageId(&id, pn);
    page = scb_doc_get_page(doc, &id);
    if (NULL == page)
    {
        // does not exist, needs to create a new one
        page = scb_page_new();
        scb_page_set_id(page, &id);
        page->style.orientation = ctrl->settings.getRotate();
    }

    // check stroke
    if (NULL == stroke)
    {
        stroke = scb_stroke_new_with_style(scb_doc_get_current_stroke_style(doc));
        stroke->style.zoom = ctrl->calZoom(ctrl->settings.getCurrentPage());
        ScbStrokesPtr strokes = scb_page_get_strokes(page);
        scb_strokes_add_stroke(strokes, stroke);
    }

    // stroke point data, in offset
    point.x = ox;
    point.y = oy;
    point.pressure = 0;
    scb_stroke_add_point(stroke, &point);
    
    // for drawing
    point.x = x;
    point.y = y;
    scb_stroke_fast_draw_point(stroke, &point);
    
    // set dirty flag now
    setScbDirty();
}

void CScribbleMgr::addPoint(const int pn, Controller * ctrl, 
                            const int px, const int py,
                            const int x, const int y, 
                            const int ox, const int oy)
{
    point.x = ox;
    point.y = oy;
    scb_stroke_add_point(stroke, &point);
    
    point.x = x;
    point.y = y;
    scb_stroke_fast_draw_point(stroke, &point);
    
    // set dirty flag now
    setScbDirty();
}

void CScribbleMgr::endStroke(const int pn, Controller * ctrl, 
                             const int px, const int py,
                             const int x, const int y, 
                             const int ox, const int oy)
{
    PV_SCBPRINTF("End stroke!");
    
    // stroke data
    point.x = ox;
    point.y = oy;
    scb_stroke_add_point(stroke, &point);
    
    // drawing
    point.x = x;
    point.y = y;
    scb_stroke_fast_draw_point_done(stroke, &point);
    
    // draw in display
    drawStroke(stroke, ctrl, px, py);

    // add & reset
    scb_doc_add_page(doc, page);
    page = NULL;
    stroke = NULL;
    pageNumber = -1;
    
    // set dirty flag now
    setScbDirty();
}

// end stroke without adding point
void CScribbleMgr::endStroke(Controller * ctrl, 
                const int px, const int py,
                const int x, const int y)
{
    // drawing
    point.x = x;
    point.y = y;
    scb_stroke_fast_draw_point_done(stroke, &point);
    
    // draw
    drawStroke(stroke, ctrl, px, py);
    
    // add & reset
    scb_doc_add_page(doc, page);
    page = NULL;
    stroke = NULL;
    pageNumber = -1;
}

void CScribbleMgr::onScribblePress(const int pn, Controller * ctrl, 
                                 const int px, const int py, 
                                 const int x, const int y, 
                                 const int ox, const int oy)
{
    if (pn > 0 && page && stroke)
    {
        addPoint(pn, ctrl, px, py,  x, y, ox, oy);
        return;
    }
    
    if (pn > 0 && NULL == page && NULL == stroke)
    {            
        beginStroke(pn, ctrl, px, py, x, y, ox, oy);
    }        
}

//////////////////////////////////////////////////////////////////////////
// 1. move from page to window(out of page)
// 2. move from window(out of page) to page
// 3. move from one page to the other
void CScribbleMgr::onScribbleMove(const int pn, Controller * ctrl,  
                                const int px, const int py,
                                const int x, const int y, 
                                const int ox, const int oy)
{
    // out of page, end stroke now.
    if (pn < 0 && stroke && page)
    {
        // end without adding it
        endStroke(ctrl, pagePoint.x, pagePoint.y, x, y);
        return;
    }

    // re-enter page bound
    if (pn > 0 && stroke == NULL && page == NULL)
    {
        beginStroke(pn, ctrl, px, py, x, y, ox, oy);
        return;
    }

    // from one page to the other, should use old px and py
    if (pn > 0 && pageNumber > 0 && pn != pageNumber && stroke && page)
    {
        endStroke(ctrl, pagePoint.x, pagePoint.y, x, y);
        beginStroke(pn, ctrl, px, py, x, y, ox, oy);
        return;
    }

    // scribble mode
    if (stroke)
    {
        addPoint(pn, ctrl, px, py, x, y, ox, oy);
    }
}

void CScribbleMgr::onScribbleRelease(const int pn, Controller * ctrl, 
                                   const int px, const int py,
                                   const int x, const int y,    
                                   const int ox, const int oy)
{
    if (pn > 0 && stroke && doc && page)
    {
        endStroke(pn, ctrl, px, py, x, y, ox, oy);
    }
}

void CScribbleMgr::setEraseDirty(GBool b)
{
    bErased = b; 
    if (b)
    {
        setScbDirty();
    }        
}     

//////////////////////////////////////////////////////////////////////////
// when erasing, no need to check whether or not go into another page
void CScribbleMgr::onErasePress(const int pn, Controller * ctrl,  
                     const int x, const int y,       // for erase redrawing
                     const int ox, const int oy)
{
    if (pn < 0) return;
    scb_page_erase_init(&eraseCtx);
    generatePageId(&id, pn);
    page = scb_doc_get_page(doc, &id);
    if (NULL == page) return;

    // record
    pageNumber = pn;
    point.x = ox; point.y = oy;
    eraseCtx.zoom = ctrl->calZoom(ctrl->settings.getCurrentPage());
    ScbStrokesPtr strokes = scb_page_erase_hit_test(page, &point, &eraseCtx);
    if (strokes)
    {
        PV_LOGPRINTF("Erase strokes!");
        
        // draw and free
        if (ctrl->settings.getRotate() == 270)
        {
            drawErasedStrokesLandscape(strokes, ctrl, x, y);
        }
        else
        {
            drawErasedStrokesPortrait(strokes, ctrl, x, y);
        }
        
        // set dirty flag
        setEraseDirty();
               
        // free
        scb_strokes_free(strokes);
    }
}

void CScribbleMgr::onEraseMove(const int pn, Controller * ctrl, 
                    const int x, const int y, 
                    const int ox, const int oy)
{
    if (pn < 0) return;
    if (pageNumber != pn)
    {
        pageNumber = pn;
        scb_page_erase_init(&eraseCtx);
        eraseCtx.zoom = ctrl->calZoom(ctrl->settings.getCurrentPage());
    }

    generatePageId(&id, pn);
    page = scb_doc_get_page(doc, &id);
    if (NULL == page) return;
     
    point.x = ox; point.y = oy;

    ScbStrokesPtr strokes = scb_page_erase_hit_test(page, &point, &eraseCtx);
    if (strokes)
    {
        PV_LOGPRINTF("Erase strokes!");
        if (ctrl->settings.getRotate() == 270)
        {
            drawErasedStrokesLandscape(strokes, ctrl, x, y);
        }
        else
        {
            drawErasedStrokesPortrait(strokes, ctrl, x, y);
        }

        // set dirty flag
        setEraseDirty();

        // free 
        scb_strokes_free(strokes);
    }
}

void CScribbleMgr::onEraseRelease(const int pn, Controller * ctrl,
                       const int x, const int y, 
                       const int ox, const int oy)
{
    if (pn < 0 || NULL == page) return;
    point.x = ox; point.y = oy;
    ScbStrokesPtr strokes = scb_page_erase_hit_test(page, &point, &eraseCtx);

    // reset
    page = NULL;
    if (strokes) 
    {
        PV_LOGPRINTF("Erase strokes!");
        if (ctrl->settings.getRotate() == 270)
        {
            drawErasedStrokesLandscape(strokes, ctrl, x, y);
        }
        else
        {
            drawErasedStrokesPortrait(strokes, ctrl, x, y);
        }
        
        // set dirty flag
        setEraseDirty();
        
        // draw and free
        scb_strokes_free(strokes);
    }                
}

//////////////////////////////////////////////////////////////////////////
// x & y is the page start position in screen
void CScribbleMgr::drawScribblePage(const int pn, Controller * ctrl, 
                                    const int x, const int y)
{
#if (PV_PROFILE_ON)
    int t1 = ctrl->getHighTimeStamp();
#endif 

    // page from page id
    generatePageId(&id, pn);
    ScbPagePtr ptr = scb_doc_get_page(doc, &id);
    if (NULL == ptr) 
    {
        PV_LOGPRINTF("page %s not found!", id.id);
        return;
    }

    if (ctrl->settings.getRotate() == 270)
    {
        drawScribblePageLandscape(ptr, ctrl, x, y);
    }
    else
    {
        drawScribblePagePortrait(ptr, ctrl, x, y);
    }

#if (PV_PROFILE_ON)
        int t2 = ctrl->getHighTimeStamp();
        PV_DUMP("Scribble Manager drawScribblePage uses %d\n", t2 - t1);
#endif 
}


int CScribbleMgr::calLineSize(const int penSize, const double strokeZoom, const double dispZoom)
{
    if (fabs(strokeZoom - dispZoom) < e)
    {
        return penSize;
    }
    int ret = (int)(penSize * strokeZoom / dispZoom);
    if (ret < 1) return 1;
    if (ret > 10) return 10;
    return ret;
}

void CScribbleMgr::drawScribblePagePortrait(ScbPagePtr ptr, Controller * ctrl, 
                                 const int x, const int y)
{
    ScbStrokesPtr strokes = scb_page_get_strokes(ptr);
    double z = ctrl->calZoom(ctrl->settings.getCurrentPage());
    if (strokes)
    {
        ScbStrokePtr stroke = NULL;
        GList *ptr = g_list_first(strokes->strokes);
        while (ptr)
        {
            stroke = (ScbStrokePtr)ptr->data;
            if (stroke)
            {
                // point data
                int count  = scb_stroke_get_point_count(stroke);
                ScbPoint * pts = (ScbPoint *)scb_stroke_get_point_data(stroke);

                // select color. 
                ScbColor color;
                scb_dev_color_to_color(&color, stroke->style.color);
                ctrl->gtkMgr.setLineColor(color.pixel);

                // select pen size
                ctrl->gtkMgr.setLineAttributes(
                    calLineSize(stroke->style.penSize, 
                    z, stroke->style.zoom));

                // draw lines now
                if (fabs(z - stroke->style.zoom) < e)
                {
                    int i = 0;
                    int x1 = x + pts[i].x, y1 = y + pts[i].y; ++i;
                    int x2, y2;
                    while (i < count)
                    {
                        x2 = x + pts[i].x, y2 = y + pts[i].y; ++i;
                        ctrl->gtkMgr.drawLine(x1, y1, x2, y2);
                        x1 = x2; y1 = y2;
                    }
                    
                    if (count <= 1)
                    {
                        ctrl->gtkMgr.drawLine(x1, y1, x1, y1);
                    }
                }
                else
                {
                    double ratio = z / stroke->style.zoom;
                    int i = 0;
                    int x1 = x + (int)(ratio * pts[i].x), y1 = y + (int)(ratio * pts[i].y); ++i;
                    int x2, y2;
                    while (i < count)
                    {
                        x2 = x + (int)(ratio * pts[i].x); y2 = y + (int)(ratio * pts[i].y); ++i;
                        ctrl->gtkMgr.drawLine(x1, y1, x2, y2);
                        x1 = x2; y1 = y2;
                    }

                    if (count <= 1)
                    {
                        ctrl->gtkMgr.drawLine(x1, y1, x1, y1);
                    }
                }
            }
            ptr = g_list_next(ptr);
        }
    }
}

void CScribbleMgr::drawScribblePageLandscape(ScbPagePtr ptr, Controller * ctrl, 
                                  const int x, const int y)
{
    ScbStrokesPtr strokes = scb_page_get_strokes(ptr);
    double z = ctrl->calZoom(ctrl->settings.getCurrentPage());
    if (strokes)
    {
        ScbStrokePtr stroke = NULL;
        GList *ptr = g_list_first(strokes->strokes);
        while (ptr)
        {
            stroke = (ScbStrokePtr)ptr->data;
            if (stroke)
            {
                // point data
                int count  = scb_stroke_get_point_count(stroke);
                ScbPoint * pts = (ScbPoint *)scb_stroke_get_point_data(stroke);

                // select color.  
                ScbColor color;
                scb_dev_color_to_color(&color, stroke->style.color);
                ctrl->gtkMgr.setLineColor(color.pixel);

                // select pen size
                ctrl->gtkMgr.setLineAttributes(
                    calLineSize(stroke->style.penSize,
                    z, stroke->style.zoom));

                // draw lines now
                if (fabs(z - stroke->style.zoom) < e)
                {
                    int i = 0;
                    int x1 = y - pts[i].x, y1 = x + pts[i].y; ++i;
                    int x2, y2;
                    while (i < count)
                    {
                        x2 = y - pts[i].x, y2 = x + pts[i].y; ++i;
                        ctrl->gtkMgr.drawLine(y1, x1, y2, x2);  // swap
                        x1 = x2; y1 = y2;
                    }
                    
                    // single point
                    if (1 >= count)
                    {
                        ctrl->gtkMgr.drawLine(y1, x1, y1, x1);
                    }
                }
                else
                {
                    double ratio = z / stroke->style.zoom;
                    int i = 0;
                    int x1 = y - (int)(ratio * pts[i].x), y1 = x + (int)(ratio * pts[i].y); ++i;
                    int x2, y2;
                    while (i < count)
                    {
                        x2 = y - (int)(ratio * pts[i].x), y2 = x + (int)(ratio * pts[i].y); ++i;
                        ctrl->gtkMgr.drawLine(y1, x1, y2, x2);
                        x1 = x2; y1 = y2;
                    }

                    // single point
                    if (1 >= count)
                    {
                        ctrl->gtkMgr.drawLine(y1, x1, y1, x1);
                    }
                }
            }
            ptr = g_list_next(ptr);
        }
    }
}

//////////////////////////////////////////////////////////////////////////
// redraw stroke by driver
void  CScribbleMgr::drawErasedStrokesPortrait(ScbStrokesPtr strokes, Controller * ctrl, 
                const int x, const int y)
{
    double z = ctrl->calZoom(ctrl->settings.getCurrentPage());
    if (strokes)
    {
        ScbStrokePtr stroke = NULL;
        GList *ptr = g_list_first(strokes->strokes);
        while (ptr)
        {
            stroke = (ScbStrokePtr)ptr->data;
            if (stroke)
            {
                // adjust position
                int count  = scb_stroke_get_point_count(stroke);
                ScbPoint * pts = (ScbPoint *)scb_stroke_get_point_data(stroke);
                
                // draw                
                if (fabs(z - stroke->style.zoom) < e)
                {
                    int i = 0;
                    while (i < count)
                    {
                        pts[i].x += x, pts[i].y += y; ++i;
                    }                        
                }
                else
                {
                    double ratio = z / stroke->style.zoom;
                    int i = 0;
                    while (i < count)
                    {
                        pts[i].x = x + (int)(pts[i].x * ratio);
                        pts[i].y = y + (int)(pts[i].y * ratio);
                        ++i;
                    }                        
                }
                stroke->style.color = SCB_DEV_COLOR_WHITE;
                scb_stroke_fast_draw(stroke);
                drawStrokeDirectly(stroke, ctrl);                
            }
            ptr = g_list_next(ptr);
        }
    }
}

void CScribbleMgr::drawErasedStrokesLandscape(ScbStrokesPtr strokes, Controller * ctrl, 
                const int x, const int y)
{
    double z = ctrl->calZoom(ctrl->settings.getCurrentPage());
    if (strokes)
    {
        ScbStrokePtr stroke = NULL;
        GList *ptr = g_list_first(strokes->strokes);
        while (ptr)
        {
            stroke = (ScbStrokePtr)ptr->data;
            if (stroke)
            {
                // adjust position
                int count  = scb_stroke_get_point_count(stroke);
                ScbPoint * pts = (ScbPoint *)scb_stroke_get_point_data(stroke);
                
                // draw                
                if (fabs(z - stroke->style.zoom) < e)
                {
                    int i = 0;
                    while (i < count)
                    {
                        pts[i].x = y - pts[i].x;
                        pts[i].y = x + pts[i].y;
                        swap(pts[i].x, pts[i].y);
                        ++i;
                    }                        
                }
                else
                {
                    double ratio = z / stroke->style.zoom;
                    int i = 0;
                    while (i < count)
                    {
                        pts[i].x = y - (int)(pts[i].x * ratio);
                        pts[i].y = x + (int)(pts[i].y * ratio);
                        swap(pts[i].x, pts[i].y);
                        ++i;
                    }                        
                }
                stroke->style.color = SCB_DEV_COLOR_WHITE;
                scb_stroke_fast_draw(stroke);
                drawStrokeDirectly(stroke, ctrl);
            }
            ptr = g_list_next(ptr);
        }
    }
}

void CScribbleMgr::drawStroke(ScbStrokePtr stroke, Controller * ctrl, 
                const int px, const int py)
{
    if (ctrl->settings.getRotate() == 0)
    {
        // point data
        int count  = scb_stroke_get_point_count(stroke);
        ScbPoint * pts = (ScbPoint *)scb_stroke_get_point_data(stroke);

        // select color. 
        ScbColor color;
        scb_dev_color_to_color(&color, stroke->style.color);
        ctrl->gtkMgr.setLineColor(color.pixel);

        // select pen size
        ctrl->gtkMgr.setLineAttributes(stroke->style.penSize);

        int i = 0;
        int x1 = px + pts[i].x, y1 = py + pts[i].y; ++i;
        int x2, y2;

        while (i < count)
        {
            x2 = px + pts[i].x, y2 = py + pts[i].y; ++i;
            ctrl->gtkMgr.drawLine(x1, y1, x2, y2);
            x1 = x2; y1 = y2;
        }
        
        if (count <= 1)
        {
            ctrl->gtkMgr.drawLine(x1, y1, x1, y1);
        }            
    }
    else if (ctrl->settings.getRotate() == 270)
    {
        // point data
        int count  = scb_stroke_get_point_count(stroke);
        ScbPoint * pts = (ScbPoint *)scb_stroke_get_point_data(stroke);

        // select color. 
        ScbColor color;
        scb_dev_color_to_color(&color, stroke->style.color);
        ctrl->gtkMgr.setLineColor(color.pixel);

        // select pen size
        ctrl->gtkMgr.setLineAttributes(stroke->style.penSize);
        int i = 0;
        int x1 = py - pts[i].x, y1 = px + pts[i].y; ++i;
        int x2, y2;
        while (i < count)
        {
            x2 = py - pts[i].x, y2 = px + pts[i].y; ++i;
            ctrl->gtkMgr.drawLine(y1, x1, y2, x2);    // swap x & y
            x1 = x2; y1 = y2;
        }
         
        if (count <= 1)
        {
            ctrl->gtkMgr.drawLine(y1, x1, y1, x1);
        }            
    }
}

void CScribbleMgr::drawStrokeDirectly(ScbStrokePtr stroke, Controller * ctrl)               
{
    // point data
    int count  = scb_stroke_get_point_count(stroke);
    ScbPoint * pts = (ScbPoint *)scb_stroke_get_point_data(stroke);

    // select color. 
    ScbColor color;
    scb_dev_color_to_color(&color, stroke->style.color);
    ctrl->gtkMgr.setLineColor(color.pixel);

    // select pen size
    ctrl->gtkMgr.setLineAttributes(stroke->style.penSize);

    for(int i = 0; i < count - 1; ++i)
    {
        ctrl->gtkMgr.drawLine(pts[i].x, pts[i].y, pts[i + 1].x, pts[i + 1].y);
    }
}

                
