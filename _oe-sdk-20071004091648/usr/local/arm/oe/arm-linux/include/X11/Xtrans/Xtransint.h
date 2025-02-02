/* $Xorg: Xtransint.h,v 1.4 2001/02/09 02:04:06 xorgcvs Exp $ */
/*

Copyright 1993, 1994, 1998  The Open Group

Permission to use, copy, modify, distribute, and sell this software and its
documentation for any purpose is hereby granted without fee, provided that
the above copyright notice appear in all copies and that both that
copyright notice and this permission notice appear in supporting
documentation.

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE OPEN GROUP BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of The Open Group shall
not be used in advertising or otherwise to promote the sale, use or
other dealings in this Software without prior written authorization
from The Open Group.

*/
/* $XFree86: xc/lib/xtrans/Xtransint.h,v 3.40 2003/08/27 21:58:28 tsi Exp $ */

/* Copyright 1993, 1994 NCR Corporation - Dayton, Ohio, USA
 *
 * All Rights Reserved
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name NCR not be used in advertising
 * or publicity pertaining to distribution of the software without specific,
 * written prior permission.  NCR makes no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * NCR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN
 * NO EVENT SHALL NCR BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
 * OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef _XTRANSINT_H_
#define _XTRANSINT_H_

/*
 * XTRANSDEBUG will enable the PRMSG() macros used in the X Transport 
 * Interface code. Each use of the PRMSG macro has a level associated with 
 * it. XTRANSDEBUG is defined to be a level. If the invocation level is =< 
 * the value of XTRANSDEBUG, then the message will be printed out to stderr. 
 * Recommended levels are:
 *
 *	XTRANSDEBUG=1	Error messages
 *	XTRANSDEBUG=2 API Function Tracing
 *	XTRANSDEBUG=3 All Function Tracing
 *	XTRANSDEBUG=4 printing of intermediate values
 *	XTRANSDEBUG=5 really detailed stuff
#define XTRANSDEBUG 2
 *
 * Defining XTRANSDEBUGTIMESTAMP will cause printing timestamps with each
 * message.
 */

#ifndef XTRANSDEBUG
# ifndef __UNIXOS2__
#  define XTRANSDEBUG 1
# else
#  define XTRANSDEBUG 1
# endif
#endif

#ifdef WIN32
# define _WILLWINSOCK_
#endif

#include "Xtrans.h"

#ifdef XTRANSDEBUG
# include <stdio.h>
#endif /* XTRANSDEBUG */

#include <errno.h>

#ifndef WIN32
# ifndef Lynx
#  include <sys/socket.h>
# else
#  include <socket.h>
# endif
# include <netinet/in.h>
# include <arpa/inet.h>
# ifdef __UNIXOS2__
#  include <sys/ioctl.h>
# endif

/*
 * Moved the setting of NEED_UTSNAME to this header file from Xtrans.c,
 * to avoid a race condition. JKJ (6/5/97)
 */

# if (defined(_POSIX_SOURCE) && !defined(AIXV3) && !defined(__QNX__)) || defined(hpux) || defined(USG) || defined(SVR4) || defined(SCO)
#  ifndef NEED_UTSNAME
#   define NEED_UTSNAME
#  endif
#  include <sys/utsname.h>
# endif

/*
 * makedepend screws up on #undef OPEN_MAX, so we define a new symbol
 */

# ifndef TRANS_OPEN_MAX

#  ifndef X_NOT_POSIX
#   ifdef _POSIX_SOURCE
#    include <limits.h>
#   else
#    define _POSIX_SOURCE
#    include <limits.h>
#    undef _POSIX_SOURCE
#   endif
#  endif
#  ifndef OPEN_MAX
#   if defined(_SC_OPEN_MAX)
#    define OPEN_MAX (sysconf(_SC_OPEN_MAX))
#   else
#    ifdef SVR4
#     define OPEN_MAX 256
#    else
#     include <sys/param.h>
#     ifndef OPEN_MAX
#      ifdef __OSF1__
#       define OPEN_MAX 256
#      else
#       ifdef NOFILE
#        define OPEN_MAX NOFILE
#       else
#        if !defined(__UNIXOS2__) && !defined(__QNX__)
#         define OPEN_MAX NOFILES_MAX
#        else
#         define OPEN_MAX 256
#        endif
#       endif
#      endif
#     endif
#    endif
#   endif
#  endif
#  if defined(_SC_OPEN_MAX)
#   define TRANS_OPEN_MAX OPEN_MAX
#  else /* !__GNU__ */
#   if OPEN_MAX > 256
#    define TRANS_OPEN_MAX 256
#   else
#    define TRANS_OPEN_MAX OPEN_MAX
#   endif
#  endif /*__GNU__*/

# endif /* TRANS_OPEN_MAX */

# ifdef __UNIXOS2__
#  define ESET(val)
# else
#  define ESET(val) errno = val
# endif
# define EGET() errno

#else /* WIN32 */

# include <limits.h>	/* for USHRT_MAX */

# define ESET(val) WSASetLastError(val)
# define EGET() WSAGetLastError()

#endif /* WIN32 */

#include <stddef.h>

#ifdef X11_t
#define X_TCP_PORT	6000
#endif

struct _XtransConnInfo {
    struct _Xtransport     *transptr;
    int		index;
    char	*priv;
    int		flags;
    int		fd;
    char	*port;
    int		family;
    char	*addr;
    int		addrlen;
    char	*peeraddr;
    int		peeraddrlen;
};

#define XTRANS_OPEN_COTS_CLIENT       1
#define XTRANS_OPEN_COTS_SERVER       2
#define XTRANS_OPEN_CLTS_CLIENT       3
#define XTRANS_OPEN_CLTS_SERVER       4


typedef struct _Xtransport {
    char	*TransName;
    int		flags;

#ifdef TRANS_CLIENT

    XtransConnInfo (*OpenCOTSClient)(
	struct _Xtransport *,	/* transport */
	char *,			/* protocol */
	char *,			/* host */
	char *			/* port */
    );

#endif /* TRANS_CLIENT */

#ifdef TRANS_SERVER
    char **	nolisten;
    XtransConnInfo (*OpenCOTSServer)(
	struct _Xtransport *,	/* transport */
	char *,			/* protocol */
	char *,			/* host */
	char *			/* port */
    );

#endif /* TRANS_SERVER */

#ifdef TRANS_CLIENT

    XtransConnInfo (*OpenCLTSClient)(
	struct _Xtransport *,	/* transport */
	char *,			/* protocol */
	char *,			/* host */
	char *			/* port */
    );

#endif /* TRANS_CLIENT */

#ifdef TRANS_SERVER

    XtransConnInfo (*OpenCLTSServer)(
	struct _Xtransport *,	/* transport */
	char *,			/* protocol */
	char *,			/* host */
	char *			/* port */
    );

#endif /* TRANS_SERVER */


#ifdef TRANS_REOPEN

    XtransConnInfo (*ReopenCOTSServer)(
	struct _Xtransport *,	/* transport */
        int,			/* fd */
        char *			/* port */
    );

    XtransConnInfo (*ReopenCLTSServer)(
	struct _Xtransport *,	/* transport */
        int,			/* fd */
        char *			/* port */
    );

#endif /* TRANS_REOPEN */


    int	(*SetOption)(
	XtransConnInfo,		/* connection */
	int,			/* option */
	int			/* arg */
    );

#ifdef TRANS_SERVER
/* Flags */
# define ADDR_IN_USE_ALLOWED	1

    int	(*CreateListener)(
	XtransConnInfo,		/* connection */
	char *,			/* port */
	unsigned int		/* flags */
    );

    int	(*ResetListener)(
	XtransConnInfo		/* connection */
    );

    XtransConnInfo (*Accept)(
	XtransConnInfo,		/* connection */
        int *			/* status */
    );

#endif /* TRANS_SERVER */

#ifdef TRANS_CLIENT

    int	(*Connect)(
	XtransConnInfo,		/* connection */
	char *,			/* host */
	char *			/* port */
    );

#endif /* TRANS_CLIENT */

    int	(*BytesReadable)(
	XtransConnInfo,		/* connection */
	BytesReadable_t *	/* pend */
    );

    int	(*Read)(
	XtransConnInfo,		/* connection */
	char *,			/* buf */
	int			/* size */
    );

    int	(*Write)(
	XtransConnInfo,		/* connection */
	char *,			/* buf */
	int			/* size */
    );

    int	(*Readv)(
	XtransConnInfo,		/* connection */
	struct iovec *,		/* buf */
	int			/* size */
    );

    int	(*Writev)(
	XtransConnInfo,		/* connection */
	struct iovec *,		/* buf */
	int			/* size */
    );

    int	(*Disconnect)(
	XtransConnInfo		/* connection */
    );

    int	(*Close)(
	XtransConnInfo		/* connection */
    );

    int	(*CloseForCloning)(
	XtransConnInfo		/* connection */
    );

} Xtransport;


typedef struct _Xtransport_table {
    Xtransport	*transport;
    int		transport_id;
} Xtransport_table;


/*
 * Flags for the flags member of Xtransport.
 */

#define TRANS_ALIAS	(1<<0)	/* record is an alias, don't create server */
#define TRANS_LOCAL	(1<<1)	/* local transport */
#define TRANS_DISABLED	(1<<2)	/* Don't open this one */
#define TRANS_NOLISTEN  (1<<3)  /* Don't listen on this one */
#define TRANS_NOUNLINK	(1<<4)	/* Dont unlink transport endpoints */

/* Flags to preserve when setting others */
#define TRANS_KEEPFLAGS	(TRANS_NOUNLINK)

/*
 * readv() and writev() don't exist or don't work correctly on some
 * systems, so they may be emulated.
 */

#if defined(CRAY) || (defined(SYSV) && defined(i386) && !defined(SCO325)) || defined(WIN32) || defined(__sxg__) || defined(__UNIXOS2__)

#define READV(ciptr, iov, iovcnt)	TRANS(ReadV)(ciptr, iov, iovcnt)

static	int TRANS(ReadV)(
    XtransConnInfo,	/* ciptr */
    struct iovec *,	/* iov */
    int			/* iovcnt */
);

#else

#define READV(ciptr, iov, iovcnt)	readv(ciptr->fd, iov, iovcnt)

#endif /* CRAY || (SYSV && i386) || WIN32 || __sxg__ || */


#if defined(CRAY) || (defined(SYSV) && defined(i386) && !defined(SCO325)) || defined(WIN32) || defined(__sxg__) || defined(__UNIXOS2__)

#define WRITEV(ciptr, iov, iovcnt)	TRANS(WriteV)(ciptr, iov, iovcnt)

static int TRANS(WriteV)(
    XtransConnInfo,	/* ciptr */
    struct iovec *,	/* iov */
    int 		/* iovcnt */
);

#else

#define WRITEV(ciptr, iov, iovcnt)	writev(ciptr->fd, iov, iovcnt)

#endif /* CRAY || WIN32 || __sxg__ */


static int is_numeric (
    char *		/* str */
);

#ifdef TRANS_SERVER
static int trans_mkdir (
    char *,		/* path */
    int			/* mode */
);
#endif

/*
 * Some XTRANSDEBUG stuff
 */

#if defined(XTRANSDEBUG)
/* add hack to the format string to avoid warnings about extra arguments
 * to fprintf.
 */
#ifdef XTRANSDEBUGTIMESTAMP
#if defined(XSERV_t) && defined(TRANS_SERVER)
/* Use ErrorF() for the X server */
#define PRMSG(lvl,x,a,b,c)	if (lvl <= XTRANSDEBUG){ \
			int hack= 0, saveerrno=errno; \
                        struct timeval tp;\
                        gettimeofday(&tp,0); \
			ErrorF(__xtransname); \
			ErrorF(x+hack,a,b,c); \
                        ErrorF("timestamp (ms): %d\n",tp.tv_sec*1000+tp.tv_usec/1000); \
			errno=saveerrno; \
			} else ((void)0)
#else
#define PRMSG(lvl,x,a,b,c)	if (lvl <= XTRANSDEBUG){ \
			int hack= 0, saveerrno=errno; \
                        struct timeval tp;\
                        gettimeofday(&tp,0); \
			fprintf(stderr, __xtransname); fflush(stderr); \
			fprintf(stderr, x+hack,a,b,c); fflush(stderr); \
                        fprintf(stderr, "timestamp (ms): %d\n",tp.tv_sec*1000+tp.tv_usec/1000); \
                        fflush(stderr); \
			errno=saveerrno; \
			} else ((void)0)
#endif /* XSERV_t && TRANS_SERVER */
#else /* XTRANSDEBUGTIMESTAMP */
#if defined(XSERV_t) && defined(TRANS_SERVER)
/* Use ErrorF() for the X server */
#define PRMSG(lvl,x,a,b,c)	if (lvl <= XTRANSDEBUG){ \
			int hack= 0, saveerrno=errno; \
			ErrorF(__xtransname); \
			ErrorF(x+hack,a,b,c); \
			errno=saveerrno; \
			} else ((void)0)
#else
#define PRMSG(lvl,x,a,b,c)	if (lvl <= XTRANSDEBUG){ \
			int hack= 0, saveerrno=errno; \
			fprintf(stderr, __xtransname); fflush(stderr); \
			fprintf(stderr, x+hack,a,b,c); fflush(stderr); \
			errno=saveerrno; \
			} else ((void)0)
#endif /* XSERV_t && TRANS_SERVER */
#endif /* XTRANSDEBUGTIMESTAMP */
#else
#define PRMSG(lvl,x,a,b,c)	((void)0)
#endif /* XTRANSDEBUG */

#endif /* _XTRANSINT_H_ */
