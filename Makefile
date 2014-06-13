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

all: ubiss_game set_color
	./run_change_tool.sh bin/ubiss_game
	./run_change_tool.sh bin/set_color
	cp victory.mp3 bin/victory.mp3

bin:
	mkdir bin

ubiss_game: ubiss_game.o bin
	$(CC) ubiss_game.o $(LD_FLAGS) $(PS_MOVE_API) -o bin/ubiss_game

set_color: set_color.o bin
	$(CC) set_color.o $(LD_FLAGS) $(PS_MOVE_API) -o bin/set_color

ubiss_game.o:	ubiss_game.c
	$(CC) $(C_FLAGS) ubiss_game.c

set_color.o: set_color.c
	$(CC) $(C_FLAGS) set_color.c 

distclean:
	rm -rf bin/

clean:
	rm *.o