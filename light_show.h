// ps2.h

#ifndef LIGHT_SHOW_H
#define LIGHT_SHOW_H


#include "drivers/mss_gpio/mss_gpio.h"


void init_lights(void);

void light_show(mss_gpio_id_t gpio_id);

#endif /*LIGHT_SHOW_H*/
