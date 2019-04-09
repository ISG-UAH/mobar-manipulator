CC		:=	g++
DEV		:=	-g -Wall
OPT		:=	-O2
CXXFLAGS	:=	-I include
LIBA	:=	-l PTHC -l PDDL -l pthread
LIBSO	:=	-fPIC -shared

# PATHS
SRC		:=	src
BIN		:=	bin
LIB		:=	lib

# SOURCES PATH
PDDL	:=	$(SRC)/PDDL
PLEX	:=	$(SRC)/plexil
GEN		:=	$(SRC)/genom
PLEX3	:=	$(BIN)/plexil-3/bin

export MOBAR_HOME=`pwd`



# MODULES, INTERFACE ADAPTERS AND PLANS
MODULES	:=	clamp proximity locomotion
ADAPTERS:=	Clamp Locomotion
PL_DEBUG:=	$(PLEX)/Debug.cfg
PL_CONFIG:=	$(PLEX)/config.xml
PL_PLAN	:=	$(PLEX)/plans/prueba
LIBNODES := MoveArm SearchBlock

all: controller plexilPDDL genom plexiladapters

# CONTROLLER
controller:
	@echo 'Building controller binaries'
	cd $(SRC); make;
	
# PDDL
plexilPDDL: libPDDL
	@echo 'Building PDDL Interface Adapter'
	export NOM=PDDL; cd $(PLEX); cp MakefilePDDL Makefile && make && rm Makefile;

libPDDL:
	@echo 'Building PDDL library'
	$(CC) $(LIBSO) $(PDDL)/*.cpp -o $(LIB)/libPDDL.so
	
# GENOM
genom: $(MODULES)
$(MODULES): %:%
	@echo 'Building target: $@'
	export MOD=$@; make genomall;
	
genomall: genommoduleinit genommodule genommoduleinstall
genommoduleinit:
	@echo 'Initial build of GenoM module: $(MOD)'
	cd $(GEN)/$(MOD); mv codels codels-backup; genom $(MOD) -i && rm -r codels && mv codels-backup codels && mkdir obj; cd obj && ../configure

genommodule:
	@echo 'Building GenoM module: $(MOD)'
	cd $(GEN)/$(MOD); genom -i $(MOD) && cd obj && make
	
genommoduleinstall:
	@echo 'Installing GenoM module: $(MOD)'
	cd $(GEN)/$(MOD)/obj && sudo make install

# PLEXIL ADAPTERS
plexiladapters: $(ADAPTERS)
$(ADAPTERS): %:%
	@echo 'Building plexil adapter: $@'
	export NOM=$@; cd $(PLEX); cp MakefileAdapter Makefile && make && rm Makefile;
	
start:
	cd $(BIN) && gnome-terminal -e "./controller_server" --tab
	
startgenom:
	h2 init && clamp -b; proximity -b; locomotion -b;

libNodes: $(LIBNODES)
$(LIBNODES): %:%
	cd $(PLEX)/plans; plexilc $@.ple
	
plexilplan: libNodes $(PL_PLAN).ple
	plexilc $(PL_PLAN).ple
	
startplexil: plexilplan
	./$(PLEX3)/universalExec -p $(PL_PLAN).plx -L $(PLEX)/plans -d $(PL_DEBUG) -c $(PL_CONFIG)
	#plexilexec -v -b -p $(PL_PLAN).plx -d $(PL_DEBUG) -c $(PL_CONFIG)
	
stop:
	killmodule clamp; killmodule proximity; killmodule locomotion; h2 end;
	
clean: deltemp
	rm -f $(PLEX)/*.so
	find -name "*.o" -exec rm {} \;

deltemp:
	find -name "*~" -exec rm {} \;
