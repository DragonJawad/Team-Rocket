// Controller.v
module Controller(
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
                    PWM1,
                    PWM2,
                    SW1,
                    SW2,
                    IN1,
                    IN2,
                    IN3,
                    IN4);

// APB Bus Interface
input PCLK,PENABLE, PSEL, PRESETN, PWRITE;
input [31:0] PWDATA;
input [31:0] PADDR;
output [31:0] PRDATA;
output PREADY, PSLVERR;
output FABINT;
output PWM1, PWM2;
input SW1, SW2;
output IN1, IN2, IN3, IN4;
reg pwm1_reg, pwm2_reg;

reg in1, in2;

reg [31:0] counterReg1;
reg [31:0] counterReg2;

assign PWM1 = pwm1_reg;
assign PWM2 = pwm2_reg;
assign IN1 = in1;
assign IN2 = in2;

always@(posedge PCLK)
begin
    in1 <= 1'b1;
    in2 <= 1'b0;

end


always@(posedge PCLK)
begin

if(~PRESETN)
begin

	counterReg1 <= 32'h00000000;

end

else begin
    if(counterReg1 >= 40000)
    begin
        counterReg1 <= 32'h00000000;
        pwm1_reg <= 1'b1;
    end

    else if(counterReg1 <= 20000) begin
        pwm1_reg <= 1'b1;
    end
    
	else begin
        pwm1_reg <= 1'b0;
    end

    counterReg1 <= counterReg1 + 1;
    end
end





endmodule