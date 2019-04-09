include $(PLEXIL_HOME)/makeinclude/standard-defs.make

OSTYPE   ?= $(shell uname -s)
MACHTYPE ?= $(shell uname -p)
OS_ARCH = $(OSTYPE)-$(MACHTYPE)
NOM 		:= PDDL
LIBRARY		:= $(NOM)Adapter
PDDL_HOME	:= $(PWD)/../PDDL

# External dependencies
MOD_NAME	:= $(shell echo $(NOM) | tr '[:upper:]' '[:lower:]')

INC_DIRS	+= $(PLEXIL_HOME)/src/third-party/pugixml/src $(PLEXIL_HOME)/src/utils $(PLEXIL_HOME)/src/exec $(PLEXIL_HOME)/src/app-framework
INC_DIRS	+= $(PDDL_HOME)
LIB_PATH	+= $(MOBAR_PATH)/lib
LIBS		+= dl pugixml PlexilAppFramework PlexilUtils PlexilExec PDDL

SRC		:= subscriber.cc
INC		:= subscriber.hh

SRC		+= $(NOM)Adapter.cc
INC		+= $(NOM)Adapter.hh

SVN_FILES       := Makefile $(SRC) $(INC)

include $(PLEXIL_HOME)/makeinclude/standard-targets.make
ifneq ($(MAKECMDGOALS),clean)
-include Makedepend
endif
