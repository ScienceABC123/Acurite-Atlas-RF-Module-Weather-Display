#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
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

// Function to calculate solar radiation from (lux / 10) (source: www.gwhiteqcarol.weebly.com/blog/convert-lux-to-watts-per-square-metre-download)

float Solar(float lux) {

	float solar;

	solar = 0.0079 * (lux * 10);

	return solar;
}

// Function to print integer as binary

int PrintIntAsBin(int num) {

	unsigned int i;
	char str[30];

	sprintf(str, "%04X", num);
	i = 0;
	while (str[i]) {
		switch (str[i]) {
			case '0':
			printf("0000");
			break;
			case '1':
			printf("0001");
			break;
			case '2':
			printf("0010");
			break;
			case '3':
			printf("0011");
			break;
			case '4':
			printf("0100");
			break;
			case '5':
			printf("0101");
			break;
			case '6':
			printf("0110");
			break;
			case '7':
			printf("0111");
			break;
			case '8':
			printf("1000");
			break;
			case '9':
			printf("1001");
			break;
			case 'A':
			printf("1010");
			break;
			case 'B':
			printf("1011");
			break;
			case 'C':
			printf("1100");
			break;
			case 'D':
			printf("1101");
			break;
			case 'E':
			printf("1110");
			break;
			case 'F':
			printf("1111");
			break;
		}
		if(str[i + 1] != 0) {
			printf("_");
		}
		i = i + 1;
	}
	return 0;
}

int main(void)
{

	char packet_buffer[27];
	float a;
	float b;
	float c;
	int i;
	float lux;
	char str[30];

// Use system calls to clear the screen and turn the cursor off

	strcpy(str, "clear");
	system(str);

//	strcpy(cmd, "setterm -cursor off");
//	system(cmd);

// Call initialization

	printf("Initializing...");
	i = RF_Mod_Init(packet_buffer);
	printf(" return code: %u\n\n", i);

	printf("Enter lower limit: ");
	scanf("%d", &i);
	a = i;
	printf("Enter upper limit: ");
	scanf("%d", &i);
	b = i;

	printf("\nLooking for W/m^2 values between %.0f and %.0f...\n\n", a, b);

// Loop forever...

	while (1) {

		// Get current lux value

		i = 1;
		while (i) {
			if (RF_Rd_Pkt(16) != -1) {

				// Get lux/solar values

				lux = (packet_buffer[6] << 8) + packet_buffer[7];

				// Exit the while loop

				i = 0;
			} else {
				usleep(700000);
			}
		}

		// Display lux values between low/high limits

		c = Solar(lux);
		i = lux;
		if ((c >= a) && (c <= b)) {
			printf("%4.0f W/m^2  ", c);
			printf("lux: %04X = ", i);
			PrintIntAsBin(i);
			printf("\n");
		}

		// Sleep for 30 seconds

		sleep(30);
	}

	return 0;

}