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
	PSMove *controller_2 = NULL;
	PSMove *controller_3 = NULL;

	controller_1 = psmove_connect_by_id(0);
	controller_2 = psmove_connect_by_id(1);
	controller_3 = psmove_connect_by_id(2);

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

	// psmove_enable_orientation(controller_1, PSMove_True);
	float x, y, z;

	float counter_1 = 0;
	float counter_2 = 0;
	float counter_3 = 0;

	while(1) {
		if (psmove_poll(controller_1))
		{
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
				counter_1 += 0.1;
				char red = (char) floor(counter_1);
				printf("RED: %.d\n", (int) red);
			}

			if (counter_1 > 100)
			{
				break;
			}

			psmove_set_leds(controller_1, (char) floor(counter_1), 0, 0);
			psmove_update_leds(controller_1);			
		}

		if (psmove_poll(controller_2))
		{
			printf("We got data\n");
			// Grab some data and normalise it
			// psmove_get_accelerometer(controller_1, &x, &y, &z);
			psmove_get_accelerometer_frame(controller_2, Frame_SecondHalf, &x, &y, &z);
			double x_sqr = pow(x, 2);
			double y_sqr = pow(y, 2);
			double z_sqr = pow(z, 2);

			double norm = sqrt(x_sqr + y_sqr + z_sqr);
			// printf("Norm: %.2f\tX: %d\tY: %d\tZ:%d\n", norm, x, y, z);
			// printf("Norm: %.2f\tX: %.2f\tY: %.2f\tZ:%.2f\n", norm, x, y, z);

			if (norm >= 2)
			{
				counter_2 += 0.1;
				char red = (char) floor(counter_2);
				printf("RED: %.d\n", (int) red);
			}

			if (counter_2 > 100)
			{
				break;
			}

			psmove_set_leds(controller_2, (char) floor(counter_2), 0, 0);
			psmove_update_leds(controller_2);			
		}

		if (psmove_poll(controller_3))
		{
			printf("We got data\n");
			// Grab some data and normalise it
			// psmove_get_accelerometer(controller_1, &x, &y, &z);
			psmove_get_accelerometer_frame(controller_3, Frame_SecondHalf, &x, &y, &z);
			double x_sqr = pow(x, 2);
			double y_sqr = pow(y, 2);
			double z_sqr = pow(z, 2);

			double norm = sqrt(x_sqr + y_sqr + z_sqr);
			// printf("Norm: %.2f\tX: %d\tY: %d\tZ:%d\n", norm, x, y, z);
			// printf("Norm: %.2f\tX: %.2f\tY: %.2f\tZ:%.2f\n", norm, x, y, z);

			if (norm >= 2)
			{
				counter_3 += 0.1;
				char red = (char) floor(counter_3);
				printf("RED: %.d\n", (int) red);
			}

			if (counter_3 > 100)
			{
				break;
			}

			psmove_set_leds(controller_3, (char) floor(counter_3), 0, 0);
			psmove_update_leds(controller_3);			
		}

	}

	// decide who to blink
	PSMove *blink_controller = NULL;
	if (counter_1 > 100)
	{
		blink_controller = controller_1;
	}
	else if(counter_2 > 100) 
	{
		blink_controller = controller_2;
	}
	else
	{
		blink_controller = controller_3;
	}

	while (1)
	{
		// Flash green
		psmove_set_leds(blink_controller, 0, 255, 0);
		psmove_update_leds(blink_controller);
		sleep(1);
		psmove_set_leds(blink_controller, 255, 255, 255);
		psmove_update_leds(blink_controller);
		sleep(1);
	}

	// Disconnect the controller
	psmove_disconnect(controller_1);
	psmove_disconnect(controller_2);
	psmove_disconnect(controller_3);

	free(controller_1);
	free(controller_2);
	free(controller_3);
	if (serial_number != NULL) free(serial_number);
	controller_1 = NULL;
	controller_2 = NULL;
	controller_3 = NULL;
	blink_controller = NULL;
	serial_number = NULL;
	return 0;
}