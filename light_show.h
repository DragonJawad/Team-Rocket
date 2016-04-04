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

// For use in the light_show function
#define LIGHTS_OFF 0
#define LIGHTS_BLUE 1
#define LIGHTS_MAIZE 2
#define LIGHTS_START 3
//#define LIGHTS_EASTER_EGG 5

// Initialize the light show (run only once)
void init_lights(void);

// Run a specific light show
void light_show(mss_gpio_id_t gpio_id);

#endif /*LIGHT_SHOW_H*/
