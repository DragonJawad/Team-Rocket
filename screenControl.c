#include "screenControl.h"
#include "drivers/CoreUARTapb/core_uart_apb.h"

UART_instance_t target_uart;

void
SCREENCONTROL_init() {
	// Formula from core_uart_apb.h: baud_value = (clock /(baud_rate * 16)) - 1
	//		clock = 100MHz, baud_rate = MSS_UART_115200_BAUD
	//		so baud_value = (100*10^6/(115200*16)) - 1 = 53.2534722222
	uint16_t baud_value = 53;

	UART_init
	(
	    &target_uart,
	    0x40050000, // APB address for this UART
	    baud_value,
	    DATA_8_BITS | NO_PARITY
	);
}

// Clears the screen
void
SCREENCONTROL_clearScreen() {
	uint8_t tx_buff_control[] = {0x7C, 0x00};
	UART_send(&target_uart, tx_buff_control, sizeof(tx_buff_control));
}

// Sets current target pixel back to top left corner (x and y)
void
SCREENCONTROL_setHome() {
	SCREENCONTROL_setXAndY(0, 0);
}

// Set cursor (x and y pixel top left coordinate) for next action
// X must be between 0-159 and Y must be between 0-127
void
SCREENCONTROL_setXAndY(uint8_t posX, uint8_t posY) {
	uint8_t tx_buff_control[] = {0x7C, 0x18, posX, 0x7C, 0x19, posY};
	UART_send(&target_uart, tx_buff_control, sizeof(tx_buff_control));
}

void
SCREENCONTROL_printStr(const char* message) { //26 characters is the length of one line on the LCD
	UART_polled_tx_string(&target_uart, ((uint8_t*)message) );
}

// Print a number in large characters (up to 1 digit currently)
// 	According to https://github.com/jawadmn/Team-Rocket/blob/master/ascii_art.txt
void
SCREENCONTROL_printLargeNum(uint32_t num, uint8_t startX, uint8_t startY){
	/*
	 * 1:
	  		1st: "_//XX___" (1 then 3 spaces)
			2nd: "//_XX___" (1 then 3 spaces)
			3rd: "___XX___" (3 then 3 spaces)
			4th: "___XX___"
			5th: "___XX___"
			6th: "___XX___"
			7th: "XXXXXXXX" (no spaces)
	 *
	 *
	 */

	// Each entry should be 8 characters wide and 7 lines tall
	char nums[10][7][9] = {	{ 		" XXXXXX ", // "\\" = 1 \ (escape character =/ )
									"XX    XX",
									"XX    XX",
									"XX    XX",
									"XX    XX",
									"XX    XX",
									" XXXXXX "},

								{ 	"  XXX   ",
									" XXXX   ",
									"   XX   ",
									"   XX   ",
									"   XX   ",
									"   XX   ",
									" XXXXXX "},

								{ 	"  XXXXX ",
									" XX   XX",
									"XX    XX",
									"    XXX ",
									"  XXX   ",
									" XX     ",
									"XXXXXXXX"},

								{ 	" XXXXXX ",
									"XX    XX",
									"      XX",
									"   XXXX ",
									"      XX",
									"XX    XX",
									" XXXXXX "},

								{ 	"XX   XX ",
									"XX   XX ",
									"XX   XX ",
									"XXXXXXXX",
									"     XX ",
									"     XX ",
									"     XX "},

								{ 	"XXXXXXXX",
									"XX      ",
									"XX      ",
									"XXXXXXX ",
									"      XX",
									"XX    XX",
									" XXXXXX "},

								{ 	" XXXXXX ",
									"XX    XX",
									"XX      ",
									"XXXXXXX ",
									"XX    XX",
									"XX    XX",
									" XXXXXX "},

								{ 	"XXXXXXXX",
									"      XX",
									"     XX ",
									"    XX  ",
									"   XX   ",
									"   XX   ",
									"   XX   "},

								{ 	" XXXXXX ",
									"XX    XX",
									"XX    XX",
									" XXXXXX",
									"XX    XX",
									"XX    XX",
									" XXXXXX "},

								{ 	" XXXXXX ",
									"XX    XX",
									"XX    XX",
									" XXXXXXX",
									"      XX",
									"XX    XX",
									" XXXXXX "},
		};

	int i;
	int targetNum = num%10; // Only display the last digit! targetNum will be 0-9
	for(i = 0; i < 7; i++) {
		// Set the starting position for the current line
		SCREENCONTROL_setXAndY(startX, startY-i*8);

		// Print out the current line
		UART_polled_tx_string(&target_uart, ((uint8_t*) *(nums[targetNum]+i)) );
	}
}

// Prints a line from (x1, y1) to (x2, y2)
void
SCREENCONTROL_printLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
  uint8_t tx_buff_control[] = {0x7C, 0x0C, x1, y1, x2, y2, 0x01};
  UART_send(&target_uart, tx_buff_control, sizeof(tx_buff_control));
}

// DOESN'T WORK! Assuming screen doesn't support it =/
void
SCREENCONTROL_printNewLine() {
	unsigned char newline[2] = {0x0A, 0x0D};
	UART_polled_tx_string(&target_uart, newline);
}
