#include "arena.h"
#include "screenControl.h"
#include "light_show.h"
#include <stdio.h>
#include <string.h>

#define LASER_ADC_THRESHOLD 130
#define BASE_APB_ADDRESS 0x40050000

// Duty cycle for ball release to release the ball/be open
#define RELEASEOPENDUTY 30  // 0.105 of period (2100us/20ms), ~11%
#define RELEASECLOSEDDUTY 5 // 0.045 of period (900us/20ms), ~5%

void
ARENA_initTeam(team_struct_t* teamPtr, const char* adc_name) {
	teamPtr->score = 0;
	teamPtr->team_ace_handler =ACE_get_channel_handle((const uint8_t *) adc_name);
}

// Returns 1 if team has scored, 0 otherwise
uint8_t
ARENA_checkIfScored(team_struct_t* teamPtr) {

	// Get the ADC val for the laser detection circuit
	uint16_t adc_val = ACE_get_ppe_sample(teamPtr->team_ace_handler);
	adc_val = adc_val >> 4; // Not exactly sure why but assuming cuz first 4 bits are unused

	// If the value is below the threshold, laser is broken
	if(adc_val < LASER_ADC_THRESHOLD) {
		return 1; // Goal
	}
	else {
		return 0; // No goal
	}
}

// Outputs score to screen, clears screen beforehand!
void
ARENA_outputScoreToScreen(team_struct_t* teamPtrA, team_struct_t* teamPtrB) {
	// Clear the screen of the other crap that was on it
	SCREENCONTROL_clearScreen();

		// NOTE: Currently doesn't handle if scores > 9 (just uses last digit)
	//-> Output both scores in a pretty format

	// Draws a line all the way down the center of the screen [(79,0) to (79,127)]
		// Technically should draw another one at x=80, but not necessary atm
	SCREENCONTROL_printLine(79, 0, 79, 127);

	// Write the score on the left side of the screen
		// Remember that (0,0) is in the bottom left
		// Center at (40, 64), need offset of (-16, -36) due to character size
		//		4*6 (4 characters of width of 6 pixels to left) and 3.5*8
	SCREENCONTROL_printLargeNum(teamPtrA->score, 16, 127-36);

	// Placeholder for score of second team on right side
		// Centered at same spot as first num but 80 pixels (half of screen width) over
	SCREENCONTROL_printLargeNum(teamPtrB->score, 16+80, 127-36);
}

// Outputs timer to first line (does not clear screen), outputs in M:SS format
void
ARENA_outputTimeLeft(uint32_t timeLeftInSeconds) {
	// Calculate the minute and seconds places independently
	unsigned int minDigit = timeLeftInSeconds/60;
	unsigned int secondsLeftOver = timeLeftInSeconds%60;

	// Create the actual string to output
	char timeOut[12];
	if(secondsLeftOver < 10) {
		sprintf(timeOut, "%u:0%u      ", minDigit, secondsLeftOver);
	}
	else {
		sprintf(timeOut, "%u:%u      ", minDigit, secondsLeftOver);
	}

	// Output the string
	SCREENCONTROL_setXAndY(0, 127); // Output to top left corner
	SCREENCONTROL_printStr(timeOut);
}

// Outputs start message to screen
void
ARENA_outputStartToScreen() {
	SCREENCONTROL_clearScreen();
	char message[] = "Game Start";
	SCREENCONTROL_printStr(message);
}

// Outputs end message to screen
void
ARENA_outputEndToScreen(int teamWonFlag) {
	SCREENCONTROL_clearScreen();
	char message[27];
	if (teamWonFlag == BLUE) {
		strcpy(message, "Game Over: Blue Team Wins!");
	}
	else if (teamWonFlag == MAIZE) {
		strcpy(message, "Game Over: Maize Team Wins!");
	}
	else {
		strcpy(message, "Game Over: Tie Game :(");
	}
	SCREENCONTROL_printStr(message);
}

uint32_t
ARENA_getInterruptStatus() {
	// Read from offset 1 aka plus 4
	volatile uint32_t * addr_ptr = (uint32_t *) (BASE_APB_ADDRESS + 4);
	// Simply return the value
	return *addr_ptr;
}

// Outputs PWM signal to open the ball release system
void
ARENA_openBallRelease() {
	// Write to offset 0
	volatile uint32_t * addr_ptr = (uint32_t *) BASE_APB_ADDRESS;

	// Write the duty cycle
	*addr_ptr = RELEASEOPENDUTY;
}

// Outputs PWM signal to close the ball release system
void
ARENA_closeBallRelease() {
	// Write to offset 0
	volatile uint32_t * addr_ptr = (uint32_t *) BASE_APB_ADDRESS;

	// Write the duty cycle
	*addr_ptr = RELEASECLOSEDDUTY;
}
