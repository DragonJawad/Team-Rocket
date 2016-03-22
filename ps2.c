// PS2 Source File
// ps2.c

#include "ps2.h"

// Flip the bits of an 8-bit value
uint8_t flip(uint8_t value) {

	uint8_t buffer = 0;
	buffer |= (value & 0x80) >> 7;
	buffer |= (value & 0x40) >> 5;
	buffer |= (value & 0x20) >> 3;
	buffer |= (value & 0x10) >> 1;
	buffer |= (value & 0x08) << 1;
	buffer |= (value & 0x04) << 3;
	buffer |= (value & 0x02) << 5;
	buffer |= (value & 0x01) << 7;
	return buffer;
}


/********** COMMAND FUNCTIONS **********/

// Poll for digital button values
void simple_poll(uint8_t * slave_buffer) {

	uint8_t master_buffer[5]  = { flip(0x01), flip(0x42), 0x00, 0xff, 0xff };
	MSS_SPI_transfer_block
	(
	    &g_mss_spi1,
	    (uint8_t *) master_buffer, 3,
	    (uint8_t *) slave_buffer, 18
	);
}
// Poll for all values
void full_poll(uint8_t * slave_buffer) {

	uint8_t master_buffer[5]  = { flip(0x01), flip(0x42), 0x00, 0xff, 0xff };
	MSS_SPI_transfer_block
	(
	    &g_mss_spi1,
	    (uint8_t *) master_buffer, 5,
	    (uint8_t *) slave_buffer, 16
	);
}

// Enter configuration mode
void enter_config(uint8_t * slave_buffer) {

	uint8_t master_buffer[5]  = { flip(0x01), flip(0x43), 0x00, flip(0x01), 0x00 };
	MSS_SPI_transfer_block
	(
	    &g_mss_spi1,
	    (uint8_t *) master_buffer, 5,
	    (uint8_t *) slave_buffer, 0
	);
}

// Exit configuration mode
void exit_config(uint8_t * slave_buffer) {

	uint8_t master_buffer[9]   = { flip(0x01), flip(0x43), 0x00, 0x00, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A };
	MSS_SPI_transfer_block
	(
	    &g_mss_spi1,
	    (uint8_t *) master_buffer, 9,
	    (uint8_t *) slave_buffer, 0
	);
}

// Turn on analog mode
void enable_analog(uint8_t * slave_buffer) {

	uint8_t master_buffer[9]   = { flip(0x01), flip(0x44), 0x00, flip(0x01), flip(0x03), 0xff, 0xff, 0xff, 0xff };
	MSS_SPI_transfer_block
	(
	    &g_mss_spi1,
	    (uint8_t *) master_buffer, 9,
	    (uint8_t *) slave_buffer, 0
	);
}

// Map motors to correct bytes
void motor_setup(uint8_t * slave_buffer) {

	uint8_t master_buffer[9]   = { flip(0x01), flip(0x4D), 0x00, 0x00, flip(0x01), 0xff, 0xff, 0xff, 0xff };
	MSS_SPI_transfer_block
	(
	    &g_mss_spi1,
	    (uint8_t *) master_buffer, 9,
	    (uint8_t *) slave_buffer, 0
	);
}

// Setup analog buttons
void button_setup(uint8_t * slave_buffer) {

	uint8_t master_buffer[9] = { flip(0x01), flip(0x4F), 0x00, 0xff, 0xff, flip(0x03), 0x00, 0x00, 0x00 };
	MSS_SPI_transfer_block
	(
	    &g_mss_spi1,
	    (uint8_t *) master_buffer, 9,
	    (uint8_t *) slave_buffer, 0
	);
}
