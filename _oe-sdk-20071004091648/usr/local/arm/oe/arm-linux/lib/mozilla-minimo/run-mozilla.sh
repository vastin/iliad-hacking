#!/bin/sh
#
# ***** BEGIN LICENSE BLOCK *****
# Version: MPL 1.1/GPL 2.0/LGPL 2.1
#
# The contents of this file are subject to the Mozilla Public License Version
# 1.1 (the "License"); you may not use this file except in compliance with
# the License. You may obtain a copy of the License at
# http://www.mozilla.org/MPL/
#
# Software distributed under the License is distributed on an "AS IS" basis,
# WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
# for the specific language governing rights and limitations under the
# License.
#
# The Original Code is mozilla.org code.
#
# The Initial Developer of the Original Code is
# Netscape Communications Corporation.
# Portions created by the Initial Developer are Copyright (C) 1998
# the Initial Developer. All Rights Reserved.
#
# Contributor(s):
#
# Alternatively, the contents of this file may be used under the terms of
# either of the GNU General Public License Version 2 or later (the "GPL"),
# or the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
# in which case the provisions of the GPL or the LGPL are applicable instead
# of those above. If you wish to allow use of your version of this file only
# under the terms of either the GPL or the LGPL, and not to allow others to
# use your version of this file under the terms of the MPL, indicate your
# decision by deleting the provisions above and replace them with the notice
# and other provisions required by the GPL or the LGPL. If you do not delete
# the provisions above, a recipient may use your version of this file under
# the terms of any one of the MPL, the GPL or the LGPL.
#
# ***** END LICENSE BLOCK *****
cmdname=`basename "$0"`
MOZ_DIST_BIN=`dirname "$0"`
MOZ_DEFAULT_NAME="./${cmdname}-bin"
MOZ_APPRUNNER_NAME="./mozilla-bin"
MOZ_VIEWER_NAME="./viewer"
MOZ_PROGRAM=""

exitcode=0
#
##
## Functions
##
##########################################################################
moz_usage()
{
echo "Usage:  ${cmdname} [options] [program]"
echo ""
echo "  options:"
echo ""
echo "    -g                   Run in debugger."
echo "    --debug"
echo ""
echo "    -d debugger          Debugger to use."
echo "    --debugger debugger"
echo ""
echo "  Examples:"
echo ""
echo "  Run the viewer"
echo ""
echo "    ${cmdname} viewer"
echo ""
echo "  Run the mozilla-bin binary"
echo ""
echo "    ${cmdname} mozilla-bin"
echo ""
echo "  Debug the viewer in a debugger"
echo ""
echo "    ${cmdname} -g viewer"
echo ""
echo "  Debug the mozilla-bin binary in gdb"
echo ""
echo "    ${cmdname} -g mozilla-bin -d gdb"
echo ""
	return 0
}
##########################################################################
moz_bail()
{
	message=$1
	echo
	echo "$cmdname: $message"
	echo
	exit 1
}
##########################################################################
moz_test_binary()
{
	binary=$1
	if [ -f "$binary" ]
	then
		if [ -x "$binary" ]
		then
			return 1
		fi
	fi
	return 0
}
##########################################################################
moz_get_debugger()
{
	debuggers="ddd gdb dbx bdb"
	debugger="notfound"
	done="no"
	for d in $debuggers
	do
		moz_test_binary /bin/type
		if [ $? -eq 1 ]
		then
			dpath=`type ${d} | awk '{print $3;}' | sed -e 's/\.$//'`	
		else 	
			dpath=`which ${d}`	
		fi
		if [ -x "$dpath" ]
		then
			debugger=$dpath
			break
		fi
	done
	echo $debugger
	return 0
}
##########################################################################
moz_run_program()
{
	prog=$MOZ_PROGRAM
	##
	## Make sure the program is executable
	##
	if [ ! -x "$prog" ]
	then
		moz_bail "Cannot execute $prog."
	fi
	##
	## Use md5sum to crc a core file.  If md5sum is not found on the system,
	## then dont debug core files.
	##
	moz_test_binary /bin/type
	if [ $? -eq 1 ]
	then
		crc_prog=`type md5sum 2>/dev/null | awk '{print $3;}' 2>/dev/null | sed -e 's/\.$//'`
	else
		crc_prog=`which md5sum 2>/dev/null`
	fi
	if [ -x "$crc_prog" ]
	then
		DEBUG_CORE_FILES=1
	fi
	if [ "$DEBUG_CORE_FILES" ]
	then
		crc_old=
		if [ -f core ]
		then
			crc_old=`$crc_prog core | awk '{print $1;}' `
		fi
	fi
	##
	## Run the program
	##
	"$prog" ${1+"$@"}
	exitcode=$?
	if [ "$DEBUG_CORE_FILES" ]
	then
		if [ -f core ]
		then
			crc_new=`$crc_prog core | awk '{print $1;}' `
		fi
	fi
	if [ "$crc_old" != "$crc_new" ]
	then
		printf "\n\nOh no!  %s just dumped a core file.\n\n" $prog
		printf "Do you want to debug this ? "
		printf "You need a lot of memory for this, so watch out ? [y/n] "
		read ans
		if [ "$ans" = "y" ]
		then
			debugger=`moz_get_debugger`
			if [ -x "$debugger" ]
			then
				echo "$debugger $prog core"

				# See http://www.mozilla.org/unix/debugging-faq.html
				# For why LD_BIND_NOW is needed
				LD_BIND_NOW=1; export LD_BIND_NOW

				$debugger "$prog" core
			else
				echo "Could not find a debugger on your system."
			fi
		fi
	fi
}
##########################################################################
moz_debug_program()
{
	prog=$MOZ_PROGRAM
	##
	## Make sure the program is executable
	##
	if [ ! -x "$prog" ]
	then
		moz_bail "Cannot execute $prog."
	fi
	if [ -n "$moz_debugger" ]
	then
		moz_test_binary /bin/type
		if [ $? -eq 1 ]
		then	
			debugger=`type $moz_debugger | awk '{print $3;}' | sed -e 's/\.$//'` 
		else
			debugger=`which $moz_debugger` 
		fi	
	else
		debugger=`moz_get_debugger`
	fi
    if [ -x "$debugger" ] 
    then
        tmpfile=`mktemp /tmp/mozargs.XXXXXX` || { echo "Cannot create temporary file" >&2; exit 1; }
        trap " [ -f \"$tmpfile\" ] && /bin/rm -f -- \"$tmpfile\"" 0 1 2 3 13 15
        echo "set args ${1+"$@"}" > $tmpfile
# If you are not using ddd, gdb and know of a way to convey the arguments 
# over to the prog then add that here- Gagan Saksena 03/15/00
        case `basename $debugger` in
            gdb) echo "$debugger $prog -x $tmpfile"
                $debugger "$prog" -x $tmpfile
		exitcode=$?
                ;;
            ddd) echo "$debugger --debugger \"gdb -x $tmpfile\" $prog"
                $debugger --debugger "gdb -x $tmpfile" "$prog"
		exitcode=$?
                ;;
            *) echo "$debugger $prog ${1+"$@"}"
                $debugger "$prog" ${1+"$@"}
		exitcode=$?
                ;;
        esac
    else
        echo "Could not find a debugger on your system." 
    fi
}
##########################################################################
##
## Command line arg defaults
##
moz_debug=0
moz_debugger=""
#
##
## Parse the command line
##
while [ $# -gt 0 ]
do
  case $1 in
    -g | --debug)
      moz_debug=1
      shift
      ;;
    -d | --debugger)
      moz_debugger=$2;
      if [ "${moz_debugger}" != "" ]; then
	shift 2
      else
        echo "-d requires an argument"
        exit 1
      fi
      ;;
    *)
      break;
      ;;
  esac
done
#
##
## Program name given in $1
##
if [ $# -gt 0 ]
then
	MOZ_PROGRAM=$1
	shift
fi
##
## Program not given, try to guess a default
##
if [ -z "$MOZ_PROGRAM" ]
then
	##
	## Try this script's name with '-bin' appended
	##
	if [ -x "$MOZ_DEFAULT_NAME" ]
	then
		MOZ_PROGRAM=$MOZ_DEFAULT_NAME
	## Try viewer (this should be deprecated)
	## 
	elif [ -x "$MOZ_VIEWER_NAME" ]
	then
		MOZ_PROGRAM=$MOZ_VIEWER_NAME
	##
	## Try mozilla-bin
	## 
	elif [ -x "$MOZ_APPRUNNER_NAME" ]
	then
		MOZ_PROGRAM=$MOZ_APPRUNNER_NAME
	fi
fi
#
#
##
## Make sure the program is executable
##
if [ ! -x "$MOZ_PROGRAM" ]
then
	moz_bail "Cannot execute $MOZ_PROGRAM."
fi
#
##
## Set MOZILLA_FIVE_HOME
##
MOZILLA_FIVE_HOME=$MOZ_DIST_BIN

if [ -z "$MRE_HOME" ]; then
    MRE_HOME=$MOZILLA_FIVE_HOME
fi
##
## Set LD_LIBRARY_PATH
LD_LIBRARY_PATH=${MOZ_DIST_BIN}:${MOZ_DIST_BIN}/plugins:${MRE_HOME}${LD_LIBRARY_PATH+":$LD_LIBRARY_PATH"}
if [ -n "$LD_LIBRARYN32_PATH" ]
then
	LD_LIBRARYN32_PATH=${MOZ_DIST_BIN}:${MOZ_DIST_BIN}/plugins:${MRE_HOME}${LD_LIBRARYN32_PATH+":$LD_LIBRARYN32_PATH"}
fi
if [ -n "$LD_LIBRARYN64_PATH" ]
then
	LD_LIBRARYN64_PATH=${MOZ_DIST_BIN}:${MOZ_DIST_BIN}/plugins:${MRE_HOME}${LD_LIBRARYN64_PATH+":$LD_LIBRARYN64_PATH"}
fi
if [ -n "$LD_LIBRARY_PATH_64" ]; then
	LD_LIBRARY_PATH_64=${MOZ_DIST_BIN}:${MOZ_DIST_BIN}/plugins:${MRE_HOME}${LD_LIBRARY_PATH_64+":$LD_LIBRARY_PATH_64"}
fi
#
#
## Set SHLIB_PATH for HPUX
SHLIB_PATH=${MOZ_DIST_BIN}:${MRE_HOME}${SHLIB_PATH+":$SHLIB_PATH"}
#
## Set LIBPATH for AIX
LIBPATH=${MOZ_DIST_BIN}:${MRE_HOME}${LIBPATH+":$LIBPATH"}
#
## Set DYLD_LIBRARY_PATH for Mac OS X (Darwin)
DYLD_LIBRARY_PATH=${MOZ_DIST_BIN}:${MRE_HOME}${DYLD_LIBRARY_PATH+":$DYLD_LIBRARY_PATH"}
#
## Set LIBRARY_PATH for BeOS
LIBRARY_PATH=${MOZ_DIST_BIN}:${MOZ_DIST_BIN}/components:${MRE_HOME}${LIBRARY_PATH+":$LIBRARY_PATH"}
#
## Set ADDON_PATH for BeOS
ADDON_PATH=${MOZ_DIST_BIN}${ADDON_PATH+":$ADDON_PATH"}
#
## Solaris Xserver(Xsun) tuning - use shared memory transport if available
if [ "$XSUNTRANSPORT" = "" ]
then 
        XSUNTRANSPORT="shmem" 
        XSUNSMESIZE="512"
        export XSUNTRANSPORT XSUNSMESIZE
fi
## Populate XPSERVERLIST if it was not set yet
if [ "$XPSERVERLIST" = "" ]
then
    if [ -f /etc/init.d/xprint ] ; then
        XPSERVERLIST="`/bin/sh /etc/init.d/xprint get_xpserverlist`"
        if [ "$XPSERVERLIST" != "" ] ; then
            export XPSERVERLIST
        fi
    fi
fi

if [ "$moz_debug" -eq 1 ]
then
  echo "MOZILLA_FIVE_HOME=$MOZILLA_FIVE_HOME"
  echo "  LD_LIBRARY_PATH=$LD_LIBRARY_PATH"
  if [ -n "$LD_LIBRARYN32_PATH" ]
  then
  	echo "LD_LIBRARYN32_PATH=$LD_LIBRARYN32_PATH"
  fi
  if [ -n "$LD_LIBRARYN64_PATH" ]
  then
  	echo "LD_LIBRARYN64_PATH=$LD_LIBRARYN64_PATH"
  fi
  if [ -n "$LD_LIBRARY_PATH_64" ]; then
  	echo "LD_LIBRARY_PATH_64=$LD_LIBRARY_PATH_64"
  fi
  if [ -n "$DISPLAY" ]; then
       echo "DISPLAY=$DISPLAY"
  fi
  if [ -n "$FONTCONFIG_PATH" ]; then
	echo "FONTCONFIG_PATH=$FONTCONFIG_PATH"
  fi
  if [ -n "$XPSERVERLIST" ]; then
       echo "XPSERVERLIST=$XPSERVERLIST"
  fi
  if [ -n "$MOZILLA_POSTSCRIPT_PRINTER_LIST" ]; then
       echo "MOZILLA_POSTSCRIPT_PRINTER_LIST=$MOZILLA_POSTSCRIPT_PRINTER_LIST"
  fi
  echo "DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH"
  echo "     LIBRARY_PATH=$LIBRARY_PATH"
  echo "       SHLIB_PATH=$SHLIB_PATH"
  echo "          LIBPATH=$LIBPATH"
  echo "       ADDON_PATH=$ADDON_PATH"
  echo "      MOZ_PROGRAM=$MOZ_PROGRAM"
  echo "      MOZ_TOOLKIT=$MOZ_TOOLKIT"
  echo "        moz_debug=$moz_debug"
  echo "     moz_debugger=$moz_debugger"
fi
#
export MOZILLA_FIVE_HOME LD_LIBRARY_PATH
export SHLIB_PATH LIBPATH LIBRARY_PATH ADDON_PATH DYLD_LIBRARY_PATH

if [ $moz_debug -eq 1 ]
then
	moz_debug_program ${1+"$@"}
else
	moz_run_program ${1+"$@"}
fi

exit $exitcode
