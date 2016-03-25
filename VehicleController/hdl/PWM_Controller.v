/**************************************\
 * File Name:      PWM_Controller.v
 * Project Name:   EECS373 Final Project
 * Created by:     Jawad Nasser
 * Modified by:    Adrian Padin
 * 			       Emily Rowland
 * 				   Ben Roland Miron
 * Start date:     24 March 2016
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
                    PWM1,
                    PWM2,
                    H_IN,
);


/* APB Bus Interface */
input PCLK, PENABLE, PSEL, PRESETN, PWRITE;
input [31:0] PWDATA;
input [7:0] PADDR;
output reg [31:0] PRDATA;
output PREADY, PSLVERR;


/* PWM Outputs */
output PWM1, PWM2;
output [3:0] H_IN; // Zero and one should never both be on
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
//`define period 40000
`define period 50000
reg [31:0] counter1;
reg [31:0] counter2;

reg [31:0] overflow1;	// Value between 0 and period
reg [31:0] overflow2;   // Value between 0 and period

reg [7:0] duty1;	    // Value between 0 and 100
reg [7:0] duty2;        // Value between 0 and 100

reg pwm1_reg, pwm2_reg;

assign PWM1 = pwm1_reg;
assign PWM2 = pwm2_reg;

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

/* Reading and writing to duty cycle and setting H bits */

/********************************************/
/*************** SETUP/READ *****************/
/********************************************/

always @(posedge PCLK)
begin

    // Default values
    if(~PRESETN)
    begin
        duty1 <= 0; // Initialize duty cycles
        duty2 <= 0;
        H_output_buffer <= 0;
        H_output <= 0;
    end //if(~PRESETN)

    else
    begin

        // Writing to duty or H_output_buffer
        if(WRITE_EN) // Writing to register
        begin
            case(PADDR[4:2])
            
                4'b0000: // Offset 0x00: Write duty cycle
                begin 
                    duty1 <= PWDATA[7:0];
                    duty2 <= PWDATA[15:8];
                end //Case 3'b000
                
                4'b0010: // Offset 0x04: Write HBridge inputs
                begin
                    //H_output_buffer <= PWDATA[3:0];
                    //H_output <= 0;
                    H_output <= PWDATA[3:0];
                end //Case 3'b001

            endcase //case(PADDR[4:2])
        end //if(WRITE_EN)
	
        // Read duty cycle or H_output
        else if(READ_EN)
        begin
            case(PADDR[4:2])
            
                4'b0000: // Offset 0x00: Read duty cycle
                begin 
                    PRDATA[7:0] <= duty1;
                    PRDATA[15:8] <= duty2;
                end //Case 3'b000

                4'b0010: // Offset 0x04: Set HBridge inputs
                begin
                    PRDATA[3:0] <= H_output;
                end //Case 3'b001

            endcase //case(PADDR[4:2])

         end //else if(READ_EN)

    end //else

end //always @(posedge PCLK)



/********************************************/
/*************** RIGHT SIDE *****************/
/********************************************/

/* Counter */

always @(posedge PCLK)
begin
    // Default values
    if(~PRESETN)
    begin
        counter1 <= 0;
        overflow1 <= 0;
        pwm1_reg <= 1'b0;

    end //if(~PRESETN)

    //If we are not resetting the system
    else
    begin

        // Calculate overflow
        overflow1 <= ((duty1 * `period) / 100);

        // Reset the counter after "period" cycles
        if (counter1 >= `period)
        begin
            counter1 <= 0;
            pwm1_reg <= 1'b1;
        end //if(counter1 >= `period)
    
        //If the counter is less han the "period" cycles
        else
        begin
        
            // If the counter is less than overflow, out is 1
            // If it is greater, out is 0
            if (counter1 > overflow1)
                pwm1_reg <= 1'b0;
            else
                pwm1_reg <= 1'b1;
                
            counter1 <= counter1 + 1;
        
        end //else (counter is less than "period" cycles

    end //else (If we are not resetting the system)

end //always @(posedge PCLK)




/********************************************/
/*************** LEFT SIDE ******************/
/********************************************/

/* Counter */

always @(posedge PCLK)
begin

    // Default values
    if(~PRESETN)
    begin
        counter2 <= 0;
        overflow2 <= 0;
        pwm2_reg <= 1'b1;
    end //if(~PRESETN)
    
    //If we are not resetting the system
    else
    begin

        // Calculate overflow
        overflow2 <= ((duty2 * `period) / 100);

        // Reset the counter after "period" cycles
        if (counter2 >= `period)
        begin
            counter2 <= 0;
            pwm2_reg <= 1'b1;
        end //if (counter2 >= `period)
    
        //If the counter is less than "period" cycles
        else
        begin
        
            // If the counter is less than overflow, out is 1
            // If it is greater, out is 0
            if (counter2 > overflow2)
                pwm2_reg <= 1'b0;
            else
                pwm2_reg <= 1'b1;
                
        counter2 <= counter2 + 1;
        
        end //else (counter is less than "period" cycles

    end //else (system is not being reset)

end //always @(posedge PCLK)


endmodule