CC		=	g++
LIBSO	=	-fPIC -shared
LIB		=	../../lib
PLEX	=	../plexil

all: 
	$(CC) $(LIBSO) *.cpp -o $(LIB)/libPDDL.so
	export NOM=PDDL; cd $(PLEX); make;