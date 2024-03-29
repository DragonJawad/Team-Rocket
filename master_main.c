/**************************************\
 * File Name:      main.c
 * Project Name:   EECS373 Final Project
 * Created by:     Adrian Padin
 * Start date:     15 March 2016
 * Last modified:  22 March 2016
\**************************************/

#include <stdio.h>
#include <inttypes.h>
#include <assert.h>
#include <stdlib.h>

#include "drivers/mss_spi/mss_spi.h"
#include "drivers/mss_uart/mss_uart.h"
#include "drivers/mss_gpio/mss_gpio.h"
#include "drivers/mss_ace/mss_ace.h"
#include "drivers/CoreUARTapb/core_uart_apb.h"
#include "ps2.h"
#include "arena.h"
#include "screenControl.h"
#include "light_show.h"
#include "linked_list.h"
#include "defines.h"

/********** GLOBAL VARIABLES AND DEFINITIONS **********/

#define COUNT_START 10

// Global controller structs
controller_t controller1;
controller_t controller2;
controller_t controller3;
controller_t controller4;

team_struct_t teamMaize;
team_struct_t teamBlue;

int timerActivatedFlag = 0; // Every timer interrupt increments this by 1
uint32_t countdownCounter = 0; // In seconds


/********** INTERRUPT HANDLERS **********/

// UART RX Interrupt Handler
void xbee_receive_data( mss_uart_instance_t * this_uart ){

    uint8_t rx_buff[4]; // keep the data buffer aligned

    // Read in data from rx_buff
	int num_bytes = MSS_UART_get_rx( this_uart, rx_buff, 4 );

	// Transmission failed
	if (num_bytes != 4) {
		return;
	}

	int car_select = rx_buff[0];
	int vibration = rx_buff[1];

	// Set the vibration value
	switch (car_select) {
		case 1: set_vibration(&controller1, 0xFF); controller1.counter = COUNT_START; break;
		case 2: set_vibration(&controller2, 0xFF); controller1.counter = COUNT_START; break;
		//case 3: set_vibration(&controller3, vibration, 100); break;
		//case 4: set_vibration(&controller4, vibration, 100); break;
		default: break;
	}
	
	//printf("Got some XBEE data: car %x, value %x\r\n", car_select, vibration);
}


// Send data to the cars (not an irq but may become one later)
void send_data_to_car(controller_t * controller) {
	
	// Data sent to cars
	uint8_t tx_buff[4] = { 	controller->select,
							flip(controller->slave_buffer[10]),
							flip(controller->slave_buffer[4]),
							flip(controller->slave_buffer[5])
	};

    MSS_UART_polled_tx(&g_mss_uart1, tx_buff, 4);
}


// Fabint handler to handle, say, interrupts from fabric to MSS
__attribute__ ((interrupt)) void Fabric_IRQHandler( void )
{
    // Get the interrupt status (which also clears the internal interrupt on our hardware)
    uint32_t status = ARENA_getInterruptStatus();
    printf("Interrupt status: %u\n\r", (unsigned int) status);

    // Check that it was really a timer interrupt
    if(status & 0x02) {
    	// Simply increment the timer
    	timerActivatedFlag++;
	}

    // If it was a button interrupt at all, there's an issue here (as not wired atm)
    //if(status & 0x01)
    //{
    //    // TODO: Assert?
    //}

    NVIC_ClearPendingIRQ( Fabric_IRQn );
}


/********** GAME CODE FUNCTIONS **********/

// Doesn't return until all 4 controllers press and hold X at the same time
void buttonWait() {

	// Wait for all controllers to press and hold X
	while ( flip(controller1.slave_buffer[10]) < 10
			// || flip(controller2.slave_buffer[10]) < 10
			//|| flip(controller2.slave_buffer[10]) < 10
			//|| flip(controller2.slave_buffer[10]) < 10
			) {

		full_capture(&controller1);
		full_capture(&controller2);
		full_capture(&controller3);
		full_capture(&controller4);
	}
}


// Sends messages to tell cars to stop
void stopCars() {

	// Send all zeros to cars
	int car_id;
	uint8_t tx_buff[4] = {0, 0, 0, 0};

	for (car_id = 1; car_id < 4; ++car_id) {
		tx_buff[0] = car_id;
		MSS_UART_polled_tx(&g_mss_uart1, tx_buff, 4);
	}
}

// All necessary setup to begin the game
void initGameSystem() {

	/********** SETUP SCREEN AND XBEES **********/

	SCREENCONTROL_init();

	MSS_UART_init (
		&g_mss_uart1,
		MSS_UART_57600_BAUD,
		(MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT)
	);

	// Interrupt handler for incoming XBee data
	MSS_UART_set_rx_handler
	(
		&g_mss_uart1,
		xbee_receive_data,
		MSS_UART_FIFO_FOUR_BYTES
	);


	/********** SETUP LIGHT SHOW **********/

	init_lights();


	/********** SETUP ACE/GOALS **********/

	ACE_init();

	ARENA_initTeam(&teamMaize, "ADCDirectInput_4");
	ARENA_initTeam(&teamBlue, "ADCDirectInput_5");
	//ARENA_outputScoreToScreen(&teamMaize, &teamBlue);


	/********** SETUP CONTROLLERS **********/

	MSS_SPI_init( &g_mss_spi1 );

	controller_init(&controller1, MSS_SPI_SLAVE_1);
	controller_init(&controller2, MSS_SPI_SLAVE_2);
	controller_init(&controller3, MSS_SPI_SLAVE_3);
	controller_init(&controller4, MSS_SPI_SLAVE_4);

	setup_all(&controller1);
	setup_all(&controller2);
	setup_all(&controller3);
	setup_all(&controller4);
}


// Returns 0 if no team won yet, returns 1 if a team won
// teamScoredFlag = 1 if Maize scored, = 2 if Blue scored
//		NOTE: TECHNICALLY DOESN'T NEED TO BE A SEPARATE FUNCTION ANYMORE
int teamScored(int teamScoredFlag) {
	// TODO: Assert flag == LIGHTS_MAIZE or == LIGHTS_BLUE

	/****** Game-now-paused stuff ******/
	// Stop the cars, game should be paused at the moment!
	stopCars();

	/****** Game over or simply score? ******/
	// If a team won...
	if(teamMaize.score == SCOREMAX) {
		// Return 1 so caller knows  game is now technically over
		return 1;
	}
	else if(teamBlue.score == SCOREMAX) {
		// Return 1 so caller knows  game is now technically over
		return 1;
	}
	// Else, a team simply scored...
	else {
		// Simply output score to screen
		ARENA_outputScoreToScreen(&teamMaize, &teamBlue);

		// TODO: Output something to screen to tell users to reset cars and ball

		// Team's light show
		light_show(teamScoredFlag);

		// Vibrate all controllers on and off four times
		int i = 0;
		for (i = 0; i < 4; ++i) {
			set_vibration(&controller1, 0xff);
			set_vibration(&controller2, 0xff);
			//set_vibration(&controller3, 0xff);
			//set_vibration(&controller4, 0xff);

			int j = 0;
			for (j = 0; j < 1000; j++);

			set_vibration(&controller1, 0);
			set_vibration(&controller2, 0);
			//set_vibration(&controller3, 0);
			//set_vibration(&controller4, 0);

			for (j = 0; j < 1000; j++);
		}

		// Return 0 as no team won yet
		return 0;
	}
}


void startGame() {
	// Reset the teams
	teamMaize.score = 0;
	teamMaize.score = 0;

	// Output the start message to the screen
	ARENA_outputStartToScreen();

	// Make sure cars don't move
	stopCars();

	// Prepare the ball to drop
	ARENA_closeBallRelease();

	// Wait for button to be pressed before continuing
	buttonWait();

	// Initialize timer variables
	countdownCounter = GAMELENGTH;
	timerActivatedFlag = 0;

	// Drop the ball
	light_show(START);
	ARENA_openBallRelease();

	// Enable fabint, particularly for 1 sec interrupts from fabric
	NVIC_EnableIRQ(Fabric_IRQn);

	ARENA_outputScoreToScreen(&teamMaize, &teamBlue);
	ARENA_outputTimeLeft(countdownCounter);
	// TODO: Count down or something till game starts?
}

// Runs game, doesn't return until game is over or button is pressed to reset game (? for latter)
void runGame() {

	//int scoreFlag = -1; // Probably not necessary but nice to have

	/********** LOOP **********/
	while(1) {

		/********** CAPTURE CONTROLLER DATA ********/
		full_capture(&controller1);
		full_capture(&controller2);
		full_capture(&controller3);
		full_capture(&controller4);

		//printf("Controller1: %d\r\n", (int) controller1.slave_buffer[10]);
		//printf("Controller2: %d\r\n", (int) controller2.slave_buffer[10]);
		//printf("Controller3: %d\r\n", (int) controller3.slave_buffer[10]);
		//printf("Controller4: %d\r\n", (int) controller4.slave_buffer[10]);

		/********** SEND DATA TO CARS ********/
		send_data_to_car(&controller1);
		//send_data_to_car(&controller2);
		//send_data_to_car(&controller3);
		//send_data_to_car(&controller4);

		// Vibration data is updated automagically by interrupts

		/********** CHECK SCORING **********/
		if (ARENA_checkIfScored(&teamMaize)) {
			teamScored(MAIZE);
		}
		else if (ARENA_checkIfScored(&teamBlue)) {
			teamScored(BLUE);
		}

		// Debugging
		//if (controller1.slave_buffer)

		/********** EGGIES!!! ***********/

		// EGGIE INITS ///
		//uint8_t light_show_end=1;
		//uint32_t gpio_inputs=0;


		/*	Current Easter Egg Codes:
			1.	Triangle -> Circle -> X -> Square -> Triangle
			2.	Right -> Down -> Left -> Up -> Right
			3.
			4.
		 */


		//uint8_t press_seq0[]= {8,9,10,11,8};
		//uint8_t press_seq1[]={4,7,5,6,4};
		//init_easter_eggie(press_seq0,0,5);
		//init_easter_eggie(press_seq1,1,5);
		//controller_t c_array[15];
		//for (int a=0; a<15; a++){
		//	c_array[a].state[0]=0;
		//	c_array[a].state[1]=
		//}

		//easter_eggie(&controller1);
		//easter_eggie(&controller2);
		//easter_eggie(&controller3);
		//easter_eggie(&controller4);

		//gpio_inputs = MSS_GPIO_get_inputs();

		//if(gpio_inputs == 0x10) { //GPIO 4??
		//	light_show_end=1;
		//}

		//else {
		//	light_show_end=0;
		//}
		/*while(list not empty && light_show_end
			keep popping off light shows
			and call light_show functions
			light show function gets called in pop
		*/
		//SET TO HIGH FOR DEBUGGING W/O UNO
		//light_show_end=1;
		//show_t * current = head;
		//while((current != NULL) && light_show_end){
		//	current=current->next;
		//	pop();
		//	play();
		//}





		//////////////////// How do the above lines fit into check scoring status?

		/*

		// Check if one of the teams scored
		if (ARENA_checkIfScored(&teamMaize)) {
			//teamMaize.already_scored = 1; // Shouldn't be necessary anymore, but keeping just in case

			// Increment the team's score
			teamMaize.score += 1;

			// Set the (probs unnecessary) flag that a team scored
			scoreFlag = LIGHTS_MAIZE;
		}
		else if (ARENA_checkIfScored(&teamBlue)) {
			// Increment the team's score
			teamBlue.score += 1;

			// Set the (probs unnecessary) flag that a team scored
			scoreFlag = LIGHTS_BLUE;
		}

		if(scoreFlag != -1) {
			// Clear the flag
			scoreFlag = -1;

			// Do stuff now that a team scored and check if game is over
			int gameOverFlag = gameScore(scoreFlag);

			// If game over, get out of here (endGame will take care of everything else)
			if(gameOverFlag) {
				// Disable the timer interrupt as no need for it anymore
				NVIC_DisableIRQ(Fabric_IRQn);

				return;
			}
			// Otherwise, wait until players ready before restarting
			else {
				// Disable the timer interrupt
				NVIC_DisableIRQ(Fabric_IRQn);

				// Pause
				buttonWait();

				// Re-enable the timer interrupt
				NVIC_EnableIRQ(Fabric_IRQn);
				timerActivatedFlag = 0; // Just in case, if a second was just about to occur, meh

				// Just to be explicit, continue onto the next run of the loop (compleeetely unnecessary other than reading-wise!)
				continue;
			}
		}

		*/

		///*
		// If timer interrupt came by..
		if(timerActivatedFlag) {
			// TODO: ASSERT == 1, cuz if > 1 somehow missed an entire second

			// Reset the flag
			timerActivatedFlag = 0;

			// Decrement the countdown
			countdownCounter--;

			if(countdownCounter == 0) {
				// TODO: Anything else? (endGame() does stuff)

				// End the game
				return;
			}
			// Otherwise, output time ONLY
			else {
				ARENA_outputTimeLeft(countdownCounter);
			}
		}

		 //*/
	}
}

// Outputs game over stuff and cleans up as necessary
void endGame() {
	// Disable the timer interrupts
	NVIC_DisableIRQ(Fabric_IRQn);

	// Stop the cars
	stopCars();

	// TODO: Output game over message on screen

	// TODO: Stop all cars explicitly, or just assume cars still stopped from before?

	int whoWon = 0;
	if(teamBlue.score == SCOREMAX) {
		whoWon = BLUE;
	}
	else if(teamMaize.score == SCOREMAX) {
		whoWon = MAIZE;
	}
	else {
		if(teamBlue.score > teamMaize.score) {
			whoWon = BLUE;
		}
		else if(teamMaize.score > teamBlue.score) {
			whoWon = MAIZE;
		}
		else {
			//??
		}
	}

	ARENA_outputEndToScreen(whoWon);

	// Simply return to show that done with end game stuff
	return;
}

int main(){

	// Initialize necessary game systems
	initGameSystem();

	/*
	set_vibration(&controller2, 0xff);
	full_capture(&controller2);
	set_vibration(&controller2, 0x00);
	full_capture(&controller2);
	*/

	// Setup game and wait till button is pressed to start game
	startGame();

	// Play the game. Returns when one team scores
	runGame();

	// Do end game stuff
	endGame();

    return(0);
}
