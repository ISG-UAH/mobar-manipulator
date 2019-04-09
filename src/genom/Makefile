
# Directorios de libreria e includes
LIB_DIR := ../lib
EXTRA_INCLUDES = -I$(OPENROBOTS)/include

# ALL
all: LIB_COMMON

# COMMON
TARGETS_COMMON = CommonSs_Ops
OBJECTS_COMMON = $(TARGETS_COMMON:%=%.o)

$(TARGETS_COMMON): %:%.c
	@echo 'Building target: $@'
	gcc $< -c -fPIC -o $@.o $(EXTRA_INCLUDES)
	
LIB_COMMON: $(TARGETS_COMMON) $(OBJECTS_COMMON)
	@echo 'Linking objets for libcommon'
	gcc -shared -fPIC $(OBJECTS_COMMON) -o $(LIB_DIR)/libcommon.so