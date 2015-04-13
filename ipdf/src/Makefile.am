bin_PROGRAMS = ipdf 

source_c =  $(top_srcdir)/inc/PDFApp.h		\
            $(top_srcdir)/inc/GtkMgr.h		\
            $(top_srcdir)/inc/PageInfo.h	\
            $(top_srcdir)/inc/PDFCore.h		\
            $(top_srcdir)/inc/controller.h	\
            $(top_srcdir)/inc/PDFView.h		\
            $(top_srcdir)/inc/RenderThread.h	\
            $(top_srcdir)/inc/PDFViewerLog.h	\
            $(top_srcdir)/inc/channels.h	\
            $(top_srcdir)/inc/Settings.h        \
            $(top_srcdir)/inc/ScribbleMgr.h     \
            $(top_srcdir)/inc/PDFLandscapeView.h\
            $(top_srcdir)/inc/PDFPortraitView.h \
            $(top_srcdir)/inc/LayoutMgr.h       \
            $(top_srcdir)/inc/DisplayList.h     \
            $(top_srcdir)/inc/utils.h           \
            $(top_srcdir)/inc/ScribbleMgr.h     \
            $(top_srcdir)/inc/GtkAuthDialog.h	\
            main.cc                             \
            PDFApp.cpp                          \
            GtkMgr.cpp                          \
            PageInfo.cpp                        \
            PDFCore.cpp                         \
            PDFView.cpp                         \
            RenderThread.cpp                    \
            channels.cpp                        \
            controller.cpp                      \
            Settings.cpp                        \
            PDFLandscapeView.cpp                \
            PDFPortraitView.cpp                 \
            LayoutMgr.cpp                       \
            DisplayList.cpp                     \
            utils.cpp                           \
            ScribbleMgr.cpp			\
            GtkAuthDialog.cpp

 
INCLUDES =  -I$(top_srcdir)/inc @LIBERDM_CFLAGS@ @LIBERIPC_CFLAGS@ @LIBERMANIFEST_CFLAGS@ @poppler_CFLAGS@   @DEPS_CFLAGS@ @LIBERSCRIBBLE_CFLAGS@

DATADIR=$(datadir)

AM_CFLAGS = @GCC_WARNINGS@ -DDATADIR=\"$(datadir)\"

ipdf_SOURCES     = $(source_c)

AM_CPPFLAGS = -I$(top_srcdir)/inc

CXXFLAGS = @CXXFLAGS@ @DEFS@ -Wall -Werror -I..  -DLOCALE_DIR=\"$(localedir)\"  -DDATA_DIR=\"$(pkgdatadir)\"


AM_CFLAGS += -Wall -Werror

AM_LDFLAGS = @LIBERDM_LIBS@ @LIBERIPC_LIBS@ @LIBERMANIFEST_LIBS@ @LIBERSCRIBBLE_LIBS@ @DEPS_LIBS@  @poppler_LIBS@  
 

