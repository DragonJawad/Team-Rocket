//////////////////////////////////////////////////////////////////////
// Created by SmartDesign Fri Mar 25 01:48:58 2016
// Version: v11.5 SP3 11.5.3.10
//////////////////////////////////////////////////////////////////////

`timescale 1 ns/100 ps

// VehicleController
module VehicleController(
    // Inputs
    MSS_RESET_N,
    UART_0_RXD,
    UART_1_RXD,
    // Outputs
    H_IN,
    PWM1,
    PWM2,
    UART_0_TXD,
    UART_1_TXD
);

//--------------------------------------------------------------------
// Input
//--------------------------------------------------------------------
input        MSS_RESET_N;
input        UART_0_RXD;
input        UART_1_RXD;
//--------------------------------------------------------------------
// Output
//--------------------------------------------------------------------
output [3:0] H_IN;
output       PWM1;
output       PWM2;
output       UART_0_TXD;
output       UART_1_TXD;
//--------------------------------------------------------------------
// Nets
//--------------------------------------------------------------------
wire          CoreAPB3_0_APBmslave0_PENABLE;
wire   [31:0] CoreAPB3_0_APBmslave0_PRDATA;
wire          CoreAPB3_0_APBmslave0_PREADY;
wire          CoreAPB3_0_APBmslave0_PSELx;
wire          CoreAPB3_0_APBmslave0_PSLVERR;
wire   [31:0] CoreAPB3_0_APBmslave0_PWDATA;
wire          CoreAPB3_0_APBmslave0_PWRITE;
wire   [3:0]  H_IN_net_0;
wire          MSS_RESET_N;
wire          PWM1_net_0;
wire          PWM2_net_0;
wire          UART_0_RXD;
wire          UART_0_TXD_net_0;
wire          UART_1_RXD;
wire          UART_1_TXD_net_0;
wire          VehicleController_MSS_0_FAB_CLK;
wire          VehicleController_MSS_0_M2F_RESET_N;
wire          VehicleController_MSS_0_MSS_MASTER_APB_PENABLE;
wire   [31:0] VehicleController_MSS_0_MSS_MASTER_APB_PRDATA;
wire          VehicleController_MSS_0_MSS_MASTER_APB_PREADY;
wire          VehicleController_MSS_0_MSS_MASTER_APB_PSELx;
wire          VehicleController_MSS_0_MSS_MASTER_APB_PSLVERR;
wire   [31:0] VehicleController_MSS_0_MSS_MASTER_APB_PWDATA;
wire          VehicleController_MSS_0_MSS_MASTER_APB_PWRITE;
wire          UART_1_TXD_net_1;
wire          UART_0_TXD_net_1;
wire          PWM1_net_1;
wire          PWM2_net_1;
wire   [3:0]  H_IN_net_1;
//--------------------------------------------------------------------
// TiedOff Nets
//--------------------------------------------------------------------
wire          GND_net;
wire          VCC_net;
wire   [31:0] IADDR_const_net_0;
wire   [31:0] PRDATAS1_const_net_0;
wire   [31:0] PRDATAS2_const_net_0;
wire   [31:0] PRDATAS3_const_net_0;
wire   [31:0] PRDATAS4_const_net_0;
wire   [31:0] PRDATAS5_const_net_0;
wire   [31:0] PRDATAS6_const_net_0;
wire   [31:0] PRDATAS7_const_net_0;
wire   [31:0] PRDATAS8_const_net_0;
wire   [31:0] PRDATAS9_const_net_0;
wire   [31:0] PRDATAS10_const_net_0;
wire   [31:0] PRDATAS11_const_net_0;
wire   [31:0] PRDATAS12_const_net_0;
wire   [31:0] PRDATAS13_const_net_0;
wire   [31:0] PRDATAS14_const_net_0;
wire   [31:0] PRDATAS15_const_net_0;
wire   [31:0] PRDATAS16_const_net_0;
//--------------------------------------------------------------------
// Bus Interface Nets Declarations - Unequal Pin Widths
//--------------------------------------------------------------------
wire   [31:0] CoreAPB3_0_APBmslave0_PADDR;
wire   [7:0]  CoreAPB3_0_APBmslave0_PADDR_0_7to0;
wire   [7:0]  CoreAPB3_0_APBmslave0_PADDR_0;
wire   [31:20]VehicleController_MSS_0_MSS_MASTER_APB_PADDR_0_31to20;
wire   [19:0] VehicleController_MSS_0_MSS_MASTER_APB_PADDR_0_19to0;
wire   [31:0] VehicleController_MSS_0_MSS_MASTER_APB_PADDR_0;
wire   [19:0] VehicleController_MSS_0_MSS_MASTER_APB_PADDR;
//--------------------------------------------------------------------
// Constant assignments
//--------------------------------------------------------------------
assign GND_net               = 1'b0;
assign VCC_net               = 1'b1;
assign IADDR_const_net_0     = 32'h00000000;
assign PRDATAS1_const_net_0  = 32'h00000000;
assign PRDATAS2_const_net_0  = 32'h00000000;
assign PRDATAS3_const_net_0  = 32'h00000000;
assign PRDATAS4_const_net_0  = 32'h00000000;
assign PRDATAS5_const_net_0  = 32'h00000000;
assign PRDATAS6_const_net_0  = 32'h00000000;
assign PRDATAS7_const_net_0  = 32'h00000000;
assign PRDATAS8_const_net_0  = 32'h00000000;
assign PRDATAS9_const_net_0  = 32'h00000000;
assign PRDATAS10_const_net_0 = 32'h00000000;
assign PRDATAS11_const_net_0 = 32'h00000000;
assign PRDATAS12_const_net_0 = 32'h00000000;
assign PRDATAS13_const_net_0 = 32'h00000000;
assign PRDATAS14_const_net_0 = 32'h00000000;
assign PRDATAS15_const_net_0 = 32'h00000000;
assign PRDATAS16_const_net_0 = 32'h00000000;
//--------------------------------------------------------------------
// Top level output port assignments
//--------------------------------------------------------------------
assign UART_1_TXD_net_1 = UART_1_TXD_net_0;
assign UART_1_TXD       = UART_1_TXD_net_1;
assign UART_0_TXD_net_1 = UART_0_TXD_net_0;
assign UART_0_TXD       = UART_0_TXD_net_1;
assign PWM1_net_1       = PWM1_net_0;
assign PWM1             = PWM1_net_1;
assign PWM2_net_1       = PWM2_net_0;
assign PWM2             = PWM2_net_1;
assign H_IN_net_1       = H_IN_net_0;
assign H_IN[3:0]        = H_IN_net_1;
//--------------------------------------------------------------------
// Bus Interface Nets Assignments - Unequal Pin Widths
//--------------------------------------------------------------------
assign CoreAPB3_0_APBmslave0_PADDR_0_7to0 = CoreAPB3_0_APBmslave0_PADDR[7:0];
assign CoreAPB3_0_APBmslave0_PADDR_0 = { CoreAPB3_0_APBmslave0_PADDR_0_7to0 };

assign VehicleController_MSS_0_MSS_MASTER_APB_PADDR_0_31to20 = 12'h0;
assign VehicleController_MSS_0_MSS_MASTER_APB_PADDR_0_19to0 = VehicleController_MSS_0_MSS_MASTER_APB_PADDR[19:0];
assign VehicleController_MSS_0_MSS_MASTER_APB_PADDR_0 = { VehicleController_MSS_0_MSS_MASTER_APB_PADDR_0_31to20, VehicleController_MSS_0_MSS_MASTER_APB_PADDR_0_19to0 };

//--------------------------------------------------------------------
// Component instances
//--------------------------------------------------------------------
//--------CoreAPB3   -   Actel:DirectCore:CoreAPB3:4.1.100
CoreAPB3 #( 
        .APB_DWIDTH      ( 32 ),
        .APBSLOT0ENABLE  ( 1 ),
        .APBSLOT1ENABLE  ( 0 ),
        .APBSLOT2ENABLE  ( 0 ),
        .APBSLOT3ENABLE  ( 0 ),
        .APBSLOT4ENABLE  ( 0 ),
        .APBSLOT5ENABLE  ( 0 ),
        .APBSLOT6ENABLE  ( 0 ),
        .APBSLOT7ENABLE  ( 0 ),
        .APBSLOT8ENABLE  ( 0 ),
        .APBSLOT9ENABLE  ( 0 ),
        .APBSLOT10ENABLE ( 0 ),
        .APBSLOT11ENABLE ( 0 ),
        .APBSLOT12ENABLE ( 0 ),
        .APBSLOT13ENABLE ( 0 ),
        .APBSLOT14ENABLE ( 0 ),
        .APBSLOT15ENABLE ( 0 ),
        .FAMILY          ( 18 ),
        .IADDR_OPTION    ( 0 ),
        .MADDR_BITS      ( 12 ),
        .SC_0            ( 0 ),
        .SC_1            ( 0 ),
        .SC_2            ( 0 ),
        .SC_3            ( 0 ),
        .SC_4            ( 0 ),
        .SC_5            ( 0 ),
        .SC_6            ( 0 ),
        .SC_7            ( 0 ),
        .SC_8            ( 0 ),
        .SC_9            ( 0 ),
        .SC_10           ( 0 ),
        .SC_11           ( 0 ),
        .SC_12           ( 0 ),
        .SC_13           ( 0 ),
        .SC_14           ( 0 ),
        .SC_15           ( 0 ),
        .UPR_NIBBLE_POSN ( 2 ) )
CoreAPB3_0(
        // Inputs
        .PRESETN    ( GND_net ), // tied to 1'b0 from definition
        .PCLK       ( GND_net ), // tied to 1'b0 from definition
        .PWRITE     ( VehicleController_MSS_0_MSS_MASTER_APB_PWRITE ),
        .PENABLE    ( VehicleController_MSS_0_MSS_MASTER_APB_PENABLE ),
        .PSEL       ( VehicleController_MSS_0_MSS_MASTER_APB_PSELx ),
        .PREADYS0   ( CoreAPB3_0_APBmslave0_PREADY ),
        .PSLVERRS0  ( CoreAPB3_0_APBmslave0_PSLVERR ),
        .PREADYS1   ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS1  ( GND_net ), // tied to 1'b0 from definition
        .PREADYS2   ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS2  ( GND_net ), // tied to 1'b0 from definition
        .PREADYS3   ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS3  ( GND_net ), // tied to 1'b0 from definition
        .PREADYS4   ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS4  ( GND_net ), // tied to 1'b0 from definition
        .PREADYS5   ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS5  ( GND_net ), // tied to 1'b0 from definition
        .PREADYS6   ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS6  ( GND_net ), // tied to 1'b0 from definition
        .PREADYS7   ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS7  ( GND_net ), // tied to 1'b0 from definition
        .PREADYS8   ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS8  ( GND_net ), // tied to 1'b0 from definition
        .PREADYS9   ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS9  ( GND_net ), // tied to 1'b0 from definition
        .PREADYS10  ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS10 ( GND_net ), // tied to 1'b0 from definition
        .PREADYS11  ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS11 ( GND_net ), // tied to 1'b0 from definition
        .PREADYS12  ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS12 ( GND_net ), // tied to 1'b0 from definition
        .PREADYS13  ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS13 ( GND_net ), // tied to 1'b0 from definition
        .PREADYS14  ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS14 ( GND_net ), // tied to 1'b0 from definition
        .PREADYS15  ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS15 ( GND_net ), // tied to 1'b0 from definition
        .PREADYS16  ( VCC_net ), // tied to 1'b1 from definition
        .PSLVERRS16 ( GND_net ), // tied to 1'b0 from definition
        .PADDR      ( VehicleController_MSS_0_MSS_MASTER_APB_PADDR_0 ),
        .PWDATA     ( VehicleController_MSS_0_MSS_MASTER_APB_PWDATA ),
        .PRDATAS0   ( CoreAPB3_0_APBmslave0_PRDATA ),
        .PRDATAS1   ( PRDATAS1_const_net_0 ), // tied to 32'h00000000 from definition
        .PRDATAS2   ( PRDATAS2_const_net_0 ), // tied to 32'h00000000 from definition
        .PRDATAS3   ( PRDATAS3_const_net_0 ), // tied to 32'h00000000 from definition
        .PRDATAS4   ( PRDATAS4_const_net_0 ), // tied to 32'h00000000 from definition
        .PRDATAS5   ( PRDATAS5_const_net_0 ), // tied to 32'h00000000 from definition
        .PRDATAS6   ( PRDATAS6_const_net_0 ), // tied to 32'h00000000 from definition
        .PRDATAS7   ( PRDATAS7_const_net_0 ), // tied to 32'h00000000 from definition
        .PRDATAS8   ( PRDATAS8_const_net_0 ), // tied to 32'h00000000 from definition
        .PRDATAS9   ( PRDATAS9_const_net_0 ), // tied to 32'h00000000 from definition
        .PRDATAS10  ( PRDATAS10_const_net_0 ), // tied to 32'h00000000 from definition
        .PRDATAS11  ( PRDATAS11_const_net_0 ), // tied to 32'h00000000 from definition
        .PRDATAS12  ( PRDATAS12_const_net_0 ), // tied to 32'h00000000 from definition
        .PRDATAS13  ( PRDATAS13_const_net_0 ), // tied to 32'h00000000 from definition
        .PRDATAS14  ( PRDATAS14_const_net_0 ), // tied to 32'h00000000 from definition
        .PRDATAS15  ( PRDATAS15_const_net_0 ), // tied to 32'h00000000 from definition
        .PRDATAS16  ( PRDATAS16_const_net_0 ), // tied to 32'h00000000 from definition
        .IADDR      ( IADDR_const_net_0 ), // tied to 32'h00000000 from definition
        // Outputs
        .PREADY     ( VehicleController_MSS_0_MSS_MASTER_APB_PREADY ),
        .PSLVERR    ( VehicleController_MSS_0_MSS_MASTER_APB_PSLVERR ),
        .PWRITES    ( CoreAPB3_0_APBmslave0_PWRITE ),
        .PENABLES   ( CoreAPB3_0_APBmslave0_PENABLE ),
        .PSELS0     ( CoreAPB3_0_APBmslave0_PSELx ),
        .PSELS1     (  ),
        .PSELS2     (  ),
        .PSELS3     (  ),
        .PSELS4     (  ),
        .PSELS5     (  ),
        .PSELS6     (  ),
        .PSELS7     (  ),
        .PSELS8     (  ),
        .PSELS9     (  ),
        .PSELS10    (  ),
        .PSELS11    (  ),
        .PSELS12    (  ),
        .PSELS13    (  ),
        .PSELS14    (  ),
        .PSELS15    (  ),
        .PSELS16    (  ),
        .PRDATA     ( VehicleController_MSS_0_MSS_MASTER_APB_PRDATA ),
        .PADDRS     ( CoreAPB3_0_APBmslave0_PADDR ),
        .PWDATAS    ( CoreAPB3_0_APBmslave0_PWDATA ) 
        );

//--------PWM
PWM PWM_0(
        // Inputs
        .PCLK    ( VehicleController_MSS_0_FAB_CLK ),
        .PENABLE ( CoreAPB3_0_APBmslave0_PENABLE ),
        .PSEL    ( CoreAPB3_0_APBmslave0_PSELx ),
        .PRESETN ( VehicleController_MSS_0_M2F_RESET_N ),
        .PWRITE  ( CoreAPB3_0_APBmslave0_PWRITE ),
        .PADDR   ( CoreAPB3_0_APBmslave0_PADDR_0 ),
        .PWDATA  ( CoreAPB3_0_APBmslave0_PWDATA ),
        // Outputs
        .PREADY  ( CoreAPB3_0_APBmslave0_PREADY ),
        .PSLVERR ( CoreAPB3_0_APBmslave0_PSLVERR ),
        .PWM1    ( PWM1_net_0 ),
        .PWM2    ( PWM2_net_0 ),
        .PRDATA  ( CoreAPB3_0_APBmslave0_PRDATA ),
        .H_IN    ( H_IN_net_0 ) 
        );

//--------VehicleController_MSS
VehicleController_MSS VehicleController_MSS_0(
        // Inputs
        .UART_0_RXD  ( UART_0_RXD ),
        .UART_1_RXD  ( UART_1_RXD ),
        .MSS_RESET_N ( MSS_RESET_N ),
        .MSSPREADY   ( VehicleController_MSS_0_MSS_MASTER_APB_PREADY ),
        .MSSPSLVERR  ( VehicleController_MSS_0_MSS_MASTER_APB_PSLVERR ),
        .MSSPRDATA   ( VehicleController_MSS_0_MSS_MASTER_APB_PRDATA ),
        // Outputs
        .UART_0_TXD  ( UART_0_TXD_net_0 ),
        .UART_1_TXD  ( UART_1_TXD_net_0 ),
        .FAB_CLK     ( VehicleController_MSS_0_FAB_CLK ),
        .M2F_RESET_N ( VehicleController_MSS_0_M2F_RESET_N ),
        .MSSPSEL     ( VehicleController_MSS_0_MSS_MASTER_APB_PSELx ),
        .MSSPENABLE  ( VehicleController_MSS_0_MSS_MASTER_APB_PENABLE ),
        .MSSPWRITE   ( VehicleController_MSS_0_MSS_MASTER_APB_PWRITE ),
        .MSSPADDR    ( VehicleController_MSS_0_MSS_MASTER_APB_PADDR ),
        .MSSPWDATA   ( VehicleController_MSS_0_MSS_MASTER_APB_PWDATA ) 
        );


endmodule
