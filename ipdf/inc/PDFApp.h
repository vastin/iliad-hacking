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

#ifndef PDFAPP_H_
#define PDFAPP_H_

#include "controller.h"
#include "goo/GooList.h"
#include "goo/GooString.h"
#define MAX_PATH    1024

//////////////////////////////////////////////////////////////////////////
// CPDFApp:
// 1. construct all data structure
// 2. application instance
//////////////////////////////////////////////////////////////////////////
class CPDFApp
{
private:
    Controller  controller; // serves as glue
    char        path[MAX_PATH];
    
protected:
    void        initialize();
    void        setupX(int * argc, char *argv[]);
    void        clear();
    void        loadSettings(GooString * fileName);
    void        saveSettings();
    void        loadScribble(GooString * fileName, GBool bEnable = gTrue);
    
public:
    CPDFApp(int * argc, char *argv[]);
    ~CPDFApp(void);
    
public:
    // methods: 
    // start -> run -> stop -> start -> run -> stop ... -> quit
    GBool       open(GooString * fileName, 
                    GooString * ownerPassword = NULL, 
                    GooString * userPassword = NULL);
    void        run();
    void        close();
    void        quit(GBool bNormal = gTrue);
    void *      getAuthData();
};

#endif
