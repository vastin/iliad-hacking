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
 
#ifndef DISPLAYLIST_H_
#define DISPLAYLIST_H_
#include "PageInfo.h"

//////////////////////////////////////////////////////////////////////////
// CDisplayList manages a list of request. 
// Not necessary to make it as thread-safe, it could only be called by 
// main thread
//////////////////////////////////////////////////////////////////////////

class CPDFView;
class CDisplayList
{
private:
    GooList     displayList;    // the pages should be displayed

public:
    CDisplayList(void);
    ~CDisplayList(void);

public:
    void clear();
    void add(TodoItem * item);
    GBool remove(const int pn);
    GBool isEmpty(); 
    void issueItems(CPDFView *);
    void renderItems(CPDFView *);  // abort and issue

};

#endif
