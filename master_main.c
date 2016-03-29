/**************************************\
 * File Name:      main.c
 * Project Name:   EECS373 Final Project
 * Created by:     Adrian Padin
 * Start date:     15 March 2016
 * Last modified:  22 March 2016
\**************************************/

#include <stdio.h>
#include <inttypes.h>
#include <assert.h>

#include "drivers/mss_spi/mss_spi.h"
#include "drivers/mss_uart/mss_uart.h"
#include "drivers/mss_gpio/mss_gpio.h"
#include "ps2.h"

/********** GLOBAL VARIABLES **********/

uint8_t vibration[5] = {0, 0, 0, 0, 0}; // Holder for vibration data (1-indexed)


/********** INTERRUPT HANDLERS **********/

// UART RX Interrupt Handler
void uart1_rx_handler( mss_uart_instance_t * this_uart ){

    uint8_t rx_buff[4];

    // Read in data from rx_buff
    int num_bytes = MSS_UART_get_rx( this_uart, rx_buff, sizeof(rx_buff) );

	while (num_bytes > 0) {
		
		// Bytes should come in groups of four
		assert(num_bytes == 4);
	
		// First byte should be the ID of the car (1, 2, 3, or 4)
		int car_select = rx_buff[0];
		assert(car_select > 0 && car_select <= 4);

		// Second byte is the vibration value
		// Set vibration values accordingly
		vibration[car_select] = rx_buff[1];

		// Other two bytes are unused
		num_bytes = MSS_UART_get_rx( this_uart, rx_buff, sizeof(rx_buff) );
	}

    return;
}





int main() {

    /********** INITIALIZATION **********/

    int i = 0;
	

    /********** SETUP UART **********/

    // One of these will have to be used by the LCD and the other by the XBees
    /*
    MSS_UART_init (
        &g_mss_uart0,
        MSS_UART_9600_BAUD,
        (MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT)
    );
    */

    MSS_UART_init (
        &g_mss_uart1,
        MSS_UART_9600_BAUD,
        (MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT)
    );


    // Interrupt handler for incoming XBee data
    MSS_UART_set_rx_handler
    (
        &g_mss_uart1,
        uart1_rx_handler,
        MSS_UART_FIFO_FOUR_BYTES
    );


    /********** SETUP GPIO **********/

    /*
    MSS_GPIO_init();

    MSS_GPIO_config
    (
        MSS_GPIO_15,
        MSS_GPIO_OUTPUT_MODE
    );

    MSS_GPIO_set_output (MSS_GPIO_15,   1);
    */


    /********** SETUP CONTROLLERS **********/

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


    /********** DATA SENT TO CARS **********/

    uint8_t tx_buff_1[4] = { 1, 0, 0, 0 };
    //uint8_t tx_buff_2[4] = { 2, 0, 0, 0 };
    //uint8_t tx_buff_3[4] = { 3, 0, 0, 0 };
    //uint8_t tx_buff_4[4] = { 4, 0, 0, 0 };


    /********** LOOP **********/

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

        // Decide which data to send
        tx_buff_1[2] = flip(controller1.slave_buffer[12]);
        tx_buff_1[3] = flip(controller1.slave_buffer[13]);

        // Debugging
        printf("sending: %x, %x", tx_buff[0], tx_buff[1]);

        MSS_UART_polled_tx(&g_mss_uart1, tx_buff_1, sizeof(tx_buff_1));
        //MSS_UART_polled_tx(&g_mss_uart1, tx_buff_2, sizeof(tx_buff_2));
        //MSS_UART_polled_tx(&g_mss_uart1, tx_buff_3, sizeof(tx_buff_3));
        //MSS_UART_polled_tx(&g_mss_uart1, tx_buff_4, sizeof(tx_buff_4));


        /********** CHANGE VIBRATION DATA **********/

		// Data is updated automagically by interrupts
        set_vibration(&controller1, vibration[1], 100);
        //set_vibration(&controller2, vibration[2], 100);
        //set_vibration(&controller3, vibration[3], 100);
        //set_vibration(&controller4, vibration[4], 100);

		
		/********** CHECK SCORING STATUS **********/

        j = 0; for (j = 0; j < 10000; ++j); // Delay (can be shortened if necessary)
    }

    // If you've made it this far, something went wrong
    return(1);
}