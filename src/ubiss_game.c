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
		printf("Usage: ubiss-game <players> where players is an int describing the number of players\n");
		return 0;
	}

	// If we've no controllers, call it a day
	if (psmove_count_connected() <= 0)
	{
		printf("I need some controllers.....\n");
		return 0;
	}
	// Set the seed for the rand gen
	srand(time(NULL));
	int numControllers = atoi(argv[1]);

	if (numControllers <= 0)
	{
		printf("I need at least one reindeer (ie one controller)\n");
		return 0;
	}

	printf("Number of controllers: %d\n", numControllers);
	Game_Struct *game_structs = NULL;
	game_structs = (Game_Struct*) malloc(numControllers * sizeof(Game_Struct));

	int allBluetooth = 1;
	for (int i=0; i < numControllers; ++i)
	{
		init(i, &game_structs[i], &allBluetooth);
	}

	if (allBluetooth == 0)
	{
		printf("All connections must be bluetooth\n");
		for (int i=0; i < numControllers; ++i)
		{
			destroy(&game_structs[i]);
		}
		return 0;
	}

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
					}
					if (game_structs[i].counter > GOAL)
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
				}
				else if (game_structs[i].state == DONT_MOVE)
				{
					// Rumble and turn blue
					psmove_set_leds(controller, 0, 0, 255);
					psmove_set_rumble(controller, 128);
					psmove_update_leds(controller);

					game_structs[i].dont_move_counter -= 0.02;
					if (game_structs[i].dont_move_counter < 0)
					{
						psmove_set_rumble(controller, 0);
						psmove_update_leds(controller);
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
		if (game_structs[i].counter >= GOAL)
		{
			blink_controller = game_structs[i].controller;
			break;
		}
	}

	// Play the victory fanfare
	system("afplay victory.mp3 &");

	// Flash green
	for (int i = 0; i < 2; ++i)
	{
		psmove_set_rumble(blink_controller, 250);
		psmove_set_leds(blink_controller, 0, 255, 0);
		psmove_update_leds(blink_controller);
		sleep(1);
		psmove_set_rumble(blink_controller, 250);
		psmove_set_leds(blink_controller, 255, 255, 255);
		psmove_update_leds(blink_controller);
		sleep(1);
	}

	// Stop the vibration
	psmove_set_rumble(blink_controller, 0);
	psmove_update_leds(blink_controller);

	// Disconnect and free
	for (int i = 0; i < numControllers; ++i)
	{
		destroy(&game_structs[i]);
	}

	blink_controller = NULL;
	return 0;
}