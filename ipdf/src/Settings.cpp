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
 
#include "Settings.h"
#include <libermanifest/ermanifest.h>
#include <liberipc/eripctoolbar.h>

static const int INVALID_PAGE_NUMBER = -1;

#if (HISTORY_DEBUG)
int HistoryItem::count = 0;
#endif
HistoryItem::HistoryItem()
: x(0)
, y(0)
, pageNumber(0)
{
#if (HISTORY_DEBUG)
    ++count;
#endif    
}

HistoryItem::~HistoryItem()
{
#if (HISTORY_DEBUG)
    --count;	
#endif	
}

//////////////////////////////////////////////////////////////////////////
// Settings:
// screenx and screeny (view x and view y)
// page mode: page image position in screen
// continous mode: scroll position. 
CSettings::CSettings(void)
{
    zoom = ZoomPage; rotate = 0; screenX = screenY = 0;
    currentPage = INVALID_PAGE_NUMBER; 
    drawAreaWidth = SCREEN_WIDTH;
    drawAreaHeight = CLIENT_AREA; 
    renderDir = RenderNext; 
    mode = ModePage; zbState = iconState_grey;
    enableScb = gTrue; currentItem = 0; 
}

CSettings::~CSettings(void)
{
    clearHistory();
}

void CSettings::setNewZoom(const double z)
{
    record.zoom = zoom;
    zoom = z; 
    record.x = screenX; 
    record.y = screenY;
    zbState = iconState_normal; 
}

GBool CSettings::zoomBack()
{
    if (InvalidZoom == record.zoom)
    {
        return gFalse;
    }

    if (mode == ModePage)
    {
        swap(zoom, record.zoom);
        swap(screenX, record.x);
        swap(screenY, record.y);
    }
    else 
    {
        double ratio = record.zoom / zoom;
        swap(zoom, record.zoom);
        if (rotate == 270)
        {
            record.x = (int)(screenX * ratio);
            swap(screenX, record.x);
            swap(screenY, record.y);
        }
        else if (rotate == 0)
        {          
            record.y = (int)(screenY * ratio);
            swap(screenY, record.y);
            swap(screenX, record.x);
        }            
    }
    if (iconState_normal == zbState)
    {
        zbState = iconState_selected;
    }
    else if (iconState_selected == zbState)
    {
        zbState = iconState_normal;
    }
    return gTrue;
}

void CSettings::disableZoomBack()
{
    record.zoom = InvalidZoom;
    zbState = iconState_grey;
}

GBool CSettings::canZoomBack()
{
    return (InvalidZoom != record.zoom);
}

void CSettings::load(const char * pathName)
{
    erManifest manifest;
    if (RET_OK == ermXmlOpenManifest(pathName, &manifest))
    {
        // usually the manifest file is existing
        ermXmlGetInt(&manifest, "/package/last-location/pagenumber", &currentPage);
    
        // zoom
        static const int MAX = 20;
        char tmp[MAX] = {0};
        ermXmlGetString(&manifest, "/package/viewer-settings/zoomfactor", tmp, MAX);
        
        if (0 == strcasecmp(tmp, "zoomPage"))
        {
            zoom = ZoomPage;
        }
        else if (0 == strcasecmp(tmp, "zoomWidth"))
        {
            zoom = ZoomWidth;
        }
        else
        {
            sscanf(tmp, "%lf", &zoom);
        }
        
        // rotation , position and mode
        ermXmlGetInt(&manifest, "/package/viewer-settings/rotation", &rotate);
        ermXmlGetInt(&manifest, "/package/viewer-settings/positionx", &screenX);
        ermXmlGetInt(&manifest, "/package/viewer-settings/positiony", &screenY);
        ermXmlGetString(&manifest, "/package/viewer-settings/mode", tmp, MAX);
        
        if (0 == strcasecmp(tmp, "continous"))
        {
            mode = ModeContinous;
        }
        else if (0 == strcasecmp(tmp, "page"))
        {
            mode = ModePage;
        }

        // check scribble state at first.
        // if the string contains any invalid characters, just disable scribble.
        enableScb = gTrue;
        if (RET_OK == ermXmlGetString(&manifest, "/package/metadata/y-metadata/modify-enable/scribble-enable", tmp, MAX))
        {
            if (strncmp(tmp, "true", MAX) != 0)
            {
                enableScb = gFalse;
            }
        }
        // if children nodes are not available, check parent node.
        else if (RET_OK == ermXmlGetAttributeString(&manifest, 
                        "/package/metadata/y-metadata/modify-enable",
                        "default", tmp, MAX))
        {
            if (strncmp(tmp, "true", MAX) != 0)
            {
                enableScb = gFalse;
            }
        }
    }    
    ermXmlClose(&manifest);    
}

void CSettings::save(const char *pathName)
{
    erManifest manifest;
    if (RET_OK == ermXmlOpenManifest(pathName, &manifest))
    {
        // usually the manifest file is existing
        
        // dump for debug
        // s.dump();
        
        // page
        if (RET_OK != ermXmlExist(&manifest, "/package/last-location/pagenumber"))
        {
            ermXmlNewString(&manifest, "/package", "last-location", "");            
            ermXmlNewString(&manifest, "/package/last-location", "pagenumber", "");            
        }
        ermXmlSetInt(&manifest, "/package/last-location/pagenumber", currentPage);                    

        // zoom 
        if (RET_OK != ermXmlExist(&manifest, "/package/viewer-settings/zoomfactor"))
        {
            ermXmlNewString(&manifest, "/package", "viewer-settings", "");            
            ermXmlNewString(&manifest, "/package/viewer-settings", "zoomfactor", "");            
        }
        if (zoom == ZoomPage)
        {
            ermXmlSetString(&manifest, "/package/viewer-settings/zoomfactor", "zoomPage");                    
        }
        else if (zoom == ZoomWidth)
        {
            ermXmlSetString(&manifest, "/package/viewer-settings/zoomfactor", "zoomWidth");                    
        }            
        else 
        {
            char tmp[20] = {0};
            snprintf(tmp, 20, "%lf", zoom);
            ermXmlSetString(&manifest, "/package/viewer-settings/zoomfactor", tmp);                    
        }
        
        // rotation
        if (RET_OK != ermXmlExist(&manifest, "/package/viewer-settings/rotation"))
        {
            ermXmlNewString(&manifest, "/package/viewer-settings", "rotation", "");            
        }
        ermXmlSetInt(&manifest, "/package/viewer-settings/rotation", rotate);                    

        // position x
        if (RET_OK != ermXmlExist(&manifest, "/package/viewer-settings/positionx"))
        {
            ermXmlNewString(&manifest, "/package/viewer-settings", "positionx", "");            
        }
        ermXmlSetInt(&manifest, "/package/viewer-settings/positionx", screenX);                    

        // position y
        if (RET_OK != ermXmlExist(&manifest, "/package/viewer-settings/positiony"))
        {
            ermXmlNewString(&manifest, "/package/viewer-settings", "positiony", "");            
        }
        ermXmlSetInt(&manifest, "/package/viewer-settings/positiony", screenY);                    

        // mode
        if (RET_OK != ermXmlExist(&manifest, "/package/viewer-settings/mode"))
        {
            ermXmlNewString(&manifest, "/package/viewer-settings", "mode", "");            
        }
        if (mode == ModeContinous)
        {
            ermXmlSetString(&manifest, "/package/viewer-settings/mode", "continous");
        }
        else 
        {
            ermXmlSetString(&manifest, "/package/viewer-settings/mode", "page");
        }

        ermXmlSaveAndClose(&manifest);
    }     
}

void CSettings::getScreenRect(rectangle & rect)
{
    if (getRotate() == 0 || getRotate() == 180)
    {
        rect.left = screenX; rect.top = screenY;
        rect.right = screenX + drawAreaWidth;
        rect.bottom = screenY + drawAreaHeight;
    }
    else if (getRotate() == 90 || getRotate() == 270)
    {
        rect.left = screenY; rect.top = screenX; 
        rect.right = screenY + drawAreaHeight;
        rect.bottom = screenX + drawAreaWidth;
    }        
}

void CSettings::setScreenPosition(const int nX, const int nY)
{
    screenX = nX; screenY = nY;
}

void CSettings::setScreenSize(const int width, const int height)
{
    drawAreaWidth = width; drawAreaHeight = height;
}

void CSettings::setScreenRect(const rectangle & rect)
{
    screenX = rect.left; screenY = rect.top;
    drawAreaWidth = rect.right - rect.left; 
    drawAreaHeight = rect.bottom - rect.top;
}

void CSettings::clearHistory()
{
    // clear the forward list.
    HistoryItemPtr ptr = 0;
    while (forward.getLength() > 0)
    {      
        ptr = (HistoryItemPtr)forward.del(0);
        delete ptr;
    }

    // clear backward list
    while (backward.getLength() > 0)
    {      
        ptr = (HistoryItemPtr)backward.del(0);
        delete ptr;
    }

    // clear current item
    delete currentItem;
    currentItem = 0;

#if (HISTORY_DEBUG)
    // Count should reach 1, because the member variable record is still not released.
    printf("\n\nHistoryItem count reaches %d\n\n", HistoryItem::count);
#endif    
}

// When current page changed, it will add the old valid page into history list.
void CSettings::setCurrentPage(const int p)
{
    // page change signal.
    if (currentPage != p && currentPage != INVALID_PAGE_NUMBER)
    {
        // add the current one into history list.
        addNewItem(currentPage);
    }

    if (currentItem == 0)
    {
        currentItem = new HistoryItem;
    }	

    // always update the page number.
    currentItem->pageNumber = p;	   
    currentPage = p;

#if (HISTORY_DEBUG)
    dumpHistory();
#endif
}

// called when jump to new page.
void CSettings::addNewItem(HistoryItemPtr ptr)
{
    // check length before adding new one.
    if (backward.getLength() >= HISTORY_MAX_LENGTH)
    {
        delete (HistoryItemPtr)backward.del(0);
    }

    backward.append(ptr);

    // clear the forward list.
    while (forward.getLength() > 0)
    {      
        ptr = (HistoryItemPtr)forward.del(0);
        delete ptr;
    }
}

void CSettings::addNewItem(const int page)
{
    HistoryItemPtr ptr = new HistoryItem;
    ptr->pageNumber = page;
    addNewItem(ptr);
}

HistoryItemPtr CSettings::goBackward()
{
    int last = backward.getLength();
    if (last <= 0)
    {
        return 0;
    }

    forward.append(currentItem);  
    currentItem = (HistoryItemPtr)backward.del(last - 1);
    currentPage = currentItem->pageNumber;
    return currentItem;
}

HistoryItemPtr CSettings::goForward()
{
    int last = forward.getLength();
    if (last <= 0)
    {
        return 0;
    }

    backward.append(currentItem);
    currentItem = (HistoryItemPtr)forward.del(last - 1);
    currentPage = currentItem->pageNumber;
    return currentItem;
}

GBool CSettings::canGoBack()
{
    return (backward.getLength() > 0);
}

GBool CSettings::canGoForward()
{
    return (forward.getLength() > 0);
}

#if (HISTORY_DEBUG)
void CSettings::dumpHistory()
{
    printf("backward pages: ");
    int count = backward.getLength();
    for(int i = 0; i < count; ++i)
    {
        HistoryItemPtr ptr = (HistoryItemPtr)backward.get(i);	    
        printf("%d\t", ptr->pageNumber);
    }
    printf("\n");

    printf("forward pages: ");
    count = forward.getLength();
    for(int i = 0; i < count; ++i)
    {
        HistoryItemPtr ptr = (HistoryItemPtr)forward.get(i);
        printf("%d\t", ptr->pageNumber);
    }
    printf("\n");
    printf("current page %d\n", currentItem->pageNumber);
}
#endif

