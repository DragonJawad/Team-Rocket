/*
 * linked_list.h
 *
 *  Created on: Apr 6, 2016
 *      Author: erowland
 */

#ifndef LINKED_LIST_H_
#define LINKED_LIST_H_

#include <inttypes.h>
#include "drivers/mss_gpio/mss_gpio.h"

// Define the struct for a light show
// Includes the type of the show
typedef struct show {
    uint32_t type;
    struct show * next;
} show_t;

// Adds to end of list
void push(uint32_t type);

//Remove first in list
void pop( void );

// Plays the show at the head pointer
void play( void );


show_t *head;

#endif /* LINKED_LIST_H_ */
