#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <time.h>

#include "psmoveapi/psmove.h"

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

void init(int id, Game_Struct *addr_struct)
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
	// Set the seed for the rand gen
	srand(time(NULL));
	int numControllers = atoi(argv[1]);
	printf("Number of controllers: %d\n", numControllers);
	// return 0;

	Game_Struct *game_structs = NULL;
	game_structs = (Game_Struct*) malloc(numControllers * sizeof(Game_Struct));

	for (int i=0; i < numControllers; ++i)
	{
		init(i, &game_structs[i]);
	}

	// sleep(2);

	printf("game_structs[0] serial_number: %s\n", game_structs[0].serial_number);

	int doLoop = 1;
	float x, y, z;
	while(doLoop) {
		for (int i=0; i < numControllers; ++i)
		{
			PSMove *controller = game_structs[i].controller;
			if (psmove_poll(controller))
			{
				psmove_get_accelerometer_frame(controller, Frame_SecondHalf, &x, &y, &z);

				// Check for the change in state
				if (game_structs[i].state == MOVE
					&& game_structs[i].counter > game_structs[i].dont_move_var)
				{
					game_structs[i].state = DONT_MOVE;
				}

				if (game_structs[i].state == MOVE 
					|| game_structs[i].state == FINITO)
				{
					double x_sqr = pow(x, 2);
					double y_sqr = pow(y, 2);
					double z_sqr = pow(z, 2);

					double norm = sqrt(x_sqr + y_sqr + z_sqr);
					printf("Norm: %.2f\n", norm);

					if (norm >= 2)
					{
						game_structs[i].counter += 0.1;
						// char red = (char) floor(game_structs[i].counter);
						// printf("Controller %d rising: %d\n", i, (int) red);
					}
					if (game_structs[i].counter > 20)
					{
						doLoop = 0;
						break;
					}

					if (game_structs[i].counter > 0.02)
					{
						game_structs[i].counter -= 0.02;
					}
					psmove_set_leds(controller, (char) floor(game_structs[i].counter), 0, 0);
					psmove_update_leds(controller);

					// char red = (char) floor(game_structs[i].counter);
					// printf("Controller %d descending: %d\n", i, (int) red);
				}
				else if (game_structs[i].state == DONT_MOVE)
				{
					psmove_set_leds(controller, 0, 0, 255);
					psmove_update_leds(controller);

					game_structs[i].dont_move_counter -= 0.02;
					if (game_structs[i].dont_move_counter < 0)
					{
						game_structs[i].state = FINITO;
					}
				}

				print_debug(game_structs[i], i);
			}
		}
	}

	// decide who to blink
	PSMove *blink_controller = NULL;
	for (int i = 0; i < numControllers; ++i)
	{
		printf("Counter: %.2f\n", game_structs[i].counter);
		if (game_structs[i].counter >= 20)
		{
			blink_controller = game_structs[i].controller;
			break;
		}
	}

	// Flash green
	for (int i = 0; i < 5; ++i)
	{
		psmove_set_leds(blink_controller, 0, 255, 0);
		psmove_update_leds(blink_controller);
		sleep(1);
		psmove_set_leds(blink_controller, 255, 255, 255);
		psmove_update_leds(blink_controller);
		sleep(1);
	}

	// Disconnect and free
	for (int i = 0; i < numControllers; ++i)
	{
		destroy(&game_structs[i]);
	}

	blink_controller = NULL;
	return 0;
}