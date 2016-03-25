/**************************************\
 * File Name:      pwm.h
 * Project Name:   EECS373 Final Project
 * Created by:     Jawad Nasser
 * Modified by:    Adrian Padin
 * 			       Emily Rowland
 * 				   Ben Miron
 * Start date:     23 March 2016
 * Last modified:  24 March 2016
 \**************************************/

#ifndef PWM_H
#define PWM_H

#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <assert.h>

#define APB_BASE_ADDR 0x40050000;

#define H_BRIDGE_FORWARD  	0x5
#define H_BRIDGE_BACKWARD 	0xA
#define H_BRIDGE_OFF	  	0x0
#define H_BRIDGE_RIGHT_MASK	0xC
#define H_BRIDGE_LEFT_MASK	0x3


// Set the duty cycle of the right side to a value between 0 and 100
void setPWMDutyRight(uint32_t duty);

//Sets the duty cycle of the left side to a value between 0 and 100
void setPWMDutyLeft(uint32_t duty);

// Get the duty cycle
uint32_t getPWMDuty(void);

// Set the H-Bridge bits for the right side
void setHBridgeInputs(uint32_t H_in);

// Get the duty cycle
uint32_t getHBridgeInputs(void);

//Calculates the duty cycle
uint32_t calc_duty(uint8_t input);

//Calculates the H Bridge Inputs for the right side of the car
uint32_t calc_HBridgeRight(uint8_t input);

//Calculates the H Bridge Inputs for the left side of the car
uint32_t calc_HBridgeLeft(uint8_t input);


#endif /* PWM_H */
