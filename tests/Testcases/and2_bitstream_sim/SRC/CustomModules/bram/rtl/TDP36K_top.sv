`include "bram.svh"
// ---- Default Nettype ----
`default_nettype wire
  // Topr level wrapper to multiplex input signals based on operating mode
  // 36, 18, or 9 bit write modes are downgraded to 32,16, or 8 bit when a read
  // port is specified to be 1,2,or 4 bits wide as the parity bit is 
  // in-accesible in 1,2, or 4 bit modes.  The write data parity bit inputs
  // are utilized as low order address bits. via the MUX_xx_i signals
  
  module TDP36K_top 
    (
     input logic         GRESET_i, //  global reset - not user controllable

     input logic         CLK_A1_i, CLK_B1_i, // clocks from global routing
     input logic         CLK_A2_i, CLK_B2_i, // clocks from global routing

     input [19:0]        RAM_ID_i,

//Preload Bus 66 inputs, 66 outputs
     input logic         PL_INIT_i, PL_ENA_i, PL_REN_i, PL_CLK_i,
     input logic [1:0]   PL_WEN_i,
     input logic [31:0]  PL_ADDR_i,
     input logic [35:0]  PL_DATA_i,

     output logic        PL_INIT_o, PL_ENA_o, PL_REN_o, PL_CLK_o,
     output logic [1:0]  PL_WEN_o,
     output logic [31:0] PL_ADDR_o,
     output logic [35:0] PL_DATA_o,

// Scan mode signals
     input logic [43:0]   SCAN_i,
     input logic         SCAN_MODE_i, 
     input logic         SCAN_EN_i, 
     output logic [43:0]  SCAN_o,

     
// Ports for 1st 18K RAMfifo    69 Inputs - 36 outputs
     input logic         FLUSH1_i,
     input logic [1:0]   BE_A1_i, BE_B1_i, // byte write enables
     input logic         WEN_A1_i, WEN_B1_i,
     input logic         REN_A1_i, REN_B1_i,
     input logic [14:0]  ADDR_A1_i, // bits 0 and 1 come from MUX
     input logic [14:0]  ADDR_B1_i,
     input logic [17:0]  WDATA_A1_i, WDATA_B1_i, // bits 16, 17 come from MUX 
     output logic [17:0] RDATA_A1_o, RDATA_B1_o,



// Ports for 2nd 18K RAMfifo  67 inputs 36 outputs
     input logic         FLUSH2_i,
     input logic [1:0]   BE_A2_i, BE_B2_i, // byte write enables
     input logic         WEN_A2_i, WEN_B2_i,
     input logic         REN_A2_i, REN_B2_i,
     input logic [13:0]  ADDR_A2_i, ADDR_B2_i, // bits 0, 1 come from MUX
     input logic [17:0]  WDATA_A2_i, WDATA_B2_i, // bits 16,17 come from MUX
     output logic [17:0] RDATA_A2_o, RDATA_B2_o,


// Common CCF bits     
     input               SPLIT_i, 
//CCF Bits specific to RAMFIFO 2  35 inputs		  		  
     input logic         SYNC_FIFO1_i,
     input logic [2:0]   RMODE_A1_i, RMODE_B1_i,
     input logic [2:0]   WMODE_A1_i, WMODE_B1_i,
     input logic         FMODE1_i, 
     input logic         POWERDN1_i, 
     input logic         SLEEP1_i,
     input logic         PROTECT1_i,
     input logic [11:0]  UPAE1_i,
     input logic [11:0]  UPAF1_i,
		  
//CCF Bits specific to RAMFIFO 2  35 inputs		  
     input logic         SYNC_FIFO2_i,
     input logic [2:0]   RMODE_A2_i, RMODE_B2_i,
     input logic [2:0]   WMODE_A2_i, WMODE_B2_i,
     input logic         FMODE2_i, 
     input logic         POWERDN2_i, 
     input logic         SLEEP2_i,
     input logic         PROTECT2_i,
     input logic [10:0]  UPAE2_i,
     input logic [10:0]  UPAF2_i,
     input logic [2:0] rwm
);


   logic                   reset_ni;
   logic                   flush1, flush2;
   
   assign reset_ni =  ~GRESET_i;
   assign flush1 = FLUSH1_i;
   assign flush2 = FLUSH2_i;
   TDP36K u0 (
              .RESET_ni(reset_ni),
              .SCAN_MODE_i(SCAN_MODE_i),
               .WEN_A1_i(WEN_A1_i),
               .WEN_B1_i(WEN_B1_i),
               .REN_A1_i(REN_A1_i),
               .REN_B1_i(REN_B1_i),
               .CLK_A1_i(CLK_A1_i),
               .CLK_B1_i(CLK_B1_i),
               .BE_A1_i(BE_A1_i),
               .BE_B1_i(BE_B1_i),
               .ADDR_A1_i(ADDR_A1_i),
               .ADDR_B1_i(ADDR_B1_i),
               .WDATA_A1_i(WDATA_A1_i),
               .WDATA_B1_i(WDATA_B1_i),
               .RDATA_A1_o({RDATA_A1_o}),
               .RDATA_B1_o({RDATA_B1_o}),
               .FLUSH1_i(FLUSH1_i),
               .SYNC_FIFO1_i(SYNC_FIFO1_i),
               .RMODE_A1_i(RMODE_A1_i),
               .RMODE_B1_i(RMODE_B1_i),
               .WMODE_A1_i(WMODE_A1_i),
               .WMODE_B1_i(WMODE_B1_i),
               .FMODE1_i(FMODE1_i),
               .POWERDN1_i(POWERDN1_i),
               .SLEEP1_i(SLEEP1_i),
               .PROTECT1_i(PROTECT1_i),
               .UPAE1_i(UPAE1_i),
               .UPAF1_i(UPAF1_i),
               .WEN_A2_i(WEN_A2_i),
               .WEN_B2_i(WEN_B2_i),
               .REN_A2_i(REN_A2_i),
               .REN_B2_i(REN_B2_i),
               .CLK_A2_i(CLK_A2_i),
               .CLK_B2_i(CLK_B2_i),
               .BE_A2_i(BE_A2_i),
               .BE_B2_i(BE_B2_i),
               .ADDR_A2_i({ADDR_A2_i}),
               .ADDR_B2_i({ADDR_B2_i}),
               .WDATA_A2_i({WDATA_A2_i}),
               .WDATA_B2_i({WDATA_B2_i}),
               .RDATA_A2_o({RDATA_A2_o}),
               .RDATA_B2_o({RDATA_B2_o}),
               .FLUSH2_i(FLUSH2_i),
               .SYNC_FIFO2_i(SYNC_FIFO2_i),
               .RMODE_A2_i(RMODE_A2_i),
               .RMODE_B2_i(RMODE_B2_i),
               .WMODE_A2_i(WMODE_A2_i),
               .WMODE_B2_i(WMODE_B2_i),
               .FMODE2_i(FMODE2_i),
               .POWERDN2_i(POWERDN2_i),
               .SLEEP2_i(SLEEP2_i),
               .PROTECT2_i(PROTECT2_i),
               .UPAE2_i(UPAE2_i),
               .UPAF2_i(UPAF2_i),
               .SPLIT_i(SPLIT_i),
               .RAM_ID_i(RAM_ID_i),
               .PL_INIT_i(PL_INIT_i),
               .PL_ENA_i(PL_ENA_i),
               .PL_REN_i(PL_REN_i),
               .PL_CLK_i(PL_CLK_i),
               .PL_WEN_i(PL_WEN_i),
               .PL_ADDR_i(PL_ADDR_i),
               .PL_DATA_i(PL_DATA_i),
               .PL_INIT_o(PL_INIT_o),
               .PL_ENA_o(PL_ENA_o),
               .PL_REN_o(PL_REN_o),
               .PL_CLK_o(PL_CLK_o),
               .PL_WEN_o(PL_WEN_o),
               .PL_ADDR_o(PL_ADDR_o),
               .PL_DATA_o(PL_DATA_o),
               .rwm(rwm)
            );
endmodule
`default_nettype none
  
