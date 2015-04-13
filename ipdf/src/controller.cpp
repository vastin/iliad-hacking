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
 
#include "controller.h"
#include "PDFViewerLog.h"
#include <libermanifest/ermanifest.h>

void Controller::validateSettings()
{
    // validate
    PDFDoc * pdfdoc = core->getPDFDoc();    
    if (settings.getCurrentPage() <= 0 ||
        settings.getCurrentPage() > pdfdoc->getNumPages())
    {
        settings.setCurrentPage(1);
    }        

    if ((settings.getZoom() > ZoomMax  ||
        settings.getZoom() < ZoomMin ) &&
        settings.getZoom() != ZoomWidth)
    {
        settings.setZoom(ZoomPage);
    }        

    if (settings.getRotate() != 0 &&
        settings.getRotate() != 90 &&
        settings.getRotate() != 180 &&
        settings.getRotate() != 270)
    {
        settings.setRotate(0);
    }        

    if (settings.getRotate() == 180)
    {
        settings.setRotate(0);
    }
    else if (settings.getRotate() == 90)
    {
        settings.setRotate(270);
    }

    PV_LOGPRINTF("Current page %d zoom factor %lf rotate %d x %d y %d mode %d\n", 
        settings.getCurrentPage(), 
        settings.getZoom(), 
        settings.getRotate(),         
        settings.getScreenX(),
        settings.getScreenY(),
        settings.getMode());
}

double Controller::calZoom(const int page)
{
    if (page < 1 ||
        page > core->getNumPages())
    {
        return DefZoom;
    }

    if (settings.getZoom() > 0)
    {
        return core->verifyZoomMax(settings.getZoom(), page);
    }

    int rot = core->getPageRotate(page);
    rot += settings.getRotate();
    if (rot >= 360) rot -= 360;
    else if (rot < 0) rot += 360;
    double uw = core->getPageWidth(page);
    double uh = core->getPageHeight(page);
    double ut;
    if (rot == 90 || rot == 270)
    {
        ut = uw; uw = uh; uh = ut;
    }
    if (ZoomPage == settings.getZoom())
    {
        uw = settings.getScreenWidth() / uw;
        uh = settings.getScreenHeight() / uh;
        return  (uw < uh) ? (uw * 100) : (uh * 100);
    }
    else if (ZoomWidth == settings.getZoom())
    {
        if (settings.getRotate() == 0 || 
            settings.getRotate() == 180)
        {   
            return (100 * settings.getScreenWidth() / uw);
        }
        else if (settings.getRotate() == 90 || 
            settings.getRotate() == 270)            
        {
            return (100 * settings.getScreenHeight() / uh);
        }
    }
    return DefZoom;
}

