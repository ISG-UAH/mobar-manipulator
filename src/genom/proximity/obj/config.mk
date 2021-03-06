
#
# Copyright (c) 2003-2013 LAAS/CNRS                        --  Mon Aug 25 2003
# All rights reserved.
#
# Redistribution  and  use in source   and binary forms,  with or without
# modification, are permitted provided that  the following conditions are
# met:
#
#   1. Redistributions  of  source code must  retain  the above copyright
#      notice, this list of conditions and the following disclaimer.
#   2. Redistributions in binary form must  reproduce the above copyright
#      notice,  this list of  conditions and  the following disclaimer in
#      the  documentation   and/or  other  materials   provided with  the
#      distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE  AUTHOR AND CONTRIBUTORS ``AS IS'' AND
# ANY  EXPRESS OR IMPLIED WARRANTIES, INCLUDING,  BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES   OF MERCHANTABILITY AND  FITNESS  FOR  A PARTICULAR
# PURPOSE ARE DISCLAIMED.  IN NO  EVENT SHALL THE AUTHOR OR  CONTRIBUTORS
# BE LIABLE FOR ANY DIRECT, INDIRECT,  INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING,  BUT  NOT LIMITED TO, PROCUREMENT  OF
# SUBSTITUTE  GOODS OR SERVICES;  LOSS   OF  USE,  DATA, OR PROFITS;   OR
# BUSINESS  INTERRUPTION) HOWEVER CAUSED AND  ON ANY THEORY OF LIABILITY,
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
# OTHERWISE) ARISING IN ANY WAY OUT OF THE  USE OF THIS SOFTWARE, EVEN IF
# ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#

host_cpu=	x86_64

abs_top_srcdir=   /home/pablich/Desktop/mobar-arm/src/genom/proximity/obj/..
abs_top_builddir= /home/pablich/Desktop/mobar-arm/src/genom/proximity/obj
module=		proximity
codelsdir=	codels
serverdir=	server
autoconfdir=	autoconf

prefix=		/usr/local
exec_prefix=	${prefix}
bindir=		${exec_prefix}/bin
libdir=		${exec_prefix}/lib
includedir=	${prefix}/include/proximity
datarootdir=	${prefix}/share
datadir=	${datarootdir}/modules

# --- Programs ----------------------------------------------------------

LANG_CXX=	no

CPP=		gcc -E
PURIFY=
SED=		sed
MKDEP=		/usr/bin/mkdep
MKINSTALLDIRS=  $(top_srcdir)/autoconf/mkinstalldirs

LIBTOOL=        $(SHELL) $(top_builddir)/libtool
CC=		gcc
LTCC=           $(LIBTOOL) --tag=CC --mode=compile $(CC)
ifeq ($(LANG_CXX),yes)
CXX=		
LTCXX=		$(LIBTOOL) --tag=CXX --mode=compile $(CXX)
endif

ifeq ($(LANG_CXX),yes)
LTLD=           $(LIBTOOL) --tag=CXX --mode=link $(PURIFY) $(CXX)
else
LTLD=           $(LIBTOOL) --tag=CC --mode=link $(PURIFY) $(CC)
endif
LTRM=           $(LIBTOOL) --mode=clean rm -f
LTINSTALL=	$(LIBTOOL) --mode=install $(INSTALL)

INSTALL_DATA=	$(INSTALL) -m 644

# --- Libraries ---------------------------------------------------------

# executables or modules
MODULE_BIN=	proximity$(BINEXT)
INIT_BIN=	proximitySendInitRqst$(BINEXT)
TEST_BIN=	proximityTest$(BINEXT)

# client libraries. These are versioned using the abi_version in the .gen
ABI_VERSION_INFO=	
CLIENT_LIB=		libproximityClient$(LIBEXT)
TCLCLIENT_LIB=		libproximityTclServ$(LIBEXT)
OPENPRSCLIENT_MOD=	modproximityOpenprs$(LIBEXT)
TCLSERVCLIENT_LIB=  libproximityTclservClient$(LIBEXT)

# Use that to link to our client library
MY_CLIENT_LIB=          $(GENOM_LIBS) $(abs_top_builddir)/server/$(OBJDIR)/$(CLIENT_LIB)

# libtool 'helper' libs -- static archives. Not installed
MODULE_LIB=		libproximityServer.la
USER_LIB=		libproximityCodels.la
POSTERWRITE_LIB=	libproximityPosterWrite.la

# --- Servers -----------------------------------------------------------


# --- Libraries required ------------------------------------------------

SERVERS_CLIENT_LIB=  
SERVERS_ENDIAN_LIB=  $(SERVERS_CLIENT_LIB)

CODEL_LIB=


# --- Flags -------------------------------------------------------------

OSAPI?=		posix


ENDIANNESS=	

GENOM_DEFINES=  
GENOM_INCLUDES= 
# Keep this until all codel have removed their #ifndef UNIX... stuff.
CPPFLAGS=	 -DUNIX
CFLAGS=		-g -O2 -pthread
LDFLAGS=	 -pthread

OBJDIR=		$(OSAPI)-obj

LIBMATH=        -lm 

RTAI_INC=	@RTAI_INC@
KERNEL_INC=	@KERNEL_INC@

XENO_CONFIG=	

# TCL_CPPFLAGS=	@TCL_CPPFLAGS@ @TCLSERV_CFLAGS@
# TCL_LDFLAGS=	@TCL_LDFLAGS@

# TRANSGEN_CFLAGS = @TRANSGEN_CFLAGS@
# TRANSGEN_LIBS = @TRANSGEN_LIBS@

# OPENPRS_CFLAGS  = @OPENPRS_CFLAGS@
# OPENPRS_LIBS    = @OPENPRS_LIBS@

# TCLSERV_CLIENT_LIB_CFLAGS = @TCLSERV_CLIENT_LIB_CFLAGS@
# TCLSERV_CLIENT_LIB_LIBS = @TCLSERV_CLIENT_LIB_LIBS@

# TCLSERV_CLIENT_LIB_OTHER_CFLAGS=  
# TCLSERV_CLIENT_LIB_OTHER_LIBS=  

GENOM_CFLAGS=	-I/opt/openrobots/include  
GENOM_LIBS=	-Wl,-rpath,/opt/openrobots/lib -L/opt/openrobots/lib -lgenom -lposterLib -lcomLib -lportLib -lrt -lm -ldl  

EXTRA_INCLUDES=	
EXTRA_LIBS=	

-include $(top_builddir)/local.mk

