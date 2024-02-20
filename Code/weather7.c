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

// Defines for colors

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
#define BURNT_ORANGE 0x00FF7034
#define OUTRAGEOUS_ORANGE 0x00FF6E4A
#define PHILIPPINE_ORANGE 0x00FF7300
#define ORANGE_GROVE 0x00E5902C
#define BOLD_ORANGE 0x00F76B0E
#define CORAL_ORANGE 0x00E4694C

#define EXTRA_BRIGHT_YELLOW 0x00FFF70A
#define BRIGHT_YELLOW 0x00FFEB2A
#define BRIGHT_GOLDEN_YELLOW 0x00FFE138
#define HOT_YELLOW 0x00FFDA00
#define SUN_YELLOW 0x00FFFD55
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

// Defines and flags

#define	ANNUAL_HIGH_TEMP 96						// Annual high temperature for Temple, TX

#define ANNUAL_LOW_TEMP 35						// Annual low temperature for Temple, TX

#define HEAT_INDEX_TEMP_MINMUM 0				// Heat index minus temperature minimum, below which heat index is ignored

#define TEMP_WIND_CHILL_MINIMUM 0				// Temperature minus wind chill minimum, below which wind chill is ignored

#define TEMP_INSIDE_FLAG 0						// Option flag for displaying inside temperature

#define RH_INSIDE_FLAG 0						// Option flag for displaying inside relative humidity

#define WIND_DIR_SPEED_THRESHOLD 0				// Wind speed threshold, below which wind direction isn't graphed

#define WINDSOCK_ANGLE 30						// Windsock drooping angle

#define BARO_FLAG 0								// Option flag for displaying barometric pressure

#define LUX_FLAG 0								// Option flag for displaying lux (adjusted)

#define DEBUG_FLAG 0							// Option flag for displaying debug information

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
unsigned int PrintStringCenter(char *a, unsigned int b, unsigned int c);						// PrintStringCenter(string_ptr, x_pos, y_pos)

unsigned int DrawRectOutline(unsigned int a, unsigned int b, unsigned int c, unsigned int d);	// DrawRectOutline(x1_pos, y1_pos, x2_pos, y2_pos)
unsigned int DrawRectFilled(unsigned int a, unsigned int b, unsigned int c, unsigned int d);	// DrawRectFilled(x1_pos, y1_pos, x2_pos, y2_pos)

unsigned int DrawCircleOutline(unsigned int a, unsigned int b, unsigned int c);					// DrawCircleOutline(x_pos, y_pos, radius)
unsigned int DrawCircleFilled(unsigned int a, unsigned int b, unsigned int c);					// DrawCircleFilled(x_pos, y_pos, radius)

unsigned int DrawLine(unsigned int a, unsigned int b, unsigned int c, unsigned int d);			// DrawLine(x1_pos, y1_pos, x2_pos, y2_pos)

unsigned int DisplayBMP(unsigned int *a, unsigned int b, unsigned int c);						// DisplayBMP(BMP_ptr, x_pos, y_pos)

// Fonts

extern unsigned int Font5x9;
extern unsigned int FontArial32;
extern unsigned int FontArial104;

// BMPs

extern unsigned int Bright_Sunlight_BMP;
extern unsigned int Sunlight_BMP;
extern unsigned int Daylight_BMP;
extern unsigned int Dim_Daylight_BMP;
extern unsigned int Sunrise_Sunset_BMP;
extern unsigned int Twilight_BMP;

extern unsigned int Moon_0_BMP;
extern unsigned int Moon_1_BMP;
extern unsigned int Moon_2_BMP;
extern unsigned int Moon_3_BMP;
extern unsigned int Moon_4_BMP;
extern unsigned int Moon_5_BMP;
extern unsigned int Moon_6_BMP;
extern unsigned int Moon_7_BMP;
extern unsigned int Moon_8_BMP;
extern unsigned int Moon_9_BMP;
extern unsigned int Moon_10_BMP;
extern unsigned int Moon_11_BMP;
extern unsigned int Moon_12_BMP;
extern unsigned int Moon_13_BMP;
extern unsigned int Moon_14_BMP;
extern unsigned int Moon_15_BMP;
extern unsigned int Moon_16_BMP;
extern unsigned int Moon_17_BMP;
extern unsigned int Moon_18_BMP;
extern unsigned int Moon_19_BMP;
extern unsigned int Moon_20_BMP;
extern unsigned int Moon_21_BMP;
extern unsigned int Moon_22_BMP;
extern unsigned int Moon_23_BMP;
extern unsigned int Moon_24_BMP;
extern unsigned int Moon_25_BMP;
extern unsigned int Moon_26_BMP;
extern unsigned int Moon_27_BMP;
extern unsigned int Moon_28_BMP;
extern unsigned int Moon_29_BMP;

/*******************************************************************************
*
* Function to draw graphing areas
*
*******************************************************************************/

int DrawGraph(int x, int y, int x_size, int y_size, int hr_size) {
	SelectColors(BLACK, BLACK);
	DrawRectFilled(x - 2, y + 2, x + x_size, y - (y_size + 1));
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
* Function to draw thick lines
*
*******************************************************************************/

int DrawThickLine(int x1, int y1, int x2, int y2) {
	DrawLine(x1, y1, x2, y2);
	DrawLine(x1, y1 - 1, x2, y2 - 1);
	DrawLine(x1 + 1, y1, x2 + 1, y2);
	DrawLine(x1, y1 + 1, x2, y2 + 1);
	DrawLine(x1 - 1, y1, x2 - 1, y2);

	return 0;
}

/*******************************************************************************
*
* Function to draw symbol for wind gust
*
*******************************************************************************/

int DrawWindGust(int x, int y) {
	DrawLine(x, y, x - 2, y + 2);
	DrawLine(x, y, x + 2, y + 2);
	DrawLine(x, y, x, y + 10);

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

	if ((tf <= 50) && (wmph >= 2.7)) {
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
		if (hi < tf) {
			hi = -999;
		}
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

	int init_flag;

	time_t curtime;
	struct tm *loc_time;
	struct tm *gmt_time;
	char day_old[10];

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
	float temp_inside;

	int wind_speed_flag;
	float wind_scale_high;
	float wind_speed;
	float wind_speed_data[5];
	int wind_speed_data_index;
	float wind_speed_past[1440];
	float wind_speed_max;
	float wind_speed_min;
	float wind_gust_data[3];
	float wind_gust;
	float wind_gust_past[1440];
	int wind_gust_count;

	int rh_flag;
	float rh;
	float rh_last;
	float rh_inside;

	int wind_dir_flag;
	float wind_dir_data[10];
	float wind_dir;
	float wind_dir_last;

	int prec_flag;
	float prec_scale_high;
	float prec_last;
	float prec;
	float prec_acc;
	float prec_acc_past[1440];
	float prec_rate;
	float prec_rate_high;
	float prec_rate_data[6];
	int prec_rate_data_index;
	float prec_rate_past[1440];
	int ltng_last;
	int ltng;
	int ltng_acc;
	int ltng_dist;
	int ltng_dist_flag;
	float ltng_dist_past[1440];
	float barometric_pressure;

	int solar_flag;
	float solar_scale_high;
	float lux_raw;
	int lux_flag1;
	int lux_flag2;
	int lux_flag3;
	int lux_flag4;
	int lux_flag5;
	int lux_flag6;
	int lux_flag7;
	float lux_last;
	float lux_adjusted;
	float solar_adjusted;
	float solar_adjusted_past[1440];
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
	float e;
	float f;
	int packet_type;
	char packet_buffer[27];
	char packet_0x01_buffer[27];
	char packet_0x02_buffer[27];
	char packet_0x03_buffer[27];
	char packet_0x10_buffer[27];
	char str[100];
	char str2[30];
	int i;
	int j;
	int k;
	int l;
	int m;
	int n;
	int o;
	int p;
	int q;
	int minute;
	int x_pos;
	int y_pos;
	int minute_last;
	int x_pos_last;
	int y_pos_last;

	int schedule_point;

	int timeout_error_count = 0;
	int checksum_error_count = 0;
	int empty_packet_error_count = 0;

	// Note: Using "sudo nano /home/pi/.bashrc" to autostart on boot

/*******************************************************************************
*
* Initialization
*
*******************************************************************************/

	// Use system call to clear the screen

	strcpy(str, "clear");
	system(str);

	// Initialize the graphics screen

	printf("Mapping video frame buffer to user memory, ");

	// Open the file for reading and writing

	fbfd = open("/dev/fb0", O_RDWR);
	if (!fbfd) {
		printf("FAILED!\n");
		return(-1);
	}

	// Get fixed screen information

	if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo)) {
		printf("FAILED!\n");
		return(-1);
	}

	// Get variable screen information

	if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo)) {
		printf("FAILED!\n");
		return(-1);
	}

	// Map framebuffer to user memory

	fbp = (char*)mmap(0,
		finfo.smem_len,
		PROT_READ | PROT_WRITE,
		MAP_SHARED,
		fbfd, 0);

	if ((int)fbp == -1) {
		printf("FAILED!\n");
		return(-1);
	} else {
		printf("successful!\n");
	}

	// Display screen resolution and color depth

	printf("Screen resolution: %dx%d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel );

	// Check to make sure frame buffer is 32bpp

	if (vinfo.bits_per_pixel != 32) {
		printf("Video frame buffer is NOT setup for 32bpp. Fix it before using the Simple ARM Graphics library!\n");
		return(-1);
	}

	// Initialize WiringPi

	printf("\nInitializing WiringPi, ");
	i = RF_Mod_Init(packet_buffer);
	if (i != 0) {
		printf("FAILED! Error code: %d\n", i);
		return(-1);
	} else {
		printf("successful!\n");
	}

	// Sleep for 5 seconds

	sleep(5);

	// Use system call to clear the screen

	strcpy(str, "clear");
	system(str);

	// Use system call to turn the cursor off

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

	// Display waiting message

	SelectFont(&FontArial32);
	SelectColors(WHITE, BLACK);
	PrintStringLeft("Waiting for start of next minute...", 0, 1039);

	// Set packet type to 0x10

	packet_type = 0x10;

	// Wait for start of next minute

	j = 1;
	while (j) {

		// Read a packet

		i = RF_Rd_Pkt(packet_type);
		if (i == 0) {
			usleep(800000);
		} else if (i == -1) {
			usleep(1100000);
		} else if (i == -2) {
			usleep(800000);
		} else if (i == -3) {
			usleep(1100000);
		}

		// Toggle packet type for next read

		if (packet_type == 0x10) {
			packet_type = 0x03;
		} else {
			packet_type = 0x10;
		}

		// Get current date/time of system

		curtime = time(NULL);

		// Get current local date and time

		loc_time = localtime(&curtime);

		// Get current seconds

		strftime(str, 40, "%S", loc_time);
		i = atoi(str);

		if (DEBUG_FLAG == 1) {
			SelectFont(&Font5x9);
			sprintf(str, ":%02d", i);
			SelectColors(LT_GRAY, BLACK);
			PrintStringLeft(str, 0, 0);
		}

		if (i == 0) {
			j = 0;
		}
	}

	// Display initialization message

	SelectFont(&FontArial32);
	SelectColors(WHITE, BLACK);
	PrintStringLeft("Taking initial readings, 1 minute...", 0, 1039);

	// Set first pass initialization flag

	init_flag = 1;

	// Initialize wind gust variables

	wind_gust_data[0] = 0;
	wind_gust_data[1] = 0;
	wind_gust_data[2] = 0;
	wind_gust = 0;
	wind_gust_count = 0;

	// Set schedule point to 5

	schedule_point = 5;

/*******************************************************************************
*
* Main Loop
*
*******************************************************************************/

	while (1) {

/*******************************************************************************
*
* Weather Data Sampling
*
* Continuously read packet types 0x03 and 0x10
*
*******************************************************************************/

		// Read a packet

		i = RF_Rd_Pkt(packet_type);
		if (i == 0) {

			// Check for packet type 0x01

			if (packet_buffer[0] == 0x01) {

				// Copy packet type 0x01

				for (j = 0; j < 27; j++) {
					packet_0x01_buffer[j] = packet_buffer[j];
				}
			}

			// Check for packet type 0x02

			if (packet_buffer[0] == 0x02) {

				// Copy packet type 0x02

				for (j = 0; j < 27; j++) {
					packet_0x02_buffer[j] = packet_buffer[j];
				}
			}

			// Check for packet type 0x03

			if (packet_buffer[0] == 0x03) {

				// Copy packet type 0x03

				for (j = 0; j < 27; j++) {
					packet_0x03_buffer[j] = packet_buffer[j];
				}
			}

			// Check for packet type 0x10

			if (packet_buffer[0] == 0x10) {

				// Copy packet type 0x10

				for (j = 0; j < 27; j++) {
					packet_0x10_buffer[j] = packet_buffer[j];
				}
			}

			usleep(800000);

		} else if (i == -1) {
			timeout_error_count = timeout_error_count + 1;
			usleep(1100000);
		} else if (i == -2) {
			checksum_error_count = checksum_error_count + 1;
			usleep(800000);
		} else if (i == -3) {
			empty_packet_error_count = empty_packet_error_count + 1;
			usleep(1100000);
		}

		// Toggle packet type for next read

		if (packet_type == 0x10) {
			packet_type = 0x03;
		} else {
			packet_type = 0x10;
		}

		// Get current date/time of system

		curtime = time(NULL);

		// Get current local date and time

		loc_time = localtime(&curtime);

		// Get current seconds

		strftime(str, 40, "%S", loc_time);
		i = atoi(str);

		if (DEBUG_FLAG == 1) {
			SelectFont(&Font5x9);
			SelectColors(BLACK, BLACK);
			DrawRectFilled(0, 0, x + 749, 9);
			sprintf(str, ":%02d    %02X    %02X    Timeout: %d    Checksum: %d    Empty Pkt: %d", i, packet_buffer[0], packet_buffer[25], timeout_error_count, checksum_error_count, empty_packet_error_count);
			SelectColors(LT_GRAY, BLACK);
			PrintStringLeft(str, 0, 0);
		}

		// Check for schedule point

		if ((i == schedule_point) || (i == schedule_point + 1)) {

/*******************************************************************************
*
* Master Scheduler
*
* This "switch" statement is the master scheduler. It determines when data is
* sampled, processed, and displayed.
*
* Sample wind speed every 10 seconds, most everything else every 30 seconds,
* average readings over 1 minute, and update the display on the minute.
*
* :05	sample wind speed
* :15	sample wind speed
* :25	sample most data
* :35	sample wind speed
* :45	sample wind speed
* :55	sample all data and average over 1 minute
* :58	initialization and data processing
* :00	update display
*
*******************************************************************************/

			switch(schedule_point) {

				case 5:						// schedule point at 5 seconds (sample wind speed)

					// Get wind speed

					wind_speed = packet_0x10_buffer[9];

					// Get wind gust data

					wind_gust_data[2] = wind_gust_data[1];
					wind_gust_data[1] = wind_gust_data[0];
					wind_gust_data[0] = packet_0x10_buffer[9];

					// Check for wind gust (NOAA definition: brief increase in speed, wind peak >= 18, (wind peak - wind lull) >= 10, usually less than 20 seconds)

					if ((wind_gust_data[0] >= 18) && (wind_gust_data[0] - fminf(wind_gust_data[1], wind_gust_data[2]) >= 10)) {
						if (wind_gust < wind_gust_data[0]) {
							wind_gust = wind_gust_data[0];
						}
						wind_gust_count = wind_gust_count + 1;
					}

					// Set for next schedule point

					schedule_point = 15;

					break;

				case 15:					// schedule point at 15 seconds (sample wind speed)

					// Sum wind speed

					wind_speed = wind_speed + packet_0x10_buffer[9];

					// Get wind gust data

					wind_gust_data[2] = wind_gust_data[1];
					wind_gust_data[1] = wind_gust_data[0];
					wind_gust_data[0] = packet_0x10_buffer[9];

					// Check for wind gust (NOAA definition: brief increase in speed, wind peak >= 18, (wind peak - wind lull) >= 10, usually less than 20 seconds)

					if ((wind_gust_data[0] >= 18) && (wind_gust_data[0] - fminf(wind_gust_data[1], wind_gust_data[2]) >= 10)) {
						if (wind_gust < wind_gust_data[0]) {
							wind_gust = wind_gust_data[0];
						}
						wind_gust_count = wind_gust_count + 1;
					}

					// Set for next schedule point

					schedule_point = 25;

					break;

				case 25:					// schedule point at 25 seconds (sample most data)

					// Get temperature variable

					a = ((packet_0x03_buffer[6] & 0xF0) >> 4);
					b = (packet_0x03_buffer[6] & 0x0F);
					c = ((packet_0x03_buffer[7] & 0xF0) >> 4);
					d = (packet_0x03_buffer[7] & 0x0F);
					if ((packet_0x03_buffer[1] & 0x10) == 0x10) {
						temp = -((100 * a) + (10 * b) + c + (d / 10));
					} else {
						temp = (100 * a) + (10 * b) + c + (d / 10);
					}

					// Get relative humidity variable

					rh = packet_0x03_buffer[8];

					// Get last lightning distance

					a = ((packet_0x03_buffer[20] & 0xF0) >> 4);
					b = (packet_0x03_buffer[20] & 0x0F);
					ltng_dist = (10 * a) + b;

					// Get UV index variable

					uv = (packet_0x10_buffer[5] & 0x0F);

					// Get lux value

					lux_raw = ((packet_0x10_buffer[6] & 0x3F) << 8) + packet_0x10_buffer[7];

					// Sum wind speed

					wind_speed = wind_speed + packet_0x10_buffer[9];

					// Get wind gust data

					wind_gust_data[2] = wind_gust_data[1];
					wind_gust_data[1] = wind_gust_data[0];
					wind_gust_data[0] = packet_0x10_buffer[9];

					// Check for wind gust (NOAA definition: brief increase in speed, wind peak >= 18, (wind peak - wind lull) >= 10, usually less than 20 seconds)

					if ((wind_gust_data[0] >= 18) && (wind_gust_data[0] - fminf(wind_gust_data[1], wind_gust_data[2]) >= 10)) {
						if (wind_gust < wind_gust_data[0]) {
							wind_gust = wind_gust_data[0];
						}
						wind_gust_count = wind_gust_count + 1;
					}

					// Get wind direction

					a = (packet_0x10_buffer[10] & 0x0F);
					b = ((packet_0x10_buffer[11] & 0xF0) >> 4);
					c = (packet_0x10_buffer[11] & 0x0F);
					wind_dir_data[1] = (100 * a) + (10 * b) + c;

					// Set for next schedule point

					schedule_point = 35;

					break;

				case 35:					// schedule point at 35 seconds (sample wind speed)

					// Sum wind speed

					wind_speed = wind_speed + packet_0x10_buffer[9];

					// Get wind gust data

					wind_gust_data[2] = wind_gust_data[1];
					wind_gust_data[1] = wind_gust_data[0];
					wind_gust_data[0] = packet_0x10_buffer[9];

					// Check for wind gust (NOAA definition: brief increase in speed, wind peak >= 18, (wind peak - wind lull) >= 10, usually less than 20 seconds)

					if ((wind_gust_data[0] >= 18) && (wind_gust_data[0] - fminf(wind_gust_data[1], wind_gust_data[2]) >= 10)) {
						if (wind_gust < wind_gust_data[0]) {
							wind_gust = wind_gust_data[0];
						}
						wind_gust_count = wind_gust_count + 1;
					}

					// Set packet type to 0x02

					packet_type = 0x02;

					// Set for next schedule point

					schedule_point = 45;

					break;

				case 45:					// schedule point at 45 seconds (sample wind speed)

					// Sum wind speed

					wind_speed = wind_speed + packet_0x10_buffer[9];

					// Get wind gust data

					wind_gust_data[2] = wind_gust_data[1];
					wind_gust_data[1] = wind_gust_data[0];
					wind_gust_data[0] = packet_0x10_buffer[9];

					// Check for wind gust (NOAA definition: brief increase in speed, wind peak >= 18, (wind peak - wind lull) >= 10, usually less than 20 seconds)

					if ((wind_gust_data[0] >= 18) && (wind_gust_data[0] - fminf(wind_gust_data[1], wind_gust_data[2]) >= 10)) {
						if (wind_gust < wind_gust_data[0]) {
							wind_gust = wind_gust_data[0];
						}
						wind_gust_count = wind_gust_count + 1;
					}

					// Get barometric pressure

					a = ((packet_0x02_buffer[8] & 0xF0) >> 4);
					b = (packet_0x02_buffer[8] & 0x0F);
					c = ((packet_0x02_buffer[9] & 0xF0) >> 4);
					d = (packet_0x02_buffer[9] & 0x0F);
					barometric_pressure = (10 * a) + b + (c / 10) + (d / 100);

					// Set packet type to 0x01

					packet_type = 0x01;

					// Set for next schedule point

					schedule_point = 55;

					break;

				case 55:					// schedule point at 55 seconds (sample all data and average over 1 minute)

					// Get current weather readings and average over last minute

					// Get battery low flag

					battery_low_flag = ((packet_0x03_buffer[1] & 0x08) >> 3);

					// Update temperature variable

					a = ((packet_0x03_buffer[6] & 0xF0) >> 4);
					b = (packet_0x03_buffer[6] & 0x0F);
					c = ((packet_0x03_buffer[7] & 0xF0) >> 4);
					d = (packet_0x03_buffer[7] & 0x0F);
					if ((packet_0x03_buffer[1] & 0x10) == 0x10) {
						temp = (temp - ((100 * a) + (10 * b) + c + (d / 10))) / 2;
					} else {
						temp = (temp + ((100 * a) + (10 * b) + c + (d / 10))) / 2;
					}

					// Update relative humidity variable

					rh = (rh + packet_0x03_buffer[8]) / 2;

					// Get lightning strikes

					ltng = (packet_0x03_buffer[18] & 0x7F);

					// Update last lightning distance

					a = ((packet_0x03_buffer[20] & 0xF0) >> 4);
					b = (packet_0x03_buffer[20] & 0x0F);
					ltng_dist = (ltng_dist + ((10 * a) + b)) / 2;

					// Update UV index variable

					uv = round((uv + (packet_0x10_buffer[5] & 0x0F)) / 2);

					// Update lux raw variable

					a = (lux_raw + (((packet_0x10_buffer[6] & 0x3F) << 8) + packet_0x10_buffer[7])) / 2;
					lux_raw = 10 * a;

					// Update wind speed variable

					wind_speed = (wind_speed + packet_0x10_buffer[9]) / 6;

					// Get wind gust data

					wind_gust_data[2] = wind_gust_data[1];
					wind_gust_data[1] = wind_gust_data[0];
					wind_gust_data[0] = packet_0x10_buffer[9];

					// Check for wind gust (NOAA definition: brief increase in speed, wind peak >= 18, (wind peak - wind lull) >= 10, usually less than 20 seconds)

					if ((wind_gust_data[0] >= 18) && (wind_gust_data[0] - fminf(wind_gust_data[1], wind_gust_data[2]) >= 10)) {
						if (wind_gust < wind_gust_data[0]) {
							wind_gust = wind_gust_data[0];
						}
						wind_gust_count = wind_gust_count + 1;
					}

					// Get wind direction

					a = (packet_0x10_buffer[10] & 0x0F);
					b = ((packet_0x10_buffer[11] & 0xF0) >> 4);
					c = (packet_0x10_buffer[11] & 0x0F);
					wind_dir_data[0] = (100 * a) + (10 * b) + c;

					// Get precipitation

					a = ((packet_0x10_buffer[19] & 0xF0) >> 4);
					b = (packet_0x10_buffer[19] & 0x0F);
					c = ((packet_0x10_buffer[20] & 0xF0) >> 4);
					d = (packet_0x10_buffer[20] & 0x0F);
					prec = (10 * a) + b + (c / 10) + (d / 100);

					// Get inside temperature

					a = ((packet_0x01_buffer[4] & 0xF0) >> 4);
					b = (packet_0x01_buffer[4] & 0x0F);
					c = ((packet_0x01_buffer[5] & 0xF0) >> 4);
					d = (packet_0x01_buffer[5] & 0x0F);
					if ((packet_0x01_buffer[1] & 0x08) == 0x08) {
						temp_inside = -((100 * a) + (10 * b) + c + (d / 10));
					} else {
						temp_inside = (100 * a) + (10 * b) + c + (d / 10);
					}

					// Get inside relative humidity

					rh_inside = packet_0x01_buffer[6];

					// Set for next schedule point

					schedule_point = 58;

					break;

				case 58:						// schedule point at 58 seconds (initialization and data processing)

/*******************************************************************************
*
* First Pass Initialization Stage 1
*
* For things that need to be initialized after first weather data sampling
*
*******************************************************************************/

					if (init_flag == 1) {

						// Initialize wind speed data variables
						for (i = 0; i < 5; i++) {
							wind_speed_data[i] = wind_speed;
						}
						wind_speed_data_index = 0;

						// Initialize relative humdity last variable

						rh_last = rh;

						// Initialize wind direction data variables by copying first samples

						for (i = 9; i > 1; i = i - 2) {
							wind_dir_data[i] = wind_dir_data[1];
							wind_dir_data[i - 1] = wind_dir_data[0];
						}

						// Initialize precipitation variables

						prec_last = prec;

						// Initialize precipitation rate data variables

						for (i = 0; i < 6; i++) {
							prec_rate_data[i] = 0;
						}
						prec_rate_data_index = 0;

						// Initialize lightning last variable

						ltng_last = ltng;

						// Initialize solar discontinuity variables

						lux_flag1 = 0;
						lux_flag2 = 0;
						lux_flag3 = 0;
						lux_flag4 = 0;
						lux_flag5 = 0;
						lux_flag6 = 0;
						lux_flag7 = 0;
						lux_last = lux_raw / 10;
					}

/*******************************************************************************
*
* Data Processing
*
* Note: Some data processing is sequence dependent (ex. the average wind speed must be computed before the wind chill can be computed)
*
*******************************************************************************/

					// Update temperature high/low variables

					if (temp_high < temp) {
						temp_high = temp;
					}
					if (temp_low > temp) {
						temp_low = temp;
					}

					// Calculate current dew point

					dew_point = DewPoint(temp, rh);

					// Calculate current heat index

					heat_index = HeatIndex(temp, rh);

					// Apply heat index filtering parameter

					if ((heat_index - temp) < HEAT_INDEX_TEMP_MINMUM) {
						heat_index = -999;
					}

					// Save wind speed data

					wind_speed_data[wind_speed_data_index] = wind_speed;
					wind_speed_data_index = wind_speed_data_index + 1;
					if (wind_speed_data_index >= 5) {
						wind_speed_data_index = 0;
					}

					// Compute wind speed averaged over last 5 minutes

					wind_speed = 0;
					for (i = 0; i < 5; i++) {
						wind_speed = wind_speed + wind_speed_data[i];
					}
					wind_speed = wind_speed / 5;

					// Update wind speed maxium and minimum variables

					if (wind_speed > wind_speed_max) {
						wind_speed_max = wind_speed;
					}
					if (wind_speed < wind_speed_min) {
						wind_speed_min = wind_speed;
					}

					// Calculate current wind chill

					wind_chill = WindChill(temp, wind_speed);

					// Apply wind chill filtering parameter

					if ((temp - wind_chill) < TEMP_WIND_CHILL_MINIMUM) {
						wind_chill = -999;
					}

					// Compute wind direction averaged over last 5 minutes

					a = 0;
					b = 0;
					for (i = 0; i < 10; i++) {
						a = a + sinf(wind_dir_data[i] * (M_PI / 180.0));
						b = b + cosf(wind_dir_data[i] * (M_PI / 180.0));
					}
					a = a / 10;
					b = b / 10;
					wind_dir = atan2f(a, b) * (180.0 / M_PI);
					if (wind_dir < 0) {
						wind_dir = wind_dir + 360;
					}

					// Update wind direction variables

					for (i = 9; i > 1; i--) {
						wind_dir_data[i] = wind_dir_data[i - 2];
					}

					// Compute any change in precipitation accumulation

					a = prec - prec_last;
					if (a < 0) {
						a = a + 10;
					}

					// Check for first precipitation

					if ((prec_acc == 0) && (a != 0)) {
						prec_flag = 1;
					}

					// Compute precipitation accumulation

					prec_acc = prec_acc + a;

					// Save current precipitation for next time

					prec_last = prec;

					// Save change in precipitation accumulation

					prec_rate_data[prec_rate_data_index] = a;
					prec_rate_data_index = prec_rate_data_index + 1;
					if (prec_rate_data_index >= 6) {
						prec_rate_data_index = 0;
					}

					// Compute precipitation rate over last 6 minutes

					prec_rate = 0;
					for (j = 0; j < 6; j++) {
						prec_rate = prec_rate + prec_rate_data[j];
					}
					prec_rate = 10 * prec_rate;

					// Update precipitation rate high variable

					if (prec_rate_high < prec_rate) {
						prec_rate_high = prec_rate;
					}

					// Compute any change in lightning strikes accumulation

					i = ltng - ltng_last;
					if (i < 0) {
						i = i + 128;
					}

					// Check for first lighting strike

					if ((ltng_acc == 0) && (i != 0)) {
						prec_flag = 1;
					}

					// Check for lighting strike

					if (i != 0) {
						ltng_dist_flag = 1;
					} else {
						ltng_dist_flag = 0;
					}

					// Compute lightning strikes accumulation

					ltng_acc = ltng_acc + i;

					// Save current lightning strikes for next time

					ltng_last = ltng;

					// lux reading discontinuity adjustment code begins

					a = lux_raw / 10;

					// 5,110 lux (40 W/m^2) discontinuity (discontinuity is 64 + 32 = 96, 96 / 4 = 24)

					if ((lux_flag1 == 0) && (a > 511)) {											// 511 is upper limit at discontinuity
						lux_flag1 = 1;
					}
					if ((lux_flag1 == 0) && ((lux_last > 415) && ((lux_last - a) > 24))) {			// greater than lower limit, and change greater than half of discontinuity
						lux_flag1 = 1;
					}
					if ((lux_flag1 == 1) && (a < 415)) {											// lower limit is upper limit minus discontinuity
						lux_flag1 = 0;
					}
					if ((lux_flag1 == 1) && ((lux_last < 511) && ((a - lux_last) > 24))) {			// lower than upper limit, and change greater than half of discontinuity
						lux_flag1 = 0;
					}

					// 13,590 lux (107 W/m^2) discontinuity (discontinuity is 64 + 32 = 96, 96 / 4 = 24)

					if ((lux_flag2 == 0) && (a > 1359)) {											// 1359 is upper limit at discontinuity
						lux_flag2 = 1;
					}
					if ((lux_flag2 == 0) && ((lux_last > 1263) && ((lux_last - a) > 24))) {			// greater than lower limit, and change greater than half of discontinuity
						lux_flag2 = 1;
					}
					if ((lux_flag2 == 1) && (a < 1263)) {											// lower limit is upper limit minus discontinuity
						lux_flag2 = 0;
					}
					if ((lux_flag2 == 1) && ((lux_last < 1359) && ((a - lux_last) > 24))) {			// lower than upper limit, and change greater than half of discontinuity
						lux_flag2 = 0;
					}

					// 24,310 lux (192 W/m^2) discontinuity (discontinuity is 128, 128 / 4 = 32)

					if ((lux_flag3 == 0) && (a > 2431)) {											// 2431 is upper limit at discontinuity
						lux_flag3 = 1;
					}
					if ((lux_flag3 == 0) && ((lux_last > 2303) && ((lux_last - a) > 32))) {			// greater than lower limit, and change greater than half of discontinuity
						lux_flag3 = 1;
					}
					if ((lux_flag3 == 1) && (a < 2303)) {											// lower limit is upper limit minus discontinuity
						lux_flag3 = 0;
					}
					if ((lux_flag3 == 1) && ((lux_last < 2431) && ((a - lux_last) > 32))) {			// lower than upper limit, and change greater than half of discontinuity
						lux_flag3 = 0;
					}

					// 46,390 lux (366 W/m^2) discontinuity (discontinuity is 256, 256 / 4 = 64)

					if ((lux_flag4 == 0) && (a > 4639)) {											// 4639 is upper limit at discontinuity
						lux_flag4 = 1;
					}
					if ((lux_flag4 == 0) && ((lux_last > 4383) && ((lux_last - a) > 64))) {			// greater than lower limit, and change greater than half of discontinuity
						lux_flag4 = 1;
					}
					if ((lux_flag4 == 1) && (a < 4383)) {											// lower limit is upper limit minus discontinuity
						lux_flag4 = 0;
					}
					if ((lux_flag4 == 1) && ((lux_last < 4639) && ((a - lux_last) > 64))) {			// lower than upper limit, and change greater than half of discontinuity
						lux_flag4 = 0;
					}

					// 68,150 lux (538 W/m^2) discontinuity (discontinuity is 512, 512 / 4 = 128)

					if ((lux_flag5 == 0) && (a > 6815)) {											// 6815 is upper limit at discontinuity
						lux_flag5 = 1;
					}
					if ((lux_flag5 == 0) && ((lux_last > 6303) && ((lux_last - a) > 128))) {		// greater than lower limit, and change greater than half of discontinuity
						lux_flag5 = 1;
					}
					if ((lux_flag5 == 1) && (a < 6303)) {											// lower limit is upper limit minus discontinuity
						lux_flag5 = 0;
					}
					if ((lux_flag5 == 1) && ((lux_last < 6815) && ((a - lux_last) > 128))) {		// lower than upper limit, and change greater than half of discontinuity
						lux_flag5 = 0;
					}

					// 89,910 lux (710 W/m^2) discontinuity (discontinuity is 512 + 128 = 640, 640 / 4 = 160)

					if ((lux_flag6 == 0) && (a > 8991)) {											// 8991 is upper limit at discontinuity
						lux_flag6 = 1;
					}
					if ((lux_flag6 == 0) && ((lux_last > 8351) && ((lux_last - a) > 160))) {		// greater than lower limit, and change greater than half of discontinuity
						lux_flag6 = 1;
					}
					if ((lux_flag6 == 1) && (a < 8351)) {											// lower limit is upper limit minus discontinuity
						lux_flag6 = 0;
					}
					if ((lux_flag6 == 1) && ((lux_last < 8991) && ((a - lux_last) > 160))) {		// lower than upper limit, and change greater than half of discontinuity
						lux_flag6 = 0;
					}

					// 111,510 lux (881 W/m^2) discontinuity (discontinuity is 512 + 128 = 640, 640 / 4 = 160)

					if ((lux_flag7 == 0) && (a > 11151)) {											// 11151 is upper limit at discontinuity
						lux_flag7 = 1;
					}
					if ((lux_flag7 == 0) && ((lux_last > 10511) && ((lux_last - a) > 160))) {		// greater than lower limit, and change greater than third of discontinuity
						lux_flag7 = 1;
					}
					if ((lux_flag7 == 1) && (a < 10511)) {											// lower limit is upper limit minus discontinuity
						lux_flag7 = 0;
					}
					if ((lux_flag7 == 1) && ((lux_last < 11151) && ((a - lux_last) > 160))) {		// lower than upper limit, and change greater than third of discontinuity
						lux_flag7 = 0;
					}

					// Save current lux reading as last lux reading for next time

					lux_last = a;

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

/*******************************************************************************
*
* First Pass Initialization Stage 2
*
* For things that need to be initialized after first data processing
*
*******************************************************************************/

					if (init_flag == 1) {
						init_flag = 0;

						// Initialize wind direction last variable

						wind_dir_last = wind_dir;

					}

					// Set for next schedule point

					schedule_point = 0;

					break;

				case 0:						// schedule point at 0 seconds (update display)

/*******************************************************************************
*
* Time and Date Display Areas
*
* Update the display once per minute on the minute
*
* Note: It can take up to 8 minutes after starting to sync to the minute
*
*******************************************************************************/

					// Get current time

					strftime(str, 40, "%-l:%M%P", loc_time);

					// Update displayed time

					SelectFont(&FontArial104);
					SelectColors(BLACK, BLACK);
					DrawRectFilled(1379, 949, 1919, 1079);
					SelectColors(WHITE, BLACK);
					PrintStringRight(str, 1919, 949);

					// Check for day not the same as old day

					strftime(str, 40, "%a", loc_time);									// ex. "Sun"
					if (strcmp(day_old, str) != 0) {
						strcpy(day_old, str);

						// Update displayed day and date

						SelectColors(BLACK, BLACK);
						DrawRectFilled(0, 949, 1253, 1079);
						strftime(str, 40, "%a, %b %-d, %Y", loc_time);					// ex. "Sun, Nov 27, 2022"
						SelectColors(WHITE, BLACK);
						PrintStringLeft(str, 0, 949);

						// New day: reset variables and erase previous past readings

						temp_high = temp;
						temp_low = temp;
						temp_high_flag = 1;
						temp_low_flag = 1;
						wind_gust_count = 0;
						wind_speed_max = wind_speed;
						wind_speed_min = wind_speed;
						wind_speed_flag = 1;
						rh_flag = 1;
						wind_dir_flag = 1;
						prec_acc = 0;
						prec_rate_high = 0;
						ltng_acc = 0;
						prec_flag = 1;
						solar_flag = 1;

						for (i = 0; i < 1440; i++) {
							temp_past[i] = -999;
							dew_point_past[i] = -999;
							wind_chill_heat_index_past[i] = -999;
							wind_speed_past[i] = -1;
							wind_gust_past[i] = -1;
							prec_acc_past[i] = -1;
							prec_rate_past[i] = -1;
							ltng_dist_past[i] = -1;
							solar_adjusted_past[i] = -1;

						}

						timeout_error_count = 0;
						checksum_error_count = 0;
						empty_packet_error_count = 0;
					}

					// Compute minute for saving data, and x position for graphing

					strftime(str, 30, "%-k", loc_time);
					strftime(str2, 30, "%M", loc_time);
					a = (60 * strtod(str, NULL)) + strtod(str2, NULL);
					minute = a;
					if (minute == 0) {
						minute_last = 0;
					} else {
						minute_last = minute - 1;
					}
					b = x_size;
					c = b * (a / 1440);
					x_pos = c;
					if (a == 0) {
						d = c;
					} else {
						d = b * ((a - 1) / 1440);
					}
					x_pos_last = d;

/*******************************************************************************
*
* Low Battery Display Area
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
						SelectColors(BRIGHT_RED, BRIGHT_RED);
						DrawRectFilled(x + 3, y + 10, x + 47, y + 112);
						SelectColors(WHITE, WHITE);
						DrawRectFilled(x + 5, y + 92, x + 45, y + 110);
					}

/*******************************************************************************
*
* Temperature Display Area
*
*******************************************************************************/

					// Calculate graph area x,y origin

					x = (h_size / 2) - (x_size + 2) - 28;
					y = (y_size * 2) - 1;

					// Save current temperature

					temp_past[minute] = temp;

					// Save current dew point

					dew_point_past[minute] = dew_point;

					// Save current wind chill/heat index

					wind_chill_heat_index_past[minute] = fmaxf(wind_chill, heat_index);

					// Display current temperature

					SelectFont(&FontArial104);
					SelectColors(BLACK, BLACK);
					DrawRectFilled(x + 88, y - 285, x + 492, y - 156);
					sprintf(str, "%.1f\xF8", temp);
					SelectColors(WHITE, BLACK);
					PrintStringCenter(str, x + 290, y - 285);

					// Check for temperature scale high change needed

					if (fmaxf(temp_high, heat_index) > temp_scale_high) {
						temp_high_flag = 1;
					}

					// Check for temperature scale low change needed

					if (wind_chill == -999) {
						if (dew_point < temp_scale_low) {
							temp_low_flag = 1;
						}
					} else {
						if (fminf(dew_point, wind_chill) < temp_scale_low) {
							temp_low_flag = 1;
						}
					}

					// Check for new/update graph

					if ((temp_high_flag == 1) || (temp_low_flag == 1)) {

						// Draw temperature area label

						SelectFont(&FontArial32);
						SelectColors(HOT_YELLOW, BLACK);
						PrintStringLeft("Temp:", x - 42, y - 285);

						// Draw thermometer symbol

						if (TEMP_INSIDE_FLAG == 1) {
							i = 10;
						} else {
							i = 0;
						}
						SelectColors(LT_GRAY, BLACK);
						DrawCircleFilled(x + 12, y - (234 - i), 3);
						DrawLine(x + 9, y - (234 - i), x + 9, y - (183 - i));
						DrawLine(x + 15, y - (234 - i), x + 15, y - (183 - i));
						SelectColors(LT_GRAY, RED);
						DrawCircleFilled(x + 12, y - (183 - i), 6);
						DrawLine(x + 16, y - (234 - i), x + 19, y - (234 - i));
						DrawLine(x + 16, y - (188 - i), x + 19, y - (188 - i));
						SelectFont(&Font5x9);
						SelectColors(LT_GRAY, BLACK);
						sprintf(str, "%d", ANNUAL_HIGH_TEMP);
						strcat(str, "\xF8\x46");
						PrintStringLeft(str, x + 24, y - (238 - i));
						sprintf(str, "%d", ANNUAL_LOW_TEMP);
						strcat(str, "\xF8\x46");
						PrintStringLeft(str, x + 24, y - (192 - i));

						// Draw new temperature graph

						DrawGraph(x, y, x_size, y_size, hr_size);

						// Check for new temperature scale high

						if (temp_high_flag == 1) {
							temp_high_flag = 0;

							if (heat_index == -999) {
								a = temp_high;
							} else {
								a = heat_index;
							}
							temp_scale_high = 10 * ceilf(a / 10);
						}

						// Check for new temperature scale low

						if (temp_low_flag == 1) {
							temp_low_flag = 0;

							if (wind_chill == -999) {
								a = dew_point;
							} else {
								a = fminf(dew_point, wind_chill);
							}
							temp_scale_low = 10 * floorf(a / 10);
						}

						// Check for temperature scale low not a number

						if (isnanf(temp_scale_low) != 0) {
							temp_scale_low = temp_scale_high;
							temp_scale_high = temp_scale_low + 10;
						}

						// Display temperature graph scale

						SelectFont(&Font5x9);
						SelectColors(BLACK, BLACK);
						DrawRectFilled(x - 5, y + 5, x - 34, y - (y_size + 18));
						sprintf(str, "\xF8\x46");
						SelectColors(LT_GRAY, BLACK);
						PrintStringRight(str, x - 5, y - (y_size + 18));
						for (i = temp_scale_low; i <= temp_scale_high; i = i + 10) {
							sprintf(str, "%d", i);
							y_pos = Y_Offset(i, temp_scale_high, temp_scale_low, y_size);
							SelectColors(LT_GRAY, BLACK);
							PrintStringRight(str, x - 5, y - (y_pos + 4));
							SelectColors(MED_GRAY, MED_GRAY);
							DrawLine(x, y - y_pos, x + (x_size - 1), y - y_pos);
						}

						// Graph any previous and current wind chills/heat indexes, dew points, and temperatures.

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

							// Graph any previous and current dew points

							if (dew_point_past[i] != -999) {
								y_pos = Y_Offset(dew_point_past[i], temp_scale_high, temp_scale_low, y_size);
								if ((i != 0) && (dew_point_past[i - 1] != -999)) {
									y_pos_last = Y_Offset(dew_point_past[i - 1], temp_scale_high, temp_scale_low, y_size);
								} else {
									y_pos_last = y_pos;
								}
								SelectColors(BRIGHT_APPLE_GREEN, BRIGHT_APPLE_GREEN);
								DrawThickLine(x + k, y - y_pos_last, x + j, y - y_pos);
							}

							// Graph any previous and current wind chills/heat indexes

							if (wind_chill_heat_index_past[i] != -999) {
								y_pos = Y_Offset(wind_chill_heat_index_past[i], temp_scale_high, temp_scale_low, y_size);
								if ((i != 0) && (wind_chill_heat_index_past[i - 1] != -999)) {
									y_pos_last = Y_Offset(wind_chill_heat_index_past[i - 1], temp_scale_high, temp_scale_low, y_size);
								} else {
									y_pos_last = y_pos;
								}
								if (wind_chill_heat_index_past[i] <= 50) {
									SelectColors(ICE_BLUE, ICE_BLUE);
								} else {
									SelectColors(BRIGHT_RED, BRIGHT_RED);
								}
								DrawThickLine(x + k, y - y_pos_last, x + j, y - y_pos);
							}

							// Graph any previous and current temperatures
							// Note: Graph temperature last so it will be on top

							if (temp_past[i] != -999) {
								y_pos = Y_Offset(temp_past[i], temp_scale_high, temp_scale_low, y_size);
								if ((i != 0) && (temp_past[i - 1] != -999)) {
									y_pos_last = Y_Offset(temp_past[i - 1], temp_scale_high, temp_scale_low, y_size);
								} else {
									y_pos_last = y_pos;
								}
								SelectColors(HOT_YELLOW, HOT_YELLOW);
								DrawThickLine(x + k, y - y_pos_last, x + j, y - y_pos);
							}
						}
					} else {

						// Graph current dew point

						y_pos = Y_Offset(dew_point, temp_scale_high, temp_scale_low, y_size);
						if (dew_point_past[minute_last] != -999) {
							y_pos_last = Y_Offset(dew_point_past[minute_last], temp_scale_high, temp_scale_low, y_size);
						} else {
							y_pos_last = y_pos;
						}
						SelectColors(BRIGHT_APPLE_GREEN, BRIGHT_APPLE_GREEN);
						DrawThickLine(x + x_pos_last, y - y_pos_last, x + x_pos, y - y_pos);

						// Graph any current wind chill or heat index

						if (wind_chill_heat_index_past[minute] != -999) {
							y_pos = Y_Offset(wind_chill_heat_index_past[minute], temp_scale_high, temp_scale_low, y_size);
							if (wind_chill_heat_index_past[minute_last] != -999) {
								y_pos_last = Y_Offset(wind_chill_heat_index_past[minute_last], temp_scale_high, temp_scale_low, y_size);
							} else {
								y_pos_last = y_pos;
							}
							if (wind_chill_heat_index_past[minute] <= 50) {
								SelectColors(ICE_BLUE, ICE_BLUE);
							} else {
								SelectColors(BRIGHT_RED, BRIGHT_RED);
							}
							DrawThickLine(x + x_pos_last, y - y_pos_last, x + x_pos, y - y_pos);
						}

						// Graph current temperature
						// Note: Graph temperature last so it will be on top

						y_pos = Y_Offset(temp, temp_scale_high, temp_scale_low, y_size);
						if (temp_past[minute_last] != -999) {
							y_pos_last = Y_Offset(temp_past[minute_last], temp_scale_high, temp_scale_low, y_size);
						} else {
							y_pos_last = y_pos;
						}
						SelectColors(HOT_YELLOW, HOT_YELLOW);
						DrawThickLine(x + x_pos_last, y - y_pos_last, x + x_pos, y - y_pos);
					}

					// Animate thermometer symbol

					if (TEMP_INSIDE_FLAG == 1) {
						i = 10;
					} else {
						i = 0;
					}
					SelectColors(BLACK, BLACK);
					DrawRectFilled(x + 10, y - (234 - i), x + 14, y - (188 - i));
					a = temp;
					if (a > ANNUAL_HIGH_TEMP) {
						a = ANNUAL_HIGH_TEMP;
					}
					if (a < ANNUAL_LOW_TEMP) {
						a = ANNUAL_LOW_TEMP;
					}
					a = (a - ANNUAL_LOW_TEMP) / (ANNUAL_HIGH_TEMP - ANNUAL_LOW_TEMP);
					a = a * ((234 - i) - (188 - i));
					j = a + (188 - i);
					SelectColors(RED, RED);
					DrawRectFilled(x + 10, y - j, x + 14, y - (188 - i));

					// Display inside temperature

					if (TEMP_INSIDE_FLAG == 1) {
						SelectFont(&Font5x9);
						SelectColors(BLACK, BLACK);
						DrawRectFilled(x - 40, y - 245, x + 92, y - 236);
						SelectColors(LT_GRAY, BLACK);
						sprintf(str, "Inside: %.1f\xF8", temp_inside);
						PrintStringLeft(str, x - 40, y - 245);
					}

					// Display additional temperature information

					SelectFont(&FontArial32);

					// Display current weather description
					// (source: arbitrary)

					SelectColors(BLACK, BLACK);
					DrawRectFilled(x + 516, y - 298, x + 897, y - 259);
					if (prec_rate != 0) {
						sprintf(str, "Raining!");
						SelectColors(DODGER_BLUE, BLACK);
					} else {
						if (wind_chill != -999) {
							a = wind_chill;
						} else if (heat_index != -999) {
							a = heat_index;
						} else {
							a = temp;
						}
						if (a > 122) {
							sprintf(str, "Scorching");
						} else if (a > 112) {
							sprintf(str, "Very Hot");
						} else if (a > 102) {
							sprintf(str, "Hot");
						} else if (a > 92) {
							sprintf(str, "Very Warm");
						} else if (a > 82) {
							sprintf(str, "Warm");
						} else if (a > 72) {
							sprintf(str, "Pleasant");
						} else if (a > 62) {
							sprintf(str, "Cool");
						} else if (a > 52) {
							sprintf(str, "Chilly");
						} else if (a > 42) {
							sprintf(str, "Cold");
						} else if (a > 32) {
							sprintf(str, "Very Cold");
						} else if (a > 22) {
							sprintf(str, "Freezing");
						} else if (a > 12) {
							sprintf(str, "Subfreezing");
						} else {
							sprintf(str, "Arctic");
						}
						if (roundf(rh) > 60) {
							strcat(str, ", Humid");
						} else if (roundf(rh) < 30) {
							strcat(str, ", Dry");
						}
						SelectColors(WHITE, BLACK);
					}
					PrintStringLeft(str, x + 516, y - 298);

					// Display today's hi/lo temperatures

					SelectColors(BLACK, BLACK);
					DrawRectFilled(x + 516, y - 250, x + 889, y - 211);
					SelectColors(WHITE, BLACK);
					PrintStringLeft("Hi:", x + 516, y - 250);
					sprintf(str, "%.1f\xF8", temp_high);
					PrintStringRight(str, x + 691, y - 250);
					PrintStringLeft("Lo:", x + 707, y - 250);
					sprintf(str, "%.1f\xF8", temp_low);
					PrintStringRight(str, x + 889, y - 250);

					// Display any current wind chill/heat index

					SelectColors(BLACK, BLACK);
					if (minute == 0) {
						DrawRectFilled(x + 516, y - 202, x + 889, y - 163);				// Clear the entire wind chill/heat index area
					} else if (wind_chill_heat_index_past[minute_last] != -999) {
						if (wind_chill_heat_index_past[minute_last] <= 50) {
							DrawRectFilled(x + 717, y - 202, x + 889, y - 163);			// Clear everything after "Wind Chill"
						} else if (wind_chill_heat_index_past[minute_last] >= 70) {
							DrawRectFilled(x + 725, y - 202, x + 889, y - 163);			// Clear everything after "Heat Index"
						}
					}
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

/*******************************************************************************
*
* Wind Speed Display Area
*
*******************************************************************************/

					// Calculate graph area x,y origin

					x = h_size - (x_size + 2);
					y = (y_size * 2) - 1;

					// Save current wind speed average

					wind_speed_past[minute] = wind_speed;

					// Check for wind gust

					if (wind_gust != 0) {
						wind_gust_past[minute] = wind_gust;
						wind_gust = 0;
					}

					// Display current wind speed average

					SelectFont(&FontArial104);
					SelectColors(BLACK, BLACK);
					DrawRectFilled(x + 155, y - 285, x + 426, y - 156);
					sprintf(str, "%.1f", wind_speed);
					SelectColors(WHITE, BLACK);
					PrintStringCenter(str, x + 290, y - 285);

					// Check for wind speed scale change

					if (fmaxf(wind_speed, wind_gust_past[minute]) > wind_scale_high) {
						wind_speed_flag = 1;
					}

					// Check for new/update graph

					if (wind_speed_flag == 1) {
						wind_speed_flag = 0;

						// Draw wind speed area label

						SelectFont(&FontArial32);
						SelectColors(BRIGHT_SKY_BLUE, BLACK);
						PrintStringLeft("Wind:", x - 42, y - 285);
						SelectFont(&Font5x9);
						PrintStringRight("Speed", x + 55, y - 245);

						// Draw windsock

						SelectColors(LT_GRAY, LT_GRAY);
						DrawLine(x + 10, y - 222, x, y - 212);
						DrawLine(x, y - 212, x + 10, y - 202);
						DrawLine(x, y - 212, x, y - 181);

						// Draw wind speed symbol

//						SelectColors(LT_GRAY, LT_GRAY);
//						DrawLine(x - 6, y - 214, x + 16, y - 214);
//						DrawLine(x - 12, y - 209, x + 31, y - 209);
//						DrawLine(x - 8, y - 204, x + 23, y - 204);
//						DrawCircleOutline(x + 16, y - 219, 5);
//						DrawCircleOutline(x + 31, y - 214, 5);
//						DrawCircleOutline(x + 23, y - 199, 5);
//						SelectColors(BLACK, BLACK);
//						DrawRectFilled(x + 11, y - 215, x + 16, y - 219);
//						DrawRectFilled(x + 26, y - 210, x + 31, y - 214);
//						DrawRectFilled(x + 18, y - 203, x + 23, y - 199);

						// Draw new wind speed graph

						DrawGraph(x, y, x_size, y_size, hr_size);

						// Calculate wind scale high

						wind_scale_high = 10 * ceil(fmaxf(wind_speed, wind_gust_past[minute]) / 10);
						if (wind_scale_high < 10) {
							wind_scale_high = 10;
						}

						// Display wind speed graph scale

						SelectFont(&Font5x9);
						SelectColors(BLACK, BLACK);
						DrawRectFilled(x - 5, y + 5, x - 42, y - (y_size + 18));
						sprintf(str, "mph");
						SelectColors(LT_GRAY, BLACK);
						PrintStringRight(str, x - 5, y - (y_size + 18));
						if (wind_scale_high == 10) {
							j = 2;
						} else {
							j = 5;
						}
						for (i = 0; i <= wind_scale_high; i = i + j) {
							sprintf(str, "%d", i);
							y_pos = Y_Offset(i, wind_scale_high, 0, y_size);
							SelectColors(LT_GRAY, BLACK);
							PrintStringRight(str, x - 5, y - (y_pos + 4));
							SelectColors(MED_GRAY, MED_GRAY);
							DrawLine(x, y - y_pos, x + (x_size - 1), y - y_pos);
						}

						// Draw dotted line for wind speed threshold, below which wind direction isn't graphed

						if (WIND_DIR_SPEED_THRESHOLD != 0) {
//							sprintf(str, "Dir (%d)", WIND_DIR_SPEED_THRESHOLD);
							y_pos = Y_Offset(WIND_DIR_SPEED_THRESHOLD, wind_scale_high, 0, y_size);
//							SelectColors(MED_GRAY, BLACK);
//							PrintStringRight(str, x - 5, y - (y_pos + 4));
							SelectColors(MED_GRAY, MED_GRAY);
							for (i = 4; i < x_size; i = i + 5) {
								DrawLine(x + i, y - y_pos, x + i, y - y_pos);
							}
						}

						// Graph any previous and current wind speeds

						for (i = 0; i <= minute; i++) {
							if (wind_speed_past[i] != -1) {
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
								y_pos = Y_Offset(wind_speed_past[i], wind_scale_high, 0, y_size);
								if ((i != 0) && (wind_speed_past[i - 1] != -1)) {
									y_pos_last = Y_Offset(wind_speed_past[i - 1], wind_scale_high, 0, y_size);
								} else {
									y_pos_last = y_pos;
								}
								SelectColors(BRIGHT_SKY_BLUE, BRIGHT_SKY_BLUE);
								DrawThickLine(x + k, y - y_pos_last, x + j, y - y_pos);
							}
						}
					} else {

						// Graph current wind speed

						y_pos = Y_Offset(wind_speed, wind_scale_high, 0, y_size);
						if (wind_speed_past[minute_last] != -1) {
							y_pos_last = Y_Offset(wind_speed_past[minute_last], wind_scale_high, 0, y_size);
						} else {
							y_pos_last = y_pos;
						}
						SelectColors(BRIGHT_SKY_BLUE, BRIGHT_SKY_BLUE);
						DrawThickLine(x + x_pos_last, y - y_pos_last, x + x_pos, y - y_pos);
					}

					// Graph any previous and current wind gusts

					for (i = 0; i <= minute; i++) {
						if (wind_gust_past[i] != -1) {
							a = i;
							b = x_size;
							c = b * (a / 1440);
							j = c;
							y_pos = Y_Offset(wind_gust_past[i], wind_scale_high, 0, y_size);
							SelectColors(EXTRA_BRIGHT_YELLOW, EXTRA_BRIGHT_YELLOW);
							DrawWindGust(x + j, y - y_pos);
						}
					}

					// Animate windsock

					SelectColors(BLACK, BLACK);
					DrawRectFilled(x + 11, y - 222, x + 70, y - 163);
					a = wind_speed;
					b = 17.26 * (sin((WINDSOCK_ANGLE / 2) * (M_PI / 180.0)) / 60);	// Minimum speed for windsock to start to inflate
					if (a < b) {
						a = 0;
					}
					b = 17.26 * ((60 - sin((WINDSOCK_ANGLE / 2) * (M_PI / 180.0))) / 60);	// Maximum speed before windsock "puffs" out
					if (a > b) {
						a = 17.26;
					}
					if (a == 0) {													// Draw windsock with no wind
						for (i = 0; i <60; i++) {
							if ((i == 0) || (i == 24) || (i == 48)) {
								SelectColors(CORAL_ORANGE, BLACK);
							}
							if ((i == 12) || (i == 36)) {
								SelectColors(LT_GRAY, BLACK);
							}
							DrawLine(x + 11, y - (222 - i), x + 11, y - (222 - i));
							DrawLine(x + 12, y - (222 - i), x + 12, y - (222 - i));
						}
					}
					if (a != 0) {													// Draw windsock with wind
						b = (a / 17.26) * 60;
						j = b;
						e = tan((WINDSOCK_ANGLE / 2) * (M_PI / 180.0));
						for (i = 0; i < 60; i++) {									// Draw horizontal portion of windsock
							if ((i == 0) || (i == 24) || (i == 48)) {
								SelectColors(CORAL_ORANGE, BLACK);
							}
							if ((i == 12) || (i == 36)) {
								SelectColors(LT_GRAY, BLACK);
							}
							k = ((i / 6) + 1) / 2;
							if (i <= j) {
								l = 0;
							} else {
								a = i - j;
								b = a / e;
								l = b;
							}
							if ((20 - (2 * k)) > l) {
								DrawLine(x + 11 + i, y - (222 - k), x + 11 + i, y - (222 - (20 - (k + l))));
							} else {
								l = i;
								m = k;
								i = 59;
							}
						}
						if (j != 60) {
							e = sinf(WINDSOCK_ANGLE * (M_PI / 180.0));
							f = cosf(WINDSOCK_ANGLE * (M_PI / 180.0));
							for (i = l; i < 60; i++) {
								if ((i == 0) || (i == 24) || (i == 48)) {
									SelectColors(CORAL_ORANGE, BLACK);
								}
								if ((i == 12) || (i == 36)) {
									SelectColors(LT_GRAY, BLACK);
								}
								k = ((i / 6) + 1) / 2;
								a = i - l;
								b = k - m;
								c = (a * f) - (b * e);
								d = (a * e) + (b * f);
								n = c + l;
								o = d + m;
								DrawLine(x + 11 + n, y - (222 - o), x + 11 + n, y - (222 - o));
								a = i - l;
								b = (20 - k) - m;
								c = (a * f) - (b * e);
								d = (a * e) + (b * f);
								p = c + l;
								q = d + m;
								DrawLine(x + 11 + n, y - (222 - o), x + 11 + p, y - (222 - q));
								if (i < 59) {
									DrawLine(x + 12 + n, y - (222 - o), x + 12 + p, y - (222 - q));
								}
							}
							e = tan((WINDSOCK_ANGLE / 2) * (M_PI / 180.0));
							for (i = 0; i < 60; i++) {								// Redraw horizontal portion of windsock
								if ((i == 0) || (i == 24) || (i == 48)) {
									SelectColors(CORAL_ORANGE, BLACK);
								}
								if ((i == 12) || (i == 36)) {
									SelectColors(LT_GRAY, BLACK);
								}
								k = ((i / 6) + 1) / 2;
								if (i <= j) {
									l = 0;
								} else {
									a = i - j;
									b = a / e;
									l = b;
								}
								if ((20 - (2 * k)) > l) {
									DrawLine(x + 11 + i, y - (222 - k), x + 11 + i, y - (222 - (20 - (k + l))));
								} else {
									i = 59;
								}
							}
						}
					}
					SelectColors(BLACK, BLACK);
					DrawRectFilled(x + 10, y - 222, x + 1, y - 202);
					SelectColors(LT_GRAY, LT_GRAY);
					DrawLine(x + 10, y - 222, x, y - 212);
					DrawLine(x, y - 212, x + 10, y - 202);

					// Display additional wind speed information

					SelectFont(&FontArial32);

					// Display current wind description - Beaufort Scale
					// (source: https://www.weather.gov/pqr/wind)

					SelectColors(BLACK, BLACK);
					DrawRectFilled(x + 516, y - 298, x + 889, y - 259);
					if (wind_speed < 1) {
						sprintf(str, "Calm");
					} else if (wind_speed < 4) {
						sprintf(str, "Light Air");
					} else if (wind_speed < 8) {
						sprintf(str, "Light Breeze");
					} else if (wind_speed < 13) {
						sprintf(str, "Gentle Breeze");
					} else if (wind_speed < 19) {
						sprintf(str, "Moderate Breeze");
					} else if (wind_speed < 25) {
						sprintf(str, "Fresh Breeze");
					} else if (wind_speed < 32) {
						sprintf(str, "Strong Breeze");
					} else if (wind_speed < 39) {
						sprintf(str, "Near Gale");
					} else if (wind_speed < 47) {
						sprintf(str, "Gale");
					} else if (wind_speed < 55) {
						sprintf(str, "Strong Gale");
					} else if (wind_speed < 64) {
						sprintf(str, "Whole Gale");
					} else if (wind_speed < 75) {
						sprintf(str, "Storm Force");
					} else {
						sprintf(str, "Hurricane Force");
					}
					SelectColors(WHITE, BLACK);
					PrintStringLeft(str, x + 516, y - 298);

					// Display today's max/min wind speeds

					SelectColors(BLACK, BLACK);
					DrawRectFilled(x + 516, y - 250, x + 889, y - 211);
					SelectColors(WHITE, BLACK);
					PrintStringLeft("Max:", x + 516, y - 250);
					sprintf(str, "%.1f", wind_speed_max);
					PrintStringRight(str, x + 700, y - 250);
					PrintStringLeft("Min:", x + 716, y - 250);
					sprintf(str, "%.1f", wind_speed_min);
					PrintStringRight(str, x + 889, y - 250);

					// Display wind gusts count

					if (minute == 0) {
						SelectColors(BLACK, BLACK);
						DrawRectFilled(x + 516, y - 202, x + 889, y - 163);
					}
					if (wind_gust_count != 0) {
						SelectColors(EXTRA_BRIGHT_YELLOW, BLACK);
						PrintStringLeft("Wind Gusts:", x + 516, y - 202);
						SelectColors(WHITE, BLACK);
						sprintf(str, "%d", wind_gust_count);
						PrintStringRight(str, x + 889, y - 202);
					}

/*******************************************************************************
*
* Relative Humidity Display Area
*
*******************************************************************************/

					// Calculate graph area x,y origin

					x = (h_size / 2) - (x_size + 2) - 28;
					y = (y_size * 4) - 1;

					// Display relative humdity

					SelectFont(&FontArial104);
					SelectColors(BLACK, BLACK);
					DrawRectFilled(x + 112, y - 285, x + 468, y - 156);
					sprintf(str, "%.0f%%", roundf(rh));
					SelectColors(WHITE, BLACK);
					PrintStringCenter(str, x + 290, y - 285);

					// Check for new graph

					if (rh_flag == 1) {
						rh_flag = 0;

						// Draw relative humidity area label

						SelectFont(&FontArial32);
						SelectColors(RAINBOW_ORANGE, BLACK);
						PrintStringLeft("RH:", x - 42, y - 285);

						// Draw RH symbol

						SelectColors(RAINBOW_ORANGE, RAINBOW_ORANGE);
						DrawCircleFilled(x + 12, y - 203, 10);
						DrawLine(x + 12, y - 226, x + 2, y - 205);
						DrawLine(x + 12, y - 226, x + 3, y - 205);
						DrawLine(x + 12, y - 226, x + 4, y - 205);
						DrawLine(x + 12, y - 226, x + 5, y - 205);
						DrawLine(x + 12, y - 226, x + 6, y - 205);
						DrawLine(x + 12, y - 226, x + 7, y - 205);
						DrawLine(x + 12, y - 226, x + 8, y - 205);
						DrawLine(x + 12, y - 226, x + 9, y - 205);
						DrawLine(x + 12, y - 226, x + 10, y - 205);
						DrawLine(x + 12, y - 226, x + 11, y - 205);
						DrawLine(x + 12, y - 226, x + 12, y - 205);
						DrawLine(x + 12, y - 226, x + 13, y - 205);
						DrawLine(x + 12, y - 226, x + 14, y - 205);
						DrawLine(x + 12, y - 226, x + 15, y - 205);
						DrawLine(x + 12, y - 226, x + 16, y - 205);
						DrawLine(x + 12, y - 226, x + 17, y - 205);
						DrawLine(x + 12, y - 226, x + 18, y - 205);
						DrawLine(x + 12, y - 226, x + 19, y - 205);
						DrawLine(x + 12, y - 226, x + 20, y - 205);
						DrawLine(x + 12, y - 226, x + 21, y - 205);
						DrawLine(x + 12, y - 226, x + 22, y - 205);
						SelectFont(&Font5x9);
						SelectColors(BLACK, RAINBOW_ORANGE);
						PrintStringCenter("RH", x + 13, y - 207);

						// Draw new relative humidity graph

						DrawGraph(x, y, x_size, y_size, hr_size);

						// Display relative humidity graph scale

						SelectFont(&Font5x9);
						SelectColors(BLACK, BLACK);
						DrawRectFilled(x - 5, y + 5, x - 30, y - (y_size + 18));
						sprintf(str, "%%");
						SelectColors(LT_GRAY, BLACK);
						PrintStringRight(str, x - 5, y - (y_size + 18));
						for (i = 0; i <= 100; i = i + 20) {
							sprintf(str, "%d", i);
							y_pos = Y_Offset(i, 100, 0, y_size);
							SelectColors(LT_GRAY, BLACK);
							PrintStringRight(str, x - 5, y - (y_pos + 4));
							SelectColors(MED_GRAY, MED_GRAY);
							DrawLine(x, y - y_pos, x + (x_size - 1), y - y_pos);
						}
					}

					// Graph relative humidity

					y_pos = Y_Offset(rh, 100, 0, y_size);
					y_pos_last = Y_Offset(rh_last, 100, 0, y_size);
					SelectColors(RAINBOW_ORANGE, RAINBOW_ORANGE);
					DrawThickLine(x + x_pos_last, y - y_pos_last, x + x_pos, y - y_pos);

					// Save current relative humidity for next time

					rh_last = rh;

					// Display inside relative humidity

					if (RH_INSIDE_FLAG == 1) {
						SelectFont(&Font5x9);
						SelectColors(BLACK, BLACK);
						DrawRectFilled(x - 40, y - 245, x + 92, y - 236);
						SelectColors(LT_GRAY, BLACK);
						sprintf(str, "Inside: %.0f%%", rh_inside);
						PrintStringLeft(str, x - 40, y - 245);
					}

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

/*******************************************************************************
*
* Wind Direction Display Area
*
*******************************************************************************/

					// Calculate graph area x,y origin

					x = h_size - (x_size + 2);
					y = (y_size * 4) - 1;

					// Display current wind direction

					SelectFont(&FontArial104);
					SelectColors(BLACK, BLACK);
					DrawRectFilled(x + 109, y - 285, x + 472, y - 156);
					if ((wind_dir >= 348.75) || (wind_dir < 11.25)) {
						sprintf(str, "North");
					}
					if ((wind_dir >= 11.25) && (wind_dir < 33.75)) {
						sprintf(str, "NNE");
					}
					if ((wind_dir >= 33.75) && (wind_dir < 56.25)) {
						sprintf(str, "NE");
					}
					if ((wind_dir >= 56.25) && (wind_dir < 78.75)) {
						sprintf(str, "ENE");
					}
					if ((wind_dir >= 78.75) && (wind_dir < 101.25)) {
						sprintf(str, "East");
					}
					if ((wind_dir >= 101.25) && (wind_dir < 123.75)) {
						sprintf(str, "ESE");
					}
					if ((wind_dir >= 123.75) && (wind_dir < 146.25)) {
						sprintf(str, "SE");
					}
					if ((wind_dir >= 146.25) && (wind_dir < 168.75)) {
						sprintf(str, "SSE");
					}
					if ((wind_dir >= 168.75) && (wind_dir < 191.25)) {
						sprintf(str, "South");
					}
					if ((wind_dir >= 191.25) && (wind_dir < 213.75)) {
						sprintf(str, "SSW");
					}
					if ((wind_dir >= 213.75) && (wind_dir < 236.25)) {
						sprintf(str, "SW");
					}
					if ((wind_dir >= 236.25) && (wind_dir < 258.75)) {
						sprintf(str, "WSW");
					}
					if ((wind_dir >= 258.75) && (wind_dir < 281.25)) {
						sprintf(str, "West");
					}
					if ((wind_dir >= 281.25) && (wind_dir < 303.75)) {
						sprintf(str, "WNW");
					}
					if ((wind_dir >= 303.75) && (wind_dir < 326.25)) {
						sprintf(str, "NW");
					}
					if ((wind_dir >= 326.25) && (wind_dir < 348.75)) {
						sprintf(str, "NNW");
					}
					SelectColors(WHITE, BLACK);
					PrintStringCenter(str, x + 290, y - 285);

					// Check for new graph

					if (wind_dir_flag == 1) {
						wind_dir_flag = 0;

						// Draw wind direction area label

						SelectFont(&FontArial32);
						SelectColors(BRIGHT_SKY_BLUE, BLACK);
						PrintStringLeft("Wind:", x - 42, y - 285);
						SelectFont(&Font5x9);
						PrintStringRight("Direction", x + 55, y - 245);

						// Draw new wind direction graph

						DrawGraph(x, y, x_size, y_size, hr_size);

						// Display wind direction graph direction and compass points

						SelectFont(&Font5x9);
						SelectColors(BLACK, BLACK);
						DrawRectFilled(x - 5, y + 11, x - 34, y - (y_size + 10));
						y_pos = Y_Offset((360 - 0), 360, 0, y_size);				// North, 0 degrees
						SelectColors(LT_GRAY, BLACK);
						PrintStringRight("North", x - 5, y - (y_pos + 10));
						PrintStringRight("0\xF8", x - 5, y - (y_pos - 2));
						SelectColors(MED_GRAY, MED_GRAY);
						DrawLine(x, y - y_pos, x + (x_size - 1), y - y_pos);
						y_pos = Y_Offset((360 - 90), 360, 0, y_size);				// East, 90 degrees
						SelectColors(LT_GRAY, BLACK);
						PrintStringRight("East", x - 5, y - (y_pos + 10));
						PrintStringRight("90\xF8", x - 5, y - (y_pos - 2));
						SelectColors(MED_GRAY, MED_GRAY);
						DrawLine(x, y - y_pos, x + (x_size - 1), y - y_pos);
						y_pos = Y_Offset((360 - 180), 360, 0, y_size);				// South, 180 degrees
						SelectColors(LT_GRAY, BLACK);
						PrintStringRight("South", x - 5, y - (y_pos + 10));
						PrintStringRight("180\xF8", x - 5, y - (y_pos - 2));
						SelectColors(MED_GRAY, MED_GRAY);
						DrawLine(x, y - y_pos, x + (x_size - 1), y - y_pos);
						y_pos = Y_Offset((360 - 270), 360, 0, y_size);				// West, 270 degrees
						SelectColors(LT_GRAY, BLACK);
						PrintStringRight("West", x - 5, y - (y_pos + 10));
						PrintStringRight("270\xF8", x - 5, y - (y_pos - 2));
						SelectColors(MED_GRAY, MED_GRAY);
						DrawLine(x, y - y_pos, x + (x_size - 1), y - y_pos);
						y_pos = Y_Offset((360 - 360), 360, 0, y_size);				// North, 360 degrees
						SelectColors(LT_GRAY, BLACK);
						PrintStringRight("North", x - 5, y - (y_pos + 10));
						PrintStringRight("360\xF8", x - 5, y - (y_pos - 2));
						SelectColors(MED_GRAY, MED_GRAY);
						DrawLine(x, y - y_pos, x + (x_size - 1), y - y_pos);

						// Display minimum wind speed for graphing

						if (WIND_DIR_SPEED_THRESHOLD != 0) {
							y_pos = Y_Offset(360, 360, 0, y_size);
							sprintf(str, "Wind Speed >= %d mph", WIND_DIR_SPEED_THRESHOLD);
							SelectColors(MED_GRAY, BLACK);
							PrintStringRight(str, (x + x_size), y - (y_pos + 12));
						}
					}

					// Graph wind direction

					y_pos = Y_Offset((360 - wind_dir), 360, 0, y_size);
					y_pos_last = Y_Offset((360 - wind_dir_last), 360, 0, y_size);
					SelectColors(BRIGHT_SKY_BLUE, BRIGHT_SKY_BLUE);
					if (wind_speed >= WIND_DIR_SPEED_THRESHOLD) {
						if (fabsf(wind_dir - wind_dir_last) < 180) {
							DrawThickLine(x + x_pos_last, y - y_pos_last, x + x_pos, y - y_pos);
						} else {
							if (wind_dir < wind_dir_last) {
								DrawThickLine(x + x_pos_last, y - y_pos_last, x + x_pos_last, y);
								DrawThickLine(x + x_pos, y - (y_size - 1), x + x_pos, y - y_pos);
							} else {
								DrawThickLine(x + x_pos_last, y - y_pos_last, x + x_pos_last, y - (y_size - 1));
								DrawThickLine(x + x_pos, y, x + x_pos, y - y_pos);
							}
						}
					}

					// Save current wind direction for next time

					wind_dir_last = wind_dir;

					// Draw compass symbol

					SelectColors(LT_GRAY, LT_GRAY);
					DrawCircleFilled(x + 12, y - 198, 33);
					SelectColors(LT_GRAY, BLACK);
					DrawCircleFilled(x + 12, y - 198, 32);
					DrawCircleOutline(x + 12, y - 198, 20);
					DrawLine(x + 12, y - 218, x + 12, y - 178);
					DrawLine(x - 8, y - 198, x + 32, y - 198);
					SelectFont(&Font5x9);
					PrintStringCenter("N", x + 13, y - 228);
					PrintStringCenter("E", x + 39, y - 202);
					PrintStringCenter("S", x + 13, y - 176);
					PrintStringCenter("W", x - 13, y - 202);
					DrawCircleOutline(x + 12, y - 198, 32);

					// Animate compass symbol

					SelectColors(BRIGHT_RED, BRIGHT_RED);
					for (i = 21; i <= 31; i++) {
						a = sinf(wind_dir * (M_PI / 180.0));
						b = cosf(wind_dir * (M_PI / 180.0));
						c = i * a;
						d = i * b;
						j = 12 + c;
						k = 198 + d;
						for (l = -15; l <= 15; l++) {
							a = sinf((wind_dir + l) * (M_PI / 180.0));
							b = cosf((wind_dir + l) * (M_PI / 180.0));
							c = 21 * a;
							d = 21 * b;
							m = 12 + c;
							n = 198 + d;
							DrawLine(x + j, y - k, x + m, y - n);
						}
					}

					// Display additional information

					SelectFont(&FontArial32);

					// Display wind direction in degrees

					SelectColors(BLACK, BLACK);
					DrawRectFilled(x + 516, y - 298, x + 889, y - 259);
					SelectColors(WHITE, BLACK);
					PrintStringLeft("Compass Dir:", x + 516, y - 298);
					sprintf(str, "%.0f\xF8", wind_dir);
					PrintStringRight(str, x + 889, y - 298);

/*******************************************************************************
*
* Precipitation Display Area
*
*******************************************************************************/

					// Calculate graph area x,y origin

					x = (h_size / 2) - (x_size + 2) - 28;
					y = (y_size * 6) - 1;

					// Save current precipitation accumulation

					prec_acc_past[minute] = prec_acc;

					// Save current precipitation rate
					// Note: Since the precipitation rate is calculated from the precipitation accumulation spread over 6 minutes
					// the precipitation rate calculated is actually the precipitation rate from 3 minutes ago

					if (minute >= 3) {
						prec_rate_past[minute - 3] = prec_rate;
					}

					// Display current precipitation

					SelectFont(&FontArial104);
					SelectColors(BLACK, BLACK);
					DrawRectFilled(x + 130, y - 285, x + 450, y - 156);
					sprintf(str, "%.2f\"", prec_acc);
					SelectColors(WHITE, BLACK);
					PrintStringCenter(str, x + 290, y - 285);

					// Check for precipitation scale change needed

					if (fmaxf(prec_acc, prec_rate_high) > prec_scale_high) {
						prec_flag = 1;
					}

					// Check for new/update graph

					if (prec_flag == 1) {
						prec_flag = 0;

						// Draw precipitation area label

						SelectFont(&FontArial32);
						SelectColors(DODGER_BLUE, BLACK);
						PrintStringLeft("Precip:", x - 42, y - 285);

						// Draw raindrop symbol

						SelectColors(DODGER_BLUE, DODGER_BLUE);
						DrawCircleFilled(x + 12, y - 203, 10);
						DrawLine(x + 12, y - 226, x + 2, y - 205);
						DrawLine(x + 12, y - 226, x + 3, y - 205);
						DrawLine(x + 12, y - 226, x + 4, y - 205);
						DrawLine(x + 12, y - 226, x + 5, y - 205);
						DrawLine(x + 12, y - 226, x + 6, y - 205);
						DrawLine(x + 12, y - 226, x + 7, y - 205);
						DrawLine(x + 12, y - 226, x + 8, y - 205);
						DrawLine(x + 12, y - 226, x + 9, y - 205);
						DrawLine(x + 12, y - 226, x + 10, y - 205);
						DrawLine(x + 12, y - 226, x + 11, y - 205);
						DrawLine(x + 12, y - 226, x + 12, y - 205);
						DrawLine(x + 12, y - 226, x + 13, y - 205);
						DrawLine(x + 12, y - 226, x + 14, y - 205);
						DrawLine(x + 12, y - 226, x + 15, y - 205);
						DrawLine(x + 12, y - 226, x + 16, y - 205);
						DrawLine(x + 12, y - 226, x + 17, y - 205);
						DrawLine(x + 12, y - 226, x + 18, y - 205);
						DrawLine(x + 12, y - 226, x + 19, y - 205);
						DrawLine(x + 12, y - 226, x + 20, y - 205);
						DrawLine(x + 12, y - 226, x + 21, y - 205);
						DrawLine(x + 12, y - 226, x + 22, y - 205);

						// Draw new precipitation graph

						DrawGraph(x, y, x_size, y_size, hr_size);

						// Calculate percipitation scale high

						prec_scale_high = fmaxf(prec_acc, prec_rate_high);
						if (prec_scale_high <= 0.5) {
							prec_scale_high = 0.5;
						} else {
							prec_scale_high = ceil(2 * prec_scale_high) / 2;
						}

						// Calculate percipitation scale sub-division

						b = prec_scale_high / 5;

						// Display precipitation graph scales

						SelectFont(&Font5x9);
						SelectColors(BLACK, BLACK);
						DrawRectFilled(x - 5, y + 5, x - 40, y - (y_size + 30));
						DrawRectFilled(x + (x_size + 37), y + 5, x + (x_size + 7), y - (y_size + 18));
						if ((prec_acc == 0) &&  (ltng_acc == 0)) {
							SelectColors(LT_GRAY, BLACK);
						} else  {
							SelectColors(DODGER_BLUE, BLACK);
						}
						if (prec_scale_high <= 1) {
							sprintf(str, "inch");
						} else {
							sprintf(str, "inches");
						}
						PrintStringRight(str, x - 5, y - (y_size + 18));
						if (prec_acc > 0) {
							sprintf(str, "in/hr");
							SelectColors(BITTER_LEMON, BLACK);
							PrintStringRight(str, x - 5, y - (y_size + 30));
						}
						if (ltng_acc > 0) {
							sprintf(str, "miles");
							SelectColors(BRIGHT_RED, BLACK);
							PrintStringLeft(str, x + (x_size + 7), y - (y_size + 18));
						}
						for (a = 0; a <= 5; a++) {
							y_pos = Y_Offset(a, 5, 0, y_size);
							SelectColors(LT_GRAY, BLACK);
							sprintf(str, "%.1f", prec_scale_high * (a / 5));
							PrintStringRight(str, x - 5, y - (y_pos + 4));
							if (ltng_acc > 0) {
								sprintf(str, "%.0f", (a * 5));
								PrintStringLeft(str, x + (x_size + 7), y - (y_pos + 4));
							}
							SelectColors(MED_GRAY, MED_GRAY);
							DrawLine(x, y - y_pos, x + (x_size - 1), y - y_pos);
						}
					}

					// Graph any previous and current precipitation rates

					for (i = 0; i <= minute; i++) {
						if (prec_rate_past[i] > 0) {
							a = i;
							b = x_size;
							c = b * (a / 1440);
							j = c;
							y_pos = Y_Offset(prec_rate_past[i], prec_scale_high, 0, y_size);
							SelectColors(BITTER_LEMON, BITTER_LEMON);
							DrawLine(x + j, y, x + j, y - y_pos);
						}
					}

					// Graph any previous and current lighting distances, on top of precipitation rates

					if (ltng_dist_flag == 1) {
						ltng_dist_flag = 0;
						ltng_dist_past[minute] = ltng_dist;
					}
					for (i = 0; i <= minute; i++) {
						if (ltng_dist_past[i] != -1) {
							a = i;
							b = x_size;
							c = b * (a / 1440);
							j = c;
							y_pos = Y_Offset(ltng_dist_past[i], 25, 0, y_size);
							SelectColors(BRIGHT_RED, BRIGHT_RED);
							DrawThickLine(x + j, y - y_pos, x + j, y - y_pos);
						}
					}

					// Graph any previous and current precipitation accumulation, on top of precipitation graph

					for (i = 0; i <= minute; i++) {
						if (prec_acc_past[i] != -1) {
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
							y_pos = Y_Offset(prec_acc_past[i], prec_scale_high, 0, y_size);
							if ((i != 0) && (prec_acc_past[i - 1] != -1)) {
								y_pos_last = Y_Offset(prec_acc_past[i - 1], prec_scale_high, 0, y_size);
							} else {
								y_pos_last = y_pos;
							}
							SelectColors(DODGER_BLUE, DODGER_BLUE);
							DrawThickLine(x + k, y - y_pos_last, x + j, y - y_pos);
						}
					}

					// Display additional percipitation information

					SelectFont(&FontArial32);

					// Display current barometric pressure

					if (BARO_FLAG == 1) {
						SelectColors(WHITE, BLACK);
						PrintStringLeft("Baro:", x + 516, y - 298);
						sprintf(str, "%.2f inHg", barometric_pressure);
						PrintStringRight(str, x + 889, y - 298);
					}

					// Display lightning strikes count

					if (minute == 0) {
						SelectColors(BLACK, BLACK);
						DrawRectFilled(x + 516, y - 250, x + 889, y - 211);
					}
					if (ltng_acc > 0) {
						SelectColors(BRIGHT_RED, BLACK);
						PrintStringLeft("Ltng Strikes:", x + 516, y - 250);
						a = ltng_acc;
						if (a < 1000) {
							sprintf(str, "%.0f", a);
						} else {
							sprintf(str, "%.0f,%03.0f", (a / 1000), fmodf(a, 1000));
						}
						SelectColors(WHITE, BLACK);
						PrintStringRight(str, x + 889, y - 250);
					}

					// Display current precipitation rate

					if (minute == 0) {
						SelectColors(BLACK, BLACK);
						DrawRectFilled(x + 516, y - 202, x + 889, y - 163);
					}
					if (prec_acc > 0) {
						SelectColors(BITTER_LEMON, BLACK);
						PrintStringLeft("Precip Rate:", x + 516, y - 202);
						sprintf(str, "%.1f\"/hr", prec_rate);
						SelectColors(WHITE, BLACK);
						PrintStringRight(str, x + 889, y - 202);
					}

/*******************************************************************************
*
* Solar Radiation Display Area
*
*******************************************************************************/

					// Calculate graph area x,y origin

					x = h_size - (x_size + 2);
					y = (y_size * 6) - 1;

					// Save current solar adjusted

					solar_adjusted_past[minute] = solar_adjusted;

					// Display current solar radiation

					SelectFont(&FontArial104);
					SelectColors(BLACK, BLACK);
					DrawRectFilled(x + 116, y - 285, x + 464, y - 156);
					a = solar_adjusted;
					SelectColors(WHITE, BLACK);
					if (a < 100) {
						sprintf(str, "%.1f", a);
					} else if (a < 1000) {
						sprintf(str, "%.0f", a);
					} else {
						sprintf(str, "%.0f,%03.0f", (a / 1000), fmodf(a, 1000));
					}
					PrintStringCenter(str, x + 290, y - 285);

					// Check for solar scale change

					if (solar_adjusted > solar_scale_high) {
						solar_flag = 1;
					}

					// Check for new/update graph

					if (solar_flag == 1) {
						solar_flag = 0;

						// Draw solar area label

						SelectFont(&FontArial32);
						SelectColors(SUN_YELLOW, BLACK);
						PrintStringLeft("Solar:", x - 42, y - 285);

						// Draw new solar graph

						DrawGraph(x, y, x_size, y_size, hr_size);

						// Calculate solar scale high

						if (solar_adjusted <= 800) {
							solar_scale_high = 800;
						} else if (solar_adjusted <= 1000) {
							solar_scale_high = 1000;
						} else {
							solar_scale_high = 1250;
						}

						// Calculate solar scale sub-division

						if (solar_scale_high <= 1000) {
							j = 200;
						} else {
							j = 250;
						}

						// Display solar graph scale

						SelectFont(&Font5x9);
						SelectColors(BLACK, BLACK);
						DrawRectFilled(x - 5, y + 5, x - 53, y - (y_size + 18));
						sprintf(str, "W/m\xFD");
						SelectColors(LT_GRAY, BLACK);
						PrintStringRight(str, x - 5, y - (y_size + 18));
						for (i = 0; i <= solar_scale_high; i = i + j) {
							if (i < 1000) {
								sprintf(str, "%d", i);
							} else {
								sprintf(str, "1,%03d", i - 1000);
							}
							y_pos = Y_Offset(i, solar_scale_high, 0, y_size);
							SelectColors(LT_GRAY, BLACK);
							PrintStringRight(str, x - 5, y - (y_pos + 4));
							SelectColors(MED_GRAY, MED_GRAY);
							DrawLine(x, y - y_pos, x + (x_size - 1), y - y_pos);
						}

						// Graph any previous and current solars, adjusted

						for (i = 0; i <= minute; i++) {
							if (solar_adjusted_past[i] != -1) {
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
								y_pos = Y_Offset(solar_adjusted_past[i], solar_scale_high, 0, y_size);
								if ((i != 0) && (solar_adjusted_past[i - 1] != -1)) {
									y_pos_last = Y_Offset(solar_adjusted_past[i - 1], solar_scale_high, 0, y_size);
								} else {
									y_pos_last = y_pos;
								}
								SelectColors(SUN_YELLOW, SUN_YELLOW);
								DrawThickLine(x + k, y - y_pos_last, x + j, y - y_pos);
							}
						}
					} else {

						// Graph current solar, adjusted

						y_pos = Y_Offset(solar_adjusted_past[minute], solar_scale_high, 0, y_size);
						if (solar_adjusted_past[minute_last] != -1) {
							y_pos_last = Y_Offset(solar_adjusted_past[minute_last], solar_scale_high, 0, y_size);
						} else {
							y_pos_last = y_pos;
						}
						SelectColors(SUN_YELLOW, SUN_YELLOW);
						DrawThickLine(x + x_pos_last, y - y_pos_last, x + x_pos, y - y_pos);
					}

					// Display lux value

					if (LUX_FLAG == 1) {
						SelectFont(&Font5x9);
						SelectColors(BLACK, BLACK);
						DrawRectFilled(x - 40, y - 245, x + 92, y - 236);
						a = lux_adjusted;
						if (a > 0) {
							if (a < 1000) {
								sprintf(str, "%.0f", a);
							} else {
								sprintf(str, "%.0f,%03.0f", (a / 1000), fmodf(a, 1000));
							}
							strcat(str, " lux (adjusted)");
							SelectColors(LT_GRAY, BLACK);
							PrintStringLeft(str, x - 40, y - 245);
						}
					}

					// Display additional solar information

					SelectFont(&FontArial32);

					// Display current solar description and draw sun symbol
					// (source: arbitrary)

					SelectColors(BLACK, BLACK);
					DrawRectFilled(x + 516, y - 298, x + 889, y - 259);
					if (LUX_FLAG == 0) {
						i = 0;
					} else {
						i = 8;
					}
					DrawRectFilled(x - 21, y - (235 - i), x + 44, y - (190 - i));
					if (solar_adjusted >= 64) {
						if (uv >= 4) {
							if (solar_adjusted >= 1024) {
								sprintf(str, "Bright Sunlight");
								DisplayBMP(&Bright_Sunlight_BMP, x - 11, y - (235 - i));
							} else {
								sprintf(str, "Sunlight");
								DisplayBMP(&Sunlight_BMP, x - 11, y - (235 - i));
							}
						} else {
							sprintf(str, "Daylight");
							DisplayBMP(&Daylight_BMP, x - 11, y - (235 - i));
						}
					} else if (solar_adjusted >= 4) {
						sprintf(str, "Dim Daylight");
						DisplayBMP(&Dim_Daylight_BMP, x - 11, y - (235 - i));
					} else if (solar_adjusted >= 1) {
						if(minute < 720) {
							sprintf(str, "Sunrise");
						} else {
							sprintf(str, "Sunset");
						}
						DisplayBMP(&Sunrise_Sunset_BMP, x - 21, y - (235 - i));
					} else if (solar_adjusted >= 0.05) {
						sprintf(str, "Twilight");
						DisplayBMP(&Twilight_BMP, x - 21, y - (235 - i));
					} else {
						sprintf(str, "Night");

						// Get current GMT date and time

						gmt_time = gmtime(&curtime);

						// Get the GMT year

						strftime(str2, 30, "%Y", gmt_time);
						a = strtod(str2, NULL);									// Y

						// Get the GMT month

						strftime(str2, 30, "%-m", gmt_time);
						b = strtod(str2, NULL);									// M

						// Get the GMT day

						strftime(str2, 30, "%-d", gmt_time);
						c = strtod(str2, NULL);									// D

						// Get the GMT hour

						strftime(str2, 30, "%k", gmt_time);
						d = strtod(str2, NULL);

						// Calculate the number of days since the last full moon
						// source: https://www.subsystems.us/uploads/9/8/9/4/98948044/moonphase.pdf

						if (b <= 2) {
							a = a - 1;
							b = b + 12;
						}
						j = a / 100;											// A
						k = j / 4;												// B
						l = 2 - j + k;											// C
						m = 365.25 * (a + 4716);								// E
						n = 30.6001 * (b + 1);									// F
						d = l + c + m + n - 1524.5 + (d / 24);					// JD
						e = (d - 2451549.5) / 29.53059;
						o = e;
						f = roundf(29.53059 * (e - o));
						p = f;
						switch(p) {
							case 0:
								DisplayBMP(&Moon_0_BMP, x - 11, y - (235 - i));
								break;
							case 1:
								DisplayBMP(&Moon_1_BMP, x - 11, y - (235 - i));
								break;
							case 2:
								DisplayBMP(&Moon_2_BMP, x - 11, y - (235 - i));
								break;
							case 3:
								DisplayBMP(&Moon_3_BMP, x - 11, y - (235 - i));
								break;
							case 4:
								DisplayBMP(&Moon_4_BMP, x - 11, y - (235 - i));
								break;
							case 5:
								DisplayBMP(&Moon_5_BMP, x - 11, y - (235 - i));
								break;
							case 6:
								DisplayBMP(&Moon_6_BMP, x - 11, y - (235 - i));
								break;
							case 7:
								DisplayBMP(&Moon_7_BMP, x - 11, y - (235 - i));
								break;
							case 8:
								DisplayBMP(&Moon_8_BMP, x - 11, y - (235 - i));
								break;
							case 9:
								DisplayBMP(&Moon_9_BMP, x - 11, y - (235 - i));
								break;
							case 10:
								DisplayBMP(&Moon_10_BMP, x - 11, y - (235 - i));
								break;
							case 11:
								DisplayBMP(&Moon_11_BMP, x - 11, y - (235 - i));
								break;
							case 12:
								DisplayBMP(&Moon_12_BMP, x - 11, y - (235 - i));
								break;
							case 13:
								DisplayBMP(&Moon_13_BMP, x - 11, y - (235 - i));
								break;
							case 14:
								DisplayBMP(&Moon_14_BMP, x - 11, y - (235 - i));
								break;
							case 15:
								DisplayBMP(&Moon_15_BMP, x - 11, y - (235 - i));
								break;
							case 16:
								DisplayBMP(&Moon_16_BMP, x - 11, y - (235 - i));
								break;
							case 17:
								DisplayBMP(&Moon_17_BMP, x - 11, y - (235 - i));
								break;
							case 18:
								DisplayBMP(&Moon_18_BMP, x - 11, y - (235 - i));
								break;
							case 19:
								DisplayBMP(&Moon_19_BMP, x - 11, y - (235 - i));
								break;
							case 20:
								DisplayBMP(&Moon_20_BMP, x - 11, y - (235 - i));
								break;
							case 21:
								DisplayBMP(&Moon_21_BMP, x - 11, y - (235 - i));
								break;
							case 22:
								DisplayBMP(&Moon_22_BMP, x - 11, y - (235 - i));
								break;
							case 23:
								DisplayBMP(&Moon_23_BMP, x - 11, y - (235 - i));
								break;
							case 24:
								DisplayBMP(&Moon_24_BMP, x - 11, y - (235 - i));
								break;
							case 25:
								DisplayBMP(&Moon_25_BMP, x - 11, y - (235 - i));
								break;
							case 26:
								DisplayBMP(&Moon_26_BMP, x - 11, y - (235 - i));
								break;
							case 27:
								DisplayBMP(&Moon_27_BMP, x - 11, y - (235 - i));
								break;
							case 28:
								DisplayBMP(&Moon_28_BMP, x - 11, y - (235 - i));
								break;
							case 29:
								DisplayBMP(&Moon_29_BMP, x - 11, y - (235 - i));
								break;
						}
					}
					SelectColors(WHITE, BLACK);
					PrintStringLeft(str, x + 516, y - 298);

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
						if (uv >= 11) {
							sprintf(str, "Extreme");
						} else if (uv >= 8) {
							sprintf(str, "Very High");
						} else if (uv >= 6) {
							sprintf(str, "High");
						} else if (uv >= 3) {
							sprintf(str, "Moderate");
						} else {
							sprintf(str, "Low");
						}
						SelectColors(WHITE, BLACK);
						PrintStringRight(str, x + 889, y - 202);
					}

					// Set for next schedule point

					schedule_point = 5;

					break;
			}
		}
	}

   return 0;
}
