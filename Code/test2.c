#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <wiringPi.h>

// Defines

// RF Module Synchronous I/O functions

unsigned int RF_Mod_Init(char *a);			// RF_Mod_Init(pb_addr)

unsigned int RF_Rd_Pkt(char a);				// RF_Rd_Pkt(pkt_type)

int main(void)
{

	unsigned int a;
	char packet_buffer[27];
	unsigned int i, j, k;
	char cmd[30];

// Use system calls to clear the screen and turn the cursor off

	strcpy(cmd, "clear");
	system(cmd);

//	strcpy(cmd, "setterm -cursor off");
//	system(cmd);

// Call initialization

	printf("Initializing...");
	a = RF_Mod_Init(packet_buffer);
	printf(" return code: %u\n\n", a);

// Loop sending read RF Module

	while (1) {

		for (k = 1; k <= 16; k++) {
			a = RF_Rd_Pkt(k);
			if ((a == -1) || (packet_buffer[0] != k)) {
				printf("%02X - Bad packet", k);
			} else {
				for (i = 0; i < 27; i++) {
					j = packet_buffer[i];
					printf("%02X ", j);
				}
				if (k == 3) {
					k = 15;
				}
			}
			printf("\n");

			usleep(700000);
		}
		printf("\n");
	}

	return 0;

}