#ifndef SCREEN_CONTROL_H_
#define SCREEN_CONTROL_H_

#include <inttypes.h>

/* Assumes using UART 1 (if not, ctrl+f replace &g_mss_uart1 in .c file)
 * Should setup like the following:
	 MSS_UART_init
		 (
			 &g_mss_uart1,
			 MSS_UART_115200_BAUD,
			 MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT
		 );
 *
 * Rest of commands at https://github.com/sparkfun/SparkFun_Graphic_LCD_Serial_Backpack_Arduino_Library/blob/V_1.0.1/src/SparkFunSerialGraphicLCD.cpp
 */

// Clears the screen
void
SCREENCONTROL_clearScreen();

// Sets current target pixel back to top left corner (x and y)
void
SCREENCONTROL_setHome();

void
SCREENCONTROL_printStr(const char* message);

// DOESN'T WORK! Assuming screen doesn't support it =/
void
SCREENCONTROL_printNewLine();

#endif /*SCORE_TRACK_H_*/
