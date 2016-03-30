#include "scoreTrack.h"
#include "screenControl.h"

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "drivers/mss_ace/mss_ace.h"

// Threshold out of 255 for laser detection circuit to count as a laser break or score
const uint32_t LASER_ADC_THRESHOLD = 100;
// Name of ADC declared in MSS hardware setup for the first laser detection
const char* LASER_A_ADC_NAME = "ADCDirectInput_2";

ace_channel_handle_t adc_handler2_laserA;
int hasScoredYetA = 0; // boolean to keep track if laser broke already or not
uint32_t scoreCountA = 0;
uint32_t lastScorePrinted = 500;

void
SCORETRACK_init() {
	adc_handler2_laserA = ACE_get_channel_handle((const uint8_t *) LASER_A_ADC_NAME);
}

void
SCORETRACK_updateCycle() {
	// Get the ADC val for the laser detection circuit
	uint16_t adcA_val = ACE_get_ppe_sample(adc_handler2_laserA);
	adcA_val = adcA_val >> 4; // Not exactly sure why but assuming cuz first 4 bits are unused

	printf("Laser A val: %u\r\n", adcA_val);

	// If the value is below the threshold, count it as a laser break
	if(adcA_val < LASER_ADC_THRESHOLD) {
		// If laser already broke aka scored already, do nothing
		if(hasScoredYetA != 0) {

		}
		// Otherwise, first time laser broke- keep track of that and increment score
		else {
			hasScoredYetA = 1;
			scoreCountA++;
		}
	}
	// Otherwise, at as if laser is currently hitting the circuit
	else {
		// Reset tracking of if already saw laser break or not
		hasScoredYetA = 0;
	}

	printf("Score val: %u\r\n", scoreCountA);

	// If score changed, print it out on the screen
	if(lastScorePrinted != scoreCountA) {
		// Clear the screen of the other crap that was on it
		SCREENCONTROL_clearScreen();

		// Print the score message to the screen
		char str[50];
		sprintf(str, "Score: %u", scoreCountA); // 7 chars long disregarding number
		SCREENCONTROL_printStr(str);

		// Print some padding to go to the next line
			// Note: For some reason fails to understand what newlines are
		char* spaceBuffer;
		if(scoreCountA < 10) {
			// Print out 26-7-2 = 18 spaces needed
			spaceBuffer = "                  ";
		}
		else if(scoreCountA < 100) {
			// Print out 17 spaces
			spaceBuffer = "                 ";
		}
		else {
			// Print out 16 spaces, assuming score ain't going above 999
			spaceBuffer = "                ";
		}
		SCREENCONTROL_printStr(spaceBuffer);

		// Draws a line all the way down the center of the screen [(79,0) to (79,127)]
			// Technically should draw another one at x=80, but not necessary atm
		SCREENCONTROL_printLine(79, 0, 79, 127);

		// Write the score on the left side of the screen
			// Remember that (0,0) is in the bottom left
			// Center at (40, 64), need offset of (-16, -36) due to character size
			//		4*6 (4 characters of width of 6 pixels to left) and 3.5*8
		SCREENCONTROL_printLargeNum(scoreCountA, 16, 127-36);

		// Placeholder for score of second team on right side
			// Centered at same spot as first num but 80 pixels (half of screen width) over
		SCREENCONTROL_printLargeNum(0, 16+80, 127-36);


		// Keep track of the new last printed score
		lastScorePrinted = scoreCountA;
	}

}
