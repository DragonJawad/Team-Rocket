/**************************************\
 * File Name:      main.c
 * Project Name:   EECS373 Final Project
 * Created by:     Adrian Padin
 * Start date:     15 March 2016
 * Last modified:  22 March 2016
\**************************************/

#include <stdio.h>
#include <inttypes.h>
#include "drivers/mss_spi/mss_spi.h"
#include "drivers/mss_uart/mss_uart.h"
#include "drivers/mss_gpio/mss_gpio.h"
#include "ps2.h"

#define APB_BASE_ADDR 0x40050000

int main() {

	/************************************/
	/********** INITIALIZATION **********/
	/************************************/

	int i = 0;
	for (i = 0; i < 1000000; ++i);


	/********************************/
	/********** SETUP UART **********/
	/********************************/

	MSS_UART_init (
		&g_mss_uart1,
		MSS_UART_9600_BAUD,
		(MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT)
	);

	uint8_t tx_buff[2] = { 0, 0 };

	/********************************/
	/********** SETUP GPIO **********/
	/********************************/

	MSS_GPIO_init();

	MSS_GPIO_config
	(
		MSS_GPIO_15,
	    MSS_GPIO_OUTPUT_MODE
	);

	MSS_GPIO_set_output (MSS_GPIO_15,	1);





	/*******************************/
	/********** SETUP SPI **********/
	/*******************************/

	MSS_SPI_init( &g_mss_spi1 );
	MSS_SPI_configure_master_mode
	(
		&g_mss_spi1,
		MSS_SPI_SLAVE_1,
		MSS_SPI_MODE3,		  // Clock starts high, data read on rising edge, data changes on falling edge
		MSS_SPI_PCLK_DIV_256, // Clock period of 390 kHz - good enough!
		MSS_SPI_BLOCK_TRANSFER_FRAME_SIZE
	);

	MSS_SPI_set_slave_select( &g_mss_spi1, MSS_SPI_SLAVE_1 );

	// Initialize controllers here
	controller_t controller1;
	controller_init(&controller1, MSS_GPIO_15);
	digital_capture(&controller1);
	setup_all(&controller1);

	// Don't vibrate yet
	set_vibration(&controller1, 0, 0);



	/*******************************/
	/********** MAIN LOOP **********/
	/*******************************/

	for (i = 0; 1; ++i) {

		full_capture(&controller1);

		printf("\r\nResponses %d:\r\n", i);
		int j; for (j = 0; j < 18; ++j) {
			printf("Buffer[%d]: %d\r\n", j, (unsigned int) flip(controller1.slave_buffer[j]));
		}

		tx_buff[0] = controller1.slave_buffer[12];
		tx_buff[1] = controller1.slave_buffer[13];

		set_vibration(&controller1, 0xFF, 10);

		MSS_UART_polled_tx (
			&g_mss_uart1,
			tx_buff,
			sizeof(tx_buff)
		);


		// Delay
		j = 0; for (j = 0; j < 10000; ++j);
	}

	MSS_SPI_clear_slave_select( &g_mss_spi1, MSS_SPI_SLAVE_1 );
}
