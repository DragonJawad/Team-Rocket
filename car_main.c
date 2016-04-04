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
#define X_LOW			0
#define X_HIGH			5000

#define Y_LOW			0
#define Y_HIGH			5000


// UART1 buffer for input
uint8_t g_rx_buff[RX_BUFF_SIZE];

// Declare a global count
// This is used to stop the car if no data is received
uint32_t count = 0;

// UART1 buffer for output
uint8_t g_tx_buff[TX_BUFF_SIZE];

// Declare globals for the ADC
ace_channel_handle_t adc_handler2;
ace_channel_handle_t adc_handler3;


/* ***************************************************** */
/* ***************** Interrupt Handlers **************** */
/* ***************************************************** */


/* ************************************
*	UART RX Interrupt Handler
*	The values should be sent in the form
*	g_rx_buff[0] = CAR ID
*	g_rx_buff[1] = first side of the car (analog stick)
*	g_rx_buff[2] = second side of the car (analog stick)
************************************ */
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
    }

    else{
    	printf("Made it to the end of the interrupt handler\r\n");
    }

    count = 0;
}


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
	adc_handler2 = ACE_get_channel_handle((const uint8_t *)"ADCDirectInput_2");
	adc_handler3 = ACE_get_channel_handle((const uint8_t *)"ADCDirectInput_3");

	uint16_t adcx, adcy;

	int i = 0;
	for (i = 0; i < 1000000; ++i);

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

		// Take ADC inputs here
		// X is connected to ADC2, Y to ADC3
		adcx = ACE_get_ppe_sample(adc_handler2);
		adcy = ACE_get_ppe_sample(adc_handler3);

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

		// If the count makes it to 1000000, turn the car off
		// (No signals were received in this time)

		if(count == 10000000){
			initHBridge();
			initPWM();
		}

		count += 1;
	}

	// If you've made it this far, something went very very wrong
	printf("RIP, yer code be ded\r\n");

	return(1);
}