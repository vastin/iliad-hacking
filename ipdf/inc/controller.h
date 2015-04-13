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

#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include "PageInfo.h"
#include "PDFCore.h"
#include "RenderThread.h"
#include "GtkMgr.h"
#include "PDFView.h"
#include "RenderThread.h"
#include <sys/time.h>
#include <liberdm/display.h>
#include "channels.h"
#include "Settings.h"
#include "ScribbleMgr.h"

//////////////////////////////////////////////////////////////////////////
//
// controller serves as access pointer, from which every component can 
// access all the other components. 
// controller is initialized, maintained and destroyed by CPDFApp class
//
// settings contains all global parameters. 
// NOTICE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// When settings.zoom is larger than value calculated from page type
// We should use the value without changing settings.zoom
//
// we should check zoom max value when
// 1. issues a new request 
// 2. receives a message from render thread ***
// 3. query a page in page list
//
//////////////////////////////////////////////////////////////////////////

class CPDFApp;
struct Controller
{
private:
    struct timeval   startStamp;
    struct timeval   now;    
    void initTimeStamp()
    {
        gettimeofday(&startStamp, NULL);
    }
public:
    CPDFApp  *      app;        // becomes NULL when quit from app
    CPDFCore *      core;       // same as app
    CPDFView *      view;       // same as app
    PageList        pageList;   // delete only when quit from app
    TodoList        todoList;   // same as pageList
    GtkMgr          gtkMgr;     // gtk related function
    CSettings       settings;   // global settings
    CRenderThread   thread;
    ThreadParam     param;
    CChannels       channels;   // all channels in iLiad
    int             updateReq;  // used to control update, when it reaches to 0, update screen
                                // it can only be increased by toolbar and pagebar
    CScribbleMgr    scbMgr;

    Controller()
    {
        core = NULL; view = NULL; app = NULL;
        param.bQuit = gFalse;
        param.pageList = &pageList;
        param.todoList = &todoList;
        param.ctrl = this;
        updateReq = 0;
        initTimeStamp();
    }

    int getTimeStamp()
    {
        gettimeofday(&now, NULL);
        return (now.tv_sec - startStamp.tv_sec);
    }        
    
    int getHighTimeStamp()
    {
        gettimeofday(&now, NULL);
        return (now.tv_sec - startStamp.tv_sec) * 1000000 +
                (now.tv_usec - startStamp.tv_usec);
    }

    void validateSettings();
    double calZoom(const int page);
};

#endif

