#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include "psmoveapi/psmove.h"

enum Game_States {
	CONNECT_CONTROLLERS,
	PLAY_GAME,
	TURN_OFF_APP
};

int main (int argc, char **argv)
{
	// Connect to one of the controllers
	PSMove *controller_1 = NULL;

	controller_1 = psmove_connect_by_id(0);

	// Write the bluetooth address of the controller to the terminal
	enum PSMove_Connection_Type type = psmove_connection_type(controller_1);
	type == Conn_Bluetooth ? printf("Connection type: bluetooth\n") : printf("Connection type: USB\n");

	// enum PSMove_Battery_Level	

	char *serial_number = NULL;
	if (type == Conn_Bluetooth)
	{
		serial_number = psmove_get_serial(controller_1);
		printf("The serial number is: %s\n", serial_number);
	}

	psmove_enable_orientation(controller_1, PSMove_True);
	float x, y, z;
	float counter = 0;
	while(1) {
		if (psmove_poll(controller_1)) {
			printf("We got data\n");
			// Grab some data and normalise it
			// psmove_get_accelerometer(controller_1, &x, &y, &z);
			psmove_get_accelerometer_frame(controller_1, Frame_SecondHalf, &x, &y, &z);
			double x_sqr = pow(x, 2);
			double y_sqr = pow(y, 2);
			double z_sqr = pow(z, 2);

			double norm = sqrt(x_sqr + y_sqr + z_sqr);
			// printf("Norm: %.2f\tX: %d\tY: %d\tZ:%d\n", norm, x, y, z);
			// printf("Norm: %.2f\tX: %.2f\tY: %.2f\tZ:%.2f\n", norm, x, y, z);

			if (norm >= 2)
			{
				counter += 0.2;
				char red = (char) floor(counter);
				psmove_set_leds(controller_1, red, 0, 0);
				psmove_update_leds(controller_1);
				printf("RED: %.d\n", (int) red);
			}
			else {
				psmove_set_leds(controller_1, 255, 255, 255);
				psmove_update_leds(controller_1);				
			}
		}
	}

	// Disconnect the controller
	psmove_disconnect(controller_1);

	free(controller_1);
	if (serial_number != NULL) free(serial_number);
	controller_1 = NULL;
	serial_number = NULL;
	return 0;
}