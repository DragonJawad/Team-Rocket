/**************************************\
 * File Name:      pwm.v
 * Project Name:   EECS373 Final Project
 * Created by:     Jawad Nasser
 * Modified by:    Adrian Padin
 * 			       Emily Rowland
 * 				   Ben Roland Miron
 * Start date:     23 March 2016
 * Last modified:  23 March 2016
 \**************************************/

module PWM(
					PCLK,
					PENABLE,
					PSEL,
					PRESETN,
					PWRITE,
					PREADY,
					PSLVERR,
					PADDR,
					PWDATA,
					PRDATA,
                    FABINT,
                    CAPTURE_SWITCH,
                    PWM1,
                    H_IN
);


/* APB Bus Interface */
input PCLK, PENABLE, PSEL, PRESETN, PWRITE;
input [31:0] PWDATA;
input [7:0] PADDR;
input CAPTURE_SWITCH;
output [31:0] PRDATA;
output PREADY, PSLVERR;


/* PWM Outputs */
output FABINT;
output PWM1;
output [3:0] H_IN1; // Zero and one should never both be on
					// Two and three should never both be on

					
/* APB Wires and Ouptuts
 * Data is ready during first cycle to make it availble on
 * the bus when PENABLE is asserted
 */
assign WRITE_EN = (PENABLE && PWRITE && PSEL);
assign READ_EN = (!PWRITE && PSEL);
assign PREADY = 1'b1;
assign PSLVERR = 1'b0;


/* PWM variables
 * System clock is 20 MHz
 * PWM operates at 500 Hz
 * 20 MHz / 500 Hz = 40,000 cycles
 */
`define period 40000
//`define period 50000
reg [31:0] counter;
reg [31:0] overflow;	// Value between 0 and period
reg [7:0] duty;			// Value between 0 and 100
reg pwm1_reg;


/* H-Bridge variables

 * H_output_buffer will receive the values
 * on the first cycle and then the output
 * will appear on H_output on the second cycle.
 
 * When H_output_buffer is written to, the values
 * H_output will all be set to zero for one cycle.
 
 * This will ensure that no two are ever on at
 * the same time.

 */
reg [3:0] H_output_buffer;
reg [3:0] H_output;
assign H_IN = H_output;


/* Counter */

always @(posedge PCLK)

// Default values
if(~PRESETN)
begin
	counter <= 0;
	overflow <= 0;
	pwm1_reg <= 1'b1;
end

else
begin

	// Calculate overflow
	overflow <= ((duty * period) / 100);

	// Reset the counter after "period" cycles
	if (counter >= period)
	begin
		counter <= 0;
		pwm1_reg <= 1'b1;
	end
	else
	begin
	
		// If the counter is less than overflow, out is 1
		// If it is greater, out is 0
		if (counter > overflow)
			pwm_reg1 <= 1'b0;
		else
			pwm_reg1 <= 1'b0;
			
		counter += 1;
	end
end


/* Reading and writing to duty cycle and setting H bits */

always @(posedge PCLK)

// Always shift buffer into outputs if different
if (H_output != H_output_buffer &&
	(!WRITE_EN) && 
	(PADDR[4:2] != 3'b001))
begin
	H_output <= H_output_buffer;
end
	

// Default values
if(~PRESETN)
begin
	duty <= 50; // Initialize to 50%
	H_output_buffer <= 0;
	H_output <= 0;
end

else
begin

	// Writing to duty or H_output_buffer
	if(WRITE_EN) // Writing to register
	begin
		case(PADDR[4:2])
		
			3'b000: // Offset 0x00: Write duty cycle
			begin 
				duty <= PWRITE[7:0];
            end
            
            3'b001: // Offset 0x04: Write HBridge inputs
            begin
                H_output_buffer <= PWRITE[3:0];
                H_output <= 0;
            end
        endcase
    end
	
	// Read duty cycle or H_output
	else if(READ_EN)
	begin
        case(PADDR[4:2])
		
	        3'b000: // Offset 0x00: Read duty cycle
            begin 
		        PWRITE[7:0] <= duty;
			end
            3'b001: // Offset 0x04: Set HBridge inputs
            begin
                PWRITE[7:0] <= H_outputs;
            end
        endcase
     end
     else begin
        /*
		NOTE: I HAVE NO IDEA WHAT THE F**K THIS IS:
		
        overflowReset       <= 1'b0;
        reset_interrupt     <= 1'b0;
        reset_capture_sync  <= 1'b0;
        reset_capture_async <= 1'b0;
        */
     end

end

endmodule