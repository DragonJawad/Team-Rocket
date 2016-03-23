module controllerV2(
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
                    H_IN1,
                    H_IN2,
                    H_IN3,
                    H_IN4);

// APB Bus Interface
input PCLK,PENABLE, PSEL, PRESETN, PWRITE;
input [31:0] PWDATA;
input [7:0] PADDR;
input CAPTURE_SWITCH;
output [31:0] PRDATA;
output PREADY, PSLVERR;
output FABINT;
output PWM1;
output H_IN1;
output H_IN2;
output H_IN3;
output H_IN4;
 
assign BUS_WRITE_EN = (PENABLE && PWRITE && PSEL);
assign BUS_READ_EN = (!PWRITE && PSEL); //Data is ready during first cycle to make it availble on the bus when PENABLE is asserted

assign PREADY = 1'b1;
assign PSLVERR = 1'b0;

handler handler_0(	.pclk(PCLK),
			    .nreset(PRESETN), 
			    .bus_write_en(BUS_WRITE_EN),
			    .bus_read_en(BUS_READ_EN),
			    .bus_addr(PADDR),
			    .bus_write_data(PWDATA),
			    .bus_read_data(PRDATA),
                .fabint(FABINT),
                .pwm(PWM1),
                .h_in1(H_IN1),
                .h_in2(H_IN2),
                .h_in3(H_IN3),
                .h_in4(H_IN4),
                .switch(CAPTURE_SWITCH)
			);

endmodule

module handler (
			pclk,
			nreset,
			bus_write_en, 
			bus_read_en,
			bus_addr,
			bus_write_data, //data_in
			bus_read_data,
            fabint,
            pwm,
            h_in1,
            h_in2,
            h_in3,
            h_in4,
            switch //Switch/Capture input
            );
	
input pclk, nreset, bus_write_en, bus_read_en;
input [7:0] bus_addr;
input [31:0] bus_write_data;
input switch;
output reg [31:0] bus_read_data;
output reg fabint;
output reg pwm;
output reg h_in1;
output reg h_in2;
output reg h_in3;
output reg h_in4;

// PWM-related variables
`define period 50000
reg [31:0] pwmCount;
reg [7:0] duty = 50;

// H_bridge crap
reg [1:0] dirtyHINput = 0;
reg [3:0] tempHInputs;
reg [3:0] HInsReg = 0;


reg [2:0] switch_syncer;

always@(posedge pclk)
if(~nreset)
  fabint   <= 1'b0;
else
  begin
    /*
    // Set fabint based off of some inputs
    if(timer_interrupt || capture_interrupt)
      fabint   <= 1'b1;
    else
      fabint   <= 1'b0;
    */
end

always@(posedge pclk)
if(~nreset)
  begin
    duty <= 50; // Initialize to 50
    dirtyHINput[0] <= 1;
    tempHInputs <= 0;
   /* h_in1 <=0;
    h_in2 <=0;
    h_in3 <=0;
    h_in4 <=0; */
  end
else begin
	if(bus_write_en) begin : WRITE
		case(bus_addr[4:2])
			3'b000: // Set duty cycle
                begin 
				duty     <= bus_write_data[5:0];
                end
            
            // 0x04
            3'b001: // Set HBridge inputs
                begin
                dirtyHINput[0] <= 1;
                HInsReg <= bus_write_data[3:0];
                end
        endcase
    end
	else if(bus_read_en) begin : READ
        case(bus_addr[4:2])
	        3'b000: // Read duty cycle
                begin 
		        bus_read_data       <= duty;
				end
            3'b001: // Set HBridge inputs
                begin
                bus_read_data <= HInsReg;
                end
        endcase
     end
     else begin
        /*
        overflowReset       <= 1'b0;
        reset_interrupt     <= 1'b0;
        reset_capture_sync  <= 1'b0;
        reset_capture_async <= 1'b0;
        */
     end

     if(dirtyHINput[1] == 1)
     begin
        dirtyHINput[0] <= 0;
     end
end

// PWM output block
always @(posedge pclk)
begin
    if (pwmCount >= `period)
        pwmCount <= 0;
    else
        pwmCount <= pwmCount + 1;
    if (pwmCount < `period*duty/100)
        pwm <= 1;
    else
        pwm <= 0;
end

always @(posedge pclk)
begin
    /*
    if(dirtyHINput[0] == 1)
    begin
        dirtyHINput[1] <= 1;
        HInsReg[0] <= 0;
        HInsReg[1] <= 0;
        HInsReg[2] <= 0;
        HInsReg[3] <= 0;
    end
    else if(dirtyHINput[1] == 1)
    begin
        dirtyHINput[1] <= 0;
        HInsReg[0] <= tempHInputs[0];
        HInsReg[1] <= tempHInputs[1];
        HInsReg[2] <= tempHInputs[2];
        HInsReg[3] <= tempHInputs[3];
    end
    */
        
    if(HInsReg[0] == 1 && HInsReg[1] == 1)
    begin
        h_in1 <= 0;
        h_in2 <= 0;
    end
    else
    begin
        h_in1 <= HInsReg[0];
        h_in2 <= HInsReg[1];
    end

    if(HInsReg[2] == 1 && HInsReg[3] == 1)
    begin
        h_in3 <= 0;
        h_in4 <= 0;
    end
    else
    begin
        h_in3 <= HInsReg[2];
        h_in4 <= HInsReg[3];
    end
end

endmodule