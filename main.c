/**************************************\
 * File Name:      main.c
 * Project Name:   EECS373 Project1
 * Created by:     Adrian Padin
 * Start date:     15 March 2016
 * Last modified:  15 March 2016
\**************************************/

#include <stdio.h>
#include <inttypes.h>
#include "drivers/mss_spi/mss_spi.h"
#include "drivers/mss_uart/mss_uart.h"
#include "drivers/mss_gpio/mss_gpio.h"
#include "ps2.h"

#define APB_BASE_ADDR 0x40050000

int main() {

	// Return values
	uint8_t slave_buffer[21];

	// Random delay
	int i = 0;
	for (i = 0; i < 1000000; ++i);


	/********** SETUP GPIO **********/
	MSS_GPIO_init();

	MSS_GPIO_config
	(
		MSS_GPIO_15,
	    MSS_GPIO_OUTPUT_MODE
	);

	MSS_GPIO_set_output (MSS_GPIO_15,	1);


	/********** SETUP SPI **********/

	MSS_SPI_init( &g_mss_spi1 );
	MSS_SPI_configure_master_mode
	(
		&g_mss_spi1,
		MSS_SPI_SLAVE_0,
		MSS_SPI_MODE3,		  // Clock starts high, data read on rising edge, data changes on falling edge
		MSS_SPI_PCLK_DIV_256, // Clock period of 390 kHz - good enough!
		MSS_SPI_BLOCK_TRANSFER_FRAME_SIZE
	);

	MSS_SPI_set_slave_select( &g_mss_spi1, MSS_SPI_SLAVE_0 );

	// Enable pressure sensitivity and motor mapping
	// Poll PS2 controller for all button values

	MSS_GPIO_set_output (MSS_GPIO_15,	0);
	simple_poll(slave_buffer);
	MSS_GPIO_set_output (MSS_GPIO_15,	1);

	for (i = 0; i < 1000; ++i);

	MSS_GPIO_set_output (MSS_GPIO_15,	0);
	enter_config(slave_buffer);
	MSS_GPIO_set_output (MSS_GPIO_15,	1);

	for (i = 0; i < 1000; ++i);

	MSS_GPIO_set_output (MSS_GPIO_15,	0);
	enable_analog(slave_buffer);
	MSS_GPIO_set_output (MSS_GPIO_15,	1);

	for (i = 0; i < 1000; ++i);

	MSS_GPIO_set_output (MSS_GPIO_15,	0);
	motor_setup(slave_buffer);
	MSS_GPIO_set_output (MSS_GPIO_15,	1);

	for (i = 0; i < 1000; ++i);

	MSS_GPIO_set_output (MSS_GPIO_15,	0);
	button_setup(slave_buffer);
	MSS_GPIO_set_output (MSS_GPIO_15,	1);

	for (i = 0; i < 1000; ++i);

	MSS_GPIO_set_output (MSS_GPIO_15,	0);
	exit_config(slave_buffer);
	MSS_GPIO_set_output (MSS_GPIO_15,	1);

	for (i = 0; i < 1000; ++i);


	/********** LOOP **********/

	for (i = 0; 1; ++i) {

		MSS_GPIO_set_output (MSS_GPIO_15,	0);
		full_poll(slave_buffer);
		MSS_GPIO_set_output (MSS_GPIO_15,	1);

		printf("\r\nResponses %d:\r\n", i);
		int j; for (j = 4; j < 8; ++j) {
			printf("Buffer[%d]: %d\r\n", j, (unsigned int) flip(slave_buffer[j]));
		}

		// Delay
		j = 0; for (j = 0; j < 100000; ++j);
	}

	MSS_SPI_clear_slave_select( &g_mss_spi1, MSS_SPI_SLAVE_0 );
}
