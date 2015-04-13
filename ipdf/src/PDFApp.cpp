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
 
#include "PDFApp.h"
#include "poppler/Object.h"
#include "config.h"
#include "poppler/GlobalParams.h"
#include "PDFViewerLog.h"
#include "poppler/SecurityHandler.h"


CPDFApp::CPDFApp(int * argc, char *argv[])
{
    controller.app = this;
    setupX(argc, argv);
    initialize();
}

CPDFApp::~CPDFApp(void)
{

}

void CPDFApp::clear()
{
    close();
    delete controller.core; controller.core = NULL;
    delete controller.view; controller.view = NULL;
    PV_LOGPRINTF("Going to remove globalParams %p", globalParams);
    delete globalParams; globalParams = NULL;
}

void CPDFApp::initialize()
{
    globalParams = new GlobalParams("");

    // initialize core
    SplashColor     white;
    white[0] = 255;
    white[1] = 255;
    white[2] = 255;
    controller.core = new CPDFCore(splashModeRGB8, 4, gFalse, white);
    controller.core->setController(&controller);
    
    // Initialize controller. notice!!! 
    // All controller parameters should be initialized here
    // there are only 3 pointers in controller

    // start render thread
    controller.thread.start(&controller.param);
}

void CPDFApp::run()
{
    gtk_main();
}

// it should be the only exit entry, this function could be called
// by viewer and terminate handler
void CPDFApp::quit(GBool bNormal)
{
    // clear viewer 
    clear();

    // save settings 
    if (bNormal)
    {
        saveSettings();
        gtk_main_quit ();
    }
    else
    {
        delete this;
        _exit(0);
    }        
}

void CPDFApp::close()
{
    controller.thread.stop();
    PV_LOGPRINTF("Stop Render Thread!");
    controller.core->close();
    PV_LOGPRINTF("Clear PDFCore!");
    controller.pageList.clear();
    PV_LOGPRINTF("Clear page list!");
    controller.todoList.clear(); 
    PV_LOGPRINTF("Clear todo list!");  
    controller.view->onClose();
}

void CPDFApp::setupX(int * argc, char *argv[])
{
    // setup X
    controller.gtkMgr.setupWindow();

    // Initialize view
    controller.view = new CPDFView(controller.gtkMgr.window, &controller);  
}

// check the page number, zoom factor and rotation
void CPDFApp::loadSettings(GooString * fileName)
{
    // load settings from manifest file
    // filename is the container path
    strncpy(path, fileName->getCString(), MAX_PATH);
    char * p = strrchr(path, '/');
    *p = 0;
    controller.settings.load(path);
    controller.validateSettings();
}

// load scribble 
void CPDFApp::loadScribble(GooString * fileName, GBool bEnable)
{
#if (PV_PROFILE_ON)
    int t1 = controller.getHighTimeStamp();
#endif 

    if (bEnable)
    {
        controller.scbMgr.open(fileName->getCString());
        controller.channels.initScribbleState(controller.scbMgr.getScbDoc());
    }
    else
    {
        PV_LOGPRINTF("Inhibit scribble by manifest!");
        controller.channels.initScribbleState(0);        
    }        

#if (PV_PROFILE_ON)
    int t2 = controller.getHighTimeStamp();
    PV_DUMP("CPDFApp::loadScribble uses %d\n", t2 - t1);
#endif 
}

void CPDFApp::saveSettings()
{
    controller.settings.save(path);
}

GBool CPDFApp::open(GooString * fileName, 
                 GooString * ownerPassword, 
                 GooString * userPassword)
{
    PV_LOGPRINTF("Open document %s", fileName->getCString());
    if (errNone == controller.core->open(fileName, ownerPassword, userPassword, controller.core))
    {
        PV_LOGPRINTF("Open document %s done!", fileName->getCString());

        // initialize page list
        PDFDoc * pdfdoc = controller.core->getPDFDoc();
        controller.pageList.reset(pdfdoc->getNumPages());

        // load settings
        loadSettings(fileName);

        // load scribble
        loadScribble(fileName, controller.settings.isScribbleEnabled());                

        // initialize the view according to settings
        controller.view->initializeViews();

        // render page(s)
        controller.view->displayPage(controller.settings.getCurrentPage(), ScrollToPosition);
        
        // init page bar
        controller.channels.pagebar_set_count(pdfdoc->getNumPages());
        return gTrue;
    }
    PV_ERRORPRINTF("Could not open file!");
    return gFalse;
}


void * CPDFApp::getAuthData()
{
    GooString * password = NULL;
    if (controller.view)
    {
        password = controller.view->getPassword();
        return password;
    }    
    return NULL;
}
