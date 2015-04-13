noinst_PROGRAMS = pdftest 

source_c = test.cc
 
INCLUDES = @poppler_CFLAGS@ 

DATADIR=$(datadir)

AM_CFLAGS = @GCC_WARNINGS@ -DDATADIR=\"$(datadir)\"

pdftest_SOURCES     = $(source_c)

AM_CPPFLAGS = -I$(top_srcdir)/inc

CXXFLAGS = @CXXFLAGS@ @DEFS@ -Wall  -I. -I.. 

AM_CFLAGS += -Wall -Werror

AM_LDFLAGS = @poppler_LIBS@
