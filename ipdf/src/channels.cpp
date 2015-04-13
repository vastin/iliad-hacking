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

#include "channels.h"
#include "liberipc/eripcviewer.h"
#include "PDFViewerLog.h"

CChannels::CChannels()
: doc(NULL)
, tbUpdateType(dmQUndefined)
{
    initialize();
}

CChannels::~CChannels()
{
}

void CChannels::initialize()
{
    // busy indicator
    erIpcStartClient(ER_BUSYD_CHANNEL, &erbusyChannel);

    // pagebar
    erIpcStartClient(ER_PAGEBAR_CHANNEL, &erpagebarChannel);
    
    // toolbar 
    erIpcStartClient(ER_TOOLBAR_CHANNEL, &ertoolbarChannel);
    tbSelectIconSet(ertoolbarChannel, ER_PDF_VIEWER_UA_ID);
    
    // disable redraw
    tbDisableUpdate(ertoolbarChannel, ER_PDF_VIEWER_UA_ID); 
    tbClearIconSet(ertoolbarChannel, ER_PDF_VIEWER_UA_ID);
    
    // trashcan
    tbAppendPlatformIcon(  ertoolbarChannel, ER_PDF_VIEWER_UA_ID, iconID_trashcan, -1);
    tbSetStatePlatformIcon(ertoolbarChannel, ER_PDF_VIEWER_UA_ID, iconID_trashcan, iconState_grey );
    tbAppendPlatformIcon(  ertoolbarChannel, ER_PDF_VIEWER_UA_ID, iconID_keyboard, -1);
    tbSetStatePlatformIcon(ertoolbarChannel, ER_PDF_VIEWER_UA_ID, iconID_keyboard, iconState_normal );
    
    // rotation
    tbAppendPlatformIcon(  ertoolbarChannel, ER_PDF_VIEWER_UA_ID, iconID_portrait_landscape, ccVwrToolbar);
    tbSetStatePlatformIcon(ertoolbarChannel, ER_PDF_VIEWER_UA_ID, iconID_portrait_landscape, iconState_normal );

    // page mode <--> continous mode
    tbAppendPlatformIcon(  ertoolbarChannel, ER_PDF_VIEWER_UA_ID, iconID_page_mode, ccVwrToolbar);
    tbSetStatePlatformIcon(ertoolbarChannel, ER_PDF_VIEWER_UA_ID, iconID_page_mode, iconState_normal );

    // zoom fit
    tbAppendPlatformIcon(  ertoolbarChannel, ER_PDF_VIEWER_UA_ID, iconID_zoomfit, ccVwrToolbar);
    tbSetStatePlatformIcon(ertoolbarChannel, ER_PDF_VIEWER_UA_ID, iconID_zoomfit, iconState_normal );
    
    // zoom in
    tbAppendPlatformIcon(  ertoolbarChannel, ER_PDF_VIEWER_UA_ID, iconID_zoomin, ccVwrToolbar);
    tbSetStatePlatformIcon(ertoolbarChannel, ER_PDF_VIEWER_UA_ID, iconID_zoomin, iconState_normal);

    // zoom back
    tbAppendPlatformIcon(  ertoolbarChannel, ER_PDF_VIEWER_UA_ID, iconID_zoomback, ccVwrToolbar);
    tbSetStatePlatformIcon(ertoolbarChannel, ER_PDF_VIEWER_UA_ID, iconID_zoomback, iconState_grey);

    // remove zoomout 
    // tbAppendPlatformIcon(  ertoolbarChannel, ER_PDF_VIEWER_UA_ID, iconID_zoomout, ccVwrToolbar);
    // tbSetStatePlatformIcon(ertoolbarChannel, ER_PDF_VIEWER_UA_ID, iconID_zoomout, iconState_normal );
    tbAppendPlatformIcon(  ertoolbarChannel, ER_PDF_VIEWER_UA_ID, iconID_pan, ccVwrToolbar);
    tbSetStatePlatformIcon(ertoolbarChannel, ER_PDF_VIEWER_UA_ID, iconID_pan, iconState_normal );
    
    // backward 
    tbAppendPlatformIcon(  ertoolbarChannel, ER_PDF_VIEWER_UA_ID, iconID_back, ccVwrToolbar);
    tbSetStatePlatformIcon(ertoolbarChannel, ER_PDF_VIEWER_UA_ID, iconID_back, iconState_grey );

    // forward
    tbAppendPlatformIcon(  ertoolbarChannel, ER_PDF_VIEWER_UA_ID, iconID_forward, ccVwrToolbar);
    tbSetStatePlatformIcon(ertoolbarChannel, ER_PDF_VIEWER_UA_ID, iconID_forward, iconState_grey);

    // draw
    tbEnableUpdate(ertoolbarChannel, ER_PDF_VIEWER_UA_ID); 
    
    nPanState = nZoominState = nZoomoutState = iconState_normal;
    nZoombackState = iconState_grey;
    nContinousState = iconState_normal;
    nRotateState = PortraitToLandscape;
    nBackwardState = nForwardState = iconState_grey;

    // scribble icons are initialized by other function.
}

//////////////////////////////////////////////////////////////////////////
// update toolbar icons according to input
void CChannels::updateScribbleIcons(ScbDocPtr pDoc)
{
    // scribble could be disabled by manifest file.
    if (pDoc == 0) 
    {
        tbAppendPlatformIcon(ertoolbarChannel, ER_PDF_VIEWER_UA_ID, iconID_pen, ccVwrToolbar);
        tbSetStatePlatformIcon(ertoolbarChannel, ER_PDF_VIEWER_UA_ID, iconID_pen, iconState_grey);
        return;
    }        

    int index  = 0;
    // scribble icon
    tbAppendPlatformIcon(ertoolbarChannel, ER_PDF_VIEWER_UA_ID, iconID_pen, ccVwrToolbar);
    if (SCB_TBS_SCRIBBLE == pDoc->context.curState)
    {
        tbSetStatePlatformIcon(ertoolbarChannel, ER_PDF_VIEWER_UA_ID, iconID_pen, iconState_selected);
    }
    
    // line icon
    if (SCB_TBS_SELECT_PEN_SIZE == pDoc->context.curState)
    {
        for(index = iconID_pen1pixelLow; index <= iconID_pen7pixelLow; ++index)
        {
            tbAppendPlatformIcon(ertoolbarChannel, ER_PDF_VIEWER_UA_ID, index, ccVwrToolbar);
        }
        index = (int)iconID_pen1pixelLow + pDoc->context.curStrokeStyle.penSize/2;
        tbSetStatePlatformIcon(ertoolbarChannel, ER_PDF_VIEWER_UA_ID, index, iconState_selected);
    }
    else
    {
        // line icon
        index = (int)iconID_pen1pixel + doc->context.curStrokeStyle.penSize/2;
        tbAppendPlatformIcon(ertoolbarChannel, ER_PDF_VIEWER_UA_ID, index, ccVwrToolbar);
    }

    // color icon
    if (SCB_TBS_SELECT_LINE_COLOR == pDoc->context.curState)
    {
        for(index = iconID_penwhiteLow ; index >=iconID_penblackLow; --index)
        {
            tbAppendPlatformIcon(ertoolbarChannel, ER_PDF_VIEWER_UA_ID, index, ccVwrToolbar);
        }
        index = (int)iconID_penblackLow + (int)SCB_DEV_COLOR_BLACK - (int)doc->context.curStrokeStyle.color;
        tbSetStatePlatformIcon(ertoolbarChannel, ER_PDF_VIEWER_UA_ID, index, iconState_selected);
    }
    else
    {
        index = (int)iconID_penblack + (int)SCB_DEV_COLOR_BLACK - (int)doc->context.curStrokeStyle.color;
        tbAppendPlatformIcon(ertoolbarChannel, ER_PDF_VIEWER_UA_ID, index, ccVwrToolbar);
    }

    // erase icon
    tbAppendPlatformIcon(ertoolbarChannel, ER_PDF_VIEWER_UA_ID, iconID_eraseline, ccVwrToolbar);
    if (SCB_TBS_ERASE == pDoc->context.curState)
    {
        tbSetStatePlatformIcon(ertoolbarChannel, ER_PDF_VIEWER_UA_ID, iconID_eraseline, iconState_selected);
    }
}

void CChannels::initScribbleState(ScbDocPtr pDoc)
{
    if (pDoc)
    {
        doc = pDoc;
        doc->context.curState = SCB_TBS_NONE;
    }        
    tbDisableUpdate(ertoolbarChannel, ER_PDF_VIEWER_UA_ID);   
    updateScribbleIcons(doc);
    tbEnableUpdate(ertoolbarChannel, ER_PDF_VIEWER_UA_ID);   
}

GBool CChannels::isScribbleIcon(const int id)
{
    if ((iconID_pen == id) ||
        (iconID_eraseline == id) ||
        (iconID_pen1pixel <= id &&  iconID_pen7pixel >= id) ||
        (iconID_penblack <= id && iconID_penwhite >= id))
    {
        return gTrue;
    }        
    return gFalse;
}

//////////////////////////////////////////////////////////////////////////
// change state
void CChannels::onScribbleIconsClicked(const int iconID, const int state)
{
    if (doc == 0)
    {
        return;
    }

    // zoom in/out and pan
    if (iconState_selected == nZoominState)
    {
        setToolbarDirty();
        setZoomIn(iconState_normal);
    }
    if (iconState_selected == nZoomoutState)
    {
        setToolbarDirty();
        setZoomOut(iconState_normal);
    }
    if (iconState_selected == nPanState)
    {
        setToolbarDirty();
        setPanState(iconState_normal);
    }      

    if (iconID == iconID_pen)
    {
        if (SCB_TBS_SCRIBBLE == doc->context.curState)
        {
            doc->context.curState = SCB_TBS_NONE;
        }
        else
        {
            doc->context.curState = SCB_TBS_SCRIBBLE;
        }
        setToolbarDirty(); 
    }
    else if (iconID_pen1pixel <= iconID && (int)iconID_pen7pixel >= iconID)
    {
        if (SCB_TBS_SELECT_PEN_SIZE != doc->context.curState)
        {
            doc->context.curState = SCB_TBS_SELECT_PEN_SIZE;
        }
        setToolbarDirty();
    }
    else if (iconID_pen1pixelLow <= iconID && iconID_pen7pixelLow >= iconID)
    {
        doc->context.curState = SCB_TBS_SCRIBBLE;
        doc->context.curStrokeStyle.penSize = ((int)iconID - (int)iconID_pen1pixelLow) * 2 + 1;
        setToolbarDirty();
    }
    else if ((int)iconID_penblack <= iconID && (int)iconID_penwhite >= iconID)
    {
        if (SCB_TBS_SELECT_LINE_COLOR != doc->context.curState)
        {
            doc->context.curState = SCB_TBS_SELECT_LINE_COLOR;
        }
        setToolbarDirty();
    }
    else if (iconID_penblackLow <= iconID && iconID_penwhiteLow >= iconID)
    {
        doc->context.curState = SCB_TBS_SCRIBBLE;
        doc->context.curStrokeStyle.color = (ScbDevColor)((int)SCB_DEV_COLOR_BLACK - ((int)iconID - (int)iconID_penblackLow));
        setToolbarDirty();   
    }
    else if (iconID_eraseline == iconID)
    {
        if (SCB_TBS_ERASE != doc->context.curState)
        {
            doc->context.curState = SCB_TBS_ERASE;
        }
        else
        {
            doc->context.curState = SCB_TBS_NONE;
        }
        setToolbarDirty();
    }

    if (isToolbarDirty())
    {
        setToolbarUpdateType();
    }
}


//////////////////////////////////////////////////////////////////////////
// busy indicator
void CChannels::busy_blink()
{
    busySetBusy(erbusyChannel, ccBusyState_Blink);
}
 
void CChannels::busy_on()
{
    busySetBusy(erbusyChannel, ccBusyState_On);
}

void CChannels::busy_off()
{
    busySetBusy(erbusyChannel, ccBusyState_Off);
}

//////////////////////////////////////////////////////////////////////////
// page bar
void CChannels::pagebar_set_count(const int count)
{
    pbReset(erpagebarChannel, ER_PDF_VIEWER_UA_ID);
    pbSetPageCount(erpagebarChannel, ER_PDF_VIEWER_UA_ID, count);
    pbRedraw(erpagebarChannel, ER_PDF_VIEWER_UA_ID);

}

void CChannels::pagebar_set_current_page(const int number)
{
    pbSetCurrentPage(erpagebarChannel, ER_PDF_VIEWER_UA_ID, number);
    pbRedraw(erpagebarChannel, ER_PDF_VIEWER_UA_ID);
    pbSynchronise(erpagebarChannel, ER_PDF_VIEWER_UA_ID);   
}

// invoked when user clicks the icons
GBool CChannels::onReceiveToolbarMsg(const int iconID, const int state)
{
    if (iconID == iconID_zoomin)
    {
        if (iconState_selected == nZoominState)
        {
            setToolbarDirty();
            setZoomIn(iconState_normal);
        }
        else if (iconState_normal == nZoominState)
        {
            setToolbarDirty();
            setZoomIn(iconState_selected);
            if (isZoomOutEnable())
            {
                setZoomOut(iconState_normal);
            }
            if (isPanEnable())
            {
                setPanState(iconState_normal);
            }  
            setScribbleNone(doc);
        }            
        PV_TBPRINTF("Zoomin %d\n", nZoominState);
    }
    else if (iconID == iconID_zoomout)
    {
        if (iconState_selected == nZoomoutState)
        {
            setToolbarDirty();
            setZoomOut(iconState_normal);
        }
        else if (iconState_normal == nZoomoutState)
        {
            setToolbarDirty();
            setZoomOut(iconState_selected); 
            if (isZoomInEnable())
            {
                setZoomIn(iconState_normal);   
            }  
            if (isPanEnable())
            {
                setPanState(iconState_normal);
            }   
            setScribbleNone(doc);                                   
        }
    }
    else if (iconID == iconID_pan)
    {
        if (iconState_normal == nPanState)
        {
            setToolbarDirty();
            setPanState(iconState_selected);
            if (isZoomInEnable())
            {
                setZoomIn(iconState_normal);   
            }  
            if (isZoomOutEnable())
            {
                setZoomOut(iconState_normal);   
            }  
            setScribbleNone(doc);
        }
        else if (iconState_selected == nPanState)
        {
            setToolbarDirty();
            setPanState(iconState_normal);
        }  
        PV_TBPRINTF("Pan %d\n", nPanState);          
    }
    else if ((iconID == iconID_pen) || 
             (iconID >= iconID_pen1pixel && iconID <= iconID_pen7pixel) ||
             (iconID >= iconID_pen1pixelLow && iconID <= iconID_pen7pixelLow) ||
             (iconID >= iconID_penblackLow && iconID <= iconID_penwhiteLow) ||
             (iconID >= iconID_penblack && iconID <= iconID_penwhite) ||
             (iconID == iconID_eraseline))
    {
        onScribbleIconsClicked(iconID, state);
    }
    return isToolbarDirty();
}
 
void CChannels::redrawToolbar(GBool bSync)
{
    if (isToolbarDirty())
    {
        // avoid redraw again and again, it's more faster
        tbDisableUpdate(ertoolbarChannel, ER_PDF_VIEWER_UA_ID);   
        tbClearIconSet(ertoolbarChannel, ER_PDF_VIEWER_UA_ID);
        
        // rotate
        if (PortraitToLandscape == nRotateState)
        {
            tbAppendPlatformIcon(  ertoolbarChannel, ER_PDF_VIEWER_UA_ID, iconID_portrait_landscape, ccVwrToolbar);
            tbSetStatePlatformIcon(ertoolbarChannel, ER_PDF_VIEWER_UA_ID, iconID_portrait_landscape, iconState_normal);
        }
        else if (LandscapeToPortrait == nRotateState)
        {
            tbAppendPlatformIcon(  ertoolbarChannel, ER_PDF_VIEWER_UA_ID, iconID_landscape_portrait, ccVwrToolbar);
            tbSetStatePlatformIcon(ertoolbarChannel, ER_PDF_VIEWER_UA_ID, iconID_landscape_portrait, iconState_selected);
        }

        // page/continous mode 
        if (iconState_normal == nContinousState)
        {
            tbAppendPlatformIcon(ertoolbarChannel, ER_PDF_VIEWER_UA_ID, iconID_page_mode, ccVwrToolbar);
            tbSetStatePlatformIcon(ertoolbarChannel, ER_PDF_VIEWER_UA_ID, iconID_page_mode, iconState_normal);
        }
        else if (3 == nContinousState)
        {
            tbAppendPlatformIcon(ertoolbarChannel, ER_PDF_VIEWER_UA_ID, iconID_continuous_mode, ccVwrToolbar);
            tbSetStatePlatformIcon(ertoolbarChannel, ER_PDF_VIEWER_UA_ID, iconID_continuous_mode, iconState_selected);
        }            

        // zoom fit
        tbAppendPlatformIcon(  ertoolbarChannel, ER_PDF_VIEWER_UA_ID, iconID_zoomfit, ccVwrToolbar);
        tbSetStatePlatformIcon(ertoolbarChannel, ER_PDF_VIEWER_UA_ID, iconID_zoomfit, iconState_normal );
    
        // zoom in
        tbAppendPlatformIcon(  ertoolbarChannel, ER_PDF_VIEWER_UA_ID, iconID_zoomin, ccVwrToolbar);
        tbSetStatePlatformIcon(ertoolbarChannel, ER_PDF_VIEWER_UA_ID, iconID_zoomin, nZoominState);

        // zoom back
        tbAppendPlatformIcon(  ertoolbarChannel, ER_PDF_VIEWER_UA_ID, iconID_zoomback, ccVwrToolbar);
        tbSetStatePlatformIcon(ertoolbarChannel, ER_PDF_VIEWER_UA_ID, iconID_zoomback, nZoombackState);

        // temp. 
        tbAppendPlatformIcon(  ertoolbarChannel, ER_PDF_VIEWER_UA_ID, iconID_pan, ccVwrToolbar);
        tbSetStatePlatformIcon(ertoolbarChannel, ER_PDF_VIEWER_UA_ID, iconID_pan, nPanState );

        // backward
        tbAppendPlatformIcon(  ertoolbarChannel, ER_PDF_VIEWER_UA_ID, iconID_back, ccVwrToolbar);
        tbSetStatePlatformIcon(ertoolbarChannel, ER_PDF_VIEWER_UA_ID, iconID_back, nBackwardState);
    
        // forward
        tbAppendPlatformIcon(  ertoolbarChannel, ER_PDF_VIEWER_UA_ID, iconID_forward, ccVwrToolbar);
        tbSetStatePlatformIcon(ertoolbarChannel, ER_PDF_VIEWER_UA_ID, iconID_forward, nForwardState);
        
        // scribble icons        
        updateScribbleIcons(doc);
        
        // repaint only once
        tbEnableUpdate(ertoolbarChannel, ER_PDF_VIEWER_UA_ID);   
        
        if (bSync)
        {
            tbSynchronise(ertoolbarChannel, ER_PDF_VIEWER_UA_ID, ccVwrToolbarSync);
        }            
        setToolbarDirty(gFalse);
    }
}

void CChannels::setScribbleNone(ScbDocPtr pDoc)
{
    if (pDoc)
    {
        pDoc->context.curState = SCB_TBS_NONE;
    }        
}

void CChannels::popupKeyboard(GBool bPopup)
{   
    if (bPopup)
    {
        tbSetStatePlatformIcon(ertoolbarChannel, ER_PDF_VIEWER_UA_ID, iconID_keyboard, iconState_selected);
    }        
    else
    {
        tbSetStatePlatformIcon(ertoolbarChannel, ER_PDF_VIEWER_UA_ID, iconID_keyboard, iconState_normal);
    }
}



