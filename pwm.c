/********************************************\
 * File Name:      		pwm.c
 * Project Name:   		EECS373 Final Project
 * Created by:     		Jawad Nasser
 * Modified by:    		Adrian Padin
 * 			       		Emily Rowland
 * 				   		Ben Miron
 * Start date:     		23 March 2016
 * Last modified:  		28 March 2016
 * Last modified by:	Ben Miron
 \*******************************************/

#include "pwm.h"
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

/* ******************************************************* */
/* ******************* HELPER FUNCTIONS ****************** */
/* ******************************************************* */

/* ******************* calc_duty ****************** */

//Calculates the duty cycle
uint32_t calc_duty(uint8_t input){
	if(input < 128){
		return (uint32_t)floor((double)((128 - input) / 1.27));
	}

	else if(input > 128){
		return (uint32_t)floor((double)((input - 128) / 1.27));
	}

	else{
		return 0;
	}
}

/* **************** calc_HBridgeRight ************* */

//Calculates the H Bridge Inputs for the right side of the car
//Expects the value to be in the form 4'b1100
uint32_t calc_HBridgeRight(uint8_t input){

	if(input > 128){
		return input | 0x1;
	}

	else if(input < 128){
		return input | 0x2;
	}

	else{
		return input;
	}
}


/* **************** calc_HBridgeLeft ************** */

//Calculates the H Bridge Inputs for the left side of the car
//Expects the value to be in the form 4'b0011
uint32_t calc_HBridgeLeft(uint8_t input){

	if(input > 128){
		return input | 0x8;
	}

	else if(input < 128){
		return input | 0x4;
	}

	else{
		return input;
	}
}


/* ******************************************************* */
/* ******************** MAIN FUNCTIONS ******************* */
/* ******************************************************* */


/* ************************************************ */
/* ******************* setPWMDuty ***************** */
/* ************************************************ */

// Set the duty cycle to a value between 0 and 100
void setPWMDuty(uint8_t buff_val) {

	// Write to offset 0
	volatile uint32_t * addr_ptr = (uint32_t *) APB_BASE_ADDR;

	// Read the current duty cycle values
	uint32_t targetVal = *addr_ptr;

	//Calculate the duty cycle
	uint32_t duty = calc_duty(buff_val);
	

	//If we are trying to control the right wheel
	if(!(buff_val & 0x01)){

		// Print the new duty cycle value
		printf("setPWMDutyRight: %u\r\n", (unsigned int)duty);

		// Duty should be between 0 and 100
		assert(duty >= 0 && duty <= 100);


		// targetVal = the new duty cycle value (in the correct bits)
		targetVal &= PWM_RIGHT_MASK;
		targetVal |= duty;

	} // if(!(buff_val & 0x01))


	// Else if we are controlling the left wheel
	else if(buff_val & 0x01){

		// Print the new duty cycle value
		printf("setPWMDutyLeft: %x\r\n", (unsigned int)duty);

		// Duty should be between 0 and 100
		assert(duty >= 0 && duty <= 100);

		// targetVal = the new duty cycle value (in the correct bits)
		targetVal &= PWM_LEFT_MASK;
		targetVal |= (duty << 8);

	} // else if(buff_val & 0x01)


	else{
		printf("Undefined LSB value, wheel not controlled\r\n");
		assert(0);
	}


	// Write targetVal back to the correct register
	*addr_ptr = targetVal;
}

/* ************************************************ */
/* ******************* getPWMDuty ***************** */
/* ************************************************ */

// Get the duty cycle
uint32_t getPWMDuty( void ) {

	// Read from offset 0
	volatile uint32_t * addr_ptr = (uint32_t *) APB_BASE_ADDR;
	return *addr_ptr;
}

/* ************************************************ */
/* **************** setHBridgeInputs ************** */
/* ************************************************ */

// Set the H-Bridge bits
void setHBridgeInputs(uint8_t buff_val) {

	// Write to offset 0
	volatile uint32_t * addr_ptr = (uint32_t *) APB_BASE_ADDR;
	
	//This moves addr_ptr to the value for the location of the H-Bridge inputs
	addr_ptr += 1;

	// Read the current H-Bridge valuess
	uint32_t targetVal = *addr_ptr;
	

	//If we are trying to control the right wheel
	if(!(buff_val & 0x01)){


		//Calculate the new H-Bridge values
		targetVal = calc_HBridgeRight((uint8_t)(targetVal & H_BRIDGE_RIGHT_MASK));


	} // if(!(buff_val & 0x01))


	// Else if we are controlling the left wheel
	else if(buff_val & 0x01){

		//Calculate the new H-Bridge values
		targetVal = calc_HBridgeRight((uint8_t)(targetVal & H_BRIDGE_LEFT_MASK));


	} // else if(buff_val & 0x01)


	else{
		printf("Undefined LSB value, H-Bridge not controlled\r\n");
		assert(0);
	}


	// These are invalid values and will harm the H-bridge
	assert(!((targetVal & H_BRIDGE_RIGHT_MASK) == 0x3));
	assert(!((targetVal & H_BRIDGE_LEFT_MASK) == 0xC));


	// Print the new H_in cycle value
	printf("setHBridgeInputs Right: %x\r\n", (unsigned int)targetVal);

	// Write 0 first for safety with h-bridge
	// *addr_ptr = 0;

	// Write to offset 4
	*addr_ptr = targetVal;
}


/* ************************************************ */
/* **************** getHBridgeInputs ************** */
/* ************************************************ */

// Get the duty cycle
uint32_t getHBridgeInputs( void ) {

	// Return the value at offset 4
	volatile uint32_t * addr_ptr = (uint32_t *) APB_BASE_ADDR;
	++addr_ptr;
	return *addr_ptr;
}

/* ************************************************ */
/* **************** initHBridge ************** */
/* ************************************************ */

// Get the duty cycle
void initHBridge( void ) {

	// Return the value at offset 4
	volatile uint32_t * addr_ptr = (uint32_t *) APB_BASE_ADDR;
	++addr_ptr;

	*addr_ptr = 0x0;
}

/* ************************************************ */
/* **************** initPWM ************** */
/* ************************************************ */

// Get the duty cycle
void initPWM( void ) {

	// Return the value at offset 4
	volatile uint32_t * addr_ptr = (uint32_t *) APB_BASE_ADDR;
	
	*addr_ptr = 0x0;
}