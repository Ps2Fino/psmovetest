#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <time.h>

#include "psmoveapi/psmove.h"

#define GOAL 100

enum State {
	MOVE,
	DONT_MOVE,
	FINITO
};

typedef struct game_struct {
	PSMove *controller;
	char *serial_number;
	float counter;
	float dont_move_counter;
	int state;
	int dont_move_var;
} Game_Struct;

void init(int id, Game_Struct *addr_struct, int *bluetooth)
{
	addr_struct->controller = NULL;
	addr_struct->controller = psmove_connect_by_id(id);

	addr_struct->serial_number = NULL;
	enum PSMove_Connection_Type type = psmove_connection_type(addr_struct->controller);
	if (type == Conn_Bluetooth)
	{
		addr_struct->serial_number = psmove_get_serial(addr_struct->controller);
		printf("Serial number: %s\n", addr_struct->serial_number);
	}
	else
	{
		addr_struct->serial_number = NULL;
		*bluetooth = 0;
	}

	addr_struct->state = MOVE;
	addr_struct->dont_move_counter = 5;
	addr_struct->dont_move_var = rand() % 100 + 1;
	addr_struct->counter = 0;
}

void destroy(Game_Struct *addr_struct)
{
	psmove_disconnect(addr_struct->controller);
	if (addr_struct->serial_number != NULL)
	{
		free(addr_struct->serial_number);
	}
}

void print_debug(Game_Struct game_struct, int id)
{
	printf("%d\tDon't move variable: %d\n", id, game_struct.dont_move_var);
	printf("%d\tCounter value: %.2f\n", id, game_struct.counter);
}

int main (int argc, char **argv)
{
	if (argc != 2)
	{
		printf("Usage: set_color <color> <value> where the color is \n");
	}

	// Get the color and the value
	char color = argv[1][0];
	int color_as_int = atoi(argv[2]);
	char color_value = (char) color_as_int;

	Game_Struct controller_struct;
	init(0, &controller_struct, &color_as_int);

	PSMove *controller = controller_struct.controller;
	if (color == 'r')
	{
		psmove_set_leds(controller, (char) color_value, 0, 0);
		psmove_update_leds(controller);
	}
	else if (color == 'g')
	{
		psmove_set_leds(controller, 0, (char) color_value, 0);
		psmove_update_leds(controller);
	}
	else if (color == 'b')
	{
		psmove_set_leds(controller, 0, 0, (char) color_value);
		psmove_update_leds(controller);
	}
	else
	{
		printf("Enter r, g or b\n");
		return 0;
	}

	sleep(5);

	// Play the victory fanfare
	// system("afplay victory.mp3 &");

	// Disconnect and free
	destroy(&controller_struct);

	return 0;
}