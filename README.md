# Acurite-Atlas-RF-Module-Weather-Display
Large weather display using the RF module from an Acurite HD display.
A little bit of background...

I bought an Acurite Atlas with HD Display about 5 years ago, and everything worked great. In September of 2020 a thunderstorm came through one morning and there was a power surge, afterwards the HD Display displayed the boot screen, and never changed. To me this indicated the HD Display had a corrupted application software load. I contacted Acurite about getting it reprogrammed and their response was – “Go buy another HD Display.” After regaining my composure, I realized I had no other choice but to go buy another HD Display for ~$100.

I kept the old HD Display and put it on my “to do list” to disassemble it and see what I could learn from it. What I eventually discovered in early 2022 was a main MIPS CPU board and an RF receiver module connected to it with a 6-wire ribbon cable (see picture “Inside of Acurite HD Display.jpg”). I realized I couldn’t do anything with the CPU board, but the RF receiver module was an entirely different matter. The 6-wire connections were conveniently labeled: GND, VCC, in_clk, in_data, out_data, out_clk. I immediately realized I could “bit-bang” this synchronous interface on a Raspberry Pi GPIO, and so I began this project.

Side Note: The 4-wire ribbon cable on the left side of the RF module connects to a small circuit board that contains the indoor temperature and humidity sensor. This little circuit board was actually manufactured attached to the RF module, but during production is detached and relocated so that the heat produced by the HD Display doesn’t affect the indoor temperature reading, much. As you can see in one of the pictures, I reattached this small circuit board to the RF module, but I never use the data from it in weather.c.

I was determined to use the scavenged RF receiver module to come up with a display that was larger, easier to read, and had much better graphs of the weather data.

The first step was to determine the interface protocol between the CPU and RF receiver. I asked Acurite if they would provide that info and they refused, even after I offered to sign an NDA to get it. So I attached my logic analyzer to the interface on my good HD Display; Yes, I know that voided the warranty (see pictures “Logic analyzer attached to good Acurite HD Display.jpg” and “Logic analyzer sampling data from Acurite HD Display.jpg”). What I discovered was a “unique” synchronous interface implementing 27-byte data packet reads. The protocol is a 64-bit address field (200uS clock period) sent to the RF receiver module, with the packet type (8-bits) repeated in the first and last 8-bits, and all others bits zero. The RF receiver module responds with a 27-byte packet (600uS clock period), with the first byte being the packet type, 24-bytes of data, 1-byte packet counter (why?), and a simple 8-bit checksum. Interestingly, during the 27-byte transfer from the RF receiver module, the data line from the CPU has its state toggled for each bit (why?). I’ve scanned all 256 possible packet types, and the only valid packet types the RF receiver module responds to are: 0x01, 0x02, 0x03, 0x10, and 0x44.

Packet type 0x01 contains:
-	Inside temperature in both Celsius and Fahrenheit in BCD format
-	Inside humidity percent in unsigned character format

Packet type 0x02 contains:
-	Atmospheric pressure in both hPa and inHg in BCD format for both a reported value and what appears to be a secondary value (why?)

Packet type 0x03 contains:
-	Channel number
-	Battery low flag
-	Outside temperature in both Celsius and Fahrenheit in BCD format
-	Outside humidity percent in unsigned character format
-	Lightning strike count in unsigned character format
-	Lightning distance in both kilometers and miles in BCD format

Packet type 0x10 contains:
-	UV index in unsigned character format
-	Light (lux / 10) in unsigned 16-bit integer format
-	Wind speed in both kph and mph in unsigned character format
-	Wind direction degrees in BCD format
-	Rain in both mm and inches in BCD format

The 0x44 packet type only ever contains 0x44 in bytes 1 and 8, so I think it’s a programming bug in the RF receiver module.

The BCD data threw me as it wasn’t consistent with what I’ve seen from several other Raspberry Pi projects using generic 433MHz RF receiver modules. Clearly Acurite’s RF receiver module is doing some data conversion before passing the data to the CPU board. I don’t know what Acurite’s Access unit uses for its RF receiver module, but I suspect it’s the same one.

More detail on these packets can be found in an Excel spreadsheet (Data Analysis.xlsx) I made during the decoding period.

The second step was connecting the RF receiver module to the Raspberry Pi’s GPIO and bit-bang the interface. I soldered a 2mm 6-pin header onto the RF receiver module in place of the 6-wire flat cable, and made a 6” cable (100mil connector for RPi GPIO and 2mm connector for RF receiver module) to make it easier to connect it all together. This also made it easy for the finishing the project as I did development work on a Raspberry Pi 3B+ but finished on a Raspberry Pi Zero 2W. 

Not wanting to reinvent the wheel, I used WiringPi (ht Gordon) as my method for “bit-banging” the Raspberry Pi GPIO interface. I wrote an ARM assembly program (rf_mod_sync_io.s) that would initialize the GPIO interface and read a packet from the RF receiver module. In the initialization call (RF_Mod_Init), the C program passes the address of a 27-character array in r0. In the read call (RF_Rd_Pkt), the specific packet type to be read is passed in the lower byte of r0, the ARM assembly program reads the packet and returns the data in the 27-charater array specified in the initialization call. The ARM assembly program abides by all the “unique” Acurite timing/behavior observed on the logic analyzer. The ARM assembly program reports an error for the following conditions: 1) the RF receiver module is taking too long (i.e. time-out), 2) the checksum fails (byte 26 = 8-bit sum of bytes 0-25), or 3) the packet is empty (i.e. bytes 1-24 are all zeros).

NOTE: I discovered the system usleep function is a bit “inaccurate” for very small delays (~100uS). After much tweaking and verifying on the logic analyzer I found calling usleep with a value of 25 gives a ~100uS delay.

Actually getting the weather station data from the RF receiver module was the easy part and I was done in late June. I spent all of July and August just tinkering with weather.c on how I was going to use the data. I entertained several different appearances on my display (1920x1080), made some personal decisions on how I liked the look, and finally decided to stop changing it in late August.

I’m including everything I created during this project in this release (test/weather C programs, RF receiver module ARM assembly program, fonts, tidbits in text files, spreadsheets, etc.).

Other items needed for this project are:

WiringPi 
source:	 https://github.com/WiringPi/WiringPi

Simple ARM Graphics Library
source: https://github.com/ScienceABC123/Simple-ARM-Graphics-Library

And of course, an Acurite RF receiver module. I’m sure there must be a few Acurite HD displays with corrupted software loads floating around, but may be hard to find. As I said, I began this project only after my Acurite HD Display got corrupted.

I’m making everything public domain effective immediately.

Enjoy!
