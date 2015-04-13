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
 
#include <stdio.h>
#include "goo/GooList.h"
#include "poppler/GlobalParams.h"
#include "poppler/Object.h"
#include "poppler/Link.h"
#include "goo/GooString.h"
#include "poppler/PDFDoc.h"
#include "splash/SplashBitmap.h"
#include "poppler/SplashOutputDev.h"
#include "splash/SplashTypes.h"
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include "PDFApp.h"
#include "PDFViewerLog.h"
#include <gtk/gtk.h>
#include <sys/time.h>
#include <sys/resource.h>

CPDFApp * pApp = NULL;
pid_t AppID;

// terminated handler. could be called by main thread and render thread
void OnTerminated(int sig)
{
    PV_DUMP("On receive signal %d pid %d\n", sig, getpid());
    {
        AppID = 0;
        pApp->quit();
        delete pApp;
        pApp = NULL;
    }        
    exit(0);
}


// simple usage
void usage()
{
    printf("\nUsage: ipdf <filename>\n");
}

void installTermHandler()
{
    // catch the SIGTERM signal
    struct sigaction on_term;
    memset(&on_term, 0x00, sizeof(on_term));
    on_term.sa_handler = OnTerminated;
    sigaction(SIGTERM, &on_term, NULL);
    
    // Also install the SIGSEGV/SIGABRT
    sigaction(SIGSEGV, &on_term, NULL);
    sigaction(SIGABRT, &on_term, NULL);

    // record the id
    AppID = getpid();
}

// limit the amount of memory our process (including threads) can occupy
void setMemLimit()
{
    struct rlimit limit;
    int ret = getrlimit(RLIMIT_AS, &limit);
    limit.rlim_cur = MAXIMUM_MEMORY_LIMIT;
    ret = setrlimit(RLIMIT_AS, &limit);
}

int main(int argc, char * argv[])
{
    if (argc != 2) 
    {
        usage();
        return 0;
    }

    // terminate handle
    installTermHandler();

    // set memory limit
    setMemLimit();

    // init gtk
    gtk_rc_parse(DATA_DIR "/ipdf.rc"); 
    gtk_init (&argc, &argv);

    // name will be removed by PDFDoc, so no need to delete in main
    GooString *name = new GooString(argv[1]);   
    
    // init application
    pApp = new CPDFApp(&argc, argv);    
    
    // open file and render
    if (pApp->open(name))
    {
        pApp->run();
    }        
    
    delete pApp;
    pApp = NULL;
    return 0;    
}


// overwrite the standard handler
void *StandardSecurityHandler::getAuthData() 
{
    if (pApp)
    {
        GooString * password = (GooString *)pApp->getAuthData();
        if (password)
        {
            return new StandardAuthData(password, password->copy()); 
        }
        return NULL;            
    }
    return NULL;
}
















