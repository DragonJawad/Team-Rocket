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

// Set cursor (x and y pixel top left coordinate) for next action
// X must be between 0-159 and Y must be between 0-127
void
SCREENCONTROL_setXAndY(uint8_t posX, uint8_t posY);

// Prints a string directly to the screen
// Characters are 8 pixels tall and 6 pixels wide
void
SCREENCONTROL_printStr(const char* message);

// Print a number in large characters (up to 1 digit currently)
// 	According to https://github.com/jawadmn/Team-Rocket/blob/master/ascii_art.txt
void
SCREENCONTROL_printLargeNum(uint32_t num, uint8_t startX, uint8_t startY);

// Prints a line from (x1, y1) to (x2, y2)
void
SCREENCONTROL_printLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);

// DOESN'T WORK! Assuming screen doesn't support it =/
// Kept just in case using different screen
void
SCREENCONTROL_printNewLine();

#endif /*SCORE_TRACK_H_*/
