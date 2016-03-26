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
#include "pwm.h"


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

	/********************************/
	/********** SETUP GPIO **********/
	/********************************/

	/*
	MSS_GPIO_init();

	MSS_GPIO_config
	(
		MSS_GPIO_15,
	    MSS_GPIO_OUTPUT_MODE
	);

	MSS_GPIO_set_output (MSS_GPIO_15,	1);
	*/

	/***************************************/
	/********** SETUP CONTROLLERS **********/
	/***************************************/

	MSS_SPI_init( &g_mss_spi1 );

	// Initialize controllers here
	controller_t controller1;
	//controller_t controller2;
	//controller_t controller3;
	//controller_t controller4;

	controller_init(&controller1, MSS_SPI_SLAVE_1);
	//controller_init(&controller2, MSS_SPI_SLAVE_2);
	//controller_init(&controller3, MSS_SPI_SLAVE_3);
	//controller_init(&controller4, MSS_SPI_SLAVE_4);

	setup_all(&controller1);
	//setup_all(&controller2);
	//setup_all(&controller3);
	//setup_all(&controller4);


	/*******************************/
	/********** MAIN LOOP **********/
	/*******************************/

	for (i = 0; 1; ++i) {


		/********** CAPTURE CONTROLLER DATA ********/

		full_capture(&controller1);
		//full_capture(&controller2);
		//full_capture(&controller3);
		//full_capture(&controller4);


		printf("\r\nResponses %d:\r\n", i);
		int j; for (j = 0; j < 16; ++j) {
			printf("Buffer[%d]: %d\r\n", j, (unsigned int) flip(controller1.slave_buffer[j]));
		}



		/********** SEND DATA TO CARS ********/

		uint8_t tx_buff[2] = { flip(controller1.slave_buffer[1]), flip(controller1.slave_buffer[3]) };
		tx_buff[1] |= 0x01;
		tx_buff[0] &= 0xFE;
		printf("sending: %x, %x", tx_buff[0], tx_buff[1]);
		MSS_UART_polled_tx(
			&g_mss_uart1,
			tx_buff,
			2 // number of bytes to send
		);


		/*
		MSS_UART_polled_tx(
			&g_mss_uart1,
			controller1.slave_buffer + 12,
			2 // number of bytes to send
		);

		MSS_UART_polled_tx(
			&g_mss_uart1,
			controller1.slave_buffer + 12,
			2 // number of bytes to send
		);

		MSS_UART_polled_tx(
			&g_mss_uart1,
			controller1.slave_buffer + 12,
			2 // number of bytes to send
		);
		 */

		/********** RECEIVE DATA FROM CARS **********/

		/********** CHANGE VIBRATION DATA **********/

		//set_vibration(&controller1, 0xFF, 10);
		//set_vibration(&controller2, 0xFF, 10);
		//set_vibration(&controller3, 0xFF, 10);
		//set_vibration(&controller4, 0xFF, 10);




		j = 0; for (j = 0; j < 10000; ++j); // Delay
	}

	// If you've made it this far, something went wrong
	return(1);
}
