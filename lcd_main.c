#include <stdio.h>
#include <inttypes.h>
#include "drivers/mss_ace/mss_ace.h"
#include "drivers/CoreUARTapb/core_uart_apb.h"
#include "arena.h"

int main(){
	ACE_init();
	/*
	MSS_UART_init
	 (
		 &g_mss_uart1,
		 MSS_UART_115200_BAUD,
		 MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT
	 );
	*/

	//SCORETRACK_init();
	team_struct_t teamMaize;
	team_struct_t teamBlue;

	ARENA_initTeam(&teamMaize, "ADCDirectInput_2");
	ARENA_initTeam(&teamBlue, "ADCDirectInput_3");

	//SCREENCONTROL_setHome();
	//SCREENCONTROL_clearScreen();

	//uint8_t tx_buff[10] = "Go Blue";
	//MSS_UART_polled_tx_string(&g_mss_uart1, tx_buff);

	SCREENCONTROL_init();
	ARENA_outputScoreToScreen(&teamMaize, &teamBlue);

    while(1){


    	if (!teamMaize.already_scored && !teamBlue.already_scored) {
    		if (ARENA_checkIfScored(&teamMaize)) {
    			ARENA_outputScoreToScreen(&teamMaize, &teamBlue);
    			teamMaize.already_scored = 1;
    			teamMaize.score += 1;
    		}

    		else if (ARENA_checkIfScored(&teamBlue)) {
    			ARENA_outputScoreToScreen(&teamMaize, &teamBlue);
    			teamBlue.already_scored = 1;
    			teamBlue.score += 1;
    		}
    	}
    	else if (teamMaize.already_scored) {
    		if (!ARENA_checkIfScored(&teamMaize)) {
    			teamMaize.already_scored = 0;
    		}
    	}
    	else {
    		if (!ARENA_checkIfScored(&teamBlue)) {
    			teamBlue.already_scored = 0;
    		}
    	}
    }
    return 0;
}
