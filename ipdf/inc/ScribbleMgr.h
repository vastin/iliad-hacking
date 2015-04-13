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
 
#ifndef SCRIBBLEMGR_H_
#define SCRIBBLEMGR_H_

#include <liberscribble/scb.h>
#include "goo/gtypes.h"
//////////////////////////////////////////////////////////////////////////
// Scribble mgr serves as proxy between iPdf and scribble library. 
// Scribble mgr also provides toolbar management for scribble
// Notice:
// When recording a point, the point coordinates should be offset value in
// the page. In landscape view, caller should change coordinates.
//
// (0,0)
//   __________
//  |  |->     |
//  | \/       |
//  | portrait |
//  |          |
//  |__________|
//
//   __________
//  |          |
//  |landscape |
//  | ^        |
//  |_|->______|
// (0,0)
// By this mean, scribble manager can redraw strokes in portrait landscape view.
// 
//////////////////////////////////////////////////////////////////////////
class Controller;
class CScribbleMgr
{
private:
    ScbPath         path;       // the path
    ScbDocPtr       doc;        // singleton document
    ScbPagePtr      page;       // tmp, page currently editing
    ScbStrokePtr    stroke;     // tmp
    int             pageNumber; // tmp
    ScbDevPoint     pagePoint;  // page start position             
    ScbDevPoint     point;
    ScbPageId       id;
    ScbPageEraseCtx eraseCtx;
    GBool           bErased;    // when strokes have been erased
    GBool           bScbDirty;  // Any changes to current scribble data: added or removed.

private:
    /////////////////////////helper function//////////////////////////////
    void    beginStroke(const int pn, Controller * ctrl, 
                const int px, const int py,     // page position
                const int x, const int y, 
                const int ox, const int oy);
    void    addPoint(const int pn, Controller * ctrl, 
                const int px, const int py,     // page position
                const int x, const int y,
                const int ox, const int oy);
    void    endStroke(const int pn, Controller * ctrl, 
                const int px, const int py,     // page position
                const int x, const int y, 
                const int ox, const int oy);
    void    endStroke(Controller * ctrl, 
                const int px, const int py,
                const int x, const int y);

    //////////////////////////// page redraw /////////////////////////////                
    void    drawScribblePagePortrait(ScbPagePtr, Controller * ctrl, 
                const int x, const int y);
    void    drawScribblePageLandscape(ScbPagePtr, Controller * ctrl, 
                const int x, const int y);
    void    drawErasedStrokesPortrait(ScbStrokesPtr, Controller * ctrl, 
                const int x, const int y);
    void    drawErasedStrokesLandscape(ScbStrokesPtr, Controller * ctrl, 
                const int x, const int y);
    void    drawStroke(ScbStrokePtr stroke, Controller * ctrl, 
                const int px, const int py);
    void    drawStrokeDirectly(ScbStrokePtr stroke, Controller * ctrl);       
    int     calLineSize(const int penSize, const double strokeZoom, const double dispZoom);     
    
    void    setScbDirty(const GBool b = gTrue);
    GBool   isScbDirty() const { return bScbDirty; }
        

public:
    CScribbleMgr(void);
    ~CScribbleMgr(void);

public:
    void    open(const char *);
    void    close(GBool bSave = gTrue);
    void    save();
    void    generatePageId(ScbPageIdPtr pid, const int pn);
    ScbDocPtr getScbDoc() { return doc; }

    /////////////////////////////// event ////////////////////////////////
    // Create new stroke when: button press & button move
    void    onScribblePress(const int pn, Controller * ctrl,
                const int px, const int py,     // page position
                const int x, const int y,       // for driver drawing
                const int ox, const int oy);    // stroke data in offset
    void    onScribbleMove(const int pn, Controller * ctrl, 
                const int px, const int py,     // page position
                const int x, const int y, 
                const int ox, const int oy);
    void    onScribbleRelease(const int pn, Controller * ctrl, 
                const int px, const int py,     // page position
                const int x, const int y, 
                const int ox, const int oy);

    ///////////////////////////// erase //////////////////////////////////
    void    onErasePress(const int pn, Controller * ctrl,  
                const int x, const int y,       // for erase redrawing
                const int ox, const int oy);    // stroke data to test
    void    onEraseMove(const int pn, Controller * ctrl, 
                const int x, const int y, 
                const int ox, const int oy);
    void    onEraseRelease(const int pn, Controller * ctrl, 
                const int x, const int y, 
                const int ox, const int oy);
    GBool   isEraseDirty() const { return bErased; }
    void    setEraseDirty(GBool b = gTrue);               

    ////////////////////////////// paint /////////////////////////////////
    void    drawScribblePage(const int pn, Controller * ctrl, 
                const int x, const int y);
};

#endif
