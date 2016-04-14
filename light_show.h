/**************************************\
 * File Name:      light_show.h
 * Project Name:   EECS373 Final Project
 * Created by:     Adrian Padin
 * Start date:     31 March 2016
 * Last modified:  4 April 2016
\**************************************/

#ifndef LIGHT_SHOW_H_
#define LIGHT_SHOW_H_

#include "ps2.h"
#include "defines.h"
#include "drivers/mss_gpio/mss_gpio.h"


void easter_eggie(controller_t * controller);

extern uint8_t eggies[NUM_EASTER_EGGS][MAX_EGGIE_LENGTH+1];
extern uint32_t GPIO_eggie[NUM_EASTER_EGGS];

// Initialize the light show (run only once)
void init_lights(void);

// Run a specific light show
void light_show(mss_gpio_id_t gpio_id);

// Turn off all lights
void turn_off_lights();

// Initialize the easter egg code (run only once)
void init_easter_eggie(uint8_t * press_sequence, uint8_t num_eggie, uint8_t eggie_length);



#endif
