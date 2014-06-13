# Root directory for the project
ROOT_DIR=/Users/danielfinnegan/Documents/UBISS-Summer-School-2014

# Root directory for the psmoveapi
PS_MOVE_ROOT=$(ROOT_DIR)/psmoveapi_3.0.0_osx108/opt/psmoveapi_3.0.0

# Compiler and flags
CC=gcc
PS_MOVE_API_HDR=$(PS_MOVE_ROOT)/include/
PS_MOVE_API_LIB=$(PS_MOVE_ROOT)/lib/
C_FLAGS=-c -Wall -I$(PS_MOVE_API_HDR)
LD_FLAGS=-L$(PS_MOVE_API_LIB) -headerpad_max_install_names
PS_MOVE_API=-lpsmoveapi

all: ubiss-game
	./run_change_tool.sh

bin:
	mkdir bin

ubiss-game: test.o bin
	$(CC) test.o $(LD_FLAGS) $(PS_MOVE_API) -o bin/ubiss-game

test.o:	test.c
	$(CC) $(C_FLAGS) test.c

distclean:
	rm -rf bin/

clean:
	rm *.o