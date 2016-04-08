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
#include "light_show.h"
#include "linked_list.h"
#include <stdlib.h>

/********** GLOBAL VARIABLES **********/

// Global controller structs
controller_t controller1;
controller_t controller2;
//controller_t controller3;
//controller_t controller4;

int counter = 0;


/********** INTERRUPT HANDLERS **********/

// UART RX Interrupt Handler
void xbee_receive_data( mss_uart_instance_t * this_uart ){

    uint8_t rx_buff[4]; // keep the data buffer aligned

    // Read in data from rx_buff
	int num_bytes = MSS_UART_get_rx( this_uart, rx_buff, 4 );
	assert(num_bytes == 4);

	int car_select = rx_buff[0];
	int vibration = rx_buff[1];

	// Set the vibration value
	switch (car_select) {
		case 1: set_vibration(&controller1, vibration); break;
		case 2: set_vibration(&controller2, vibration); break;
		//case 3: set_vibration(&controller3, vibration, 100); break;
		//case 4: set_vibration(&controller4, vibration, 100); break;
		default: break;
	}
	
	counter = 0;
	printf("Got some XBEE data: car %x, value %x\r\n", car_select, vibration);
}

// Send data to the cars (not an irq but may become one later)
void send_data_to_car(controller_t * controller) {
	
	// Data sent to cars
	uint8_t tx_buff[4] = { 	controller->select,
							flip(controller->slave_buffer[3]),
							flip(controller->slave_buffer[1]),
							0 };

	// Debugging
	//printf("sending: %x, %x, %x\r\n", tx_buff[0], tx_buff[1], tx_buff[2]);

    MSS_UART_polled_tx(&g_mss_uart1, tx_buff, 4);
}

void goal_scored() {
	
    // Pause timer?

    // team's light show
    light_show(LIGHTS_BLUE);
	
    // Update score

    // Display score

    // Vibrate all controllers on and off four times
    int i = 0;
    for (i = 0; i < 4; ++i) {
	
        set_vibration(&controller1, 0xff);
        set_vibration(&controller2, 0xff);
        set_vibration(&controller3, 0xff);
        set_vibration(&controller4, 0xff);

        int j = 0;
        for (j = 0; j < 1000; j++);
	
        set_vibration(&controller1, 0);
        set_vibration(&controller2, 0);
        set_vibration(&controller3, 0);
        set_vibration(&controller4, 0);

        for (j = 0; j < 1000; j++);
    }

    // Wait for all controllers to press and hold X
    while ( controller1.slave_buffer[10] < flip(10) &&
            controller2.slave_buffer[10] < flip(10) &&
            controller3.slave_buffer[10] < flip(10) &&
            controller4.slave_buffer[10] < flip(10)) {
	
        full_capture(&controller1);
        full_capture(&controller2);
        full_capture(&controller3);
        full_capture(&controller4);
    }

    // Start of game light show
    light_show(LIGHTS_START);
	
    // Drop the ball (pwn signal)
    // drop_ball();
	
    // Return to star the game
    return;
}

int main() {

    /********** INITIALIZATION **********/

    int i = 0, j = 0;
    MSS_GPIO_set_output(5, 0);
    MSS_GPIO_set_output(6, 0);
    MSS_GPIO_set_output(7, 0);
    MSS_GPIO_set_output(8, 0);
    /********** SETUP UART **********/

    MSS_UART_init (
        &g_mss_uart1,
        MSS_UART_57600_BAUD,
        (MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT)
    );

    // Interrupt handler for incoming XBee data
    MSS_UART_set_rx_handler
    (
        &g_mss_uart1,
        xbee_receive_data,
        MSS_UART_FIFO_FOUR_BYTES
    );


    /********** SETUP LIGHT SHOW **********/

    init_lights();


    /********** SETUP CONTROLLERS **********/

    MSS_SPI_init( &g_mss_spi1 );

    controller_init(&controller1, MSS_SPI_SLAVE_1);
    controller_init(&controller2, MSS_SPI_SLAVE_2);
    //controller_init(&controller3, MSS_SPI_SLAVE_3);
    //controller_init(&controller4, MSS_SPI_SLAVE_4);

    setup_all(&controller1);
    setup_all(&controller2);
    //setup_all(&controller3);
    //setup_all(&controller4);

	
    /********** LOOP **********/

    for (i = 0; 1; ++i) {

        /********** CAPTURE CONTROLLER DATA ********/
        full_capture(&controller1);
        full_capture(&controller2);
        //full_capture(&controller3);
        //full_capture(&controller4);


        //printf("\r\nResponses %d:\r\n", i);
        //for (j = 0; j < 5; ++j) {
        //    printf("Buffer[%d]: %d\r\n", j, (unsigned int) flip(controller1.slave_buffer[j]));
        //}


        /********** SEND DATA TO CARS ********/
        send_data_to_car(&controller1);
        send_data_to_car(&controller2);
        //send_data_to_car(&controller3);
        //send_data_to_car(&controller4);

		// Vibration data is updated automagically by interrupts

		
		// Testing out the light shows
		/*
		if (controller1.slave_buffer[10] > 0) {
			light_show(LIGHTS_BLUE);
		}
		else if (controller1.slave_buffer[9] > 0) {
			light_show(LIGHTS_MAIZE);
		}
		else if (controller1.slave_buffer[8] > 0) {
			light_show(LIGHTS_START);
		}
		else {
			light_show(LIGHTS_OFF);
		}
		//*/


        /********** EGGIES!!! ***********/
         // EGGIE INITS ///
        uint8_t light_show_end=1;
        uint32_t gpio_inputs=0;

        head->next=NULL;
        head->type=LIGHTS_OFF;

        /* Current Easter Egg Codes:

		1.	Triangle -> Circle -> X -> Square -> Triangle
		2.	Right -> Down -> Left -> Up -> Right
		3.	
		4.	

		*/

        easter_eggie(&controller1);
        easter_eggie(&controller2);
        //easter_eggie(&controller3);
        //easter_eggie(&controller4);
        gpio_inputs = MSS_GPIO_get_inputs();

        if(gpio_inputs == 0x10) { //GPIO 4??
            light_show_end=1;
        }

        else {
            light_show_end=0;
        }
        /*while(list not empty && light_show_end
            keep popping off light shows
            and call light_show functions
            light show function gets called in pop
        */
       show_t * current = head;
     while((current != NULL) && light_show_end){
            pop(head);
            current=current->next;
        }


		/********** CHECK SCORING STATUS **********/


        //set_vibration(&controller1,0xFF);
        //set_vibration(&controller2,0xFF);
		
		if (controller1.slave_buffer[10] > 0) {
			set_vibration(&controller1, 0xff);
			counter = 0;
		}
		
		//*
        for (j = 0; j < 10000; ++j); // Delay
        counter++;
        int x = counter;

        ///*
        if (counter > 50) {
        	set_vibration(&controller1, 0);
        	set_vibration(&controller2, 0);
        }
        //*/
    }

    // If you've made it this far, something went wrong
    return(1);
}
