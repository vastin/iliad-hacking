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
 
#include "utils.h"



//////////////////////////////////////////////////////////////////////////
// return size of area intersection
int rectangle::isIntersect(const rectangle & r2) const
{
    if ((r2.left > right  || r2.right < left || 
         r2.top > bottom  || r2.bottom < top ))
    {
        return 0;
    }
    // calculate size
    int width  = min(right, r2.right) - max(left, r2.left);
    int height = min(bottom, r2.bottom) - max(top, r2.top);
    return width * height;
}

//////////////////////////////////////////////////////////////////////////
// return height of intersection, ignore horizontal area
int rectangle::isVerInter(const rectangle & r2) const
{
    if (r2.top > bottom || r2.bottom < top)
    {
        return 0;
    }
    int height = min(bottom, r2.bottom) - max(top, r2.top);
    return height;
}

//////////////////////////////////////////////////////////////////////////
// return width of intersection
int rectangle::ivHorInter(const rectangle & r2) const
{
    if (r2.left > right || r2.right < left)
    {
        return 0;
    }
    int width = min(right, r2.right) - max(left, r2.left);
    return width;
}

void rectangle::normalize()
{
    if (left > right) { swap(left, right);}
    if (top > bottom) { swap(top, bottom);}
}

GBool rectangle::hitTest(const point & pt) const
{
    return (left <= pt.x && right >= pt.x &&
            top <= pt.y && bottom >= pt.y);
}

GBool rectangle::hitTest(const int x, const int y) const
{
    return (left <= x && right >= x &&
            top <= y && bottom >= y);
}

