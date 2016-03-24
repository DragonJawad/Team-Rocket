/**************************************\
 * File Name:      pwm.h
 * Project Name:   EECS373 Final Project
 * Created by:     Jawad Nasser
 * Modified by:    Adrian Padin
 * 			       Emily Rowland
 * 				   Ben Roland Miron
 * Start date:     23 March 2016
 * Last modified:  23 March 2016
 \**************************************/

#include <stdio.h>
#include <inttypes>
#include <cassert>

#define APB_BASE_ADDR 0x40050000;

#define H_BRIDGE_FORWARD  0x5
#define H_BRIDGE_BACKWARD 0xA
#define H_BRIDGE_OFF	  0x0


// Set the duty cycle to a value between 0 and 100
void setPWMDuty(int duty) {
	
	// Duty should be between 0 and 100
	assert(duty >= 0 && duty <= 100);
	
	// Write to offset 0
	*((uint32_t *) APB_BASE_ADDR) = duty;
}

// Get the duty cycle
uint32_t getPWMDuty(void) {

	// Read from offset 0
	return *((uint32_t *) APB_BASE_ADDR);
}

// Set the H-Bridge bits
void setHBridgeInputs(uint32_t H_in) {
	
	// Duty should only be 0x0, 0xA, or 0x5
	assert(
		H_in == H_BRIDGE_OFF      || 
		H_in == H_BRIDGE_FORWARD  ||
		H_in == H_BRIDGE_BACKWARD
	);
	
	// Write to offset 4
	*((uint32_t *) (APB_BASE_ADDR + 4)) = duty;
}


// Get the duty cycle
uint32_t getHBridgeInputs(void) {

	// Read from offset 4
	return *((uint32_t *) (APB_BASE_ADDR + 4));
}


int main()
{

	while( 1 )
	{
		setHBridgeInputs(H_BRIDGE_FORWARD);
		setHBridgeInputs(H_BRIDGE_OFF);
		setHBridgeInputs(H_BRIDGE_BACKWARD);
		setHBridgeInputs(H_BRIDGE_OFF);
	}
}
