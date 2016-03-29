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

#define PWM_RIGHT_MASK	0xFF00
#define PWM_LEFT_MASK	0x00FF


// Set the duty cycle of the correct wheel to a value between 0 and 100
void setPWMDuty(uint8_t buff_val);

// Get the duty cycle
uint32_t getPWMDuty( void );

// Set the H-Bridge bits for the car
void setHBridgeInputs(uint8_t buff_val);

// Get the duty cycle
uint32_t getHBridgeInputs( void );

//Initialize the HBridge to 0x00
void initHBridge( void );

//Initialize the PWM signal to 0
void initPWM( void );

#endif /* PWM_H */
