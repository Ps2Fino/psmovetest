# Compiler and flags
CC=gcc
PS_MOVE_API_HDR=include/
PS_MOVE_API_LIB=lib/
C_FLAGS=-c -Wall -I$(PS_MOVE_API_HDR)
LD_FLAGS=-L$(PS_MOVE_API_LIB) -headerpad_max_install_names
PS_MOVE_API=-lpsmoveapi
SRC=src/
ASSETS=assets/

all: ubiss_game set_color
	@echo Running utils/change_tool.sh on bin/ubiss_game...
	@utils/run_change_tool.sh bin/ubiss_game
	@echo Running utils/change_tool.sh on bin/set_color...
	@utils/run_change_tool.sh bin/set_color
	@echo Setting up the bin directory...
	@cp $(ASSETS)/victory.mp3 bin/victory.mp3

bin:
	@mkdir bin

ubiss_game: ubiss_game.o bin
	@echo Linking bin/ubiss_game with the PS Move library...
	@$(CC) ubiss_game.o $(LD_FLAGS) $(PS_MOVE_API) -o bin/ubiss_game

set_color: set_color.o bin
	@echo Linking bin/set_color with the PS Move library...
	@$(CC) set_color.o $(LD_FLAGS) $(PS_MOVE_API) -o bin/set_color

ubiss_game.o:	$(SRC)/ubiss_game.c
	@echo Compiling the sources for the ubiss/game...
	@$(CC) $(C_FLAGS) $(SRC)/ubiss_game.c

set_color.o: $(SRC)/set_color.c
	@echo Compiling the sources for the set_color demo...
	@$(CC) $(C_FLAGS) $(SRC)/set_color.c 

distclean:
	@echo Full clean...
	@rm -f *.o
	@rm -rf bin/

clean:
	@echo Cleaning...
	@rm -f *.o
