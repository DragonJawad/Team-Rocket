
/********************************************\
 * File Name:      		main.c
 * Project Name:   		EECS373 Final Project
 * Created by:     		Jawad Nasser
 * Modified by:    		Adrian Padin
 * 			       		Emily Rowland
 * 				   		Ben Miron
 * Start date:     		23 March 2016
 * Last modified:  		28 March 2016
 * Last modified by:	Ben Miron
 \*******************************************/

#include <stdio.h>
#include <inttypes.h>
#include "drivers/mss_uart/mss_uart.h"
#include "drivers/mss_gpio/mss_gpio.h"
#include "pwm.h"
#include "drivers/mss_ace/mss_ace.h"

/************************************/
/******** Define and Globals ********/
/************************************/

// Define for the RX_BUFF for UART1
#define RX_BUFF_SIZE    4

// Define for the TX_BUFF for UART1
#define TX_BUFF_SIZE	4

// Define the identifier for this car
#define CAR_ID			0x01

// Define the address for GPIO
#define GPIO_ADDR		0x40050000

// Define thresholds to turn on the controller vibration
#define X_LOW			100
#define X_HIGH			220

#define Y_LOW			100
#define Y_HIGH			220


// UART1 buffer for input
uint8_t g_rx_buff[RX_BUFF_SIZE];

// Declare a global count
// This is used to stop the car if no data is received
uint32_t count = 0;

// UART1 buffer for output
uint8_t g_tx_buff[TX_BUFF_SIZE];

// Declare globals for the ADC
ace_channel_handle_t adc_handler2_x;
ace_channel_handle_t adc_handler3_y;


/* ***************************************************** */
/* ***************** Interrupt Handlers **************** */
/* ***************************************************** */


/* ************************************
*	UART RX Interrupt Handler
*	The values should be sent in the form
*	g_rx_buff[0] = CAR ID
*	g_rx_buff[1] = first side of the car (analog stick)
*	g_rx_buff[2] = second side of the car (analog stick)
*
*	Data should be sent back in the form
*	g_tx_buff[0] = CAR ID
*	g_tx_buff[1] = Accel data
************************************ */

// Order of bytes:
// Byte 1: Car ID
// Byte 2: X Data
// Byte 3: Left Data
// Byte 4: Right Data
void uart1_rx_handler( mss_uart_instance_t * this_uart ) {

	uint8_t rx_buff[4];
	int num_bytes = MSS_UART_get_rx(this_uart, rx_buff, 4);

	if (num_bytes != 4) return; // transmission failed
	if (rx_buff[0] != CAR_ID) return; // wrong car

	int left = rx_buff[2], right = rx_buff[3];


	// Percentage of pwm on each side
	int left_scale = 100, right_scale = 100;

	// Turn if one of the two is non-zero and the other is zero
	if (left > 0 && right == 0) {
		left_scale = ((255 - (int) left) * 100) / 255;
	}

	if (left == 0 && right > 0) {
		right_scale = ((255 - (int) right) * 100) / 255;
	}

	// Calculate left and right pwm values by scaling from X input
	// Moving slower (lower pwm) on one side will cause the car
	// to turn more in that direction
	left = ((int) rx_buff[1] * left_scale) / 100;
	right = ((int) rx_buff[1] * right_scale) / 100;

	// Set values
	setPWMDuty((left / 2 + 128) & 0xFE);
	setPWMDuty((right / 2 + 128) | 0x01);
	setHBridgeInputs((left / 2 + 128) & 0xFE);
	setHBridgeInputs((right / 2 + 128) | 0x01);


	// Add in H-bridge stuff for going backwards
	// Also add adc and vibration stuff
}


/*
void uart1_rx_handler( mss_uart_instance_t * this_uart ){
	int data_received = 0;

	// data_received = the number of bytes that were saved into g_rx_buff
	// This value should be 2, since we are receiving 2 bytes of data at a time
    data_received = MSS_UART_get_rx( this_uart, g_rx_buff, RX_BUFF_SIZE );

    // If data_received is not 8, we received the incorrect bytes
    if (data_received != RX_BUFF_SIZE){
    	printf("Received incorrect number of bytes. %d\r\n", (unsigned int)data_received);
    }


    // Otherwise, the data was received correctly
    // Now, check if the first byte is the correct identifier (CAR 1)
    else if(g_rx_buff[0] != CAR_ID){
    	printf("Different car\r\n");
    }

    // Otherwise, it should be this car
    else if(g_rx_buff[0] == CAR_ID){

    	// Print the data that has been received
    	printf("\nData received! %x\r\n", g_rx_buff[1]);
    	printf("Second byte received: %x\r\n", g_rx_buff[2]);


    	setPWMDuty(g_rx_buff[1]);
    	setHBridgeInputs(g_rx_buff[1]);

    	setPWMDuty(g_rx_buff[2]);
    	setHBridgeInputs(g_rx_buff[2]);

    	// Testing crap
    	if(g_rx_buff[1] == 128) {
    		MSS_GPIO_set_output(0, 1);
    	}
    	else {
    		MSS_GPIO_set_output(0, 0);
    	}
    }

    else{
    	printf("Made it to the end of the interrupt handler\r\n");
    }

    count = 0;

    // Take ADC inputs here
	// X is connected to ADC2, Y to ADC3
	uint16_t adcx, adcy;
	uint8_t tx_output = 0;
	adcx = ACE_get_ppe_sample(adc_handler2_x);
	adcy = ACE_get_ppe_sample(adc_handler3_y);

	// Shift over the first 4 useless bits
	adcx = adcx>>4;
	adcy = adcy>>4;

	printf("x: %u | y: %u\r\n", adcx, adcy);

	// If both X and Y are valid, OR the two values into the output
	if((adcy > Y_HIGH || adcy < Y_LOW) && (adcx > X_HIGH || adcx < X_LOW)){
		tx_output = adcx | adcy;
	}
	// Else if X is valid, that should be the output
	else if(adcx > X_HIGH || adcx < X_LOW) {
		tx_output = adcx;
	}
	// Else if Y is valid, that should be the output
	else if(adcy > Y_HIGH || adcy < Y_LOW){
		tx_output = adcy;
	}

	// Finally, only send data if there's any valid output
	if(tx_output != 0) {
		// Set the tx buff
		g_tx_buff[0] = CAR_ID;
		g_tx_buff[1] = tx_output;

		// Send out the accelerometer data
		MSS_UART_polled_tx(&g_mss_uart1, g_tx_buff, sizeof(g_tx_buff));
	}
}
*/


/* ***************************************************** */
/* ********************** MAIN ************************* */
/* ***************************************************** */

int main() {

	/* ******************************** */
	/* ******** INITIALIZATION ******** */
	/* ******************************** */

	// printf("\r\n\r\nHBridge: %x\r\n", (int) getHBridgeInputs());
	initHBridge();
	initPWM();

	// Init the ADC for the accelerometers
	ACE_init();

	// Initialize the ADC handlers
	adc_handler2_x = ACE_get_channel_handle((const uint8_t *)"ADCDirectInput_2");
	adc_handler3_y = ACE_get_channel_handle((const uint8_t *)"ADCDirectInput_3");

	/* **************************** */
	/* ******** SETUP UART ******** */
	/* **************************** */

	MSS_UART_init (
		&g_mss_uart1,
		MSS_UART_57600_BAUD,
		(MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT)
	);

	// MSS_UART_FIFO_EIGHT_BYTES
	// MSS_UART_FIFO_SINGLE_BYTE
	MSS_UART_set_rx_handler
	(
		     &g_mss_uart1,
		     uart1_rx_handler,
		     MSS_UART_FIFO_FOUR_BYTES
	);


	/* *************************** */
	/* ******** MAIN LOOP ******** */
	/* *************************** */

	while(1) {
		// Reset the RX Buffer
		g_tx_buff[1] &= 0x00;

		/*uint16_t adcx, adcy;
		adcx = ACE_get_ppe_sample(adc_handler2_x);
		adcy = ACE_get_ppe_sample(adc_handler3_y);

		// Shift over the first 4 useless bits
		adcx = adcx>>4;
		adcy = adcy>>4;
		*/

		//printf("x: %u | y: %u\r\n", adcx, adcy);

		/*
		// Take ADC inputs here
		// X is connected to ADC2, Y to ADC3
		adcx = ACE_get_ppe_sample(adc_handler2_x);
		adcy = ACE_get_ppe_sample(adc_handler3_y);

		// If the Y value is outside the threshold, turn on vibration
		if(adcy > Y_HIGH || adcy < Y_LOW){
			g_tx_buff[1] |= 0x01;
		}

		// If the X value is outside the threshold, turn on vibrations
		if(adcx > X_HIGH || adcx < X_LOW){
			g_tx_buff[1] |= 0x01;
		}

		//Transmit the vibration data via xBee connected to UART1 if the value is non-zero
		if(g_tx_buff[1] == 0x01){
			MSS_UART_polled_tx(&g_mss_uart1, g_tx_buff, sizeof(g_tx_buff));
		}
		*/



		// If the count makes it to 10000, turn the car off
		// (No signals were received in this time)

		if(count == 1000){
			initHBridge();
			initPWM();
		}

		count += 1;

	}

	// If you've made it this far, something went very very wrong
	printf("RIP, yer code be ded\r\n");

	return(1);
}
