


#  WARNING 
#  WARNING 	DO NOT CHANGE ANYTHING IN THIS FILE
#  WARNING 	it is replaced at each module generation
#  WARNING
#  WARNING      If you want to install additional files, use the user-install
#  WARNING	target in codels/Makefile.in (see the comments in this file)
#  WARNING 




# --- paths -------------------------------------------------------------

top_builddir=  	..
top_srcdir=	../..
srcdir=		../../server
builddir=     	.

INSTALL=  	/usr/bin/install -c
VPATH=		$(srcdir)

include $(top_builddir)/config.mk
include $(top_srcdir)/$(autoconfdir)/config.$(OSAPI).mk


# --- flags -------------------------------------------------------------

CPPFLAGS+= $(ENDIANNESS)
CPPFLAGS+= -I. -I$(top_srcdir) -I$(srcdir)
CPPFLAGS+= $(GENOM_DEFINES) $(GENOM_INCLUDES) $(GENOM_CFLAGS)

# Put this BEFORE including genom.mk since genom.mk defines the regen and regen-user targets
all: \
	dirs client \
	server

# GenoM flags
include $(top_srcdir)/$(autoconfdir)/genom.mk


# --- source files ------------------------------------------------------

module_src= \
	proximityCntrlTask.c	\
	proximityModuleInit.c	\
	proximityproximityTask.c	\

client_src= \
	proximityError.c	\
	proximityEndian.c	\
	proximityPrint.c		\
	proximityPrintXML.c	\
	proximityScan.c		\
	proximityPosterLib.c	\
	proximityMsgLib.c	\
	proximityPosterShowLib.c	\
	proximityPosterXMLLib.c	\
	proximityPosterReadLib.c	\

client_includes = \
	server/proximityType.h \
	server/proximityScan.h \
	server/proximityPrintXML.h \
	server/proximityPrint.h \
	server/proximityPosterLibStruct.h \
	server/proximityError.h \
	server/proximityEndian.h \
	server/proximityPosterLib.h \
	proximityStruct.h \
 \
	server/proximityPosterXMLLib.h \
	server/proximityPosterWriteLibProto.h \
	server/proximityPosterShowLib.h \
	server/proximityPosterReadLibProto.h \
	server/proximityMsgLib.h \

init_src= \
	proximityInit.c

test_src= \
	proximityTest.c

connect_src= \
	proximityConnectLib.c

posterwrite_src= \
	proximityPosterWriteLib.c

client_obj=	$(client_src:%.c=$(OBJDIR)/%.lo)

module_obj=	$(module_src:%.c=$(OBJDIR)/%.lo)
init_obj=	$(init_src:%.c=$(OBJDIR)/%.lo)
test_obj=	$(test_src:%.c=$(OBJDIR)/%.lo)
connect_obj=	$(connect_src:%.c=$(OBJDIR)/%.lo)
posterwrite_obj=$(posterwrite_src:%.c=$(OBJDIR)/%.lo)


tclclient_src= \
	tcl/proximityTcl.c	\

tclrequires_src= \
	proximityPosterLib.c	\
	proximityEndian.c  \
	proximityError.c

tclclient_obj=	$(tclclient_src:%.c=$(OBJDIR)/%.lo)
tclrequires_obj=$(tclrequires_src:%.c=$(OBJDIR)/%.lo)

tclclient_tcl= \
	tcl/proximityClient.tcl

openprsclient_src= \
	openprs/proximityDecodeOpenprs.c \
	openprs/proximityEncodeOpenprs.c \
	openprs/proximityRequestsOpenprs.c
oprsclient_obj=$(openprsclient_src:%.c=$(OBJDIR)/%.lo)
oprsclient_headers = \
	openprs/proximityDecodeOpenprs.h \
	openprs/proximityEncodeOpenprs.h \
	openprs/proximityRequestsOpenprsProto.h
oprsclient_data = \
	openprs/proximityOpenprs.inc openprs/proximityOpenprs.opf openprs/proximityOpenprs.sym

tclservclient_src=\
	tclservClient/proximityError.c \
	tclservClient/proximityTclservClientEncode.c \
	tclservClient/proximityTclservClientDecode.c \
	tclservClient/proximityTclservClientMsgLib.c
tclservclient_obj=$(tclservclient_src:%.c=$(OBJDIR)/%.lo)
tclservclient_headers=\
	tclservClient/proximityError.h \
	tclservClient/proximityTclservClientEncode.h \
	tclservClient/proximityTclservClientDecode.h \
	tclservClient/proximityTclservClientMsgLib.h

python_data = \
	struct.py

# --- what to do --------------------------------------------------------


dirs: $(OBJDIR)
#dirs: $(OBJDIR)/tcl
#dirs: $(OBJDIR)/openprs
#dirs: $(OBJDIR)/tclservClient

$(OBJDIR):
	mkdir -p $@

$(OBJDIR)/tcl:
	mkdir -p $@

$(OBJDIR)/openprs:
	mkdir -p $@

$(OBJDIR)/tclservClient:
	mkdir -p $@

server: $(OBJDIR)/$(MODULE_LIB)
server: $(OBJDIR)/$(INIT_BIN)
server: $(OBJDIR)/$(POSTERWRITE_LIB)

client:: $(OBJDIR)/$(CLIENT_LIB)
client:: $(OBJDIR)/$(TEST_BIN) $(OBJDIR)/$(CONNECT)
#client:: $(OBJDIR)/$(TCLCLIENT_LIB)
#client:: $(OBJDIR)/$(OPENPRSCLIENT_MOD)
#client:: $(OBJDIR)/$(TCLSERVCLIENT_LIB)

# --- libraries ---------------------------------------------------------

$(OBJDIR)/$(MODULE_LIB): $(module_obj)
	$(LTLD) -o $@ $(LDFLAGS) $(module_obj)

$(OBJDIR)/$(POSTERWRITE_LIB): $(posterwrite_obj)
	$(LTLD) -o $@ $(LDFLAGS) $(posterwrite_obj)

ifeq ($(ABI_VERSION_INFO),)
LIBTOOL_VERSION_INFO = -avoid-version
else
LIBTOOL_VERSION_INFO = -version-info $(ABI_VERSION_INFO)
endif

$(OBJDIR)/$(CLIENT_LIB): $(client_obj)
	$(LTLD) -o $@ $(LDFLAGS) $(client_obj) $(SERVERS_CLIENT_LIB) \
		$(LIBTOOL_LINKOPT) $(LIBTOOL_VERSION_INFO) $(GENOM_LIBS)

$(OBJDIR)/$(TCLCLIENT_LIB): $(tclclient_obj) $(tclrequires_obj)
	$(LTLD) $(CFLAGS) $(TCLSERV_CFLAGS) $(tclclient_obj)		\
		$(tclrequires_obj) $(SERVERS_ENDIAN_LIB) -o $@		\
		$(LIBTOOL_LINKOPT) $(GENOM_LIBS)			\
		$(LDFLAGS) $(TCL_LDFLAGS) $(LIBTOOL_VERSION_INFO)

$(OBJDIR)/$(OPENPRSCLIENT_MOD): $(oprsclient_obj) $(OBJDIR)/$(CLIENT_LIB) $(OBJDIR)/$(TCLSERVCLIENT_LIB) 
	$(LTLD) -module -o $@ $(CFLAGS) $(oprsclient_obj) $(SERVERS_ENDIAN_LIB) \
		$(LIBTOOL_LINKOPT_OPENPRSCLIENT) $(OBJDIR)/$(CLIENT_LIB) \
		$(OBJDIR)/$(TCLSERVCLIENT_LIB) $(LDFLAGS) $(LIBTOOL_VERSION_INFO)

$(OBJDIR)/$(TCLSERVCLIENT_LIB): $(tclservclient_obj)
	$(LTLD) $(CFLAGS) $(tclservclient_obj) -o $@ \
		$(LIBTOOL_LINKOPT_TCLSERV_CLIENT) $(TCLSERV_CLIENT_LIB_LIBS) \
		$(TCLSERV_CLIENT_LIB_OTHER_LIBS) \
		$(LDFLAGS) $(LIBTOOL_VERSION_INFO)

# --- binaries ----------------------------------------------------------

$(OBJDIR)/$(INIT_BIN): $(init_obj) $(OBJDIR)/$(CLIENT_LIB)
	$(LTLD) $(CFLAGS) $(init_obj) -o $@ $(LDFLAGS)	\
		$(LIBTOOL_LINKOPT) $(LTLK_CLIENT_LIB)	\
		$(SERVERS_CLIENT_LIB) $(GENOM_LIBS)

$(OBJDIR)/$(TEST_BIN): $(test_obj) $(OBJDIR)/$(CLIENT_LIB)
	$(LTLD) $(CFLAGS) $(test_obj) -o $@  $(LDFLAGS) 	\
		$(LIBTOOL_LINKOPT) $(LTLK_CLIENT_LIB)		\
		$(SERVERS_CLIENT_LIB) $(GENOM_LIBS)


# --- basic rules -------------------------------------------------------

$(tclclient_obj): $(OBJDIR)/%.lo: %.c
	$(LTCC) -c $(CPPFLAGS) $(TCL_CPPFLAGS) -DDATADIR='"$(datadir)"' \
		$(CFLAGS) -o $@ $<  $(LIBTOOL_COPT)

$(oprsclient_obj): $(OBJDIR)/%.lo: %.c
	$(LTCC) -c $(CPPFLAGS) $(TRANSGEN_CFLAGS) $(TCLSERV_CLIENT_LIB_CFLAGS) \
		$(CFLAGS) -o $@ $<  $(LIBTOOL_COPT)

$(tclservclient_obj): $(OBJDIR)/%.lo: %.c
	$(LTCC) -c $(CPPFLAGS) $(TCLSERV_CLIENT_LIB_CFLAGS) \
		$(TCLSERV_CLIENT_LIB_OTHER_CFLAGS) \
		$(CFLAGS) -o $@ $< $(LIBTOOL_COPT)

$(OBJDIR)/%.lo: %.c
	$(LTCC) -c $(CPPFLAGS) $(CFLAGS) -o $@ $< $(LIBTOOL_COPT)


# --- install -----------------------------------------------------------

.PHONY: install install-bin install-data

install: all install-bin install-data

# Install libraries first
install-bin:
	$(MKINSTALLDIRS) ${DESTDIR}$(libdir)
	$(LTINSTALL) $(OBJDIR)/$(CLIENT_LIB) ${DESTDIR}$(libdir)/$(CLIENT_LIB)
#	$(MKINSTALLDIRS) ${DESTDIR}$(libdir)/tclserv
#	$(LTINSTALL) $(OBJDIR)/$(TCLCLIENT_LIB) ${DESTDIR}$(libdir)/tclserv/$(TCLCLIENT_LIB)
#	$(MKINSTALLDIRS) ${DESTDIR}$(libdir)/tclservClient
#	$(LTINSTALL) $(OBJDIR)/$(TCLSERVCLIENT_LIB) ${DESTDIR}$(libdir)/tclservClient/$(TCLSERVCLIENT_LIB)
#	$(MKINSTALLDIRS) ${DESTDIR}$(libdir)/openprs
#	$(LTINSTALL) $(OBJDIR)/$(OPENPRSCLIENT_MOD) ${DESTDIR}$(libdir)/openprs/$(OPENPRSCLIENT_MOD)
	$(MKINSTALLDIRS) ${DESTDIR}$(bindir)
	$(LTINSTALL) $(OBJDIR)/$(INIT_BIN) ${DESTDIR}$(bindir)/$(INIT_BIN)
	$(LTINSTALL) $(OBJDIR)/$(TEST_BIN) ${DESTDIR}$(bindir)/$(TEST_BIN)

install-data:
	$(MKINSTALLDIRS) ${DESTDIR}$(includedir)/server
	@for f in $(client_includes); do \
		$(INSTALL_DATA) $(top_srcdir)/$$f ${DESTDIR}$(includedir)/$$f;\
	done
	$(MKINSTALLDIRS) ${DESTDIR}$(datadir)/gen
	$(INSTALL_DATA) $(top_srcdir)/$(GENFILE) ${DESTDIR}$(datadir)/gen
#	$(MKINSTALLDIRS) ${DESTDIR}$(datadir)/tcl
#	$(INSTALL_DATA) $(srcdir)/$(tclclient_tcl) ${DESTDIR}$(datadir)/$(tclclient_tcl)
#	$(MKINSTALLDIRS) ${DESTDIR}$(datadir)/openprs
#	$(SED) -e 's|@MODULEDATADIR@|${DESTDIR}$(datadir)/openprs|' $(srcdir)/openprs/proximityOpenprs.inc.input >  $(srcdir)/openprs/proximityOpenprs.inc
#	( cd $(srcdir) && $(INSTALL_DATA) $(oprsclient_data) ${DESTDIR}$(datadir)/openprs )
#	$(MKINSTALLDIRS) ${DESTDIR}$(includedir)/server/openprs
#	( cd $(srcdir) && $(INSTALL_DATA) $(oprsclient_headers) ${DESTDIR}$(includedir)/server/openprs )
#	$(MKINSTALLDIRS) ${DESTDIR}$(includedir)/server/tclservClient
#	( cd $(srcdir) && $(INSTALL_DATA) $(tclservclient_headers) ${DESTDIR}$(includedir)/server/tclservClient )
#	$(MKINSTALLDIRS) ${DESTDIR}$(datadir)/python/${module}
#	touch ${DESTDIR}$(datadir)/python/${module}/__init__.py
#	cd $(srcdir) && $(INSTALL_DATA) $(python_data) ${DESTDIR}$(datadir)/python/${module}/

# --- clean -------------------------------------------------------------

.PHONY: clean

CLEAN_FILES = \
    $(client_obj) $(OBJDIR)/$(CLIENT_LIB) \

CLEAN_FILES += \
    $(module_obj) $(OBJDIR)/$(MODULE_LIB) \
    $(posterwrite_obj) $(OBJDIR)/$(POSTERWRITE_LIB) \
    $(connect_obj) \
    $(init_obj) $(OBJDIR)/$(INIT_BIN) \
    $(test_obj) $(OBJDIR)/$(TEST_BIN)

CLEAN_FILES += $(tclclient_obj) $(OBJDIR)/$(TCLCLIENT_LIB)
CLEAN_FILES += $(oprsclient_obj) $(OBJDIR)/$(OPENPRSCLIENT_MOD)
CLEAN_FILES += $(tclservclient_obj) $(OBJDIR)/$(TCLSERVCLIENT_LIB)

clean:
	$(LTRM) $(CLEAN_FILES)
	rm -f $(OBJDIR)/dependencies
	-if test -d $(OBJDIR); then rmdir $(OBJDIR); fi

distclean: clean
	rm -f Makefile


# --- dependencies ------------------------------------------------------

.PHONY: depend

make_depend=\
  $(MKDEP) -c"$(CC)" -o$(OBJDIR)/dependencies -d$(OBJDIR) -t.lo $(CPPFLAGS)

ifeq ($(findstring clean,$(MAKECMDGOALS)),)
depend $(OBJDIR)/dependencies:: $(OBJDIR);

depend $(OBJDIR)/dependencies::  $(client_src)
	$(make_depend) $?

depend $(OBJDIR)/dependencies:: $(module_src)
	$(make_depend) $?

depend $(OBJDIR)/dependencies:: $(posterwrite_src)
	$(make_depend) $?

depend $(OBJDIR)/dependencies::   $(connect_src)
	$(make_depend) $?

depend $(OBJDIR)/dependencies::  $(test_src)
	$(make_depend) $?

depend $(OBJDIR)/dependencies::  $(init_src)
	$(make_depend) $?

#depend $(OBJDIR)/dependencies:: $(tclclient_src)
#	$(make_depend) $(TCL_CPPFLAGS) $?

#depend $(OBJDIR)/dependencies:: $(tclrequires_src)
#	$(make_depend) $(TCL_CPPFLAGS) $?

#depend $(OBJDIR)/dependencies:: $(openprsclient_src)
#	$(make_depend) $(TRANSGEN_CFLAGS) $?

#depend $(OBJDIR)/dependencies:: $(tclservclient_src)
#	$(make_depend) $?

-include $(OBJDIR)/dependencies
endif

