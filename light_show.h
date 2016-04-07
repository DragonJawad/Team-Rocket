/**************************************\
 * File Name:      light_show.h
 * Project Name:   EECS373 Final Project
 * Created by:     Adrian Padin
 * Start date:     31 March 2016
 * Last modified:  4 April 2016
\**************************************/

#ifndef LIGHT_SHOW_H
#define LIGHT_SHOW_H

#include "drivers/mss_gpio/mss_gpio.h"
#include "ps2.h"


// For use in the light_show function
#define LIGHTS_OFF 0
#define LIGHTS_BLUE 1
#define LIGHTS_MAIZE 2
#define LIGHTS_START 3
#define LIGHTS_EGG1 5
#define LIGHTS_EGG2 6
#define LIGHTS_EGG3 7
#define LIGHTS_EGG4 8


#define NUM_EASTER_EGGS 2
#define MAX_EGGIE_LENGTH 5
#define NUM_GPIO_PINS 10

uint8_t eggies[NUM_EASTER_EGGS][MAX_EGGIE_LENGTH];
uint32_t GPIO_eggie[NUM_EASTER_EGGS];


// Initialize the light show (run only once)
void init_lights(void);

// Run a specific light show
void light_show(mss_gpio_id_t gpio_id);

void easter_eggie(controller_t * controller);

#endif /*LIGHT_SHOW_H*/
