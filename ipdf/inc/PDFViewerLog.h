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

#ifndef _PDFVIEWER_LOG_H_
#define _PDFVIEWER_LOG_H_

#define PV_LOGGING_ON       0 
#define PV_WARNING_ON       1
#define PV_ERROR_ON         1
#define PV_DUMP_ON          1
#define PV_MEM_ON           1
#define PV_LIST_ON          1
#define PV_PROFILE_ON       1
#define PV_SCB_ON           0
#define PV_TOOLBAR_ON       0 

#define SYSLOG_ON           1
#if (SYSLOG_ON)
#include <syslog.h>
#endif

#if (PV_LOGGING_ON)
#if (SYSLOG_ON)
#define PV_LOGPRINTF(x, args...) \
{\
    syslog(LOG_INFO, "(PV_L)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
    fprintf(stderr, "(PV_L)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
}
#else
#define PV_LOGPRINTF(x, args...) fprintf(stderr, "(PV_L)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#endif
#else
#define PV_LOGPRINTF(x, args...) do {} while (0)
#endif

#if (PV_WARNING_ON)
#if (SYSLOG_ON)
#define PV_WARNPRINTF(x, args...) \
{\
    syslog(LOG_WARNING, "(PV_W)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
    fprintf(stderr, "(PV_W)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
}
#else
#define PV_WARNPRINTF(x, args...) fprintf(stderr, "(PV_W)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#endif
#else
#define PV_WARNPRINTF(x, args...) do {} while (0)
#endif

#if (PV_ERROR_ON)
#if (SYSLOG_ON)
#define PV_ERRORPRINTF(x, args...) \
{\
    syslog(LOG_ERR, "(PV_E)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
    fprintf(stderr, "(PV_E)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
}
#else
#define PV_ERRORPRINTF(x, args...) fprintf(stderr, "(PV_E)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#endif
#else
#define PV_ERRORPRINTF(x, args...) do {} while (0)
#endif

#if (PV_DUMP_ON)
#if (SYSLOG_ON)
#define PV_DUMP(x, args...) \
{\
    syslog(LOG_INFO,  x , ##args);\
    fprintf(stderr,  x , ##args);\
}
#else
#define PV_DUMP(x, args...) fprintf(stderr,  x , ##args)
#endif
#else
#define PV_DUMP(x, args...) do {} while (0)
#endif

#if (PV_LIST_ON)
#if (SYSLOG_ON)
#define PV_LISTPRINTF(x, args...) \
{\
    syslog(LOG_INFO,  x , ##args);\
    fprintf(stderr,  x , ##args);\
}
#else
#define PV_LISTPRINTF(x, args...) fprintf(stderr,  x , ##args)
#endif
#else
#define PV_LISTPRINTF(x, args...) do {} while (0)
#endif


#if (PV_MEM_ON)
#if (SYSLOG_ON)
#define PV_MEM(x, args...) \
{\
    syslog(LOG_INFO, "(PV_M)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
    fprintf(stderr, "(PV_M)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
}
#else
#define PV_MEM(x, args...) fprintf(stderr, "(PV_M)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#endif
#else
#define PV_MEM(x, args...) do {} while(0)
#endif


#if (PV_SCB_ON)
#if (SYSLOG_ON)
#define PV_SCBPRINTF(x, args...) \
{\
    syslog(LOG_INFO, "(PV_S)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
    fprintf(stderr, "(PV_S)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
}
#else
#define PV_SCBPRINTF(x, args...) fprintf(stderr, "(PV_S)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#endif
#else
#define PV_SCBPRINTF(x, args...) do {} while (0)
#endif


#if (PV_TOOLBAR_ON)
#if (SYSLOG_ON)
#define PV_TBPRINTF(x, args...) \
{\
    syslog(LOG_INFO, "(PV_TB)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
    fprintf(stderr, "(PV_TB)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args);\
}
#else
#define PV_TBPRINTF(x, args...) fprintf(stderr, "(PV_TB)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#endif
#else
#define PV_TBPRINTF(x, args...) do {} while (0)
#endif


#endif //

