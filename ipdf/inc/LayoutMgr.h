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
 
#ifndef LAYOUTMGR_H_
#define LAYOUTMGR_H_

#include "PageInfo.h"


//////////////////////////////////////////////////////////////////////////
// layout manager is a helper class used by portrait view and landscape view 
// to record pages layout. 
//////////////////////////////////////////////////////////////////////////
struct Controller;
class CLayoutMgr
{
private:
    rectangle   bound;          // page mode and continous mode
    rectangle*  posTab;         // page position in virtual screen
    int         posLen;         // count

public:
    CLayoutMgr(void);
    ~CLayoutMgr(void);

public:
    void    clear();

    ////////////////////// for continous mode /////////////////////////////
    void    getPageRect(const int page, rectangle & rect);
    const rectangle & getPageRect(const int page) const; 
    double  initPagesPosition(Controller * ctrl);
    int     getPagesVisible(int & start, int & end, const rectangle & rect);
    int     hitTest(const point & pt, const int start , const int end);
    int     hitTest(const int x, const int y, const int start , const int end);

    ////////////////////////// for page mode //////////////////////////////
    void    updatePageRect(const int x, const int y, const int width, const int height);
    GBool   hitTest(const int x, const int y);

    ///////////// convert between screen and virtual screen ////////////
    void    screenToVirtualScreen(Controller * ctrl, rectangle & rect);
    void    screenToVirtualScreen(Controller * ctrl, point & pt);
    void    virtualScreenToScreen(Controller * ctrl, const rectangle & rect);
    void    getVirtualScreen(rectangle & rect) { rect = bound; }
    const rectangle & getVirtualScreen() const { return bound; }

    // portrait <-> landscape
    void    rotate(Controller * ctrl, const int angle);

};

#endif
