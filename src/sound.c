#include <stdlib.h>
#include <stdio.h>

int main (int argc, char **argv)
{
	system("afplay victory.mp3 &");
	printf ("I didn't wait\n");
	return 0;
}