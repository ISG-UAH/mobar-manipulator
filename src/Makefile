KDL_INCLUDE_DIR := /usr/include/kdl
KDL_LIB_DIR := /usr/lib

EIGEN2_PATH :=/usr/include/eigen2

# Directorios de libreria e includes
LIB_DIR := ../lib
BIN_DIR := ../bin
INCLUDE_DIR := ../include
INCLUDE_GENOM := ../genom

EXTRA_LIBS = `PKG_CONFIG_PATH=/opt/openrobots/lib/pkgconfig:/usr/lib/pkgconfig pkg-config orocos-kdl --libs --cflags` -llog4cpp -lnsl -lm -L$(LIB_DIR)
EXTRA_LIBS2 = -lcommunication -llocomotion
EXTRA_INCLUDES = -I$(INCLUDE_DIR) -I$(INCLUDE_GENOM) -I$(EIGEN2_PATH) -I$(KDL_INCLUDE_DIR) -I.

CFLAGS=-c -fPIC
LIBFLAGS = -shared -fPIC -ldl -rdynamic -lnsl -lpthread -export-dynamic -shared-libgcc

# ALL
all: LIB_COM LIB_LOC RPC

# COMMUNICATION
TARGETS_COM = communication Serial clamp proximitysensors
OBJECTS_COM = $(TARGETS_COM:%=%.o)

$(TARGETS_COM): %:%.cpp
	@echo 'Building target: $@'
	gcc $< -c -fPIC -o $@.o $(EXTRA_INCLUDES)
	cp $@.h* $(INCLUDE_DIR)/ 
	
LIB_COM: $(TARGETS_COM) $(OBJECTS_COM)
	@echo 'Linking objets for libcommunication'
	gcc -shared -fPIC $(OBJECTS_COM) -o $(LIB_DIR)/libcommunication.so

# LOCOMOTION
TARGETS_LOC = chainiksolverpos_nr_jl_we locomotion
OBJECTS_LOC = $(TARGETS_LOC:%=%.o)

$(TARGETS_LOC): %:%.cpp
	@echo 'Building target: $@'
	gcc $< -c -fPIC -o $@.o $(EXTRA_INCLUDES)
	cp $@.h* $(INCLUDE_DIR)/ 
	
LIB_LOC: $(TARGETS_LOC) $(OBJECTS_LOC)
	@echo 'Linking objets for liblocomotion'
	gcc -shared -fPIC $(OBJECTS_LOC) -o $(LIB_DIR)/liblocomotion.so	 $(EXTRA_LIBS) -lcommunication
	
# RPC Server
RPC: clnt server

SERVER = controller_server
SOURCES_SVC.c = controller_server
SOURCES_CLNT.h = controller.h
TARGETS_CLNT.c = controller_clnt controller_xdr controller_svc

OBJECTS_SVC = $(SOURCES_SVC.c:%=%.o)
OBJECTS_CLNT = $(TARGETS_CLNT.c:%=%.o)

$(OBJECTS_SVC) : $(TARGETS_SVC.c)

$(SOURCES_SVC.c): %:%.cpp
	@echo 'Building target: $@'
	gcc $< -c -fPIC -lnsl -o $@.o $(EXTRA_INCLUDES)
	
$(TARGETS_CLNT.c): %:%.c
	@echo 'Building target: $@'
	gcc $< -c -fPIC -lnsl -o $@.o

clnt: $(TARGETS_CLNT.c) $(OBJECTS_CLNT)
	@echo 'Linking objets for libcontroller'
	gcc $(LIBFLAGS) $(OBJECTS_CLNT) -o $(LIB_DIR)/libcontroller.so
	
server : $(TARGETS_CLNT.c) $(OBJECTS_CLNT) $(SOURCES_SVC.c)
	@echo 'Linking objets for $(SERVER) program'
	g++ -o $(BIN_DIR)/$(SERVER) $(SOURCES_SVC.c).o $(OBJECTS_CLNT) $(EXTRA_INCLUDES) $(EXTRA_LIBS) $(EXTRA_LIBS2)
	cp log4cpp.properties $(BIN_DIR)


# TEST
TESTP := testlocomotion

test: $(TESTP).cpp
	gcc -c -o $(TESTP).o $(TESTP).cpp $(EXTRA_LIBS) $(EXTRA_LIBS2) $(EXTRA_INCLUDES)
	gcc -o $(TESTP) $(TESTP).o $(EXTRA_LIBS) $(EXTRA_LIBS2) $(EXTRA_INCLUDES)
	
testclamp: clamp.cpp clamp.h testclamp.cpp
	g++ -c -o clamp.o clamp.cpp
	g++ -c -o testclamp.o testclamp.cpp -g
	g++ testclamp.o clamp.o -o testclamp -lm

clean:
	rm *.o $(LIB_DIR)/*.so
