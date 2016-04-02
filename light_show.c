// lightshow.c

#include "light_show.h"



void init_lights(void) {

    MSS_GPIO_init();

    MSS_GPIO_config(MSS_GPIO_0, MSS_GPIO_OUTPUT_MODE);
    MSS_GPIO_config(MSS_GPIO_1, MSS_GPIO_OUTPUT_MODE);
    MSS_GPIO_config(MSS_GPIO_2, MSS_GPIO_OUTPUT_MODE);
    MSS_GPIO_config(MSS_GPIO_3, MSS_GPIO_OUTPUT_MODE);
    MSS_GPIO_config(MSS_GPIO_4, MSS_GPIO_INPUT_MODE);

    MSS_GPIO_set_output(MSS_GPIO_0, 1);
    MSS_GPIO_set_output(MSS_GPIO_1, 1);
    MSS_GPIO_set_output(MSS_GPIO_2, 1);
    MSS_GPIO_set_output(MSS_GPIO_3, 1);
}

void light_show(mss_gpio_id_t gpio_id) {

	// Turn the pin off, wait a bit, turn it on
    MSS_GPIO_set_output(gpio_id, 0);
    int i = 0;
    while (i < 100000) ++i;
    MSS_GPIO_set_output(gpio_id, 1);
}
