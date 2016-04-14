/**************************************\
 * File Name:      light_show.c
 * Project Name:   EECS373 Final Project
 * Created by:     Adrian Padin
 * Start date:     31 March 2016
 * Last modified:  4 April 2016
\**************************************/


#include "light_show.h"
#include "linked_list.h"
#include <assert.h>
#include <stdio.h>

uint8_t eggies[NUM_EASTER_EGGS][MAX_EGGIE_LENGTH+1];
uint32_t GPIO_eggie[NUM_EASTER_EGGS];
// Initialize the light show (run only once)
void init_lights(void) {

    MSS_GPIO_init();

	// Initialize all possible light shows as outputs
    MSS_GPIO_config(BLUE, MSS_GPIO_OUTPUT_MODE);
    MSS_GPIO_config(MAIZE, MSS_GPIO_OUTPUT_MODE);
    MSS_GPIO_config(START, MSS_GPIO_OUTPUT_MODE);
    MSS_GPIO_config(EGG1, MSS_GPIO_OUTPUT_MODE);
    MSS_GPIO_config(EGG2, MSS_GPIO_OUTPUT_MODE);
    MSS_GPIO_config(EGG3, MSS_GPIO_OUTPUT_MODE);
    MSS_GPIO_config(EGG4, MSS_GPIO_OUTPUT_MODE);

	// Initalize return input
    MSS_GPIO_config(MSS_GPIO_8, MSS_GPIO_INPUT_MODE);

    // Start with all lights off
    turn_off_lights();
}

// Run a specific light show
void light_show(mss_gpio_id_t gpio_id) {

	// Turn the pin off
    MSS_GPIO_set_output(gpio_id, 0);

    // Wait for the ACK for a few cycles
    int i;
    for (i = 0; i < 10000; ++i) {
    	if (MSS_GPIO_get_inputs() & MSS_GPIO_8_MASK) {
    		break;
    	}
	}
}

// Turn off all lights
void turn_off_lights() {
	// All outputs are active low/passive high
	MSS_GPIO_set_output(BLUE, 1);
	MSS_GPIO_set_output(MAIZE, 1);
	MSS_GPIO_set_output(START, 1);
	MSS_GPIO_set_output(EGG1, 1);
	MSS_GPIO_set_output(EGG2, 1);
	MSS_GPIO_set_output(EGG3, 1);
	MSS_GPIO_set_output(EGG4, 1);
}





// Initialize the Easter Egg key press sequences
// Press sequence is a pointer to an array of the keys to be pressed
// num_eggie is the number of the easter egg code that we are planning on initializing
// eggie_length is the length of the current press sequence
void init_easter_eggie(uint8_t * press_sequence, uint8_t num_eggie, uint8_t eggie_length){
	int i = 0;

	// Iterate through the press sequence while there are still elements
	while(i < eggie_length){

		// Make sure that i never goes past the Max Length
		assert(i < MAX_EGGIE_LENGTH);

		// Assign the value at the correct place in the array to the press sequence
		eggies[num_eggie][i] = press_sequence[i];
		//printf("eggies %d\n", eggies[num_eggie][i]);

		i++;

	}

	// Add a -1 at the end of the sequence so the software can tell when sequence is completed
	eggies[num_eggie][i] = -1;

	// Make sure that we are not going to an undefined GPIO Pin, then assign the correct GPIO Pin
	assert(5 + num_eggie <= NUM_GPIO_PINS);
	GPIO_eggie[num_eggie] = 5 + num_eggie;

}


/********** EASTER EGG CODE ***********/
// The controller stores the current state of the controller
// As more easter egg keys are entered, the state will increase
// At the final state (the last easter egg key press), 
// an element will be added to the list of easter egg shows to run.



void easter_eggie(controller_t * controller){
	//Iterate through to check all eggs
	int i;
	for(i=0; i<NUM_EASTER_EGGS; i++){
	    // increase state until we've reached sequence end
		uint8_t * j = controller->state + i;
		printf("state: %d\n\r", *j);
		printf("buffer value: %d\n\r", controller->slave_buffer[eggies[i][*j]]);
		printf("other pressed ret value: %d\n\r",other_pressed(controller->slave_buffer,eggies[i][*j]));
	    // check that next in sequence is pressed and nothing else
	    // checks by checking slave buffer at desired seq index
	    if((controller->slave_buffer[eggies[i][*j]] > 0) && !other_pressed(controller->slave_buffer,eggies[i][*j])){
	            // increase state if following eggie sequence
	            controller->state[i]=controller->state[i]+1;
	            // if now reached end of sequence push onto list
	            if(eggies[i][*j] == -1){
	            	printf("reached -1");
	                // light show type to push is num_eggie + 5 for correct GPIO
	                push(i+5);
	                // reset state back to zero
	                controller->state[i]=0;
	            }//inner if
	        }//outer if
	    else if(other_pressed(controller->slave_buffer, *j)){
	            controller->state[i]=0;
	        }// end elseif
	    }// end for
}// end func




