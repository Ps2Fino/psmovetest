# Root directory for the project
ROOT_DIR=/Users/danielfinnegan/Documents/UBISS-Summer-School-2014

# Root directory for the psmoveapi
PS_MOVE_ROOT=$(ROOT_DIR)/psmoveapi_3.0.0_osx108/opt/psmoveapi_3.0.0

# Compiler and flags
CC=gcc
PS_MOVE_API_HDR=$(PS_MOVE_ROOT)/include/
PS_MOVE_API_LIB=$(PS_MOVE_ROOT)/lib/
C_FLAGS=-c -Wall -I$(PS_MOVE_API_HDR)
LD_FLAGS=-L$(PS_MOVE_API_LIB)
PS_MOVE_API=-lpsmoveapi

all: test
	./run_change_tool.sh

test: test.o
	$(CC) test.o $(LD_FLAGS) $(PS_MOVE_API) -o test

test.o:	test.c
	$(CC) $(C_FLAGS) test.c

clean:
	rm *.o test