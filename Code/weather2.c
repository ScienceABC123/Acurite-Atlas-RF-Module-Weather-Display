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

#define	WHITE 0x00FFFFFF
#define BLACK 0x00000000
#define	DK_GRAY	0x001F1F1F
#define	MED_GRAY 0x006F6F6F
#define	LT_GRAY 0x00BFBFBF

#define RED 0x00FF0000
#define GREEN 0x0000FF00
#define BLUE 0x000000FF

// The colors below are courtesy of www.color-name.com

#define BRIGHT_RED 0x00FF160C

#define RAINBOW_ORANGE 0x00FF8F33
#define BRIGHT_ORANGE 0x00FF8B28

#define EXTRA_BRIGHT_YELLOW 0x00FFF70A
#define BRIGHT_YELLOW 0x00FFEB2A
#define BRIGHT_GOLDEN_YELLOW 0x00FFE138
#define HOT_YELLOW 0x00FFDA00
#define SUNGLOW 0x00FFCC33

#define BITTER_LEMON 0x00B5E61D
#define LIME_GREEN 0x007AB226
#define BRIGHT_APPLE_GREEN 0x0056BD00
#define BRIGHT_GREEN 0x0066FF00
#define YELLOW_GREEN 0x009ACD32

#define BLUE_SHINE 0x004199FD
#define DODGER_BLUE 0x001F89FD
#define UNESCO_BLUE 0x000077D4
#define TRUE_BLUE 0x000071CE
#define RUSSIAN_BLUE 0x002A9AF6
#define BRIGHT_BLUE 0x00183EFA
#define BRIGHT_NAVY_BLUE 0x001974D2
#define BRIGHT_SKY_BLUE 0x0077C4FE
#define ICE_BLUE 0x0099FFFF

#define SHINY_PURPLE 0x00B941FF
#define PURPLE_GLITTER 0x008E3FE2
#define BRIGHT_PURPLE 0x00A908B5
#define FRESH_PURPLE 0x00D260E1

// RF Module Synchronous I/O functions

unsigned int RF_Mod_Init(char *a);																// RF_Mod_Init(pb_addr)

unsigned int RF_Rd_Pkt(char a);																	// RF_Rd_Pkt(pkt_type)

// Simple ARM Graphics Library functions

unsigned int SAGL_Init(char *a, unsigned int b, unsigned int c);								// SAGL_Init(fb_addr, h_size, v_size)

unsigned int ScreenClear(unsigned int a);														// ScreenClear(color)

unsigned int SelectColors(unsigned int a, unsigned int b);										// SelectColors(f_color, b_color)

unsigned int SelectFont(unsigned int *a);														// SelectFont(font_addr)
unsigned int PrintStringLeft(char *a, unsigned int b, unsigned int c);							// PrintStringLeft(string_ptr, x_pos, y_pos)
unsigned int PrintStringRight(char *a, unsigned int b, unsigned int c);							// PrintStringRight(string_ptr, x_pos, y_pos)

unsigned int DrawRectOutline(unsigned int a, unsigned int b, unsigned int c, unsigned int d);	// DrawRectOutline(x1_pos, y1_pos, x2_pos, y2_pos)
unsigned int DrawRectFilled(unsigned int a, unsigned int b, unsigned int c, unsigned int d);	// DrawRectFilled(x1_pos, y1_pos, x2_pos, y2_pos)

unsigned int DrawCircleFilled(unsigned int a, unsigned int b, unsigned int c);					// DrawCircleFilled(x_pos, y_pos, radius)

unsigned int DrawLine(unsigned int a, unsigned int b, unsigned int c, unsigned int d);			// DrawLine(x1_pos, y1_pos, x2_pos, y2_pos)

// Fonts

extern unsigned int Font5x9;
extern unsigned int FontArial32;
extern unsigned int FontArial104;

 /*******************************************************************************
 *
 * Function to draw graphing areas
 *
 *******************************************************************************/

int DrawGraph(int x, int y, int x_size, int y_size, int hr_size) {
	SelectColors(BLACK, BLACK);
	DrawRectFilled(x - 1, y + 1, x + x_size, y - y_size);
	SelectColors(DK_GRAY, DK_GRAY);
	DrawRectFilled(x + (1 * hr_size), y, x + ((2 * hr_size) - 1), y - (y_size - 1));
	DrawRectFilled(x + (3 * hr_size), y, x + ((4 * hr_size) - 1), y - (y_size - 1));
	DrawRectFilled(x + (5 * hr_size), y, x + ((6 * hr_size) - 1), y - (y_size - 1));
	DrawRectFilled(x + (7 * hr_size), y, x + ((8 * hr_size) - 1), y - (y_size - 1));
	DrawRectFilled(x + (9 * hr_size), y, x + ((10 * hr_size) - 1), y - (y_size - 1));
	DrawRectFilled(x + (11 * hr_size), y, x + ((12 * hr_size) - 1), y - (y_size - 1));
	DrawRectFilled(x + (13 * hr_size), y, x + ((14 * hr_size) - 1), y - (y_size - 1));
	DrawRectFilled(x + (15 * hr_size), y, x + ((16 * hr_size) - 1), y - (y_size - 1));
	DrawRectFilled(x + (17 * hr_size), y, x + ((18 * hr_size) - 1), y - (y_size - 1));
	DrawRectFilled(x + (19 * hr_size), y, x + ((20 * hr_size) - 1), y - (y_size - 1));
	DrawRectFilled(x + (21 * hr_size), y, x + ((22 * hr_size) - 1), y - (y_size - 1));
	DrawRectFilled(x + (23 * hr_size), y, x + ((24 * hr_size) - 1), y - (y_size - 1));
	SelectColors(MED_GRAY, BLACK);
	DrawLine(x + (0 * hr_size), y, x + (0 * hr_size), y - (y_size - 1));
	DrawLine(x + (6 * hr_size), y, x + (6 * hr_size), y - (y_size - 1));
	DrawLine(x + (12 * hr_size), y, x + (12 * hr_size), y - (y_size - 1));
	DrawLine(x + (18 * hr_size), y, x + (18 * hr_size), y - (y_size - 1));
	DrawLine(x + (24 * hr_size), y, x + (24 * hr_size), y - (y_size - 1));

	return 0;
}

 /*******************************************************************************
 *
 * Function to draw thick lines
 *
 *******************************************************************************/

int DrawThickLine(int x1, int y1, int x2, int y2) {
	DrawLine(x1, y1, x2, y2);
	DrawLine(x1, (y1 - 1), x2, (y2 - 1));
	DrawLine((x1 + 1), y1, (x2 + 1), y2);
	DrawLine(x1, (y1 + 1), x2, (y2 + 1));
	DrawLine((x1 - 1), y1, (x2 - 1), y2);

	return 0;
}

 /*******************************************************************************
 *
 * Function to calculate dew point
 * (source: www.https://www.weather.gov/media/epz/wxcalc/wetBulbTdFromRh.pdf)
 *
 *******************************************************************************/

float DewPoint(float tf, float rh) {

	float tc;
	float es;
	float dpc;
	float dpf;

	tc = (tf - 32) * (5 / 9.0);

	es = 6.11 * pow(10, ((7.5 * tc) / (237.3 + tc)));
	dpc = (237.2 * log((es * rh) / 611)) / ((7.5 * log(10)) - log((es * rh) / 611));

	dpf = (dpc * (9 / 5.0)) + 32;

	return dpf;
}

 /*******************************************************************************
 *
 * Function to calculate wind chill
 * (source: www.weather.gov/media/epz/wxcalc/windChill.pdf)
 *
 *******************************************************************************/

float WindChill(float tf, float wmph) {

	float wc;

	if (tf <= 50) {
		wc = 35.74 + (0.6215 * tf) - (35.75 * pow(wmph, 0.16)) + (0.4275 * tf * pow(wmph, 0.16));
	} else {
		wc = -999;
	}

	return wc;
}

 /*******************************************************************************
 *
 * Function to calculate heat index
 * (source: https://www.ncbi.nlm.nih.gov/pmc/articles/PMC3801457/table/t1/?report=objectonly, no. 9 algorithm)
 *
 *******************************************************************************/

float HeatIndex(float tf, float rh) {

	float tf2;
	float tf3;
	float rh2;
	float rh3;
	float hi;

	if (tf >= 75) {
		tf2 = tf * tf;
		tf3 = tf2 * tf;
		rh2 = rh  * rh;
		rh3 = rh2 * rh;
		hi = 16.923 + (1.85212e-1 * tf) + (5.37941 * rh) - (1.00254e-1 * tf * rh) + (9.41695e-3 * tf2) + (7.28898e-3 * rh2) + (3.45372e-4 * tf2 * rh) - (8.14971e-4 * tf * rh2) + (1.02102e-5 * tf2 * rh2) - (3.8646e-5 * tf3) + (2.91583e-5 * rh3) + (1.42721e-6 * tf3 * rh) + (1.97483e-7 * tf * rh3) - (2.18429e-8 * tf3 * rh2) + (8.43296e-10 * tf2 * rh3) - (4.81975e-11 * tf3 * rh3) + 0.5;
	} else {
		hi = -999;
	}

	return hi;
}

 /*******************************************************************************
 *
 * Function to calculate solar radiation from lux
 * (source: www.gwhiteqcarol.weebly.com/blog/convert-lux-to-watts-per-square-metre-download)
 *
 *******************************************************************************/

float Solar(float lux) {

	float solar;

	solar = 0.0079 * lux;

	return solar;
}

 /*******************************************************************************
 *
 * Function to compute y offset for graphing
 *
 *******************************************************************************/

float Y_Offset(float value, float scale_high, float scale_low, float y_range) {

	float y_offset;

	y_offset = (y_range - 1) * ((value - scale_low) / (scale_high - scale_low));

	return y_offset;
}

 /*******************************************************************************
 *
 * Main program
 *
 *******************************************************************************/

int main(int argc, char* argv[]) {
	int fbfd = 0;
	struct fb_var_screeninfo vinfo;
	struct fb_fix_screeninfo finfo;
	char *fbp = 0;

	int h_size;
	int v_size;

	time_t curtime;
	struct tm *loc_time;
	char time_old[30];
	char day_old[30];

	int battery_low_flag;

	int temp_high_flag;
	int temp_low_flag;
	float temp_scale_high;
	float temp_scale_low;
	float temp_high;
	float temp_low;
	float temp;
	float temp_past[1440];
	float dew_point;
	float dew_point_past[1440];
	float wind_chill;
	float heat_index;
	float wind_chill_heat_index_past[1440];

	int wind_speed_flag;
	float wind_scale_high;
	float wind_speed;
	float wind_speed_past[5];
	int wind_speed_past_index = -1;
	float wind_speed_avg_past[1440];
	float wind_peak;
	float wind_lull;
	float wind_peak_high;
	float wind_gust_past[1440];

	int rh_flag;
	float rh;
	float rh_last;

	int wind_dir_flag;
	float wind_dir[4];
	float wind_dir_avg;

	int prec_flag;
	float prec_scale_high;
	float prec_last = -1;
	float prec;
	float prec_acc;
	float prec_acc_past[1440];
	float prec_rate;
	float prec_rate_data[6];
	int prec_rate_data_index;
	float prec_rate_past[1440];
	int ltng_last;
	int ltng;
	int ltng_acc;
	int ltng_dist;

	int solar_flag;
	float lux_raw;
	int lux_flag1;
	int lux_flag2;
	int lux_flag3;
	int lux_flag4;
	int lux_flag5;
	int lux_flag6;
	int lux_flag7;
	float lux_last_reading;
	float lux_adjusted;
	float solar_adjusted;
	float solar_adjusted_last;
	float solar_peak;
	float uv;

	int hr_size;
	int x_size;
	int y_size;
	int x;
	int y;

	float a;
	float b;
	float c;
	float d;
	char packet_buffer[27];
	char str[30];
	char str2[30];
	int i;
	int j;
	int k;
	int minute;
	int x_pos;
	int y_pos;
	int minutem1;
	int xm1_pos;
	int ym1_pos;

	int debug = 0;

	// Note: Using "@reboot sleep 30 && sudo /home/pi/Weather/weather" in Cron to autostart on boot
		// Sub-note: On RPi 3B+ use "sleep 10" on RPi Zero 2W use "sleep 30"
	// Note: Changed /boot/cmdline.txt from "console=tty1" to "console=tty3" and added " loglevel=3 quiet logo.nologo vt.global_cursor_default=0" to the end

	// Use system call to clear the screen

	strcpy(str, "clear");
	system(str);

	// Initialize the graphics screen

	printf("Initializing the graphics screen...\n");

	// Open the file for reading and writing

	fbfd = open("/dev/fb0", O_RDWR);
	if (!fbfd) {
		return(-1);
	}

	// Get fixed screen information

	if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo)) {
		return(-1);
	}

	// Get variable screen information

	if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo)) {
		return(-1);
	}

	// Display screen resolution and color depth

	printf("%dx%d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel );

	// Check to make sure frame buffer is 32bpp

	if (vinfo.bits_per_pixel != 32) {
		printf("The frame buffer is NOT setup for 32bpp. Fix it before using the Simple ARM Graphics library!\n");
		return(-1);
	}

	// Map framebuffer to user memory

	fbp = (char*)mmap(0,
		finfo.smem_len,
		PROT_READ | PROT_WRITE,
		MAP_SHARED,
		fbfd, 0);

	if ((int)fbp == -1) {
		return(-1);
	}

	// Initialize WiringPi

	printf("\nInitializing WiringPi...\n");
	i = RF_Mod_Init(packet_buffer);
	if (i != 0) {
		printf("WiringPi failed to initialize. Error code: %d\n", i);
		return(-1);
	}

	// Sleep for 1 second

	sleep(1);

	// Use system calls to clear the screen and turn the cursor off

	strcpy(str, "clear");
	system(str);

	strcpy(str, "setterm -cursor off");
	system(str);

	// Initialize the Simple ARM Graphics library

	SAGL_Init(fbp, vinfo.xres, vinfo.yres);

	// Graph area dimensions

	h_size = vinfo.xres;		// 1920
	v_size = vinfo.yres;		// 1080

	hr_size = 37;
	x_size = hr_size * 24;
	y_size = trunc(v_size / 7);

	// Clear the screen

	ScreenClear(BLACK);

	// Display patience message

	SelectFont(&FontArial32);
	SelectColors(WHITE, BLACK);
	PrintStringLeft("Taking initial readings, ~1 minute...", 0, 1039);

	// Initialize solar discontinuity variables

	lux_flag1 = 0;
	lux_flag2 = 0;
	lux_flag3 = 0;
	lux_flag4 = 0;
	lux_flag5 = 0;
	lux_flag6 = 0;
	lux_flag7 = 0;
	lux_last_reading = 0;

	// Initialize precipitation rate variables

	for (i = 0; i < 6; i++) {
		prec_rate_data[i] = 0;
	}
	prec_rate_data_index = 0;

	// Initialize wind direction variables

	wind_dir[3] = -1;

 /*******************************************************************************
 *
 * Main loop
 *
 *******************************************************************************/

	while (1) {

 /*******************************************************************************
 *
 * Weather data sampling
 *
 * Read wind speed every 10 seconds, everything else every 30 seconds, and
 * average readings over 1 minute
 *
 *******************************************************************************/

		if (debug == 1) {
			SelectFont(&Font5x9);
			SelectColors(WHITE, BLACK);
			PrintStringLeft("1", 0, 0);
		}

		// Read packet type 0x03

		i = 1;
		while (i) {
			j = RF_Rd_Pkt(3);
			if (j != -1) {

				// Exit the while loop

				i = 0;
			} else {
				usleep(700000);
			}
		}

		usleep(700000);

		// Read packet type 0x10

		i = 1;
		while (i) {
			j = RF_Rd_Pkt(16);
			if (j != -1) {

				// Get wind speed

				wind_speed = packet_buffer[9];

				// Set wind peak and lull

				wind_peak = packet_buffer[9];
				wind_lull = packet_buffer[9];

				// Exit the while loop

				i = 0;
			} else {
				usleep(700000);
			}
		}

		// Sleep for 9 seconds

		sleep(9);

		if (debug == 1) {
			PrintStringLeft("2", 0, 0);
		}

		// Read packet type 0x03

		i = 1;
		while (i) {
			j = RF_Rd_Pkt(3);
			if (j != -1) {

				// Exit the while loop

				i = 0;
			} else {
				usleep(700000);
			}
		}

		usleep(700000);

		// Read packet type 0x10

		i = 1;
		while (i) {
			j = RF_Rd_Pkt(16);
			if (j != -1) {

				// Sum wind speed

				wind_speed = wind_speed + packet_buffer[9];

				// Update wind peak and lull

				if (wind_peak < packet_buffer[9]) {
					wind_peak = packet_buffer[9];
				}
				if (wind_lull > packet_buffer[9]) {
					wind_lull = packet_buffer[9];
				}

				// Exit the while loop

				i = 0;
			} else {
				usleep(700000);
			}
		}

		// Sleep for 9 seconds

		sleep(9);

		if (debug == 1) {
			PrintStringLeft("3", 0, 0);
		}

		// Read packet type 0x03

		i = 1;
		while (i) {
			j = RF_Rd_Pkt(3);
			if (j != -1) {

				// Get temperature variable

				a = ((packet_buffer[6] & 0xF0) >> 4);
				b = (packet_buffer[6] & 0x0F);
				c = ((packet_buffer[7] & 0xF0) >> 4);
				d = (packet_buffer[7] & 0x0F);
				if ((packet_buffer[1] & 0x10) == 0x10) {
					temp = -((100 * a) + (10 * b) + c + (d / 10));
				} else {
					temp = (100 * a) + (10 * b) + c + (d / 10);
				}

				// Get relative humidity variable

				rh = packet_buffer[8];

				// Exit the while loop

				i = 0;
			} else {
				usleep(700000);
			}
		}

		usleep(700000);

		// Read packet type 0x10

		i = 1;
		while (i) {
			j = RF_Rd_Pkt(16);
			if (j != -1) {

				// Get UV index variable

				uv = (packet_buffer[5] & 0x0F);

				// Get lux value

				lux_raw = ((packet_buffer[6] & 0x3F) << 8) + packet_buffer[7];

				// Sum wind speed

				wind_speed = wind_speed + packet_buffer[9];

				// Update wind peak and lull

				if (wind_peak < packet_buffer[9]) {
					wind_peak = packet_buffer[9];
				}
				if (wind_lull > packet_buffer[9]) {
					wind_lull = packet_buffer[9];
				}

				// Get wind direction

				a = (packet_buffer[10] & 0x0F);
				b = ((packet_buffer[11] & 0xF0) >> 4);
				c = (packet_buffer[11] & 0x0F);
				wind_dir[0] = (100 * a) + (10 * b) + c;

				// Exit the while loop

				i = 0;
			} else {
				usleep(700000);
			}
		}

		// Sleep for 9 seconds

		sleep(9);

		if (debug == 1) {
			PrintStringLeft("4", 0, 0);
		}

		// Read packet type 0x03

		i = 1;
		while (i) {
			j = RF_Rd_Pkt(3);
			if (j != -1) {

				// Exit the while loop

				i = 0;
			} else {
				usleep(700000);
			}
		}

		usleep(700000);

		// Read packet type 0x10

		i = 1;
		while (i) {
			j = RF_Rd_Pkt(16);
			if (j != -1) {

				// Sum wind speed

				wind_speed = wind_speed + packet_buffer[9];

				// Update wind peak and lull

				if (wind_peak < packet_buffer[9]) {
					wind_peak = packet_buffer[9];
				}
				if (wind_lull > packet_buffer[9]) {
					wind_lull = packet_buffer[9];
				}

				// Exit the while loop

				i = 0;
			} else {
				usleep(700000);
			}
		}

		// Sleep for 9 seconds

		sleep(9);

		if (debug == 1) {
			PrintStringLeft("5", 0, 0);
		}

		// Read packet type 0x03

		i = 1;
		while (i) {
			j = RF_Rd_Pkt(3);
			if (j != -1) {

				// Exit the while loop

				i = 0;
			} else {
				usleep(700000);
			}
		}

		usleep(700000);

		// Read packet type 0x10

		i = 1;
		while (i) {
			j = RF_Rd_Pkt(16);
			if (j != -1) {

				// Sum wind speed

				wind_speed = wind_speed + packet_buffer[9];

				// Update wind peak and lull

				if (wind_peak < packet_buffer[9]) {
					wind_peak = packet_buffer[9];
				}
				if (wind_lull > packet_buffer[9]) {
					wind_lull = packet_buffer[9];
				}

				// Exit the while loop

				i = 0;
			} else {
				usleep(700000);
			}
		}

		// Sleep for 9 seconds

		sleep(9);

		if (debug == 1) {
			PrintStringLeft("6", 0, 0);
		}

		// Get current weather readings and average over last minute

		// Read packet type 0x03

		i = 1;
		while (i) {
			j = RF_Rd_Pkt(3);
			if (j != -1) {

				// Get battery low flag

				battery_low_flag = ((packet_buffer[1] & 0x08) >> 3);

				// Update temperature variable

				a = ((packet_buffer[6] & 0xF0) >> 4);
				b = (packet_buffer[6] & 0x0F);
				c = ((packet_buffer[7] & 0xF0) >> 4);
				d = (packet_buffer[7] & 0x0F);
				if ((packet_buffer[1] & 0x10) == 0x10) {
					temp = (temp - ((100 * a) + (10 * b) + c + (d / 10))) / 2;
				} else {
					temp = (temp + ((100 * a) + (10 * b) + c + (d / 10))) / 2;
				}

				// Update relative humidity variable

				rh = (rh + packet_buffer[8]) / 2;

				// Update the lightning strikes variable

				ltng = packet_buffer[18];

				// Update the lightning distance variable

				a = ((packet_buffer[20] & 0xF0) >> 4);
				b = (packet_buffer[20] & 0x0F);
				ltng_dist = (10 * a) + b;

				// Exit the while loop

				i = 0;
			} else {
				usleep(700000);
			}
		}

		usleep(700000);

		// Read packet type 0x10

		i = 1;
		while (i) {
			j = RF_Rd_Pkt(16);
			if (j != -1) {

				// Update UV index variable

				uv = round((uv + (packet_buffer[5] & 0x0F)) / 2);

				// Update lux raw variable

				a = (lux_raw + (((packet_buffer[6] & 0x3F) << 8) + packet_buffer[7])) / 2;
				lux_raw = 10 * a;

				// Update wind speed variable

				wind_speed = (wind_speed + packet_buffer[9]) / 6;

				// Update wind peak and lull

				if (wind_peak < packet_buffer[9]) {
					wind_peak = packet_buffer[9];
				}
				if (wind_lull > packet_buffer[9]) {
					wind_lull = packet_buffer[9];
				}

				// Get wind direction

				a = (packet_buffer[10] & 0x0F);
				b = ((packet_buffer[11] & 0xF0) >> 4);
				c = (packet_buffer[11] & 0x0F);
				wind_dir[1] = (100 * a) + (10 * b) + c;

				// Update precipitation variable

				a = ((packet_buffer[19] & 0xF0) >> 4);
				b = (packet_buffer[19] & 0x0F);
				c = ((packet_buffer[20] & 0xF0) >> 4);
				d = (packet_buffer[20] & 0x0F);
				prec = (10 * a) + b + (c / 10) + (d / 100);

				// Exit the while loop

				i = 0;
			} else {
				usleep(700000);
			}
		}

 /*******************************************************************************
 *
 * Time and date
 *
 * Update the display once per minute on the minute
 *
 * Note: It can take up to 8 minutes after starting to sync to the minute
 *
 *******************************************************************************/

		i = 1;
		while (i) {

			// Get current date/time of system

			curtime = time(NULL);

			// Get current local date and time

			loc_time = localtime(&curtime);

			// Check for time not the same as old time

			strftime(str, 40, "%-l:%M%P", loc_time);
			if (strcmp(time_old, str) != 0) {

				// Update displayed time

				SelectFont(&FontArial104);
				SelectColors(BLACK, BLACK);
				DrawRectFilled(1380, 950, 1919, 1079);
				SelectColors(WHITE, BLACK);
				PrintStringRight(str, 1919, 950);
				strcpy(time_old, str);

				// Check for date not the same as old date

				strftime(str, 40, "%a, %b %-d, %Y", loc_time);
				if (strncmp(day_old, str, 3) != 0) {

					// Update displayed day and date

					SelectColors(BLACK, BLACK);
					DrawRectFilled(0, 950, 1370, 1079);
					SelectColors(WHITE, BLACK);
					PrintStringLeft(str, 0, 950);
					strcpy(day_old, str);

					// New day: reset variables and erase previous past readings

					temp_high = temp;
					temp_low = temp;
					temp_high_flag = 1;
					temp_low_flag = 1;
					wind_peak_high = wind_peak;
					wind_speed_flag = 1;
					rh_last = rh;
					rh_flag = 1;
					wind_dir_flag = 1;
					prec_acc = 0;
					if (prec_last == -1) {
						prec_last = prec;
					}
					ltng_acc = 0;
					ltng_last = ltng;
					prec_flag = 1;
					solar_adjusted_last = -1;
					solar_peak = 0;
					solar_flag = 1;

					for (i = 0; i < 1440; i++) {
						temp_past[i] = -999;
						dew_point_past[i] = -999;
						wind_chill_heat_index_past[i] = -999;
						wind_speed_avg_past[i] = -1;
						wind_gust_past[i] = -1;
						prec_acc_past[i] = -1;
						prec_rate_past[i] = 0;
					}
				}

				// Exit the while loop

				i = 0;
			}
		}

		// Compute minute for saving data, and x positions for graphing

		strftime(str, 30, "%-k", loc_time);
		strftime(str2, 30, "%M", loc_time);
		a = (60 * strtod(str, NULL)) + strtod(str2, NULL);
		minute = a;
		if (minute == 0) {
			minutem1 = 0;
		} else {
			minutem1 = minute - 1;
		}
		b = x_size;
		c = b * (a / 1440);
		x_pos = c;
		if (a == 0) {
			d = c;
		} else {
			d = b * ((a - 1) / 1440);
		}
		xm1_pos = d;

/*******************************************************************************
*
* Low Battery Area
*
*******************************************************************************/

		x = 1263;
		y = 945;
		SelectColors(BLACK, BLACK);
		DrawRectFilled(x, y, x + 50, y + 115);

		// If battery low flag set, display battery low symbol

		if (battery_low_flag == 1) {
			SelectColors(WHITE, WHITE);
			DrawRectFilled(x + 16, y, x + 34, y + 7);
			DrawRectFilled(x, y + 7, x + 50, y + 115);
			SelectColors(BLACK, BLACK);
			DrawRectFilled(x + 3, y + 10, x + 47, y + 112);
			SelectColors(BRIGHT_RED, BRIGHT_RED);
			DrawRectFilled(x + 5, y + 92, x + 45, y + 110);
		}

/*******************************************************************************
*
* Temperature Area
*
*******************************************************************************/

		// Calculate graph area x,y origin

		x = (h_size / 2) - (x_size + 2) - 28;
		y = (y_size * 2) - 1;

		// Update temperature high/low variables

		if (temp_high < temp) {
			temp_high = temp;
		}
		if (temp_low > temp) {
			temp_low = temp;
		}

		// Calculate current dew point

		dew_point = DewPoint(temp, rh);

		// Calculate current wind chill

		wind_chill = WindChill(temp, wind_speed);

		// Calculate current heat index

		heat_index = HeatIndex(temp, rh);

		// Save current temperature

		temp_past[minute] = temp;

		// Save current dew point

		dew_point_past[minute] = dew_point;

		// Save current wind chill/heat index

		wind_chill_heat_index_past[minute] = fmaxf(wind_chill, heat_index);

		// Display current temperature

		SelectFont(&FontArial104);
		SelectColors(BLACK, BLACK);
		DrawRectFilled(x + 110, y - 285, x + 517, y - 156);
		sprintf(str, "%.1f\xF8", temp);
		SelectColors(WHITE, BLACK);
		PrintStringLeft(str, x + 110, y - 285);

		// Check for temperature scale change needed

		if (fmaxf(temp_high, heat_index) > temp_scale_high) {
			temp_high_flag = 1;
		}

		// Check for temperature scale change needed

		if (wind_chill != -999) {
			if (fminf(fminf(temp_low, dew_point), wind_chill) < temp_scale_low) {
				temp_low_flag = 1;
			}
		} else {
			if (fminf(temp_low, dew_point) < temp_scale_low) {
				temp_low_flag = 1;
			}
		}

		// Check for new/update graph

		if ((temp_high_flag == 1) || (temp_low_flag == 1)) {

			// Draw temperature area label

			SelectFont(&FontArial32);
			SelectColors(HOT_YELLOW, BLACK);
			PrintStringLeft("Temp:", x - 42, y - 285);

			// Draw new temperature graph

			DrawGraph(x, y, x_size, y_size, hr_size);

			// Calculate temperature scale high

			if (temp_high_flag == 1) {
				if (heat_index != -999) {
					a = fmaxf(temp_high, heat_index);
				} else {
					a = temp_high;
				}
				temp_scale_high = 10 * ceilf(a / 10);
				temp_high_flag = 0;
			}

			// Calculate temperature scale low

			if (temp_low_flag == 1) {
				if (wind_chill != -999) {
					a = fminf(fminf(temp_low, dew_point), wind_chill);
				} else {
					a = fminf(temp_low, dew_point);
				}
				temp_scale_low = 10 * floorf(a / 10);
				temp_low_flag = 0;
			}

			// Display temperature graph scale

			SelectFont(&Font5x9);
			SelectColors(BLACK, BLACK);
			DrawRectFilled(x - 5, y + 5, x - 32, y - (y_size + 3));
			for (i = temp_scale_low; i <= temp_scale_high; i = i + 10) {
				sprintf(str, "%d\xF8\x46", i);
				y_pos = Y_Offset(i, temp_scale_high, temp_scale_low, y_size);
				SelectColors(LT_GRAY, BLACK);
				PrintStringRight(str, x - 5, y - (y_pos + 4));
				SelectColors(MED_GRAY, BLACK);
				DrawLine(x, y - y_pos, x + (x_size - 1), y - y_pos);
			}

			// Graph any previous and current wind chill/heat index, dew point, and temperature.

			for (i = 0; i <= minute; i++) {
				a = i;
				b = x_size;
				c = b * (a / 1440);
				j = c;
				if (i == 0) {
					d = j;
				} else {
					d = b * ((a - 1) / 1440);
				}
				k = d;

				// Graph any previous and current wind chill/heat index

				if (wind_chill_heat_index_past[i] != -999) {
					y_pos = Y_Offset(wind_chill_heat_index_past[i], temp_scale_high, temp_scale_low, y_size);
					if ((i != 0) && (wind_chill_heat_index_past[i - 1] != -999)) {
						ym1_pos = Y_Offset(wind_chill_heat_index_past[i - 1], temp_scale_high, temp_scale_low, y_size);
					} else {
						ym1_pos = y_pos;
					}
					if (wind_chill_heat_index_past[i] <= 50) {
						SelectColors(ICE_BLUE, ICE_BLUE);
					} else {
						SelectColors(BRIGHT_RED, BRIGHT_RED);
					}
					DrawThickLine(x + k, y - ym1_pos, x + j, y - y_pos);
				}

				// Graph any previous and current dew point

				if (dew_point_past[i] != -999) {
					y_pos = Y_Offset(dew_point_past[i], temp_scale_high, temp_scale_low, y_size);
					if ((i != 0) && (dew_point_past[i - 1] != -999)) {
						ym1_pos = Y_Offset(dew_point_past[i - 1], temp_scale_high, temp_scale_low, y_size);
					} else {
						ym1_pos = y_pos;
					}
					SelectColors(BRIGHT_APPLE_GREEN, BRIGHT_APPLE_GREEN);
					DrawThickLine(x + k, y - ym1_pos, x + j, y - y_pos);
				}

				// Graph any previous and current temperature
				// Note: Graph temperature last so it will be on top

				if (temp_past[i] != -999) {
					y_pos = Y_Offset(temp_past[i], temp_scale_high, temp_scale_low, y_size);
					if ((i != 0) && (temp_past[i - 1] != -999)) {
						ym1_pos = Y_Offset(temp_past[i - 1], temp_scale_high, temp_scale_low, y_size);
					} else {
						ym1_pos = y_pos;
					}
					SelectColors(HOT_YELLOW, HOT_YELLOW);
					DrawThickLine(x + k, y - ym1_pos, x + j, y - y_pos);
				}
			}
		} else {

			// Graph any current wind chill or heat index

			if (wind_chill_heat_index_past[minute] != -999) {
				y_pos = Y_Offset(wind_chill_heat_index_past[minute], temp_scale_high, temp_scale_low, y_size);
				if (wind_chill_heat_index_past[minutem1] != -999) {
					ym1_pos = Y_Offset(wind_chill_heat_index_past[minutem1], temp_scale_high, temp_scale_low, y_size);
				} else {
					ym1_pos = y_pos;
				}
				if (wind_chill_heat_index_past[minute] <= 50) {
					SelectColors(ICE_BLUE, ICE_BLUE);
				} else {
					SelectColors(BRIGHT_RED, BRIGHT_RED);
				}
				DrawThickLine(x + xm1_pos, y - ym1_pos, x + x_pos, y - y_pos);
			}

			// Graph current dew point

			y_pos = Y_Offset(dew_point, temp_scale_high, temp_scale_low, y_size);
			if (dew_point_past[minutem1] != -999) {
				ym1_pos = Y_Offset(dew_point_past[minutem1], temp_scale_high, temp_scale_low, y_size);
			} else {
				ym1_pos = y_pos;
			}
			SelectColors(BRIGHT_APPLE_GREEN, BRIGHT_APPLE_GREEN);
			DrawThickLine(x + xm1_pos, y - ym1_pos, x + x_pos, y - y_pos);

			// Graph current temperature
			// Note: Graph temperature last so it will be on top

			y_pos = Y_Offset(temp, temp_scale_high, temp_scale_low, y_size);
			if (temp_past[minutem1] != -999) {
				ym1_pos = Y_Offset(temp_past[minutem1], temp_scale_high, temp_scale_low, y_size);
			} else {
				ym1_pos = y_pos;
			}
			SelectColors(HOT_YELLOW, HOT_YELLOW);
			DrawThickLine(x + xm1_pos, y - ym1_pos, x + x_pos, y - y_pos);
		}

		// Display additional temperature information

		SelectFont(&FontArial32);

		// Display today's high temperature

		SelectColors(BLACK, BLACK);
		DrawRectFilled(x + 516, y - 298, x + 889, y - 259);
		SelectColors(WHITE, BLACK);
		PrintStringLeft("High:", x + 516, y - 298);
		sprintf(str, "%.1f\xF8", temp_high);
		PrintStringRight(str, x + 889, y - 298);

		// Display today's low temperature

		SelectColors(BLACK, BLACK);
		DrawRectFilled(x + 516, y - 250, x + 889, y - 211);
		SelectColors(WHITE, BLACK);
		PrintStringLeft("Low:", x + 516, y - 250);
		sprintf(str, "%.1f\xF8", temp_low);
		PrintStringRight(str, x + 889, y - 250);

		// Clear wind chill/heat index area

		SelectColors(BLACK, BLACK);
		DrawRectFilled(x + 516, y - 202, x + 889, y - 163);

		// Display current wind chill/heat index

		if (wind_chill_heat_index_past[minute] != -999) {
			if (wind_chill_heat_index_past[minute] <= 50) {
				sprintf(str, "Wind Chill:");
				SelectColors(ICE_BLUE, BLACK);
			} else {
				sprintf(str, "Heat Index:");
				SelectColors(BRIGHT_RED, BLACK);
			}
			PrintStringLeft(str, x + 516, y - 202);
			sprintf(str, "%.1f\xF8", wind_chill_heat_index_past[minute]);
			SelectColors(WHITE, BLACK);
			PrintStringRight(str, x + 889, y - 202);
		}

		// Display current temperature description
		// (source: arbitrary)

		SelectColors(BLACK, BLACK);
		DrawRectFilled(x - 42, y - 237, x + 110, y - 198);
		if (temp <= 19) {
			sprintf(str, "Frigid");
		} else if (temp <= 32) {
			sprintf(str, "Frzing");
		} else if (temp <= 49) {
			sprintf(str, "Cold");
		} else if (temp <= 65) {
			sprintf(str, "Cool");
		} else if (temp <= 74) {
			sprintf(str, "Comfy");
		} else if (temp <= 85) {
			sprintf(str, "Warm");
		} else if (temp <= 99) {
			sprintf(str, "Hot");
		} else if (temp <= 109) {
			sprintf(str, "Searing");
		} else {
			sprintf(str, "Blazing");
		}
		SelectColors(WHITE, BLACK);
		PrintStringLeft(str, x - 42, y - 237);

/*******************************************************************************
*
* Wind Speed Area
*
*******************************************************************************/

		// Calculate graph area x,y origin

		x = h_size - (x_size + 2);
		y = (y_size * 2) - 1;

		// Initialize wind speed past variables

		if (wind_speed_past_index == -1) {
			for (i = 0; i < 5; i++) {
				wind_speed_past[i] = wind_speed;
			}
			wind_speed_past_index = 0;
		}

		// Save current wind speed

		wind_speed_past[wind_speed_past_index] = wind_speed;
		wind_speed_past_index = wind_speed_past_index + 1;
		if (wind_speed_past_index >= 5) {
			wind_speed_past_index = 0;
		}

		// Compute current average wind speed (averaged over last 5 minutes)

		a = 0;
		for (i = 0; i < 5; i++) {
			a = a + wind_speed_past[i];
		}
		wind_speed_avg_past[minute] = a / 5;

		// Check for wind gust (NOAA definition: wind peak >= 18, AND wind peak - wind lull >= 10)

		if ((wind_peak >= 18) && (wind_peak - wind_lull) >= 10) {

			// Save wind gust

			wind_gust_past[minute] = wind_peak;
		}

		// Check for wind peak high change

		if (wind_peak_high < wind_peak) {
			wind_peak_high = wind_peak;
		}

		// Display current average wind speed

		SelectFont(&FontArial104);
		SelectColors(BLACK, BLACK);
		DrawRectFilled(x + 110, y - 285, x + 468, y - 156);
		sprintf(str, "%.1f", wind_speed_avg_past[minute]);
		SelectColors(WHITE, BLACK);
		PrintStringLeft(str, x + 110, y - 285);

		// Check for wind speed scale change

		if (wind_peak > wind_scale_high) {
			wind_speed_flag = 1;
		}

		// Check for new/update graph

		if (wind_speed_flag == 1) {

			// Draw wind speed area label

			SelectFont(&FontArial32);
			SelectColors(BRIGHT_SKY_BLUE, BLACK);
			PrintStringLeft("Wind:", x - 42, y - 285);
			SelectFont(&Font5x9);
			PrintStringRight("Speed", x + 55, y - 247);

			// Draw new wind speed graph

			DrawGraph(x, y, x_size, y_size, hr_size);

			// Calculate wind scale high

			wind_scale_high = 5 * ceil(wind_peak / 5);
			if (wind_scale_high < 20) {
				wind_scale_high = 20;
			}

			// Display wind speed graph scale

			SelectFont(&Font5x9);
			SelectColors(BLACK, BLACK);
			DrawRectFilled(x - 5, y + 5, x - 34, y - (y_size + 3));
			for (i = 0; i <= wind_scale_high; i = i + 5) {
				sprintf(str, "%dmph", i);
				y_pos = Y_Offset(i, wind_scale_high, 0, y_size);
				SelectColors(LT_GRAY, BLACK);
				PrintStringRight(str, x - 5, y - (y_pos + 4));
				SelectColors(MED_GRAY, BLACK);
				DrawLine(x, y - y_pos, x + (x_size - 1), y - y_pos);
			}

			// Graph any previous and current average wind speeds, and any wind gusts

			for (i = 0; i <= minute; i++) {
				a = i;
				b = x_size;
				c = b * (a / 1440);
				j = c;
				if (i == 0) {
					d = j;
				} else {
					d = b * ((a - 1) / 1440);
				}
				k = d;

				// Graph any previous and current wind speeds

				if (wind_speed_avg_past[i] != -1) {
					y_pos = Y_Offset(wind_speed_avg_past[i], wind_scale_high, 0, y_size);
					if ((i != 0) && (wind_speed_avg_past[i - 1] != -1)) {
						ym1_pos = Y_Offset(wind_speed_avg_past[i - 1], wind_scale_high, 0, y_size);
					} else {
						ym1_pos = y_pos;
					}
					SelectColors(BRIGHT_SKY_BLUE, BRIGHT_SKY_BLUE);
					DrawThickLine(x + k, y - ym1_pos, x + j, y - y_pos);
				}

				// Graph any previous and current wind gusts

				if (wind_gust_past[i] != -1) {
					y_pos = Y_Offset(wind_gust_past[i], wind_scale_high, 0, y_size);
					SelectColors(EXTRA_BRIGHT_YELLOW, EXTRA_BRIGHT_YELLOW);
					DrawRectFilled((x + j) - 1, (y - y_pos) - 1, (x + j) + 1, (y - y_pos) + 1);
				}
			}

			wind_speed_flag = 0;

		} else {

			// Graph current wind speed

			y_pos = Y_Offset(wind_speed_avg_past[minute], wind_scale_high, 0, y_size);
			if (wind_speed_avg_past[minutem1] != -1) {
				ym1_pos = Y_Offset(wind_speed_avg_past[minutem1], wind_scale_high, 0, y_size);
			} else {
				ym1_pos = y_pos;
			}
			SelectColors(BRIGHT_SKY_BLUE, BRIGHT_SKY_BLUE);
			DrawThickLine(x + xm1_pos, y - ym1_pos, x + x_pos, y - y_pos);

			// Check for current wind gust

			if (wind_gust_past[minute] != -1) {

				// Graph current wind gust

				y_pos = Y_Offset(wind_gust_past[minute], wind_scale_high, 0, y_size);
				SelectColors(BRIGHT_YELLOW, BRIGHT_YELLOW);
				DrawRectFilled((x + x_pos) - 1, (y - y_pos) - 1, (x + x_pos) + 1, (y - y_pos) + 1);
			}
		}

		// Display additional wind speed information

		SelectFont(&FontArial32);

		// Display peak wind speed

		SelectColors(BLACK, BLACK);
		DrawRectFilled(x + 516, y - 298, x + 889, y - 259);
		SelectColors(WHITE, BLACK);
		PrintStringLeft("Peak:", x + 516, y - 298);
		sprintf(str, "%.0f mph", wind_peak_high);
		PrintStringRight(str, x + 889, y - 298);

		// Check for current wind gust

		SelectColors(BLACK, BLACK);
		DrawRectFilled(x + 516, y - 250, x + 889, y - 211);
		if (wind_gust_past[minute] != -1) {

			// Display current wind gust message

			SelectColors(EXTRA_BRIGHT_YELLOW, BLACK);
			PrintStringLeft("Wind Gust!", x + 516, y - 250);
		}

		// Display current wind description

		SelectColors(BLACK, BLACK);
		DrawRectFilled(x + 516, y - 202, x + 889, y - 163);

		// Wind Descriptions - Beaufort Scale
		// (source: https://www.weather.gov/pqr/wind)

		if ((wind_lull == 0) && (wind_peak <=1)) {
			sprintf(str, "Calm");
		} else if (wind_peak <= 3) {
			sprintf(str, "Light Air");
		} else if (wind_peak <= 7) {
			sprintf(str, "Light Breeze");
		} else if (wind_peak <= 12) {
			sprintf(str, "Gentle Breeze");
		} else if (wind_peak <= 18) {
			sprintf(str, "Moderate Breeze");
		} else if (wind_peak <= 24) {
			sprintf(str, "Fresh Breeze");
		} else if (wind_peak <= 31) {
			sprintf(str, "Strong Breeze");
		} else if (wind_peak <= 38) {
			sprintf(str, "Near Gale");
		} else if (wind_peak <= 46) {
			sprintf(str, "Gale");
		} else if (wind_peak <= 54) {
			sprintf(str, "Strong Gale");
		} else if (wind_peak <= 63) {
			sprintf(str, "Storm");
		} else if (wind_peak <= 73) {
			sprintf(str, "Violent Storm");
		} else {
			sprintf(str, "Hurricane");
		}
		SelectColors(WHITE, BLACK);
		PrintStringLeft(str, x + 516, y - 202);

/*******************************************************************************
*
* Relative Humidity Area
*
*******************************************************************************/

		// Calculate graph area x,y origin

		x = (h_size / 2) - (x_size + 2) - 28;
		y = (y_size * 4) - 1;

		// Display relative humdity

		SelectFont(&FontArial104);
		SelectColors(BLACK, BLACK);
		DrawRectFilled(x + 147, y - 285, x + 501, y - 156);
		sprintf(str, "%.0f%%", rh);
		SelectColors(WHITE, BLACK);
		PrintStringLeft(str, x + 147, y - 285);

		// Check for new graph

		if (rh_flag == 1) {

			// Draw relative humidity area label

			SelectFont(&FontArial32);
			SelectColors(RAINBOW_ORANGE, BLACK);
			PrintStringLeft("RH:", x - 42, y - 285);

			// Draw new relative humidity graph

			DrawGraph(x, y, x_size, y_size, hr_size);

			// Display relative humidity graph scale

			SelectFont(&Font5x9);
			SelectColors(BLACK, BLACK);
			DrawRectFilled(x - 5, y + 5, x - 28, y - (y_size + 3));
			for (i = 0; i <= 100; i = i + 20) {
				sprintf(str, "%d%%", i);
				y_pos = Y_Offset(i, 100, 0, y_size);
				SelectColors(LT_GRAY, BLACK);
				PrintStringRight(str, x - 5, y - (y_pos + 4));
				SelectColors(MED_GRAY, BLACK);
				DrawLine(x, y - y_pos, x + (x_size - 1), y - y_pos);
			}

			rh_flag = 0;
		}

		// Graph relative humidity

		y_pos = Y_Offset(rh, 100, 0, y_size);
		ym1_pos = Y_Offset(rh_last, 100, 0, y_size);
		SelectColors(RAINBOW_ORANGE, RAINBOW_ORANGE);
		DrawThickLine(x + xm1_pos, y - ym1_pos, x + x_pos, y - y_pos);

		// Save current relative humidity

		rh_last= rh;

		// Display additional relative humidity information

		SelectFont(&FontArial32);

		// Display dew point

		SelectColors(BLACK, BLACK);
		DrawRectFilled(x + 516, y - 298, x + 889, y - 259);
		SelectColors(BRIGHT_APPLE_GREEN, BLACK);
		PrintStringLeft("Dew Point:", x + 516, y - 298);
		sprintf(str, "%.1f\xF8", dew_point);
		SelectColors(WHITE, BLACK);
		PrintStringRight(str, x + 889, y - 298);

		// Display humidity description
		// (source: multiple, but arbitrary)

		SelectColors(BLACK, BLACK);
		DrawRectFilled(x - 42, y - 237, x + 110, y - 198);
		if (rh < 30) {
			sprintf(str, "Dry");
		} else if (rh <= 60) {
			sprintf(str, "Comfy");
		} else {
			sprintf(str, "Humid");
		}
		SelectColors(WHITE, BLACK);
		PrintStringLeft(str, x - 42, y - 237);

/*******************************************************************************
*
* Wind Direction Area
*
*******************************************************************************/

		// Calculate graph area x,y origin

		x = h_size - (x_size + 2);
		y = (y_size * 4) - 1;

		// Initialize wind direction variables by copying first samples

		if (wind_dir[3] == -1) {
			for (i = 0; i < 2; i++) {
				wind_dir[3 - i] = wind_dir[1 - i];
			}
		}

			// Calculate average wind direction over last two minutes

		a = 0;
		b = 0;
		for (i = 0; i < 4; i++) {
			a = a + sinf(wind_dir[i] * (M_PI / 180.0));
			b = b + cosf(wind_dir[i] * (M_PI / 180.0));
		}
		a = a / 4;
		b = b / 4;
		wind_dir_avg = atan2f(a, b) * (180.0 / M_PI);
		if (wind_dir_avg < 0) {
			wind_dir_avg = wind_dir_avg + 360;
		}

		// Update wind direction variables

		for (i = 2; i < 3; i++) {
			wind_dir[i] = wind_dir[i - 2];
		}

		// Display current wind direction

		SelectFont(&FontArial104);
		SelectColors(BLACK, BLACK);
		DrawRectFilled(x + 110, y - 285, x + 470, y - 156);
		if ((wind_dir_avg >= 348.75) || (wind_dir_avg < 11.25)) {
			sprintf(str, "North");
		}
		if ((wind_dir_avg >= 11.25) && (wind_dir_avg < 33.75)) {
			sprintf(str, "NNE");
		}
		if ((wind_dir_avg >= 33.75) && (wind_dir_avg < 56.25)) {
			sprintf(str, "NE");
		}
		if ((wind_dir_avg >= 56.25) && (wind_dir_avg < 78.75)) {
			sprintf(str, "ENE");
		}
		if ((wind_dir_avg >= 78.75) && (wind_dir_avg < 101.25)) {
			sprintf(str, "East");
		}
		if ((wind_dir_avg >= 101.25) && (wind_dir_avg < 123.75)) {
			sprintf(str, "ESE");
		}
		if ((wind_dir_avg >= 123.75) && (wind_dir_avg < 146.25)) {
			sprintf(str, "SE");
		}
		if ((wind_dir_avg >= 146.25) && (wind_dir_avg < 168.75)) {
			sprintf(str, "SSE");
		}
		if ((wind_dir_avg >= 168.75) && (wind_dir_avg < 191.25)) {
			sprintf(str, "South");
		}
		if ((wind_dir_avg >= 191.25) && (wind_dir_avg < 213.75)) {
			sprintf(str, "SSW");
		}
		if ((wind_dir_avg >= 213.75) && (wind_dir_avg < 236.25)) {
			sprintf(str, "SW");
		}
		if ((wind_dir_avg >= 236.25) && (wind_dir_avg < 258.75)) {
			sprintf(str, "WSW");
		}
		if ((wind_dir_avg >= 258.75) && (wind_dir_avg < 281.25)) {
			sprintf(str, "West");
		}
		if ((wind_dir_avg >= 281.25) && (wind_dir_avg < 303.75)) {
			sprintf(str, "WNW");
		}
		if ((wind_dir_avg >= 303.75) && (wind_dir_avg < 326.25)) {
			sprintf(str, "NW");
		}
		if ((wind_dir_avg >= 326.25) && (wind_dir_avg < 348.75)) {
			sprintf(str, "NNW");
		}
		SelectColors(WHITE, BLACK);
		PrintStringLeft(str, x + 110, y - 285);

		// Check for new graph

		if (wind_dir_flag == 1) {

			// Draw wind direction area label

			SelectFont(&FontArial32);
			SelectColors(BRIGHT_SKY_BLUE, BLACK);
			PrintStringLeft("Wind:", x - 42, y - 285);
			SelectFont(&Font5x9);
			PrintStringRight("Direction", x + 55, y - 247);

			// Draw new wind direction graph

			DrawGraph(x, y, x_size, y_size, hr_size);

			// Display wind direction graph compass points

			SelectFont(&Font5x9);
			SelectColors(BLACK, BLACK);
			DrawRectFilled(x - 5, y + 5, x - 34, y - (y_size + 3));
			y_pos = Y_Offset((360 - 0), 360, 0, y_size);				// 0 degrees, North
			SelectColors(LT_GRAY, BLACK);
			PrintStringRight("North", x - 5, y - (y_pos + 4));
			SelectColors(MED_GRAY, BLACK);
			DrawLine(x, y - y_pos, x + (x_size - 1), y - y_pos);
			y_pos = Y_Offset((360 - 90), 360, 0, y_size);				// 90 degrees, East
			SelectColors(LT_GRAY, BLACK);
			PrintStringRight("East", x - 5, y - (y_pos + 4));
			SelectColors(MED_GRAY, BLACK);
			DrawLine(x, y - y_pos, x + (x_size - 1), y - y_pos);
			y_pos = Y_Offset((360 - 180), 360, 0, y_size);				// 180 degrees, South
			SelectColors(LT_GRAY, BLACK);
			PrintStringRight("South", x - 5, y - (y_pos + 4));
			SelectColors(MED_GRAY, BLACK);
			DrawLine(x, y - y_pos, x + (x_size - 1), y - y_pos);
			y_pos = Y_Offset((360 - 270), 360, 0, y_size);				// 270 degrees, West
			SelectColors(LT_GRAY, BLACK);
			PrintStringRight("West", x - 5, y - (y_pos + 4));
			SelectColors(MED_GRAY, BLACK);
			DrawLine(x, y - y_pos, x + (x_size - 1), y - y_pos);
			y_pos = Y_Offset((360 - 360), 360, 0, y_size);				// 360 degrees, North
			SelectColors(LT_GRAY, BLACK);
			PrintStringRight("North", x - 5, y - (y_pos + 4));
			SelectColors(MED_GRAY, BLACK);
			DrawLine(x, y - y_pos, x + (x_size - 1), y - y_pos);

			wind_dir_flag = 0;
		}

		// Graph current wind direction

		y_pos = Y_Offset((360 - wind_dir_avg), 360, 0, y_size);
		SelectColors(BRIGHT_SKY_BLUE, BRIGHT_SKY_BLUE);
		DrawCircleFilled(x + x_pos, y - y_pos, 1);

		// Display additional information

		SelectFont(&FontArial32);

		// Display wind direction in degrees

		SelectFont(&Font5x9);
		SelectColors(BLACK, BLACK);
		DrawRectFilled(x + 32, y - 232, x + 55, y - 223);
		sprintf(str, "%.0f\xF8", wind_dir_avg);
		SelectColors(LT_GRAY, BLACK);
		PrintStringRight(str, x + 55, y - 232);

/*******************************************************************************
*
* Precipitation Area
*
*******************************************************************************/

		// Calculate graph area x,y origin

		x = (h_size / 2) - (x_size + 2) - 28;
		y = (y_size * 6) - 1;

		// Compute any change in precipitation accumulation

		a = prec - prec_last;
		if (a < 0) {
			a = a + 10;
		}

		// Compute precipitation accumulation

		prec_acc = prec_acc + a;

		// Save current precipitation for next minute

		prec_last = prec;

		// Save change in precipitation accumulation

		prec_rate_data[prec_rate_data_index] = a;
		prec_rate_data_index = prec_rate_data_index + 1;
		if (prec_rate_data_index >= 6) {
			prec_rate_data_index = 0;
		}

		// Compute precipitation rate

		a = 0;
		for (j = 0; j < 6; j++) {
			a = a + prec_rate_data[j];
		}
		prec_rate = 10 * a;

		// Save current precipitation accumulation and precipitation rate

		prec_acc_past[minute] = prec_acc;
		prec_rate_past[minute] = prec_rate;

		// Display current precipitation

		SelectFont(&FontArial104);
//		SelectColors(BLACK, BLACK);
//		DrawRectFilled(x + 147, y - 285, x + 468, y - 156);
		sprintf(str, "%.2f\"", prec_acc);
		SelectColors(WHITE, BLACK);
		PrintStringLeft(str, x + 147, y - 285);

		// Check for precipitation scale change needed

		if (fmaxf(prec_acc, prec_rate) > prec_scale_high) {
			prec_flag = 1;
		}

		// Check for new/update graph

		if (prec_flag == 1) {

			// Draw precipitation area label

			SelectFont(&FontArial32);
			SelectColors(DODGER_BLUE, BLACK);
			PrintStringLeft("Precip:", x - 42, y - 285);

			// Draw new precipitation graph

			DrawGraph(x, y, x_size, y_size, hr_size);

			// Calculate percipitation scale high

			prec_scale_high = fmaxf(prec_acc, prec_rate);
			if (prec_scale_high <= 0.5) {
				prec_scale_high = 0.5;
			} else {
				prec_scale_high = ceil(prec_scale_high);
			}

			// Calculate percipitation scale sub-division

			if (prec_scale_high == 0.5) {
				b = 0.1;
			} else if (prec_scale_high == 1) {
				b = 0.2;
			} else {
				b = 0.5;
			}

			// Display precipitation graph scale

			SelectFont(&Font5x9);
			SelectColors(BLACK, BLACK);
			DrawRectFilled(x - 5, y + 5, x - 31, y - (y_size + 3));
			for (a = 0; a <= prec_scale_high; a = a + b) {
				sprintf(str, "%.2f\"", a);
				y_pos = Y_Offset(a, prec_scale_high, 0, y_size);
				SelectColors(LT_GRAY, BLACK);
				PrintStringRight(str, x - 5, y - (y_pos + 4));
				SelectColors(MED_GRAY, BLACK);
				DrawLine(x, y - y_pos, x + (x_size - 1), y - y_pos);
			}

			prec_flag = 0;
		}

		// Graph any previous and current precipitation rates

		for (i = 0; i <= minute; i++) {
			a = i;
			b = x_size;
			c = b * (a / 1440);
			j = c;

			if (prec_rate_past[i] > 0) {
				y_pos = Y_Offset(prec_rate_past[i], prec_scale_high, 0, y_size);
				SelectColors(BITTER_LEMON, BLACK);
				DrawLine(x + j, y, x + j, y - y_pos);
			}
		}

		// Graph any previous and current precipitation accumulation, on top of precipitation rates

		for (i = 0; i <= minute; i++) {
			a = i;
			b = x_size;
			c = b * (a / 1440);
			j = c;
			if (i == 0) {
				d = j;
			} else {
				d = b * ((a - 1) / 1440);
			}
			k = d;

			if (prec_acc_past[i] != -1) {
				y_pos = Y_Offset(prec_acc_past[i], prec_scale_high, 0, y_size);
				if ((i != 0) && (prec_acc_past[i - 1] != -1)) {
					ym1_pos = Y_Offset(prec_acc_past[i - 1], prec_scale_high, 0, y_size);
				} else {
					ym1_pos = y_pos;
				}
				SelectColors(DODGER_BLUE, DODGER_BLUE);
				DrawThickLine(x + k, y - ym1_pos, x + j, y - y_pos);
			}
		}

		// Display additional percipitation information

		SelectFont(&FontArial32);

		// Compute any change in lightning strikes accumulation

		a = ltng - ltng_last;
		if (a < 0) {
			a = a + 256;
		}

		// Compute lightning strikes accumulation

		ltng_acc = ltng_acc + a;

		// Save current lightning strikes for next minute

		ltng_last = ltng;

		// Clear lightning area

		SelectColors(BLACK, BLACK);
		DrawRectFilled(x + 516, y - 298, x + 889, y - 259);
		DrawRectFilled(x + 516, y - 250, x + 889, y - 211);

		// Check for lightning

		if (ltng_acc > 0) {

			// Display lightning strikes count

			SelectColors(BRIGHT_RED, BLACK);
			PrintStringLeft("Ltng Strikes:", x + 516, y - 298);
			sprintf(str, "%d", ltng_acc);
			SelectColors(WHITE, BLACK);
			PrintStringRight(str, x + 889, y - 298);

			// Display last lightning strike distance

			SelectColors(BRIGHT_RED, BLACK);
			PrintStringLeft("Ltng Dist:", x + 516, y - 250);
			sprintf(str, "%d mi", ltng_dist);
			SelectColors(WHITE, BLACK);
			PrintStringRight(str, x + 889, y - 250);
		}

		// Clear precipitation rate area

		SelectColors(BLACK, BLACK);
		DrawRectFilled(x + 516, y - 202, x + 889, y - 163);

		// Display current precipitation rate

		if (prec_acc > 0) {
			SelectColors(BITTER_LEMON, BLACK);
			PrintStringLeft("Precip Rate:", x + 516, y - 202);
			sprintf(str, "%.1f\"/hr", prec_rate);
			SelectColors(WHITE, BLACK);
			PrintStringRight(str, x + 889, y - 202);
		}

		// Display current precipitation description
		// (source: https://www.baranidesign.com/faq-articles/2020/1/19/rain-rate-intensity-classification)

		SelectColors(BLACK, BLACK);
		DrawRectFilled(x - 42, y - 237, x + 147, y - 198);
		if (prec_rate == 0) {
			sprintf(str, " ");
		} else if (prec_rate <= 0.1) {
			sprintf(str, "Light");
		} else if (prec_rate <= 0.3) {
			sprintf(str, "Moderate");
		} else if (prec_rate <= 2) {
			sprintf(str, "Heavy");
		} else {
			sprintf(str, "Violent");
		}
		SelectColors(WHITE, BLACK);
		PrintStringLeft(str, x - 42, y - 237);

/*******************************************************************************
*
* Solar Area
*
*******************************************************************************/

		// Calculate graph area x,y origin

		x = h_size - (x_size + 2);
		y = (y_size * 6) - 1;

		// lux reading discontinuity adjustment code begins

		a = lux_raw / 10;

		// 5,110 lux (40 W/m^2) discontinuity (discontinuity is 64 + 32 = 96, 96 / 4 = 24)

		if ((lux_flag1 == 0) && (a > 511)) {															// 511 is upper limit at discontinuity
			lux_flag1 = 1;
		}
		if ((lux_flag1 == 0) && ((lux_last_reading > 415) && ((lux_last_reading - a) > 24))) {			// greater than lower limit, and change greater than half of discontinuity
			lux_flag1 = 1;
		}
		if ((lux_flag1 == 1) && (a < 415)) {															// lower limit is upper limit minus discontinuity
			lux_flag1 = 0;
		}
		if ((lux_flag1 == 1) && ((lux_last_reading < 511) && ((a - lux_last_reading) > 24))) {			// lower than upper limit, and change greater than half of discontinuity
			lux_flag1 = 0;
		}

		// 13,590 lux (107 W/m^2) discontinuity (discontinuity is 64 + 32 = 96, 96 / 4 = 24)

		if ((lux_flag2 == 0) && (a > 1359)) {															// 1359 is upper limit at discontinuity
			lux_flag2 = 1;
		}
		if ((lux_flag2 == 0) && ((lux_last_reading > 1263) && ((lux_last_reading - a) > 24))) {			// greater than lower limit, and change greater than half of discontinuity
			lux_flag2 = 1;
		}
		if ((lux_flag2 == 1) && (a < 1263)) {															// lower limit is upper limit minus discontinuity
			lux_flag2 = 0;
		}
		if ((lux_flag2 == 1) && ((lux_last_reading < 1359) && ((a - lux_last_reading) > 24))) {			// lower than upper limit, and change greater than half of discontinuity
			lux_flag2 = 0;
		}

		// 24,310 lux (192 W/m^2) discontinuity (discontinuity is 128, 128 / 4 = 32)

		if ((lux_flag3 == 0) && (a > 2431)) {															// 2431 is upper limit at discontinuity
			lux_flag3 = 1;
		}
		if ((lux_flag3 == 0) && ((lux_last_reading > 2303) && ((lux_last_reading - a) > 32))) {			// greater than lower limit, and change greater than half of discontinuity
			lux_flag3 = 1;
		}
		if ((lux_flag3 == 1) && (a < 2303)) {															// lower limit is upper limit minus discontinuity
			lux_flag3 = 0;
		}
		if ((lux_flag3 == 1) && ((lux_last_reading < 2431) && ((a - lux_last_reading) > 32))) {			// lower than upper limit, and change greater than half of discontinuity
			lux_flag3 = 0;
		}

		// 46,390 lux (366 W/m^2) discontinuity (discontinuity is 256, 256 / 4 = 64)

		if ((lux_flag4 == 0) && (a > 4639)) {															// 4639 is upper limit at discontinuity
			lux_flag4 = 1;
		}
		if ((lux_flag4 == 0) && ((lux_last_reading > 4383) && ((lux_last_reading - a) > 64))) {			// greater than lower limit, and change greater than half of discontinuity
			lux_flag4 = 1;
		}
		if ((lux_flag4 == 1) && (a < 4383)) {															// lower limit is upper limit minus discontinuity
			lux_flag4 = 0;
		}
		if ((lux_flag4 == 1) && ((lux_last_reading < 4639) && ((a - lux_last_reading) > 64))) {			// lower than upper limit, and change greater than half of discontinuity
			lux_flag4 = 0;
		}

		// 68,150 lux (538 W/m^2) discontinuity (discontinuity is 512, 512 / 4 = 128)

		if ((lux_flag5 == 0) && (a > 6815)) {															// 6815 is upper limit at discontinuity
			lux_flag5 = 1;
		}
		if ((lux_flag5 == 0) && ((lux_last_reading > 6303) && ((lux_last_reading - a) > 128))) {		// greater than lower limit, and change greater than half of discontinuity
			lux_flag5 = 1;
		}
		if ((lux_flag5 == 1) && (a < 6303)) {															// lower limit is upper limit minus discontinuity
			lux_flag5 = 0;
		}
		if ((lux_flag5 == 1) && ((lux_last_reading < 6815) && ((a - lux_last_reading) > 128))) {		// lower than upper limit, and change greater than half of discontinuity
			lux_flag5 = 0;
		}

		// 89,910 lux (710 W/m^2) discontinuity (discontinuity is 512 + 128 = 640, 640 / 4 = 160)

		if ((lux_flag6 == 0) && (a > 8991)) {															// 8991 is upper limit at discontinuity
			lux_flag6 = 1;
		}
		if ((lux_flag6 == 0) && ((lux_last_reading > 8351) && ((lux_last_reading - a) > 160))) {		// greater than lower limit, and change greater than half of discontinuity
			lux_flag6 = 1;
		}
		if ((lux_flag6 == 1) && (a < 8351)) {															// lower limit is upper limit minus discontinuity
			lux_flag6 = 0;
		}
		if ((lux_flag6 == 1) && ((lux_last_reading < 8991) && ((a - lux_last_reading) > 160))) {		// lower than upper limit, and change greater than half of discontinuity
			lux_flag6 = 0;
		}

		// 111,510 lux (881 W/m^2) discontinuity (discontinuity is 512 + 128 = 640, 640 / 4 = 160)

		if ((lux_flag7 == 0) && (a > 11151)) {															// 11151 is upper limit at discontinuity
			lux_flag7 = 1;
		}
		if ((lux_flag7 == 0) && ((lux_last_reading > 10511) && ((lux_last_reading - a) > 160))) {		// greater than lower limit, and change greater than third of discontinuity
			lux_flag7 = 1;
		}
		if ((lux_flag7 == 1) && (a < 10511)) {															// lower limit is upper limit minus discontinuity
			lux_flag7 = 0;
		}
		if ((lux_flag7 == 1) && ((lux_last_reading < 11151) && ((a - lux_last_reading) > 160))) {		// lower than upper limit, and change greater than third of discontinuity
			lux_flag7 = 0;
		}

		// Save current lux reading as last lux reading for next time

		lux_last_reading = a;

		a = a + (lux_flag1 * 96);
		a = a + (lux_flag2 * 96);
		a = a + (lux_flag3 * 128);
		a = a + (lux_flag4 * 256);
		a = a + (lux_flag5 * 512);			// was 384
		a = a + (lux_flag6 * 640);			// was 512, 768
		a = a + (lux_flag7 * 640);			// was 768,

		// Total of adjustments: 2,368 (23,680 lux)

		// Scaling to bring value back to 163,830 lux maximum (maximum for a lux/10 14-bit value)

		b =  a / 18751.0;					// 18,751 = 16,383 + 2,368
		c =  0.126287 * b;					// 0.126287 = 1 - (16,383 / 18,751)
		a = (1 - c) * a;

		lux_adjusted = 10 * a;

		// lux reading discontinuity adjustment code ends

		// Calculate solar(adjusted)

		solar_adjusted = Solar(lux_adjusted);
		if (solar_adjusted_last == -1) {
			solar_adjusted_last = solar_adjusted;
		}

		// Update solar peak

		if (solar_peak < solar_adjusted) {
			solar_peak = solar_adjusted;
		}

		// Display current solar radiation

		SelectFont(&FontArial104);
		SelectColors(BLACK, BLACK);
		DrawRectFilled(x + 147, y - 285, x + 603, y - 156);
		a = solar_adjusted;
		if (a < 1000) {
			sprintf(str, "%.0f", a);
		} else {
			sprintf(str, "%.0f,%03.0f", (a / 1000), fmodf(a, 1000));
		}
		SelectColors(WHITE, BLACK);
		PrintStringLeft(str, x + 147, y - 285);

		// Check for new/update graph

		if (solar_flag == 1) {

			// Draw solar area label

			SelectFont(&FontArial32);
			SelectColors(SUNGLOW, BLACK);
			PrintStringLeft("Solar:", x - 42, y - 285);

			// Draw new solar graph

			DrawGraph(x, y, x_size, y_size, hr_size);

			// Display solar graph scale

			SelectFont(&Font5x9);
			SelectColors(BLACK, BLACK);
			DrawRectFilled(x - 5, y + 5, x - 55, y - (y_size + 3));
			for (i = 0; i <= 1200; i = i + 200) {
				if (i < 1000) {
					sprintf(str, "%dW/m\xFD", i);
				} else {
					sprintf(str, "1,%03dW/m\xFD", i - 1000);
				}
				y_pos = Y_Offset(i, 1200, 0, y_size);
				SelectColors(LT_GRAY, BLACK);
				PrintStringRight(str, x - 5, y - (y_pos + 4));
				SelectColors(MED_GRAY, BLACK);
				DrawLine(x, y - y_pos, x + (x_size - 1), y - y_pos);
			}

			solar_flag = 0;
		}

		// Graph current solar - adjusted

		y_pos = Y_Offset(solar_adjusted, 1200, 0, y_size);
		ym1_pos = Y_Offset(solar_adjusted_last, 1200, 0, y_size);
		SelectColors(SUNGLOW, SUNGLOW);
		DrawThickLine(x + xm1_pos, y - ym1_pos, x + x_pos, y - y_pos);

		// Save current solar - adjusted

		solar_adjusted_last = solar_adjusted;

		// Display lux value

		SelectFont(&Font5x9);
		SelectColors(BLACK, BLACK);
		DrawRectFilled(x - 40, y - 192, x + 91, y - 183);
		a = lux_adjusted;
		if (a > 0) {
			if (a < 1000) {
				sprintf(str, "%.0f", a);
			} else {
				sprintf(str, "%.0f,%03.0f", (a / 1000), fmodf(a, 1000));
			}
			strcat(str, " lux - adjusted");
			SelectColors(LT_GRAY, BLACK);
			PrintStringLeft(str, x - 40, y - 192);
		}

		// Display additional solar information

		SelectFont(&FontArial32);

		// Display today's peak solar

		SelectColors(BLACK, BLACK);
		DrawRectFilled(x + 516, y - 298, x + 889, y - 259);
		a = solar_peak;
		if (a > 0) {
			SelectColors(WHITE, BLACK);
			PrintStringLeft("Peak:", x + 516, y - 298);
			if (a < 1000) {
				sprintf(str, "%.0f W/m\xFD", a);
			} else {
				sprintf(str, "%.0f,%03.0f W/m\xFD", (a / 1000), fmodf(a, 1000));
			}
			SelectColors(WHITE, BLACK);
			PrintStringRight(str, x + 889, y - 298);
		}

		// Display current UV index and description

		SelectColors(BLACK, BLACK);
		DrawRectFilled(x + 516, y - 250, x + 889, y - 211);
		DrawRectFilled(x + 516, y - 202, x + 889, y - 163);
		if (uv > 0) {

			// Display current UV index

			SelectColors(SHINY_PURPLE, BLACK);
			PrintStringLeft("UV Index:", x + 516, y - 250);
			sprintf(str, "%.0f", uv);
			SelectColors(WHITE, BLACK);
			PrintStringRight(str, x + 889, y - 250);

			// Display current UV description
			// (source: https://www.epa.gov/sunsafety/uv-index-scale-0)

			SelectColors(SHINY_PURPLE, BLACK);
			PrintStringLeft("UV Risk:", x + 516, y - 202);
			if (uv <= 2) {
				sprintf(str, "Low");
			} else if (uv <= 5) {
				sprintf(str, "Moderate");
			} else if (uv <= 7) {
				sprintf(str, "High");
			} else if (uv <= 10) {
				sprintf(str, "Very High");
			} else {
				sprintf(str, "Extreme");
			}
			SelectColors(WHITE, BLACK);
			PrintStringRight(str, x + 889, y - 202);
		}

		// Display current solar description
		// (source: https://elinetechnology.com/tools-and-downloads/lux-light-illumination-chart/)

		SelectColors(BLACK, BLACK);
		DrawRectFilled(x - 42, y - 237, x + 132, y - 198);
		if (lux_adjusted < 1.0764) {
			sprintf(str, "Night");
		} else if (lux_adjusted <= 10.764) {
			sprintf(str, "Twilight");
		} else if (lux_adjusted <= 107.64) {
			sprintf(str, "Dark");
		} else if (lux_adjusted <= 1076.4) {
			sprintf(str, "Overcast");
		} else if (lux_adjusted <= 10764) {
			sprintf(str, "Daylight");
		} else {
			sprintf(str, "Sunlight");
		}
		SelectColors(WHITE, BLACK);
		PrintStringLeft(str, x - 42, y - 237);
	}

   return 0;
}