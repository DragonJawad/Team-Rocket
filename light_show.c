/**************************************\
 * File Name:      light_show.c
 * Project Name:   EECS373 Final Project
 * Created by:     Adrian Padin
 * Start date:     31 March 2016
 * Last modified:  4 April 2016
\**************************************/

#include "light_show.h"
#include "linked_list.h"
#include "ps2.h"

// Initialize the light show (run only once)
void init_lights(void) {

    MSS_GPIO_init();

	// Initialize all possible light shows as outputs
    MSS_GPIO_config(LIGHTS_OFF, MSS_GPIO_OUTPUT_MODE);
    MSS_GPIO_config(LIGHTS_BLUE, MSS_GPIO_OUTPUT_MODE);
    MSS_GPIO_config(LIGHTS_MAIZE, MSS_GPIO_OUTPUT_MODE);
    MSS_GPIO_config(LIGHTS_START, MSS_GPIO_OUTPUT_MODE);

    // Iterate through the light shows for each GPIO pin
    for(int i = 5; i < 5 + NUM_EASTER_EGGS){
    	assert(i < NUM_GPIO_PINS);
	    MSS_GPIO_config(i, MSS_GPIO_OUTPUT_MODE);
	}
	
	// Initalize return input
    MSS_GPIO_config(MSS_GPIO_4, MSS_GPIO_INPUT_MODE);

	// All outputs are active low/passive high
    MSS_GPIO_set_output(LIGHTS_OFF, 1);
    MSS_GPIO_set_output(LIGHTS_BLUE, 1);
    MSS_GPIO_set_output(LIGHTS_MAIZE, 1);
    MSS_GPIO_set_output(LIGHTS_START, 1);

    // Iterate through the GPIO outputs, setting them all to high
    for(int i = 5; i < 5 + NUM_EASTER_EGGS){
    	assert(i < NUM_GPIO_PINS);
	    MSS_GPIO_set_output(i, 1);
	}
    
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

		i++;

	}

	// Add a -1 at the end of the sequence so the software can tell when sequence is completed
	eggies[num_eggie][i] = -1;

	// Make sure that we are not going to an undefined GPIO Pin, then assign the correct GPIO Pin
	assert(5 + num_eggie <= NUM_GPIO_PINS);
	GPIO_eggie[num_eggie] = 5 + num_eggie;

}

// Run a specific light show
void light_show(mss_gpio_id_t gpio_id) {

	// Turn the pin off
    MSS_GPIO_set_output(gpio_id, 0);

    // Wait a bit for the Arduino to read the signal
    int i = 0;
    while (i < 100000) ++i;

    // Turn the pin back on
    MSS_GPIO_set_output(gpio_id, 1);
}


/********** EASTER EGG CODE ***********/
// The controller stores the current state of the controller
// As more easter egg keys are entered, the state will increase
// At the final state (the last easter egg key press), 
// an element will be added to the list of easter egg shows to run.



void easter_eggie(controller_t * controller){

	// Switch the current state of the controller
    switch(controller->state) {

        case 0 : // Starting in state 0
        	// If the triangle button was pressed
            if(controller->slave_buffer[8]>0){
                controller->state=1;
            }

            // If the right button (D Pad) was pressed
            else if(controller->slave_buffer[4]>0){
                controller->state=2;
            }
            break;

        case 1 : // If the controller is currently in state 1 (Triangle was pressed)

        	// If circle was pressed
            if(controller->slave_buffer[9] > 0 && other_pressed(controller, 9)){
                controller->state=2;
            }

            // OR if Left button (D Pad) was pressed
            else if(controller->slave_buffer[5]>0){
                controller->state=2;
            }
            break;


        case 2 :
            if(controller->slave_buffer[10]>0){
                controller->state=3;
            }
            else if(controller->slave_buffer[6]>0){
                controller->state=3;
            }
            break;
        case 3 :
           if(controller->slave_buffer[11]>0){
                controller->state=4;
            }
            else if(controller->slave_buffer[7]>0){
                controller->state=4;
            }
        case 4 :
            if(controller->slave_buffer[8]>0){
                controller->state=0;
                //light_show(LIGHTS_EGG1);
                // insert light_show into linked list
                push(head, LIGHTS_EGG1);
            }
            else if(controller->slave_buffer[4]>0){
                controller->state=0;
                //light_show(LIGHTS_EGG2);
                // insert light_show into linked list
                push(head, LIGHTS_EGG2);
            }
            break;
        default:
            controller->state=0;
            //if(light_show_end) light_show(LIGHTS_OFF);
            break;

    }// end switch
}// end func



