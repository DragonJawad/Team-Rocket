#include "arena.h"
#include "screenControl.h"

#define LASER_ADC_THRESHOLD 100

void
ARENA_initTeam(team_struct_t* teamPtr, const char* adc_name) {
	teamPtr->already_scored = 0;
	teamPtr->score = 0;
	teamPtr->team_ace_handler =ACE_get_channel_handle((const uint8_t *) adc_name);
}

// Returns 0 if didn't score
uint8_t
ARENA_checkIfScored(team_struct_t* teamPtr) {
	// Get the ADC val for the laser detection circuit
	uint16_t adc_val = ACE_get_ppe_sample(teamPtr->team_ace_handler);
	adc_val = adc_val >> 4; // Not exactly sure why but assuming cuz first 4 bits are unused

	// If the value is below the threshold, count it as a laser break
		// aka goallllll
	if(adc_val < LASER_ADC_THRESHOLD) {
		// Increment score
		//teamPtr->score += 1;

		// Return 1 to indicate score happened
		return 1;
	}
	// Otherwise, act as if laser is currently hitting the circuit
	else {
		// No goal, so return 0
		return 0;
	}
}

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
