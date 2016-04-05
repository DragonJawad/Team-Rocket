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

#define MAX_BUFFER_SIZE 21
#define BUFFER_SIZE 16


/********** CONTROLLER STRUCT **********/

typedef struct controller {
	uint8_t select;		// Controller ID
	uint8_t vibration;
	uint8_t slave_buffer[MAX_BUFFER_SIZE];
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
