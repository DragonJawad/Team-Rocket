// PS2 Source File
// ps2.c

#include "ps2.h"
#include "drivers/mss_spi/mss_spi.h"
#include "drivers/mss_gpio/mss_gpio.h"

/***************************************/
/********** CONTROLLER STRUCT **********/
/***************************************/

/* Struct definition for reference
typedef struct controller {
	int counter;
	mss_spi_slave_t select;
	uint8_t vibration;
	uint8_t slave_buffer[MAX_BUFFER_SIZE];
} controller_t;
*/

// Initialize the controller struct
void controller_init(controller_t * controller, mss_spi_slave_t select) {

	// Initialize all values
	controller->select = select;
	controller->vibration = 0;
	controller->counter = 0;
	int i;
	for (i = 0; i < MAX_BUFFER_SIZE; ++i) {
		controller->slave_buffer[i] = 0;
	}
}



/**************************************/
/********** HELPER FUNCTIONS **********/
/**************************************/

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





/***************************************/
/********** COMMAND FUNCTIONS **********/
/***************************************/

// Poll for just digital values
// static void digital_poll(uint8_t * slave_buffer);

// Poll for all values
// static void full_poll(uint8_t * slave_buffer);

// Enter configuration mode
static void enter_config(uint8_t * slave_buffer);

// Exit configuration mode
static void exit_config(uint8_t * slave_buffer);

// Turn on analog mode
static void enable_analog(uint8_t * slave_buffer);

// Map motors to correct bytes
static void motor_setup(uint8_t * slave_buffer);

// Setup analog buttons
static void button_setup(uint8_t * slave_buffer);



// Poll for digital button values
void digital_capture(controller_t * controller) {

	// Select controller
	MSS_SPI_set_slave_select( &g_mss_spi1, controller->select );

	uint8_t master_buffer[3]  = { 0x80, flip(0x42), 0x00 };
	MSS_SPI_transfer_block
	(
	    &g_mss_spi1,
	    (uint8_t *) master_buffer, 3,
	    (uint8_t *) controller->slave_buffer, 18
	);

	// Deselect controller
	MSS_SPI_clear_slave_select( &g_mss_spi1, controller->select );

}
// Poll for all values
void full_capture(controller_t * controller) {

	// Handle vibration counter
	uint8_t vibration = 0;

	// If counter reached 0, reset vibration
	if (controller->counter > 0) {
		vibration = controller->vibration;
		controller->counter -= 1;
	}

	// Select controller
	MSS_SPI_set_slave_select( &g_mss_spi1, controller->select );

	uint8_t master_buffer[5]  = { 0x80, flip(0x42), 0x00, vibration, vibration };
	MSS_SPI_transfer_block
	(
	    &g_mss_spi1,
	    (uint8_t *) master_buffer, 5,
	    (uint8_t *) controller->slave_buffer, 16
	);

	// Select controller
	MSS_SPI_clear_slave_select( &g_mss_spi1, controller->select );
}

// Perform all setup tasks on this controller
void setup_all(controller_t * controller) {

	// Select controller
	MSS_SPI_set_slave_select( &g_mss_spi1, controller->select );

	uint8_t * slave_buffer = (uint8_t *) (controller->slave_buffer);

	int i = 0;

	// Enter configuration mode
	enter_config(slave_buffer);

	for (i = 0; i < 200; ++i); // Delay

	// Enable analog mode on the controller
	enable_analog(slave_buffer);

	for (i = 0; i < 200; ++i); // Delay

	// Setup the motor to map to the proper bytes
	motor_setup(slave_buffer);

	for (i = 0; i < 200; ++i); // Delay

	// Configure the buttons to send analog data
	button_setup(slave_buffer);

	for (i = 0; i < 200; ++i); // Delay

	// Exit configuration mode
	exit_config(slave_buffer);
}

// Set the controller to vibrate for a certain amount of time
void set_vibration(controller_t * controller, uint8_t vibration, int counter) {

	controller->vibration = vibration;
	controller->counter = counter;
}

// Enter configuration mode
void enter_config(uint8_t * slave_buffer) {

	uint8_t master_buffer[5]  = { 0x80, flip(0x43), 0x00, 0x80, 0x00 };
	MSS_SPI_transfer_block
	(
	    &g_mss_spi1,
	    (uint8_t *) master_buffer, 5,
	    (uint8_t *) slave_buffer, 0
	);
}

// Exit configuration mode
void exit_config(uint8_t * slave_buffer) {

	uint8_t master_buffer[9] = { 0x80, flip(0x43), 0x00, 0x00, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A };
	MSS_SPI_transfer_block
	(
	    &g_mss_spi1,
	    (uint8_t *) master_buffer, 9,
	    (uint8_t *) slave_buffer, 0
	);
}

// Turn on analog mode
void enable_analog(uint8_t * slave_buffer) {

	uint8_t master_buffer[9] = { 0x80, flip(0x44), 0x00, 0x80, 0xC0, 0xFF, 0xFF, 0xFF, 0xFF };
	MSS_SPI_transfer_block
	(
	    &g_mss_spi1,
	    (uint8_t *) master_buffer, 9,
	    (uint8_t *) slave_buffer, 0
	);
}

// Map motors to correct bytes
void motor_setup(uint8_t * slave_buffer) {

	uint8_t master_buffer[9] = { 0x80, flip(0x4D), 0x00, 0x00, 0x80, 0xFF, 0xFF, 0xFF, 0xFF };
	MSS_SPI_transfer_block
	(
	    &g_mss_spi1,
	    (uint8_t *) master_buffer, 9,
	    (uint8_t *) slave_buffer, 0
	);
}

// Setup analog buttons
void button_setup(uint8_t * slave_buffer) {

	uint8_t master_buffer[9] = { 0x80, flip(0x4F), 0x00, 0xFF, 0xFF, 0xC0, 0x00, 0x00, 0x00 };
	MSS_SPI_transfer_block
	(
	    &g_mss_spi1,
	    (uint8_t *) master_buffer, 9,
	    (uint8_t *) slave_buffer, 0
	);
}
