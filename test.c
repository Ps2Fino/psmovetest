#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include "psmoveapi/psmove.h"

typedef struct game_struct {
	PSMove *controller;
	char *serial_number;
	int counter;
} Game_Struct;

void init(int id, Game_Struct *addr_struct)
{
	addr_struct = (Game_Struct*) malloc(sizeof(Game_Struct));
	addr_struct->controller = NULL;
	addr_struct->controller = psmove_connect_by_id(id);

	addr_struct->serial_number = NULL;
	enum PSMove_Connection_Type type = psmove_connection_type(addr_struct->controller);
	if (type == Conn_Bluetooth)
		addr_struct->serial_number = psmove_get_serial(addr_struct->controller);
	else
		addr_struct->serial_number = NULL;

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

int main (int argc, char **argv)
{
	int numControllers = atoi(argv[1]);

	Game_Struct **game_structs = NULL;
	game_structs = (Game_Struct**) malloc(numControllers * sizeof(Game_Struct*));

	for (int i=0; i < numControllers; ++i)
	{
		game_structs[i] = NULL;
		init(i, game_structs[i]);
	}

	float x, y, z;
	while(1) {
		for (int i=0; i < numControllers; ++i)
		{
			PSMove *controller = game_structs[i]->controller;
			if (psmove_poll(controller))
			{
				psmove_get_accelerometer_frame(controller, Frame_SecondHalf, &x, &y, &z);
				double x_sqr = pow(x, 2);
				double y_sqr = pow(y, 2);
				double z_sqr = pow(z, 2);
				int counter = game_structs[i]->counter;

				double norm = sqrt(x_sqr + y_sqr + z_sqr);
				if (norm >= 2)
				{
					counter += 0.1;
					game_structs[i]->counter = counter;
					char red = (char) floor(counter);
					printf("Controller %d: %.d\n", i, (int) red);
				}

				if (game_structs[i]->counter > 100)
				{
					break;
				}

				psmove_set_leds(controller, (char) floor(counter), 0, 0);
				psmove_update_leds(controller);	
			}
		}

	}

	// decide who to blink
	PSMove *blink_controller = NULL;
	for (int i = 0; i < numControllers; ++i)
	{
		if (game_structs[i]->counter > 100)
		{
			blink_controller = game_structs[i]->controller;
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
		destroy(game_structs[i]);
	}

	blink_controller = NULL;
	return 0;
}