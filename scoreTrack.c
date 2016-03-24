#include "scoreTrack.h"

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
}
