# Copyright (c) 2006-2009, Universities Space Research Association (USRA).
#  All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in the
#       documentation and/or other materials provided with the distribution.
#     * Neither the name of the Universities Space Research Association nor the
#       names of its contributors may be used to endorse or promote products
#       derived from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY USRA ``AS IS'' AND ANY EXPRESS OR IMPLIED
# WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL USRA BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
# OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
# TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
# USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include $(PLEXIL_HOME)/makeinclude/standard-defs.make

#OSTYPE   ?= $(shell uname -s)
#MACHTYPE ?= $(shell uname -p)
#OS_ARCH = $(OSTYPE)-$(MACHTYPE)

#LIBRARY = SampleAdapter
#SRC = types.cc subscriber.cc sample_system.cc SampleAdapter.cc
#INC = types.hh subscriber.hh sample_system.hh SampleAdapter.hh

#INC_DIRS += $(PLEXIL_HOME)/third-party/pugixml/src $(PLEXIL_HOME)/src/utils $(PLEXIL_HOME)/src/exec $(PLEXIL_HOME)/src/app-framework

#LIBS		+= dl pugixml PlexilAppFramework PlexilUtils PlexilExec

#LIB_TARGET = lib$(LIB_NAME)$(LIB_EXT)

#include $(PLEXIL_HOME)/makeinclude/standard-targets.make

#test1:
#	plexilexec -p plans/plan1.plx

#-include Makedepend

include $(PLEXIL_HOME)/makeinclude/standard-defs.make

LIBRARY		:= $(NOM)Adapter

# External dependencies
GENOM_MOD	:= /home/goac/Desktop/MoBAr/src/GenoMmodules/$(NOM)
INC_DIRS	+= $(PLEXIL_HOME)/third-party/pugixml/src $(PLEXIL_HOME)/src/utils $(PLEXIL_HOME)/src/exec $(PLEXIL_HOME)/src/app-framework $(GENOM_MOD) $(GENOM_MOD)/server /opt/openrobots/include
LIB_PATH	+= -L/usr/local/lib
LIBS		+= dl pugixml PlexilAppFramework PlexilUtils PlexilExec algo3drovClient

SRC		:= types.cc subscriber.cc sample_system.cc
INC		:= types.hh subscriber.hh sample_system.hh

SRC		+= $(NOM)Adapter.cc
INC		+= $(NOM)Adapter.hh

SVN_FILES       := Makefile $(SRC) $(INC)

include $(PLEXIL_HOME)/makeinclude/standard-targets.make
ifneq ($(MAKECMDGOALS),clean)
-include Makedepend
endif
