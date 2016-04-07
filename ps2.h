/**************************************\
 * File Name:      ps2.h
 * Project Name:   EECS373 Final Project
 * Created by:     Adrian Padin
 * Start date:     22 March 2016
 * Last modified:  4 April 2016
\**************************************/

#ifndef PS2_H
#define PS2_H

#include <inttypes.h>
#include "drivers/mss_spi/mss_spi.h"
#include "light_show.h"

#define MAX_BUFFER_SIZE 21
#define BUFFER_SIZE 16




/********** CONTROLLER STRUCT **********/

/* 	slave_buffer Data in Analog Mode:
*	[0] = Right Analog Stick X
*	[1] = Right Analog Stick Y
*	[2] = Left Analog Stick X
*	[3] = Left Analog Stick Y
*	[4] = Right Button Analog
*	[5] = Left Button Analog
*	[6] = Up Button Analog
*	[7] = Down Button Analog
*	[8] = Triangle Button Analog
*	[9] = Circle Button Analog
*	[10] = X Button Analog
*	[11] = Square Button Analog
*	[12] = L1 Button Analog
*	[13] = R1 Button Analog
*	[14] = L2 Button Analog
*	[15] = R2 Button Analog
*/

typedef struct controller {
	uint8_t select;		// Controller ID
	uint8_t vibration;
	uint8_t slave_buffer[MAX_BUFFER_SIZE];
	uint8_t state[NUM_EASTER_EGGS];
} controller_t;


// Initialize the controller struct
void controller_init(controller_t * controller, uint8_t select);


/********** HELPER FUNCTIONS **********/

// Flip the bits of an 8-bit value
uint8_t flip(uint8_t value);


/********** COMMAND FUNCTIONS **********/

// Poll for digital button values
void digital_capture(controller_t * controller);

// Poll for all values
void full_capture(controller_t * controller);

// Perform all setup tasks needed to read analog buttons and control motors
void setup_all(controller_t * controller);

// Set the controller to vibrate for a certain amount of time
void set_vibration(controller_t * controller, uint8_t vibration);



#endif /* PS2_H */

