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

#ifndef UTILS_H_
#define UTILS_H_
#include "goo/gtypes.h"

inline int min(const int a, const int b)
{
    return (a > b)? b: a;
}

inline int max(const int a, const int b)
{
    return (a > b)? a : b;
}

// not template
inline void swap(int & a, int & b)
{
    int t = a; a = b; b = t;
}

inline void swap(double & a, double & b)
{
    double t = a; a = b; b = t;
}

struct point
{
    int x, y;
};

struct rectangle
{
    int left;
    int top;
    int right;
    int bottom;

    void    normalize();
    int     isIntersect(const rectangle & r2) const;
    int     isVerInter(const rectangle & r2) const;
    int     ivHorInter(const rectangle & r2) const;
    GBool   hitTest(const point & pt) const;
    GBool   hitTest(const int x, const int y) const;
};

#endif
