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

#ifndef CHANNELS_H_
#define CHANNELS_H_

#include <liberipc/eripc.h>
#include <liberipc/eripcbusyd.h>
#include <liberipc/eripcpagebar.h>
#include <liberipc/eripctoolbar.h>
#include <liberdm/erdm.h>
#include "goo/gtypes.h"
#include <liberscribble/scb.h>


//////////////////////////////////////////////////////////////////////////
// All channels used by iLiad: busy indicator, pagebar, ipc and toolbar
//////////////////////////////////////////////////////////////////////////
#define PortraitToLandscape  90
#define LandscapeToPortrait  -90
#define RotateDirty          0

class CChannels
{
public:
    erClientChannel_t erbusyChannel;
    erClientChannel_t erpagebarChannel;
    erClientChannel_t ertoolbarChannel;
    erServerChannel_t erIpcChannel;

private:        
    // toolbar state, should use hash table. TBD
    int         nPanState;
    int         nZoominState;
    int         nZoomoutState;
    int         nRotateState;
    int         nContinousState;
    int         nZoombackState;
    int         nBackwardState;
    int         nForwardState;
    ScbDocPtr   doc;        // maintain scribble state
    GBool       tbDirty;    
    eDmQuality  tbUpdateType;
    
    // init all
    void    initialize();
public:
    CChannels();
    ~CChannels();

    /////////////////////////////// busy light //////////////////////////
    void    busy_blink();
    void    busy_on();
    void    busy_off();

    ////////////////////////////// page bar /////////////////////////////
    void    pagebar_set_count(const int count);
    void    pagebar_set_current_page(const int number);
    
    ////////////////////////////// toolbar /////////////////////////////
    GBool   onReceiveToolbarMsg(const int iconID, const int state); 
    void    setPanState(const int state) { nPanState = state; }
    GBool   isPanEnable() const { return iconState_selected == nPanState; }
    int     getPanState() const { return nPanState; }
    void    setZoomIn(const int state) { nZoominState = state; }
    GBool   isZoomInEnable() const { return iconState_selected == nZoominState; }
    int     getZoomInState() const { return nZoominState; }
    void    setZoomOut(const int state) { nZoomoutState = state; }
    GBool   isZoomOutEnable() const { return iconState_selected == nZoomoutState; }
    int     getZoomOutState() const { return nZoomoutState; }
    void    setRotateState(const int state) { nRotateState = state; }
    int     getRotateState() const { return nRotateState; }
    void    setContinousState(const int state) { nContinousState = state; }
    int     getContinousState() const { return nContinousState; }
    void    setZoombackState(const int state) { nZoombackState = state; }
    int     getZoombackState() const { return nZoombackState; }
    int     getBackwardState() const { return nBackwardState; }
    void    setBackwardState(const int state) { nBackwardState = state; }
    int     getForwardState() const { return nForwardState; }
    void    setForwardState(const int state) { nForwardState = state; }

    GBool   isToolbarDirty() const { return tbDirty;}    
    eDmQuality
            getToolbarUpdateType() const { return  tbUpdateType; }
    void    setToolbarDirty(GBool b = gTrue){ tbDirty = b;  }
    void    setToolbarUpdateType(const eDmQuality t = dmQTyping) { tbUpdateType = t;}
    void    redrawToolbar(GBool bSync = gTrue); 
    void    popupKeyboard(GBool bPopup = gTrue);        

    //////////////////////////// for scribble ////////////////////////////
    void    updateScribbleIcons(ScbDocPtr pDoc);
    void    initScribbleState(ScbDocPtr pDoc);
    void    onScribbleIconsClicked(const int iconID, const int state);
    void    setScribbleNone(ScbDocPtr pDoc);

    GBool   isScribbleEnable() const { return (doc && doc->context.curState == SCB_TBS_SCRIBBLE); }
    GBool   isEraseEnable() const { return (doc && doc->context.curState == SCB_TBS_ERASE); }
    GBool   isScribbleIcon(const int id);
};


#endif

