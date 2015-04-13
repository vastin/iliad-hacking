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
 
#include "DisplayList.h"
#include "PDFView.h"
#include "controller.h"

CDisplayList::CDisplayList(void)
{
}

CDisplayList::~CDisplayList(void)
{
    clear();
}

void CDisplayList::clear()
{
    while (displayList.getLength() > 0)
    {
        delete (TodoItem *)displayList.del(0);
    }
}

void CDisplayList::add(TodoItem * item)
{
    displayList.append(item);
}

GBool CDisplayList::remove(const int pn)
{
    TodoItem * pItem = NULL;
    for(int i = 0; i < displayList.getLength(); ++i)
    {
        pItem = (TodoItem *)displayList.get(i);
        if (pItem && pn == pItem->pageNumber)
        {
            delete pItem;
            displayList.del(i);
            return gTrue;
        }
    }
    return gFalse;
}

GBool CDisplayList::isEmpty()
{
    return ( 0 == displayList.getLength());
}

void CDisplayList::issueItems(CPDFView *view)
{
    TodoItem * pItem = NULL;
    for(int i = 0; i < displayList.getLength(); ++i)
    {
        pItem = (TodoItem *)displayList.get(i);
        if (pItem && 0 != pItem->timeStamp)
        {
            // only add items that are not in rendering
            view->issueItem(pItem->pageNumber);
        }
    }  
    view->ctrl->thread.signal();
}

void CDisplayList::renderItems(CPDFView *view)
{
    TodoItem * pItem = NULL;
    for(int i = 1; i < displayList.getLength(); ++i)
    {
        pItem = (TodoItem *)displayList.get(i);
        if (pItem)
        {
            view->issueItem(pItem->pageNumber);
        }
    }
    pItem = (TodoItem *)displayList.get(0);
    TodoItem * item = new TodoItem(*pItem);
    view->ctrl->thread.renderPageNow(item);
}
