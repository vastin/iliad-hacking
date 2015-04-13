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
 
#include "LayoutMgr.h"
#include "controller.h"
#include "PDFViewerLog.h"

CLayoutMgr::CLayoutMgr(void)
: posTab(NULL)
, posLen(0)
{
}

CLayoutMgr::~CLayoutMgr(void)
{
}

void CLayoutMgr::clear()
{
    if (posTab)
    {
        delete [] posTab; posTab = NULL; posLen = 0;
    }
}

double CLayoutMgr::initPagesPosition(Controller * ctrl)
{
    if (posTab && posLen != ctrl->core->getNumPages())
    {
        delete [] posTab; posTab = NULL; posLen = ctrl->core->getNumPages();
    }
    
    if (NULL == posTab)
    {
        posLen = ctrl->core->getNumPages();
        posTab = new rectangle[posLen];
    }

    // bound will record out-bound of virtual screen
    bound.left = bound.top = bound.right = bound.bottom = 0;

    // go through page list
    double zoom  = ctrl->calZoom(ctrl->settings.getCurrentPage());
    // PV_LOGPRINTF("Zoom now %g! posLen %d posTab %p", zoom, posLen, posTab);
    int width = 0, height = 0, tmp = 0, rot = 0;
    PDFDoc * pdfdoc = ctrl->core->getPDFDoc();
    
    for(int i = 1; i <= posLen; ++i)
    {
        width = (int)(pdfdoc->getPageCropWidth(i) * zoom * 0.01 + 0.5);
        height = (int)(pdfdoc->getPageCropHeight(i) * zoom  * 0.01 + 0.5);
        rot = /*ctrl->settings.getRotate() + */pdfdoc->getPageRotate(i); 
        if (rot >= 360) rot -= 360; 
        else if (rot < 0) rot += 360;
        if (rot == 90 || rot == 270)
        {
            tmp = width; width = height; height = tmp;
        }
        
        if (bound.right < width)
        {
            bound.right = width;
        }
        posTab[i - 1].right = width;
        posTab[i - 1].top = bound.bottom;
        posTab[i - 1].bottom = posTab[i - 1].top + height;
        bound.bottom += height + 1;
    }

    // adjust x position to make it in the center of virtual screen
    for(int i = 1; i <= posLen; ++i)
    {
        posTab[i - 1].left = (bound.right - posTab[i - 1].right) / 2;
        posTab[i - 1].right += posTab[i - 1].left;
        // PV_LOGPRINTF("item %d:\t\t%d %d %d %d", i, posTab[i - 1].left, posTab[i - 1].top, 
        //    posTab[i - 1].right, posTab[i - 1].bottom);
    }
    
    return zoom;
}

void CLayoutMgr::getPageRect(const int page, rectangle & rect)
{
    rect = posTab[page - 1];
}

const rectangle & CLayoutMgr::getPageRect(const int page) const
{
    return posTab[page - 1];
}

int CLayoutMgr::getPagesVisible(int & start, int & end, const rectangle & rect)
{
    start = -1; end = -1;
    int max = 0, tmp = 0, index = -1;
    for(int i = 0; i < posLen; ++i)
    {
        if (rect.bottom < posTab[i].top) return index;
        if ((tmp = rect.isIntersect(posTab[i])) > 0)
        {
            if (start == -1) 
            {
                start = i + 1; end = start; 
            }
            else
            {
                ++end;
            }
            if (max < tmp) { max = tmp; index = i + 1; }
        }
    }
    return index;
}

//////////////////////////////////////////////////////////////////////////
// check point in page [start, end], returns page number if hits
// otherwise returns -1 
// screenToVirtualScreen(ctrl, pt);
// hitTest(pt, start, end)
int CLayoutMgr::hitTest(const point & pt, const int start , const int end)
{
    for(int i = start; i <= end; ++i)
    {
        if (posTab[i - 1].hitTest(pt))
        {
            return i;
        }
    }
    return -1;
}

int CLayoutMgr::hitTest(const int x, const int y, const int start , const int end)
{
    for(int i = start; i <= end; ++i)
    {
        if (posTab[i - 1].hitTest(x, y))
        {
            return i;
        }
    }
    return -1;
}


//////////////////////////////////////////////////////////////////////////
// convert from screen to virtual screen
void CLayoutMgr::screenToVirtualScreen(Controller * ctrl, rectangle & rect)
{
    // to virtual screen
    rect.left += ctrl->settings.getScreenX();
    rect.top += ctrl->settings.getScreenY();
    rect.right += ctrl->settings.getScreenX();
    rect.bottom += ctrl->settings.getScreenY();
}

void CLayoutMgr::screenToVirtualScreen(Controller * ctrl, point & pt)
{
    pt.x += ctrl->settings.getScreenX();
    pt.y += ctrl->settings.getScreenY();
}

// called when continous mode to page mode
void CLayoutMgr::virtualScreenToScreen(Controller * ctrl, const rectangle & rect)
{
    ctrl->settings.setScreenX(rect.left);
    ctrl->settings.setScreenY(rect.top);
}

//////////////////////////////////////////////////////////////////////////
// We should change screenX and screenY here according to current page
//////////////////////////////////////////////////////////////////////////
void CLayoutMgr::rotate(Controller * ctrl, const int angle)
{
    CPageInfo * page = ctrl->pageList.getPage(ctrl->settings.getCurrentPage());
    if (NULL == page) return;

    // calculate intersection
    rectangle rect; rect.left = 0; rect.top = 0; 
    rect.right = ctrl->settings.getScreenWidth();
    rect.bottom = ctrl->settings.getScreenHeight();
    if (ctrl->settings.getScreenX() > 0) rect.left = ctrl->settings.getScreenX();
    if (ctrl->settings.getScreenY() > 0) rect.top = ctrl->settings.getScreenY();
    if (rect.right > ctrl->settings.getScreenX() + page->bitmap->getWidth())
    {
        rect.right = ctrl->settings.getScreenX() + page->bitmap->getWidth();
    }
    if (rect.bottom > ctrl->settings.getScreenY() + page->bitmap->getHeight())
    {
        rect.bottom = ctrl->settings.getScreenY() + page->bitmap->getHeight();
    }
    point center;
    center.x = (rect.left + rect.right) / 2;
    center.y = (rect.top + rect.bottom) / 2;
    int distX = 0, distY = 0;

    // calculate the new position 
    if (angle == 90)
    {
        distX = center.x - ctrl->settings.getScreenX();
        distY = page->bitmap->getHeight() - center.y + ctrl->settings.getScreenY();
        ctrl->settings.setScreenX(center.x - distY);
        ctrl->settings.setScreenY(center.y - distX);
    }
    else if (angle == -90)
    {
        distX = page->bitmap->getWidth() - center.x + ctrl->settings.getScreenX();
        distY = center.y - ctrl->settings.getScreenY();
        ctrl->settings.setScreenX(center.x - distY);
        ctrl->settings.setScreenY(center.y - distX);
    }
}

void CLayoutMgr::updatePageRect(const int x, const int y, const int width, const int height)
{
    bound.left = x; bound.top = y;
    bound.right = x + width; 
    bound.bottom = y + height;
}

GBool CLayoutMgr::hitTest(const int x, const int y)
{
    return bound.hitTest(x, y);
}

