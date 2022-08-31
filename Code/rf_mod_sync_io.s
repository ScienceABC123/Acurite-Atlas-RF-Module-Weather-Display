/*******************************************************************************
*
* Accurite 433MHz RF Module Synchronous I/O
*
* Note: Uses WiringPi
*
* Copyright (c) 2022
*
*******************************************************************************/

/*******************************************************************************
*
* Revision History
* ----------------------------------------------------
* 1.0	5/16/22		PMW	Initial creation begun
*	5/24/22		PMW	Test and debug completed
*
*******************************************************************************/

// Functions currently supported

		.global	RF_Mod_Init		// RF_Mod_Init(pb_addr)

		.global	RF_Rd_Pkt		// RF_Rd_Pkt(pkt_type)

// External functions called

		.extern	wiringPiSetupGpio	// WiringPi initialization
		.extern	pinMode			// WiringPi set GPIO pin mode
		.extern	digitalWrite		// WiringPi write GPIO output pin state
		.extern	digitalRead		// WiringPi read GPIO input pin state

		.extern	sleep			// System sleep
		.extern	usleep			// System microsecond sleep

// Equates (i.e. defines)

		.equ	NULL, 0

		.equ	GPIO4, 4
		.equ	GPIO17, 17
		.equ	GPIO22, 22
		.equ	GPIO27, 27

		.equ	SCLK_IN, GPIO4
		.equ	SDAT_IN, GPIO17
		.equ	SCLK_OUT, GPIO27
		.equ	SDAT_OUT, GPIO22

		.equ	INPUT, 0
		.equ	OUTPUT, 1

		.equ	LOW, 0
		.equ	HIGH, 1

		.equ	TIMEOUT, 100000

// Variable Section

		.data

		.align	2
		.type	PB_ADDR, %object
		.size	PB_ADDR, 4
PB_ADDR:	.space	4

// Code Section

		.text

		.align	2
		.arch	armv8-a
		.syntax	unified
		.arm
		.fpu	vfp

// Functions

/*******************************************************************************
*
* NOTE: THIS FUNCTION MUST BE CALLED FIRST!
*
* RF_Mod_Init - Saves the packet buffer address for future read packet function
* calls, and initializes the Raspberry Pi GPIO for reading packets from the RF
* module.
*
* Raspberry Pi GPIO is configured as follows:
*  RPi		RF module
*  ------	---------
*  GPIO4	sclk_in
*  GPIO17	sdat_in
*  GPIO27	sclk_out
*  GPIO22	sdat_out
*
* Note: An input to the RF module is an output from the Raspeberry Pi, and an
* output from the RF module is an input to the Raspeberry Pi.
*
* RF_Mod_Init(pb_addr)
*  parameters:
*    r0: pb_addr			// packet buffer address (27 bytes)
*  return code:
*    r0: 0				// no error
*	 <>0				// WiringPi initialization error code
*
*******************************************************************************/

		.type	RF_Mod_Init, %function

RF_Mod_Init:

		push	{lr}			// Push lr on stack

		movw	r3, #:lower16:PB_ADDR	// Save packet buffer address
		movt	r3, #:upper16:PB_ADDR
		str	r0, [r3]

		bl	wiringPiSetupGpio	// Call WiringPi initialization
		orrs	r0, r0			// Check for WiringPi initialization failed
		bne	rfmodinit1		// Branch if WiringPi initialization failed

// Configure SCLK_IN as an output, and set it low

		mov	r0, #SCLK_IN		// Configure SCLK_IN as an output
		mov	r1, #OUTPUT
		bl	pinMode

		mov	r0, #SCLK_IN		// Set SCLK_IN low
		mov	r1, #LOW
		bl	digitalWrite

// Configure SDAT_IN as an output, and set it low

		mov	r0, #SDAT_IN		// Configure SDAT_IN as an output
		mov	r1, #OUTPUT
		bl	pinMode

		mov	r0, #SDAT_IN		// Set SDAT_IN low
		mov	r1, #LOW
		bl	digitalWrite

		mov	r0, #1			// Delay for 1S to force outputs low
		bl	sleep

// Configure SCLK_OUT as an input

		mov	r0, #SCLK_OUT		// Configure SCLK_OUT as an input
		mov	r1, #INPUT
		bl	pinMode

// Configure SDAT_OUT as an input

		mov	r0, #SDAT_OUT		// Configure SDAT_OUT as an input
		mov	r1, #INPUT
		bl	pinMode

		mov	r0, #0			// No error

rfmodinit1:

		pop	{lr}			// Pop lr off stack
		bx	lr			// Return

/*******************************************************************************
*
* RF_Rd_Pkt - Reads a particluar 27 byte packet type from the RF module, and
* saves it to the packet buffer.
*
* RF_Rd_Pkt(pkt_type)
*  parameters:
*    r0: pkt_type			// packet type (0x03, 0x10, 0x01, etc.)
*  return code:
*    r0: 0				// no error
*
*******************************************************************************/

		.type	RF_Rd_Pkt, %function

RF_Rd_Pkt:

		push	{r4-r11, lr}		// Push r4-r11 and lr on stack

// Format the 64-bit address for the RF module (r4 = 0xnn000000, r5 = 0x000000nn)

		and	r5, r0, #255		// Mask-off the upper 24-bits of the packet type to form the lower 32-bits
		lsl	r4, r5, #24		// Shift the packet type 24-bits to form the upper 32-bits

// Send the 64-bit address to the RF module

// Send the 32-bits of the upper 64-bit address

		mov	r6, #32			// Set bit counter for the remaining 31-bits of the upper 64-bit address

rfrdpkt1:

		lsls	r4, r4, #1		// Shift the next address bit into carry
		movcc	r1, #LOW		// Set SDAT_IN low if carry clear
		movcs	r1, #HIGH		// Set SDAT_IN high if carry set
		mov	r0, #SDAT_IN
		bl	digitalWrite

		mov	r0, #SCLK_IN		// Set SCLK_IN high
		mov	r1, #HIGH
		bl	digitalWrite

		mov	r0, #25			// Delay 100uS
		bl	usleep

		mov	r0, #SCLK_IN		// Set SCLK_IN low
		mov	r1, #LOW
		bl	digitalWrite

		mov	r0, #25			// Delay 100uS
		bl	usleep

		subs	r6, r6, #1		// Decrement bit counter
		bgt	rfrdpkt1		// Branch if bit counter > 0

// Send the 32-bits of the lower 64-bit address

		mov	r6, #32			// Set bit counter for the 32-bits of the lower 64-bit address

rfrdpkt2:

		lsls	r5, r5, #1		// Shift the next address bit into carry
		movcc	r1, #LOW		// Set SDAT_IN low if carry clear
		movcs	r1, #HIGH		// Set SDAT_IN high if carry set
		mov	r0, #SDAT_IN
		bl	digitalWrite

		mov	r0, #SCLK_IN		// Set SCLK_IN high
		mov	r1, #HIGH
		bl	digitalWrite

		mov	r0, #25			// Delay 100uS
		bl	usleep

		mov	r0, #SCLK_IN		// Set SCLK_IN low
		mov	r1, #LOW
		bl	digitalWrite

		mov	r0, #25			// Delay 100uS
		bl	usleep

		subs	r6, r6, #1		// Decrement bit counter
		bgt	rfrdpkt2		// Branch if bit counter > 0

		mov	r1, #LOW		// Set SDAT_IN low
		mov	r8, r1			// Save SDAT_IN value for toggling latter
		mov	r0, #SDAT_IN
		bl	digitalWrite

// Setup to read the 27-byte packet from the RF module

		movw	r4, #:lower16:PB_ADDR	// Get packet buffer address
		movt	r4, #:upper16:PB_ADDR
		ldr	r4, [r4]

		mov	r5, #27			// Set byte counter to 27

// Read the 27-byte packet from the RF module

rfrdpkt3:

		mov	r6, #8			// Set bit counter to 8

		movw	r7, #:lower16:TIMEOUT	// Set timeout counter to
		movt	r7, #:upper16:TIMEOUT

// Wait for SCLK_OUT high

rfrdpkt4:

		subs	r7, r7, #1		// Decrement timeout counter
		beq	rfrdpkt8		// Branch if timeout counter = 0

		mov	r0, #SCLK_OUT		// Get SCLK_OUT value
		bl	digitalRead

		cmp	r0, #HIGH		// Check for SCLK_OUT high
		bne	 rfrdpkt4		// Branch if SCLK_OUT is not high

// Wait for SCLK_OUT low

rfrdpkt5:

		subs	r7, r7, #1		// Decrement timeout counter
		beq	rfrdpkt8		// Branch if timeout counter = 0

		mov	r0, #SCLK_OUT		// Get SCLK_OUT value
		bl	digitalRead

		cmp	r0, #LOW		// Check for SCLK_OUT low
		bne	 rfrdpkt5		// Branch if SCLK_OUT is not low

// Get data bit from RF module

		mov	r0, #SDAT_OUT		// Get SDAT_OUT value (i.e. data bit)
		bl	digitalRead

// Save data bit in data byte

		and	r0, r0, #1		// Mask-off everything except the data bit
		lsl	r9, r9, #1		// Shift the data byte by one bit
		orr	r9, r9, r0		// Or data bit into data byte

// Toggle SDAT_IN

		eor	r8, r8, #1		// Toggle SDAT_IN value
		mov	r1, r8			// Toggle SDAT_IN
		mov	r0, #SDAT_IN
		bl	digitalWrite

		subs	r6, r6, #1		// Decrement bit counter
		bgt	rfrdpkt4		// Branch if bit counter > 0

// Save the data byte in the packet buffer

		strb	r9, [r4], #1		// Save the data byte in the packet buffer and increment packet buffer pointer

		subs	r5, r5, #1		// Decrement byte counter
		bgt	rfrdpkt3		// Branch if byte counter > 0

		mov	r1, #LOW		// Set SDAT_IN low
		mov	r0, #SDAT_IN
		bl	digitalWrite

// Verify checksum (byte 26 = 8-bit sum of bytes 0-25)

		movw	r4, #:lower16:PB_ADDR	// Get packet buffer address
		movt	r4, #:upper16:PB_ADDR
		ldr	r4, [r4]

		mov	r5, #26			// Set byte counter to 26

		mov	r6, #0			// Set byte sum to 0

rfrdpkt6:

		ldrb	r7, [r4], #1		// Get byte and increment packet buffer pointer
		add	r6, r6, r7		// Add byte to byte sum

		subs	r5, r5, #1		// Decrement byte counter
		bgt	rfrdpkt6		// Branch if byte counter > 0

		and	r6, r6, #255		// Mask off byte sum bits 8-31

		ldrb	r7, [r4]		// Get checksum byte
		cmp	r6, r7			// Check byte sum against checksum byte
		bne	rfrdpkt9		// Branch if byte sum isn't same as checksum byte

// Verify the packet isn't empty

		movw	r4, #:lower16:PB_ADDR	// Get packet buffer address
		movt	r4, #:upper16:PB_ADDR
		ldr	r4, [r4]
		add	r4, r4, #1		// Increment packet buffer address

		mov	r5, #24			// Set byte counter to 24

		mov	r6, #0			// Set byte sum to 0

rfrdpkt7:

		ldrb	r7, [r4], #1		// Get byte and increment packet buffer pointer
		add	r6, r6, r7		// Add byte to byte sum

		subs	r5, r5, #1		// Decrement byte counter
		bgt	rfrdpkt7		// Branch if byte counter > 0

		cmp	r6, #0			// Check if all bytes are zero
		beq	rfrdpkt9		// Branch if packet is empty

		mov	r0, #0			// No error
		bal	rfrdpkt10

rfrdpkt8:

		mov	r1, #LOW		// Set SDAT_IN low
		mov	r0, #SDAT_IN
		bl	digitalWrite

rfrdpkt9:

		mov	r0, #-1			// Error

rfrdpkt10:

		pop	{r4-r11, lr}		// Pop r4-r11 and lr off stack
		bx	lr			// Return

		.end
