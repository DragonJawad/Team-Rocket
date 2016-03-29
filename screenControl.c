#include "screenControl.h"
#include "drivers/mss_uart/mss_uart.h"

// Clears the screen
void
SCREENCONTROL_clearScreen() {
	uint8_t tx_buff_control[] = {0x7C, 0x00};
	MSS_UART_polled_tx(&g_mss_uart1, tx_buff_control, sizeof(tx_buff_control));
}

// Sets current target pixel back to top left corner (x and y)
void
SCREENCONTROL_setHome() {
	uint8_t tx_buff_control[] = {0x7C, 0x18, 0x00, 0x7C, 0x19, 0x00};
	MSS_UART_polled_tx(&g_mss_uart1, tx_buff_control, sizeof(tx_buff_control));
}

/*
void
SCREENCONTROL_printStr(uint8_t str[]) { //26 characters is the length of one line on the LCD
	MSS_UART_polled_tx_string(&g_mss_uart1, str);
}
*/

void
SCREENCONTROL_printStr(const char* message) { //26 characters is the length of one line on the LCD
	MSS_UART_polled_tx_string(&g_mss_uart1, ((uint8_t*)message) );
}

// DOESN'T WORK! Assuming screen doesn't support it =/
void
SCREENCONTROL_printNewLine() {
	unsigned char newline[2] = {0x0A, 0x0D};
	MSS_UART_polled_tx_string(&g_mss_uart1, newline);
}