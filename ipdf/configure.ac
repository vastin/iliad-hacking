dnl ----- Process this file with autoconf to produce a configure script -----

AC_INIT(ipdf, 0.29)
AC_CONFIG_SRCDIR(src/main.cc)
AC_PREREQ(2.57)

AM_INIT_AUTOMAKE([foreign])
AC_CONFIG_HEADERS(config.h)
AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_VERSION([0.14.1])
localedir='${datadir}/locale'
AC_SUBST(localedir)


dnl ----- Checks for programs ---------------------------------------------
AC_PROG_LIBTOOL
AC_PROG_CXX
AC_STDC_HEADERS
AC_PROG_INSTALL

dnl ----- Args -------------------------------------------------------------

AC_ARG_ENABLE(debug,
  [  --enable-debug          enable debug ( verbose ) build],
     enable_debug=$enableval, enable_debug=no )

PKG_CHECK_MODULES(FC, fontconfig >= 2.2)
AC_SUBST(FC_CFLAGS)
AC_SUBST(FC_LIBS)

PKG_CHECK_MODULES(LIBERIPC, liberipc >= 0.1,, 
			    AC_MSG_ERROR([*** Required EReader IPC Library (liberipc) 0.1 not installed ***]))
AC_SUBST(LIBERIPC_CFLAGS)
AC_SUBST(LIBERIPC_LIBS)

PKG_CHECK_MODULES(LIBERMANIFEST, libermanifest >= 0.1,, 
			    AC_MSG_ERROR([*** Required EReader Manifest Library (libermanifest) 0.1 not installed ***]))
AC_SUBST(LIBERMANIFEST_CFLAGS)
AC_SUBST(LIBERMANIFEST_LIBS)

PKG_CHECK_MODULES(LIBERDM, liberdm >= 0.1,, 
			    AC_MSG_ERROR([*** Required EReader display manager Library (liberdm) 0.1 not installed ***]))
AC_SUBST(LIBERDM_CFLAGS)
AC_SUBST(LIBERDM_LIBS)

dnl -- Registry lib is not needed and not used at the moment
dnl PKG_CHECK_MODULES(LIBERREG, liberreg >= 0.1,, 
dnl 			    AC_MSG_ERROR([*** Required EReader Registry Library (liberreg) 0.1 not installed ***]))
dnl AC_SUBST(LIBERREG_CFLAGS)
dnl AC_SUBST(LIBERREG_LIBS)

PKG_CHECK_MODULES(DEPS, gtk+-2.0 >= 2.2 glib-2.0 >= 2.2 gthread-2.0 gtk+-x11-2.0 >= 2.6 )
AC_SUBST(DEPS_CFLAGS)
AC_SUBST(DEPS_LIBS)

PKG_CHECK_MODULES(LIBERSCRIBBLE, liberscribble >= 0.1,,
                            AC_MSG_ERROR([*** Required EReader Scribble Library (liberscribble) 0.1 not installed ***]))
AC_SUBST(LIBERSCRIBBLE_CFLAGS)
AC_SUBST(LIBERSCRIBBLE_LIBS)

PKG_CHECK_MODULES(poppler, poppler >= 0.4,,
                        AC_MSG_ERROR([*** Required libpoppler not installed ***]))
AC_SUBST(poppler_CFLAGS)
AC_SUBST(poppler_LIBS)

AC_OUTPUT([
Makefile
src/Makefile 
data/Makefile
po/Makefile.in
test/Makefile
])

dnl ==========================================================================
echo "
                       ipdf viewer $VERSION
                  ==============================

        prefix:                 ${prefix}
        source code location:   ${srcdir}
        Building with Debug:    ${enable_debug}

"
