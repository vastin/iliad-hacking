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
 
#include "PageInfo.h"
#include <pthread.h>
#include "PDFViewerLog.h"
#include "controller.h"
#include <math.h>

static const double e = 0.001;

CPageInfo::CPageInfo(void)
{
    initialize();
}

CPageInfo::~CPageInfo(void)
{
    clear();
}

void CPageInfo::initialize()
{
    memset(this, 0, sizeof(CPageInfo));
}

void CPageInfo::clear()
{
    if (bitmap)
    {
        delete bitmap; bitmap = NULL;
    }
    if (links) 
    {
        delete links; links = NULL;
    }
    if (text) 
    {
        delete text; text = NULL;
    }
}

GBool CPageInfo::isSame(const int pn, const double zoom, const int rot)
{
    if (pn == pageNumber && rot == rotate)
    {
        return (fabs(zoom - pageZoom) < e);
    }
    return gFalse;
}

double CPageInfo::calZoom()
{
    return pageZoom;
}
//////////////////////////////////////////////////////////////////////////
// page list
//////////////////////////////////////////////////////////////////////////
PageList::PageList()
{
    length = 0; list = NULL;
    initialize();
}

PageList::~PageList()
{
    clear();
    delete [] list; list = NULL; length = 0;
    pthread_mutex_destroy(&mutex);
}
    
void PageList::initialize()
{
    pthread_mutex_init(&mutex, NULL);
}

void PageList::clear()
{
    pthread_mutex_lock(&mutex);
    for(int i = 0; i < length ; ++i)
    {
        if (list[i])
        {
            delete list[i];
            list[i] = NULL;
        }            
    }
    pthread_mutex_unlock(&mutex);
}

void PageList::reset(const int size)
{
    pthread_mutex_lock(&mutex);
    for(int i = 0; i < length ; ++i)
    {
        if (list[i])
        {
            delete list[i];
            list[i] = NULL;
        }            
    }
    length = size; 
    if (list) 
    {
        delete [] list; list = NULL;
    }
    list = new PageInfoPtr[size];        
    memset(list, 0, length * sizeof(PageInfoPtr));
    pthread_mutex_unlock(&mutex);
}

void PageList::addPage(CPageInfo * info)
{
    if (NULL == info) return;
    int index = info->pageNumber;
    if (index < 1 || index > length) return;
    pthread_mutex_lock(&mutex);
    --index;
    if (list[index] == NULL)
    {
        list[index] = info;
    }
    else
    {
        /*
        PV_DUMP("Page already exist, parameters:\n");
        PV_DUMP("Original page %d\t\t", list[index]->pageNumber);
        PV_DUMP("zoom %lf\t", list[index]->pageZoom);       
        PV_DUMP("rotate %d\t", list[index]->rotate);     
        PV_DUMP("time %d\n", list[index]->timestamp);
        
        PV_DUMP("New page %d\t\t", info->pageNumber);
        PV_DUMP("zoom %lf\t", info->pageZoom);       
        PV_DUMP("rotate %d\t", info->rotate);     
        PV_DUMP("time %d\n", info->timestamp);
        */
        delete list[index]; list[index] = info;
    }
    pthread_mutex_unlock(&mutex);
}

//////////////////////////////////////////////////////////////////////////
// Two steps:
// 1. find oldest page in unprotected area 
// 2. find page most far from current page
void PageList::removeOldestOne(const int center)
{
    int index = -1, i;
    int min   = 0x7fffffff;
    CPageInfo * pItem = NULL;
    pthread_mutex_lock(&mutex);

    // find oldest page in unprotected area
    for(i = 0; i < center - ProtectWidth && i < length; ++i)
    {
        pItem = list[i];
        if (pItem && min > pItem->timestamp)
        {
            index = i; min = pItem->timestamp;
        }
    }
    for(i = center + ProtectWidth; i < length; ++i)
    {
        pItem = list[i];
        if (pItem && min > pItem->timestamp)
        {
            index = i; min = pItem->timestamp;
        }
    }
    if (index >= 0  && index < length)
    {
        PV_LOGPRINTF("delete page %d", list[index]->pageNumber);        
        delete list[index]; list[index] = NULL;
        pthread_mutex_unlock(&mutex);
        return;
    }

    // remove page most far from current page
    index = -1;
    int dist = 0, pos = -1;
    for(i = -ProtectWidth; i <= ProtectWidth; ++i)
    {
        index = center + i - 1;
        if (index < 0) index = 0;
        else if (index >= length) index = length - 1;
        if (list[index] && abs(index + 1 - center) > dist)
        {
            pos = index;
            dist = abs(index + 1 - center);
        }
    }    
    if (pos >= 0 && pos != center - 1) 
    {
        PV_LOGPRINTF("delete page %d\n", list[pos]->pageNumber);  
        delete list[pos]; list[pos] = NULL;
    }
    pthread_mutex_unlock(&mutex);
}

CPageInfo * PageList::getPage(const int number)
{
    CPageInfo * pItem = NULL;
    if (number < 1 || number > length) return NULL;
    pthread_mutex_lock(&mutex);
    pItem = list[number - 1];
    pthread_mutex_unlock(&mutex);
    return pItem;
}

// check a certain page 
CPageInfo * PageList::checkPage(const CPageInfo & info)
{
    CPageInfo * pItem = getPage(info.pageNumber);
    if (NULL == pItem) return NULL;
    if (pItem->isSame(info.pageNumber, info.pageZoom, info.rotate))
    {
        return pItem;
    }
    return NULL;
}

CPageInfo * PageList::checkPage(const TodoItem * todo)
{
    if (NULL == todo) return NULL;
    CPageInfo * pItem = getPage(todo->pageNumber);
    if (NULL == pItem) return NULL;
    if (pItem->isSame(todo->pageNumber, todo->zoom, todo->rotate))
    {
        return pItem;
    }
    return NULL;
}

void PageList::dump()
{
    PV_DUMP("\n\nPageList:\n");
    CPageInfo * pItem = NULL;
    pthread_mutex_lock(&mutex);
    for(int i = 0; i < length; ++i)
    {
        pItem = list[i];
        if (pItem)
        {
            PV_DUMP("page %d\t", pItem->pageNumber);
            PV_DUMP("zoom %lf\t", pItem->pageZoom);   
            PV_DUMP("rotate %d\t", pItem->rotate);         
            PV_DUMP("time %d\n", pItem->timestamp);
        }
    }
    PV_DUMP("\n\n");
    pthread_mutex_unlock(&mutex);
}


//////////////////////////////////////////////////////////////////////////
// Todo item
//////////////////////////////////////////////////////////////////////////
TodoItem::TodoItem()
{
    zoom  = -1;
    rotate = 0;
    pageNumber = 1;
    timeStamp = 0;
}

TodoItem::~TodoItem()
{
}

TodoItem::TodoItem(const TodoItem & right)
: zoom (right.zoom)
, rotate(right.rotate)
, pageNumber(right.pageNumber)
, timeStamp(right.timeStamp)
{
}

//////////////////////////////////////////////////////////////////////////
// Todo list
//////////////////////////////////////////////////////////////////////////
TodoList::TodoList()
{
    initialize();
}

TodoList::~TodoList()
{
    clear();
    delete list; list = NULL;
    pthread_mutex_destroy(&mutex); 
}

void TodoList::initialize()
{
    pthread_mutex_init(&mutex, NULL);
    list = new GooList();
}

void TodoList::clear()
{
    pthread_mutex_lock(&mutex);
    TodoItem *pItem = NULL;
    while (list->getLength())
    {
        pItem = (TodoItem *)list->del(0);
        delete pItem;
    }
    pthread_mutex_unlock(&mutex);
}

void TodoList::removeDup(TodoItem * item)
{
    int i = 0, length = list->getLength();
    TodoItem * p = NULL;
    while (i < length)
    {
        p = (TodoItem *)list->get(i); 
        if ( p && item && p->pageNumber == item->pageNumber && item->timeStamp > p->timeStamp)
        {
            // PV_LOGPRINTF("Remove duplicated item %d", item->pageNumber);
            list->del(i); --length;
        }
        else
        {
            ++i;
        }
    }        
}

void TodoList::pushItem(TodoItem * item, GBool bHead, GBool bRemoveDup)
{
    if (NULL == item) return;
    pthread_mutex_lock(&mutex);

    if (bRemoveDup)
    {
        removeDup(item);
    }
    if (bHead)
    {
        list->insert(0, item);
    }
    else
    {
        list->append(item);
    }
    pthread_mutex_unlock(&mutex);
}

TodoItem * TodoList::popItem()
{
    TodoItem * item = NULL;
    pthread_mutex_lock(&mutex);
    if (list->getLength() > 0) 
    {
        item = (TodoItem *)list->del(0);
    }
    pthread_mutex_unlock(&mutex);
    return item;
}
