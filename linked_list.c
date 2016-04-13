/*
 * linked_list.c
 *
 *  Created on: Apr 6, 2016
 *      Author: erowland
 */
#include "linked_list.h"
#include "light_show.h"
#include <assert.h>
#include <stdlib.h>
#include "drivers/mss_gpio/mss_gpio.h"

show_t * head = NULL;

// Adds to end of list
void push(uint32_t type) {
    show_t * current = head;

    while (current->next != NULL) {
        current = current->next;
    }
    current->next = malloc(sizeof(show_t));
    current->next->type = type;
    current->next->next = NULL;
}

//Remove first in list
void pop( void ){

	// Declare a temporary pointer to head
	show_t * temp = head;

	if(head != NULL){

		// Move head to the next element
		head = head->next;

		// Free the data at the previous location of head
		free(temp);
	}
	
}

// Plays the show at the head pointer
void play( void ){

	// Play the show
	light_show(head->type);
}


