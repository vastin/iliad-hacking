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
 
#include "RenderThread.h"
#include <sys/time.h>
#include <time.h>
#include <errno.h>
#include "controller.h"
#include "PDFViewerLog.h"
#include <unistd.h>
#include <gdk/gdkx.h>

CRenderThread::CRenderThread(void)
: bRunning(gFalse)
{
    initialize();
}

CRenderThread::~CRenderThread(void)
{
    destroy();
}

CRenderThread::threadState CRenderThread::abortState = Initialized;

void CRenderThread::initialize()
{
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
    pthread_mutex_init(&abort, NULL);
    renderingPage = NULL;
}

void CRenderThread::destroy()
{
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    pthread_mutex_destroy(&abort);
}

GBool CRenderThread::abortCheckFunc(void *data)
{
    if (abortState == ToAbort)
    {
        // PV_LOGPRINTF("Abort page!");
        return gTrue;
    }
    return gFalse;
    
}

extern void OnTerminated(int sig);
static void installThreadTermHandler()
{
    // catch the SIGTERM signal
    struct sigaction on_term;
    memset(&on_term, 0x00, sizeof(on_term));
    on_term.sa_handler = OnTerminated;
    sigaction(SIGTERM, &on_term, NULL);
}

void CRenderThread::start(ThreadParam * p)
{
    if (bRunning) 
    {
        PV_ERRORPRINTF("Thread already running!");
        return;
    }
    param = p;
    param->bQuit = gFalse;
    param->thread = this;
    renderingPage = NULL;
    abortState = Initialized;
    if (pthread_create(&thread, NULL, threadProc, p) != 0)
    {
        PV_ERRORPRINTF("Could not create thread!");
        return;
    }
    PV_LOGPRINTF("Create render thread done!");
    bRunning = gTrue;
}

void CRenderThread::stop()
{
    if (!bRunning) return;

    abortState = Initialized;
    param->bQuit = gTrue;
    pthread_mutex_lock(&mutex);
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
    pthread_join(thread, NULL);
    bRunning = gFalse;
    renderingPage = NULL;
}

void CRenderThread::clearTodoList()
{
    param->todoList->clear();
}

void CRenderThread::addTodoItem(TodoItem * item, GBool bHead)
{
    param->todoList->pushItem(item, bHead);
}

void CRenderThread::signal()
{
    pthread_mutex_lock(&mutex);
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
}

void CRenderThread::adjustPageList(const int current)
{
    // memory       allocate         pages
    // 50MB  ------- 20 MB   ------   35 
    // 30MB  ------- 30 MB   ------    8 ( 250%)
    static const int MEM = 1024 * 1024 * 15;    // 49.1 MB used from experiment, ~ 35 pages cached

    {
        char * p = (char *)malloc(MEM);
        if (p)
        {
            // PV_MEM("Memory is enough %d bytes!", MEM);
            free(p);
            return;
        }
        else 
        {
            // PV_MEM("Not enough memory, try to release some pages!");
            for(int i = 0; i < RemovePages; ++i)
            {
                param->pageList->removeOldestOne(current);
            }                
        }
    }
}

void CRenderThread::renderPageNow(TodoItem * item)
{
    // if it's already rendering ignore the request
    pthread_mutex_lock(&abort);
    if (abortState == Rendering && 
        renderingPage &&
        renderingPage->isSame(item->pageNumber, item->zoom, item->rotate))
    {
        PV_LOGPRINTF("Page %d is in rendering!", item->pageNumber);
        delete item;
        pthread_mutex_unlock(&abort);
        return;
    }        

    if (abortState == Rendering)
    {
        abortState = ToAbort;
    }
    param->todoList->pushItem(item, gTrue);
    signal();
    pthread_mutex_unlock(&abort);
}

GBool CRenderThread::isInRendering(TodoItem * item)
{
    pthread_mutex_lock(&abort);
    if (abortState == Rendering && 
        renderingPage &&
        renderingPage->isSame(item->pageNumber, item->zoom, item->rotate))
    {
        pthread_mutex_unlock(&abort);
        return gTrue;
    }        
    pthread_mutex_unlock(&abort);
    return gFalse;
}


// ugly, I have to reuse KeyPress. 
// PDFView has to check the time field
static void postEvent(ThreadParam * param, CPageInfo *page)
{
    XKeyEvent xev;
    xev.type = KeyPress;
    xev.display = param->ctrl->gtkMgr.display;
    xev.root = RootWindow(param->ctrl->gtkMgr.display, param->ctrl->gtkMgr.screenNum);
    xev.window = GDK_WINDOW_XWINDOW(param->ctrl->gtkMgr.window->window);
    xev.subwindow = None;
    xev.time = 0;
    xev.x = page->pageNumber;
    xev.y = 1;
    xev.x_root = -1;
    xev.y_root = -1;
    xev.same_screen = 1;
    xev.state = page->pageNumber;
    xev.keycode = page->pageNumber; //XKeysymToKeycode(param->ctrl->xMgr.display, 0x0000);
    XSendEvent(param->ctrl->gtkMgr.display, 
        GDK_WINDOW_XWINDOW(param->ctrl->gtkMgr.window->window), 1, KeyPressMask, (XEvent *)&xev);
    XFlush(xev.display);
}

//////////////////////////////////////////////////////////////////////////
// rendering thread
// 1. check quit flag
// 2. check todo list
// 3. check page list
// 4. check memory
// 5. construct page item
// 6. record current rendering page
// 7. 
void * CRenderThread::threadProc(void * pvoid)
{
    // install 
    installThreadTermHandler();


    // param
    ThreadParam * param = (ThreadParam *)pvoid;

    // loop
    for(;;)
    {
        if (param->bQuit)
        {
            PV_LOGPRINTF("Quit from thread!");
            return NULL;
        }
        
        // check todo list
        TodoItem *pItem = param->todoList->popItem();
        if (NULL == pItem) 
        {
            pthread_mutex_lock(&param->thread->mutex);
            PV_LOGPRINTF("Waiting...");
            pthread_cond_wait(&param->thread->cond, &param->thread->mutex); 
            pthread_mutex_unlock(&param->thread->mutex);
            continue;   // quit or do something 
        }
    
        // check page list 
        CPageInfo * page = param->pageList->checkPage(pItem);
        if (page) 
        {
            delete pItem;
            continue;
        }
        
        // memory control
        param->thread->adjustPageList(param->ctrl->settings.getCurrentPage());
                    
        // construct a new page item
        page = new CPageInfo;
        page->pageNumber = pItem->pageNumber;
        page->pageZoom   = pItem->zoom;
        page->rotate     = pItem->rotate;   // global settings
        page->timestamp  = pItem->timeStamp;
        delete pItem;
        
        // protect, renderingPage could be accessed by main thread and helper thread in same time
        pthread_mutex_lock(&param->thread->abort);
        param->thread->renderingPage = page;
        param->thread->abortState = Rendering;
        pthread_mutex_unlock(&param->thread->abort);
        
        // go
        RenderRet ret = param->ctrl->core->renderPage(*page, 
                gFalse, gTrue, gTrue, abortCheckFunc, NULL);
    
        // reset                
        pthread_mutex_lock(&param->thread->abort);
        param->thread->renderingPage = NULL;
        param->thread->abortState = RenderFinished;
        pthread_mutex_unlock(&param->thread->abort);                                        

        // check result
        if (ret != Render_Done)
        {
            if (ret == Render_Abort)
            {
                PV_LOGPRINTF("Page %d is aborted!", page->pageNumber);
            }
            delete page;
            continue;
        }
        // to protect page, use request time stamp
        // page->timestamp = param->ctrl->getTimeStamp();
        param->pageList->addPage(page);

        // send a message to the main window
        postEvent(param, page);
    }
    return NULL;
}
