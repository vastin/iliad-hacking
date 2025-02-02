/* $Xorg: ICEmsg.h,v 1.4 2001/02/09 02:03:26 xorgcvs Exp $ */
/******************************************************************************


Copyright 1993, 1998  The Open Group

Permission to use, copy, modify, distribute, and sell this software and its
documentation for any purpose is hereby granted without fee, provided that
the above copyright notice appear in all copies and that both that
copyright notice and this permission notice appear in supporting
documentation.

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
OPEN GROUP BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of The Open Group shall not be
used in advertising or otherwise to promote the sale, use or other dealings
in this Software without prior written authorization from The Open Group.

Author: Ralph Mor, X Consortium
******************************************************************************/
/* $XFree86$ */

#ifndef _ICEMSG_H_
#define _ICEMSG_H_

#include <X11/Xfuncproto.h>

#include <X11/ICE/ICEconn.h>

_XFUNCPROTOBEGIN

/*
 * Function prototypes for internal ICElib functions
 */

extern Status _IceRead (
#if NeedFunctionPrototypes
    IceConn		/* iceConn */,
    unsigned long	/* nbytes */,
    char *		/* ptr */
#endif
);

extern void _IceReadSkip (
#if NeedFunctionPrototypes
    IceConn		/* iceConn */,
    unsigned long	/* nbytes */
#endif
);

extern void _IceWrite (
#if NeedFunctionPrototypes
    IceConn		/* iceConn */,
    unsigned long	/* nbytes */,
    char *		/* ptr */
#endif
);


extern void _IceErrorBadMinor (
#if NeedFunctionPrototypes
    IceConn		/* iceConn */,
    int			/* majorOpcode */,
    int			/* offendingMinor */,
    int			/* severity */
#endif
);

extern void _IceErrorBadState (
#if NeedFunctionPrototypes
    IceConn		/* iceConn */,
    int			/* majorOpcode */,
    int			/* offendingMinor */,
    int			/* severity */
#endif
);

extern void _IceErrorBadLength (
#if NeedFunctionPrototypes
    IceConn		/* iceConn */,
    int			/* majorOpcode */,
    int			/* offendingMinor */,
    int			/* severity */
#endif
);

extern void _IceErrorBadValue (
#if NeedFunctionPrototypes
    IceConn		/* iceConn */,
    int			/* majorOpcode */,
    int			/* offendingMinor */,
    int			/* offset */,
    int			/* length */,
    IcePointer		/* value */
#endif
);


/*
 * Macro to check if IO operations are valid on an ICE connection.
 */

#define IceValidIO(_iceConn) _iceConn->io_ok


/*
 * Macros for writing messages.
 */

#define IceGetHeader(_iceConn, _major, _minor, _headerSize, _msgType, _pMsg) \
    if ((_iceConn->outbufptr + _headerSize) > _iceConn->outbufmax) \
        IceFlush (_iceConn); \
    _pMsg = (_msgType *) _iceConn->outbufptr; \
    _pMsg->majorOpcode = _major; \
    _pMsg->minorOpcode = _minor; \
    _pMsg->length = (_headerSize - SIZEOF (iceMsg)) >> 3; \
    _iceConn->outbufptr += _headerSize; \
    _iceConn->send_sequence++

#define IceGetHeaderExtra(_iceConn, _major, _minor, _headerSize, _extra, _msgType, _pMsg, _pData) \
    if ((_iceConn->outbufptr + \
	_headerSize + ((_extra) << 3)) > _iceConn->outbufmax) \
        IceFlush (_iceConn); \
    _pMsg = (_msgType *) _iceConn->outbufptr; \
    if ((_iceConn->outbufptr + \
	_headerSize + ((_extra) << 3)) <= _iceConn->outbufmax) \
        _pData = (char *) _pMsg + _headerSize; \
    else \
        _pData = NULL; \
    _pMsg->majorOpcode = _major; \
    _pMsg->minorOpcode = _minor; \
    _pMsg->length = ((_headerSize - SIZEOF (iceMsg)) >> 3) + (_extra); \
    _iceConn->outbufptr += (_headerSize + ((_extra) << 3)); \
    _iceConn->send_sequence++

#define IceSimpleMessage(_iceConn, _major, _minor) \
{ \
    iceMsg *_pMsg; \
    IceGetHeader (_iceConn, _major, _minor, SIZEOF (iceMsg), iceMsg, _pMsg); \
}

#define IceErrorHeader(_iceConn, _offendingMajorOpcode, _offendingMinorOpcode, _offendingSequenceNum, _severity, _errorClass, _dataLength) \
{ \
    iceErrorMsg	*_pMsg; \
\
    IceGetHeader (_iceConn, _offendingMajorOpcode, ICE_Error, \
	SIZEOF (iceErrorMsg), iceErrorMsg, _pMsg); \
    _pMsg->length += (_dataLength); \
    _pMsg->offendingMinorOpcode = _offendingMinorOpcode; \
    _pMsg->severity = _severity; \
    _pMsg->offendingSequenceNum = _offendingSequenceNum; \
    _pMsg->errorClass = _errorClass; \
}


/*
 * Write data into the ICE output buffer.
 */

#define IceWriteData(_iceConn, _bytes, _data) \
{ \
    if ((_iceConn->outbufptr + (_bytes)) > _iceConn->outbufmax) \
    { \
	IceFlush (_iceConn); \
        _IceWrite (_iceConn, (unsigned long) (_bytes), _data); \
    } \
    else \
    { \
        memcpy (_iceConn->outbufptr, _data, _bytes); \
        _iceConn->outbufptr += (_bytes); \
    } \
}

#ifndef WORD64

#define IceWriteData16(_iceConn, _bytes, _data) \
    IceWriteData (_iceConn, _bytes, (char *) _data)

#define IceWriteData32(_iceConn, _bytes, _data) \
    IceWriteData (_iceConn, _bytes, (char *) _data)

#else /* WORD64 */

/* IceWriteData16 and IceWriteData32 defined in misc.c for WORD64 */

#endif /* WORD64 */


/*
 * The IceSendData macro bypasses copying the data to the
 * ICE connection buffer and sends the data directly.  If necessary,
 * the ICE connection buffer is first flushed.
 */

#define IceSendData(_iceConn, _bytes, _data) \
{ \
    if (_iceConn->outbufptr > _iceConn->outbuf) \
	IceFlush (_iceConn); \
    _IceWrite (_iceConn, (unsigned long) (_bytes), _data); \
}


/*
 * Write pad bytes.  Used to force 32 or 64 bit alignment.
 * A maxium of 7 pad bytes can be specified.
 */

#define IceWritePad(_iceConn, _bytes) \
{ \
    if ((_iceConn->outbufptr + (_bytes)) > _iceConn->outbufmax) \
    { \
        char _dummy[7]; \
	IceFlush (_iceConn); \
        _IceWrite (_iceConn, (unsigned long) (_bytes), _dummy); \
    } \
    else \
    { \
        _iceConn->outbufptr += (_bytes); \
    } \
}


/*
 * Macros for reading messages.
 */

#define IceReadCompleteMessage(_iceConn, _headerSize, _msgType, _pMsg, _pData)\
{ \
    unsigned long _bytes; \
    IceReadMessageHeader (_iceConn, _headerSize, _msgType, _pMsg); \
    _bytes = (_pMsg->length << 3) - (_headerSize - SIZEOF (iceMsg)); \
    if ((_iceConn->inbufmax - _iceConn->inbufptr) >= _bytes) \
    { \
	_IceRead (_iceConn, _bytes, _iceConn->inbufptr); \
	_pData = _iceConn->inbufptr; \
	_iceConn->inbufptr += _bytes; \
    } \
    else \
    { \
	_pData = (char *) malloc ((unsigned) _bytes); \
        if (_pData) \
	    _IceRead (_iceConn, _bytes, _pData); \
        else \
	    _IceReadSkip (_iceConn, _bytes); \
    } \
}

#define IceDisposeCompleteMessage(_iceConn, _pData) \
    if ((char *) _pData < _iceConn->inbuf || \
	(char *) _pData >= _iceConn->inbufmax) \
        free ((char *) _pData);


#define IceReadSimpleMessage(_iceConn, _msgType, _pMsg) \
    _pMsg = (_msgType *) (_iceConn->inbuf);

#define IceReadMessageHeader(_iceConn, _headerSize, _msgType, _pMsg) \
{ \
    _IceRead (_iceConn, \
	(unsigned long) (_headerSize - SIZEOF (iceMsg)), \
	_iceConn->inbufptr); \
    _pMsg = (_msgType *) (_iceConn->inbuf); \
    _iceConn->inbufptr += (_headerSize - SIZEOF (iceMsg)); \
}

#define IceReadData(_iceConn, _bytes, _pData) \
    _IceRead (_iceConn, (unsigned long) (_bytes), (char *) _pData); \

#ifndef WORD64

#define IceReadData16(_iceConn, _swap, _bytes, _pData) \
{ \
    _IceRead (_iceConn, (unsigned long) (_bytes), (char *) _pData); \
}

#define IceReadData32(_iceConn, _swap, _bytes, _pData) \
{ \
    _IceRead (_iceConn, (unsigned long) (_bytes), (char *) _pData); \
}

#else /* WORD64 */

/* IceReadData16 and IceReadData32 defined in misc.c for WORD64 */

#endif /* WORD64 */


/*
 * Read pad bytes (for 32 or 64 bit alignment).
 * A maxium of 7 pad bytes can be specified.
 */

#define IceReadPad(_iceConn, _bytes) \
{ \
    char _dummy[7]; \
    _IceRead (_iceConn, (unsigned long) (_bytes), _dummy); \
}

_XFUNCPROTOEND

#endif /* _ICEMSG_H_ */
