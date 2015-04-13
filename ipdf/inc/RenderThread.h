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

#ifndef RENDERTHREAD_H_
#define RENDERTHREAD_H_
#include "PageInfo.h"
#include <signal.h>
#include <pthread.h>


//////////////////////////////////////////////////////////////////////////
// Render Thread controller
//////////////////////////////////////////////////////////////////////////
struct Controller;
class CRenderThread;
struct ThreadParam
{
    TodoList *  todoList;
    PageList *  pageList;
    GBool       bQuit;
    Controller* ctrl;
    CRenderThread * thread;
};

class CRenderThread
{
private:
    enum threadState
    {
        Initialized     = 0, 
        Rendering       = 1, 
        RenderFinished  = 2,
        ToAbort         = 3,
    };

    pthread_t       thread; // render thread
    pthread_cond_t  cond;   // condition variable
    pthread_mutex_t mutex;  // to protect cond
    pthread_mutex_t abort;  // to protect abort variables
    GBool           bRunning;
    CPageInfo *     renderingPage;
    ThreadParam *   param;

    void    initialize();
    void    destroy();
    void    adjustPageList(const int center);       // memory control
    static void *  threadProc(void *); 
    static GBool   abortCheckFunc(void *data);
    static threadState abortState;  // used to control abort
       
public:
    CRenderThread(void);
    ~CRenderThread(void);
public:
    void    start(ThreadParam * param);
    void    stop();
    void    clearTodoList();
    void    addTodoItem(TodoItem * item, GBool bHead = gTrue);
    void    signal();
    void    renderPageNow(TodoItem * item);     // abort & render right now
    GBool   isInRendering(TodoItem * item);     // query state
};

#endif
