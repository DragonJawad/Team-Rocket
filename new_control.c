
// Order of bytes:
// Byte 1: Car ID
// Byte 2: X Data
// Byte 3: Left Data
// Byte 4: Right Data
void uart1_rx_handler( mss_uart_instance_t * this_uart ) {
	
	uint8_t rx_buff[4];
	int num_bytes = MSS_UART_get_rx(this_uart, rx_buff, 4);
	
	if (num_bytes != 4) return; // transmission failed
	if (rx_buff[0] != CAR_ID) return; // wrong car
	
	int left = rx_buff[2], right = rx_buff[3];

	
	// Percentage of pwm on each side
	int left_scale = 100, right_scale = 100;
	
	// Turn if one of the two is non-zero and the other is zero
	if (left > 0 && right == 0) {
		left_scale = ((255 - (int) left) * 100) / 255;
	}
	
	if (left == 0 && right > 0) {
		right_scale = ((255 - (int) right) * 100) / 255;
	}

	// Calculate left and right pwm values by scaling from X input
	// Moving slower (lower pwm) on one side will cause the car
	// to turn more in that direction
	left = ((int) rx_buff[1] * left_scale) / 100;
	right = ((int) rx_buff[1] * right_scale) / 100;
	
	// Set values
	volatile uint32_t * addr_ptr = (uint32_t *) APB_BASE_ADDR;
	addr_ptr += 1; // offset of 4 is pwm
	addr_ptr = (left << 8) | right;
	
	// Add in H-bridge stuff for going backwards
	// Also add adc and vibration stuff
}

