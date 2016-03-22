// PS2 Header File
// ps2.h

#ifndef PS2_H
#define PS2_H

#include "inttypes.h"
#include "drivers/mss_spi/mss_spi.h"

/********** HELPER FUNCTIONS **********/

// Flip the bits of an 8-bit value
uint8_t flip(uint8_t value);


/********** COMMAND FUNCTIONS **********/

// Poll PS2 controller for digital button values
void simple_poll(uint8_t * slave_buffer);

// Poll PS2 controller for digital button values
void full_poll(uint8_t * slave_buffer);

// Enter configuration mode
void enter_config(uint8_t * slave_buffer);

// Exit configuration mode
void exit_config(uint8_t * slave_buffer);

// Turn on analog mode
void enable_analog(uint8_t * slave_buffer);

// Map motors to correct bytes
void motor_setup(uint8_t * slave_buffer);

// Setup analog buttons
void button_setup(uint8_t * slave_buffer);

#endif /* PS2_H */
