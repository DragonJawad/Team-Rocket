//////////////////////////////////////////////////////////////////////
// Created by SmartDesign Sun Mar 20 22:58:23 2016
// Version: v11.5 SP3 11.5.3.10
//////////////////////////////////////////////////////////////////////

`timescale 1 ns/100 ps

// xBeeTransmit
module xBeeTransmit(
    // Inputs
    MSS_RESET_N,
    UART_0_RXD,
    UART_1_RXD,
    // Outputs
    UART_0_TXD,
    UART_1_TXD
);

//--------------------------------------------------------------------
// Input
//--------------------------------------------------------------------
input  MSS_RESET_N;
input  UART_0_RXD;
input  UART_1_RXD;
//--------------------------------------------------------------------
// Output
//--------------------------------------------------------------------
output UART_0_TXD;
output UART_1_TXD;
//--------------------------------------------------------------------
// Nets
//--------------------------------------------------------------------
wire   MSS_RESET_N;
wire   UART_0_RXD;
wire   UART_0_TXD_net_0;
wire   UART_1_RXD;
wire   UART_1_TXD_net_0;
wire   UART_1_TXD_net_1;
wire   UART_0_TXD_net_1;
//--------------------------------------------------------------------
// Top level output port assignments
//--------------------------------------------------------------------
assign UART_1_TXD_net_1 = UART_1_TXD_net_0;
assign UART_1_TXD       = UART_1_TXD_net_1;
assign UART_0_TXD_net_1 = UART_0_TXD_net_0;
assign UART_0_TXD       = UART_0_TXD_net_1;
//--------------------------------------------------------------------
// Component instances
//--------------------------------------------------------------------
//--------xBeeTransmit_MSS
xBeeTransmit_MSS xBeeTransmit_MSS_0(
        // Inputs
        .UART_0_RXD  ( UART_0_RXD ),
        .UART_1_RXD  ( UART_1_RXD ),
        .MSS_RESET_N ( MSS_RESET_N ),
        // Outputs
        .UART_0_TXD  ( UART_0_TXD_net_0 ),
        .UART_1_TXD  ( UART_1_TXD_net_0 ) 
        );


endmodule
