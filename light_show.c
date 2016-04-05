/**************************************\
 * File Name:      light_show.c
 * Project Name:   EECS373 Final Project
 * Created by:     Adrian Padin
 * Start date:     31 March 2016
 * Last modified:  4 April 2016
\**************************************/

#include "light_show.h"

// Initialize the light show (run only once)
void init_lights(void) {

    MSS_GPIO_init();

	// Initialize all possible light shows as outputs
    MSS_GPIO_config(LIGHTS_OFF, MSS_GPIO_OUTPUT_MODE);
    MSS_GPIO_config(LIGHTS_BLUE, MSS_GPIO_OUTPUT_MODE);
    MSS_GPIO_config(LIGHTS_MAIZE, MSS_GPIO_OUTPUT_MODE);
    MSS_GPIO_config(LIGHTS_START, MSS_GPIO_OUTPUT_MODE);
	
	// Initalize return input
    MSS_GPIO_config(MSS_GPIO_4, MSS_GPIO_INPUT_MODE);

	// All outputs are active low/passive high
    MSS_GPIO_set_output(LIGHTS_OFF, 1);
    MSS_GPIO_set_output(LIGHTS_BLUE, 1);
    MSS_GPIO_set_output(LIGHTS_MAIZE, 1);
    MSS_GPIO_set_output(LIGHTS_START, 1);
}

// Run a specific light show
void light_show(mss_gpio_id_t gpio_id) {

	// Turn the pin off, wait a bit, turn it on
    MSS_GPIO_set_output(gpio_id, 0);
    int i = 0;
    while (i < 100000) ++i;
    MSS_GPIO_set_output(gpio_id, 1);
}
