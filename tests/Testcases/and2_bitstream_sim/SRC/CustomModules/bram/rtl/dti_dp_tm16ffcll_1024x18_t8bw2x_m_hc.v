/********************************************************************/
/*  Copyright 1998 - 2022 Dolphin Technology, Inc.                  */
/*  This memory compiler and any data created by it are proprietary */
/*  and confidential information of Dolphin Technology, Inc. and    */
/*  can only be used or viewed with written permission from         */
/*  Dolphin Technology, Inc.                                        */
/*  tsmc16nmffcll, version 1p1p53 Rev_1.1                           */
/********************************************************************/

`timescale 1ns/1ps

`undef  ISOLATION
`undef  CLKINV
`define BIT_WRITE
`define BIST_TEST
`undef  ASYNCHRONOUS_WRITE
`undef  OUTPUT_ENABLE
`undef  LOW_LEAK1
`undef  LOW_LEAK2
`undef  PWR_GATE1
`undef  PWR_GATE2
`undef  WRITE_ASSIST
`undef  READ_ASSIST
`undef  COL_RED
`undef  ROW_RED

`define SDFVERSION_2
`undef  SDFVERSION_3
//`define SDFVERSION_3
//`undef  SDFVERSION_2
//****** Please choose the SDF Version to be used . Default is set to  SDFVERSION_2 (Version 2.0) which defines $setup and $hold seperately. Select SDFVERSION_3 for $setuphold ******

`define COLLISION
`celldefine

module dti_dp_tm16ffcll_1024x18_t8bw2x_m_hc (DO_A, DO_B, A_A, A_B, T_A_A, T_A_B, DI_A, DI_B, T_DI_A, 
       T_DI_B, T_BE_N, CE_N_A, CE_N_B, T_CE_N_A, T_CE_N_B, GWE_N_A, GWE_N_B, T_GWE_N_A, T_GWE_N_B, 
       BWE_N_A, BWE_N_B, T_BWE_N_A, T_BWE_N_B, T_RWM_A, T_RWM_B, CLK_A, CLK_B);
output [17:0] DO_A;             // Port A Data Output
output [17:0] DO_B;             // Port B Data Output
input  [9:0] A_A;               // Port A Address
input  [9:0] A_B;               // Port B Address
input  [9:0] T_A_A;             // Port A Bist Address
input  [9:0] T_A_B;             // Port B Bist Address
input  [17:0] DI_A;             // Port A Data Input
input  [17:0] DI_B;             // Port B Data Input
input  [17:0] T_DI_A;           // Port A Bist Data Input
input  [17:0] T_DI_B;           // Port B Bist Data Input
input  T_BE_N;                  // Bist Enable --- Active Low
input  CE_N_A;                  // Port A Chip Select Enable --- Active Low
input  CE_N_B;                  // Port B Chip Select Enable --- Active Low
input  T_CE_N_A;                // Port A Bist Chip Select Enable --- Active Low
input  T_CE_N_B;                // Port B Bist Chip Select Enable --- Active Low
input  GWE_N_A;                 // Port A Global Write Enable --- Active Low
input  GWE_N_B;                 // Port B Global Write Enable --- Active Low
input  T_GWE_N_A;               // Port A Bist Global Write Enable --- Active Low
input  T_GWE_N_B;               // Port B Bist Global Write Enable --- Active Low
input  [17:0] BWE_N_A;          // Port A Bit Write Enable --- Active Low
input  [17:0] BWE_N_B;          // Port B Bit Write Enable --- Active Low
input  [17:0] T_BWE_N_A;        // Port A Bist Bit Write Enable --- Active Low
input  [17:0] T_BWE_N_B;        // Port B Bist Bit Write Enable --- Active Low
input  [2:0] T_RWM_A;           // Port A Adjustment for Sense Amp delay
input  [2:0] T_RWM_B;           // Port B Adjustment for Sense Amp delay
input  CLK_A;                   // Port A Clock
input  CLK_B;                   // Port B Clock

wire CLK_A_INT;                     // Port A Clock --- Active Low
wire CLK_B_INT;                     // Port B Clock --- Active Low
wire CE_N_A_INT;                    // Port A Chip Select Enable --- Active Low
wire CE_N_B_INT;                    // Port B Chip Select Enable --- Active Low
wire GWE_N_A_INT;                   // Port A Global Write Enable --- Active Low
wire GWE_N_B_INT;                   // Port B Global Write Enable --- Active Low
wire [9:0] A_A_INT;                // Port A Address
wire [9:0] A_B_INT;                // Port B Address
wire [17:0] DI_A_INT;               // Port A Data Input
wire [17:0] DI_B_INT;               // Port B Data Input
wire [17:0] BWE_N_A_INT;            // Port A Bit Write Enable --- Active Low
wire [17:0] BWE_N_B_INT;            // Port B Bit Write Enable --- Active Low
wire [17:0] BYWE_N_A_INT;           // Port A Byte Write Enable --- Active Low
wire [17:0] BYWE_N_B_INT;           // Port B Byte Write Enable --- Active Low
wire OE_N_A_INT;                    // Port A Output Enable --- Active Low
wire OE_N_B_INT;                    // Port B Output Enable --- Active Low
wire T_AWT_N_A_INT;                 // Port A Asynchronous Test Write Through --- Active Low
wire T_AWT_N_B_INT;                 // Port B Asynchronous Test Write Through --- Active Low
wire [2:0] T_RWM_A_INT;             // Port A Adjustment for Sense Amp delay
wire [2:0] T_RWM_B_INT;             // Port B Adjustment for Sense Amp delay
wire [2:0] T_DLY_A_INT;             // Port A Adjustment for Write Assist delay
wire [2:0] T_DLY_B_INT;             // Port B Adjustment for Write Assist delay
wire [1:0] DS_INT;                // Adjustment for Memory Supply Voltage when deep sleep mode
wire LOLEAK_N_INT;                // Low Leak Enable for Logic --- Active Low
wire LKRB_N_INT;                  // Low Leak Enable for Memory Array --- Active Low
wire COREPWS_N_INT;               // Power Down Enable for Memory Array --- Active Low
wire P_PWS_N_INT;                 // Power Down Enable for Logic --- Active Low
wire T_BE_N_INT;                  // Bist Enable --- Active Low
wire T_CE_N_A_INT;                  // Port A Bist Chip Select Enable --- Active Low
wire T_CE_N_B_INT;                  // Port B Bist Chip Select Enable --- Active Low
wire T_GWE_N_A_INT;                 // Port A Bist Global Write Enable --- Active Low
wire T_GWE_N_B_INT;                 // Port B Bist Global Write Enable --- Active Low
wire [9:0] T_A_A_INT;              // Port A Bist Address
wire [9:0] T_A_B_INT;              // Port B Bist Address
wire [17:0] T_DI_A_INT;             // Port A Bist Data Input
wire [17:0] T_DI_B_INT;             // Port B Bist Data Input
wire [17:0] T_BWE_N_A_INT;          // Port A Bist Bit Write Enable --- Active Low
wire [17:0] T_BWE_N_B_INT;          // Port A Bist Bit Write Enable --- Active Low
wire T_OE_N_A_INT;                    // Port A Bist Output Enable --- Active Low
wire T_OE_N_B_INT;                    // Port B Bist Output Enable --- Active Low
//Internal registers
wire  cntrlA;
wire  cntrlB;
wire  [17:0] Dout_A;
wire  [17:0] Dout_B;
wire  [17:0] Dout_A_INT;
wire  [17:0] Dout_B_INT;
wire  [17:0] DO_temp_A;
wire  [17:0] DO_temp_B;
reg   [17:0] DataIn_R_A;
reg   [17:0] DataIn_R_B;
reg   [9:0] Address_R_A;
reg   [9:0] Address_R_B;
reg   GWe_R_A;
reg   GWe_R_B;
reg   CE_N_R_A;
reg   CE_N_R_B;
reg   Clock_A;
reg   Clock_B;
reg   [17:0] memArray [1023:0];
reg   [17:0] Dout_TR_A;
reg   [17:0] Dout_TR_B;
wire condition_pwr0;            // Light sleep
wire condition_pwr1;            // Deep sleep
wire condition_pwr2;            // PG retention
wire condition_pwr3;            // Shutdown

reg   [17:0] tmpDataIn_R_A;
reg   [17:0] tmpDataIn_R_B;
reg   [17:0] DataIn_Reg;
reg   [17:0] We_R_A;
reg   [17:0] We_R_B;
reg   [17:0] tmpDataIn_R_W_A;
reg   [17:0] tmpDataIn_R_W_B;
integer  i_A;
integer  i_B;
integer  j_A;
integer  j_B;
integer  ok;
integer  ok_a;
integer  ok_b;
integer  in_A;
integer  in_B;
reg   coll_w_a;
reg   coll_w_b;
reg   coll_r_a;
reg   coll_r_b;
reg   flag_wr_portA;
reg   flag_wr_portB;
reg   flag_rd_portA;
reg   flag_rd_portB;
integer  m;
// Isolation block
`ifdef ISOLATION
  `ifdef CLKINV
    assign CLK_A_INT           = (ISOL_N===1'b0) ? 1'b0 : ( (ISOL_N===1'b1) ? ((condition_pwr2 || condition_pwr3) ? 0: ~CLK_A) : 'bx );
    assign CLK_B_INT           = (ISOL_N===1'b0) ? 1'b0 : ( (ISOL_N===1'b1) ? ((condition_pwr2 || condition_pwr3) ? 0: ~CLK_B) : 'bx );
  `else
    assign CLK_A_INT           = (ISOL_N===1'b0) ? 1'b0 : ( (ISOL_N===1'b1) ? ((condition_pwr2 || condition_pwr3) ? 0:  CLK_A) : 'bx );
    assign CLK_B_INT           = (ISOL_N===1'b0) ? 1'b0 : ( (ISOL_N===1'b1) ? ((condition_pwr2 || condition_pwr3) ? 0:  CLK_B) : 'bx );
  `endif
  assign CE_N_A_INT          = (ISOL_N===1'b0) ? 1'b1        : ( (ISOL_N===1'b1) ? CE_N_A         : 'bx );
  assign CE_N_B_INT          = (ISOL_N===1'b0) ? 1'b1        : ( (ISOL_N===1'b1) ? CE_N_B         : 'bx );
  assign GWE_N_A_INT         = (ISOL_N===1'b0) ? 1'b1        : ( (ISOL_N===1'b1) ? GWE_N_A        : 'bx );
  assign GWE_N_B_INT         = (ISOL_N===1'b0) ? 1'b1        : ( (ISOL_N===1'b1) ? GWE_N_B        : 'bx );
  assign A_A_INT             = (ISOL_N===1'b0) ? {9{1'b0}}  : ( (ISOL_N===1'b1) ? A_A            : 'bx );
  assign A_B_INT             = (ISOL_N===1'b0) ? {9{1'b0}}  : ( (ISOL_N===1'b1) ? A_B            : 'bx );
  assign DI_A_INT            = (ISOL_N===1'b0) ? {18{1'b0}}  : ( (ISOL_N===1'b1) ? DI_A           : 'bx );
  assign DI_B_INT            = (ISOL_N===1'b0) ? {18{1'b0}}  : ( (ISOL_N===1'b1) ? DI_B           : 'bx );
  `ifdef BIT_WRITE
    assign BWE_N_A_INT       = (ISOL_N===1'b0) ? {18{1'b1}}  : ( (ISOL_N===1'b1) ? BWE_N_A        : 'bx );
    assign BWE_N_B_INT       = (ISOL_N===1'b0) ? {18{1'b1}}  : ( (ISOL_N===1'b1) ? BWE_N_B        : 'bx );
  `else
    `ifdef BYTE_WRITE
      assign BWE_N_A_INT[0]  =  BYWE_N_A[0] ;
      assign BWE_N_B_INT[0]  =  BYWE_N_B[0] ;
      assign BWE_N_A_INT[1]  =  BYWE_N_A[0] ;
      assign BWE_N_B_INT[1]  =  BYWE_N_B[0] ;
      assign BWE_N_A_INT[2]  =  BYWE_N_A[0] ;
      assign BWE_N_B_INT[2]  =  BYWE_N_B[0] ;
      assign BWE_N_A_INT[3]  =  BYWE_N_A[0] ;
      assign BWE_N_B_INT[3]  =  BYWE_N_B[0] ;
      assign BWE_N_A_INT[4]  =  BYWE_N_A[0] ;
      assign BWE_N_B_INT[4]  =  BYWE_N_B[0] ;
      assign BWE_N_A_INT[5]  =  BYWE_N_A[0] ;
      assign BWE_N_B_INT[5]  =  BYWE_N_B[0] ;
      assign BWE_N_A_INT[6]  =  BYWE_N_A[0] ;
      assign BWE_N_B_INT[6]  =  BYWE_N_B[0] ;
      assign BWE_N_A_INT[7]  =  BYWE_N_A[0] ;
      assign BWE_N_B_INT[7]  =  BYWE_N_B[0] ;
      assign BWE_N_A_INT[8]  =  BYWE_N_A[1] ;
      assign BWE_N_B_INT[8]  =  BYWE_N_B[1] ;
      assign BWE_N_A_INT[9]  =  BYWE_N_A[1] ;
      assign BWE_N_B_INT[9]  =  BYWE_N_B[1] ;
      assign BWE_N_A_INT[10]  =  BYWE_N_A[1] ;
      assign BWE_N_B_INT[10]  =  BYWE_N_B[1] ;
      assign BWE_N_A_INT[11]  =  BYWE_N_A[1] ;
      assign BWE_N_B_INT[11]  =  BYWE_N_B[1] ;
      assign BWE_N_A_INT[12]  =  BYWE_N_A[1] ;
      assign BWE_N_B_INT[12]  =  BYWE_N_B[1] ;
      assign BWE_N_A_INT[13]  =  BYWE_N_A[1] ;
      assign BWE_N_B_INT[13]  =  BYWE_N_B[1] ;
      assign BWE_N_A_INT[14]  =  BYWE_N_A[1] ;
      assign BWE_N_B_INT[14]  =  BYWE_N_B[1] ;
      assign BWE_N_A_INT[15]  =  BYWE_N_A[1] ;
      assign BWE_N_B_INT[15]  =  BYWE_N_B[1] ;
      assign BWE_N_A_INT[16]  =  BYWE_N_A[2] ;
      assign BWE_N_B_INT[16]  =  BYWE_N_B[2] ;
      assign BWE_N_A_INT[17]  =  BYWE_N_A[2] ;
      assign BWE_N_B_INT[17]  =  BYWE_N_B[2] ;
    `else
      assign BWE_N_A_INT       = (ISOL_N===1'b0) ? {18{1'b1}}  : ( (ISOL_N===1'b1) ? 'b0          : 'bx );
      assign BWE_N_B_INT       = (ISOL_N===1'b0) ? {18{1'b1}}  : ( (ISOL_N===1'b1) ? 'b0          : 'bx );
    `endif
  `endif
  `ifdef OUTPUT_ENABLE
    assign OE_N_A_INT        = (ISOL_N===1'b0) ? 1'b1        : ( (ISOL_N===1'b1) ? OE_N_A         : 'bx );
    assign OE_N_B_INT        = (ISOL_N===1'b0) ? 1'b1        : ( (ISOL_N===1'b1) ? OE_N_B         : 'bx );
  `else
    assign OE_N_A_INT        = 1'b0;
    assign OE_N_B_INT        = 1'b0;
  `endif
  `ifdef ASYNCHRONOUS_WRITE
    assign T_AWT_N_A_INT     = (ISOL_N===1'b0) ? 1'b1        : ( (ISOL_N===1'b1) ? T_AWT_N_A      : 'bx );
    assign T_AWT_N_B_INT     = (ISOL_N===1'b0) ? 1'b1        : ( (ISOL_N===1'b1) ? T_AWT_N_B      : 'bx );
  `else
    assign T_AWT_N_A_INT     = 1'b1;
    assign T_AWT_N_B_INT     = 1'b1;
  `endif
   assign T_RWM_A_INT       = (ISOL_N===1'b0) ? 3'b011      : ( (ISOL_N===1'b1) ? T_RWM_A        : 'bx );
   assign T_RWM_B_INT       = (ISOL_N===1'b0) ? 3'b011      : ( (ISOL_N===1'b1) ? T_RWM_B        : 'bx );
  `ifdef WRITE_ASSIST
    assign T_DLY_A_INT       = (ISOL_N===1'b0) ? {3{1'b0}}   : ( (ISOL_N===1'b1) ? T_DLY_A        : 'bx );
    assign T_DLY_B_INT       = (ISOL_N===1'b0) ? {3{1'b0}}   : ( (ISOL_N===1'b1) ? T_DLY_B        : 'bx );
  `else
    assign T_DLY_A_INT       = (ISOL_N===1'b0) ? {3{1'b0}}   : ( (ISOL_N===1'b1) ? 'b0          : 'bx );
    assign T_DLY_B_INT       = (ISOL_N===1'b0) ? {3{1'b0}}   : ( (ISOL_N===1'b1) ? 'b0          : 'bx );
  `endif
  `ifdef LOW_LEAK1
    assign LOLEAK_N_INT    = (ISOL_N===1'b0) ? 1'b1        : ( (ISOL_N===1'b1) ? LOLEAK_N     : 'bx );
    assign LKRB_N_INT      = (ISOL_N===1'b0) ? 1'b1        : ( (ISOL_N===1'b1) ? LKRB_N       : 'bx );
    assign DS_INT[0]        = (ISOL_N===1'b0) ? 1'b1        : ( (ISOL_N===1'b1) ? DS[0]       : 'bx );
    assign DS_INT[1]        = (ISOL_N===1'b0) ? 1'b1        : ( (ISOL_N===1'b1) ? DS[1]       : 'bx );
  `else
    `ifdef LOW_LEAK2
      assign LOLEAK_N_INT    = (ISOL_N===1'b0) ? 1'b1      : ( (ISOL_N===1'b1) ? LOLEAK_N     : 'bx );
      assign LKRB_N_INT      = (ISOL_N===1'b0) ? 1'b1      : ( (ISOL_N===1'b1) ? 'b1          : 'bx );
      assign DS_INT[0]        = (ISOL_N===1'b0) ? 1'b1        : ( (ISOL_N===1'b1) ? DS[0]       : 'bx );
      assign DS_INT[1]        = (ISOL_N===1'b0) ? 1'b1        : ( (ISOL_N===1'b1) ? DS[1]       : 'bx );
    `else
      assign LOLEAK_N_INT    = (ISOL_N===1'b0) ? 1'b1      : ( (ISOL_N===1'b1) ? 'b1          : 'bx );
      assign LKRB_N_INT      = (ISOL_N===1'b0) ? 1'b1      : ( (ISOL_N===1'b1) ? 'b1          : 'bx );
    `endif
  `endif
  `ifdef PWR_GATE1
    assign COREPWS_N_INT   = (ISOL_N===1'b0) ? 1'b1        : ( (ISOL_N===1'b1) ? COREPWS_N    : 'bx );
    assign P_PWS_N_INT     = (ISOL_N===1'b0) ? 1'b1        : ( (ISOL_N===1'b1) ? P_PWS_N      : 'bx );
  `else
    `ifdef PWR_GATE2
      assign COREPWS_N_INT   = (ISOL_N===1'b0) ? 1'b1      : ( (ISOL_N===1'b1) ? 'b1          : 'bx );
      assign P_PWS_N_INT     = (ISOL_N===1'b0) ? 1'b1      : ( (ISOL_N===1'b1) ? P_PWS_N      : 'bx );
    `else
      assign COREPWS_N_INT   = (ISOL_N===1'b0) ? 1'b1      : ( (ISOL_N===1'b1) ? 'b1          : 'bx );
      assign P_PWS_N_INT     = (ISOL_N===1'b0) ? 1'b1      : ( (ISOL_N===1'b1) ? 'b1          : 'bx );
    `endif
  `endif
  `ifdef ROW_RED
    assign RENF_INT        = (ISOL_N===1'b0) ? {1'b0}      : ( (ISOL_N===1'b1) ? RENF         : 'bx );
    assign RENS_INT        = (ISOL_N===1'b0) ? {1'b0}      : ( (ISOL_N===1'b1) ? RENS         : 'bx );
    assign RRAF_INT          = (ISOL_N===1'b0) ? {10{1'b0}}  : ( (ISOL_N===1'b1) ? RRAF         : 'bx );
    assign RRAS_INT          = (ISOL_N===1'b0) ? {10{1'b0}}  : ( (ISOL_N===1'b1) ? RRAS         : 'bx );
  `else
    assign RENF_INT            = (ISOL_N===1'b0) ? {1'b0}      : ( (ISOL_N===1'b1) ? 'b0          : 'bx );
    assign RENS_INT            = (ISOL_N===1'b0) ? {1'b0}      : ( (ISOL_N===1'b1) ? 'b0          : 'bx );
    assign RRAF_INT          = (ISOL_N===1'b0) ? {10{1'b0}}  : ( (ISOL_N===1'b1) ? 'b0          : 'bx );
    assign RRAS_INT          = (ISOL_N===1'b0) ? {10{1'b0}}  : ( (ISOL_N===1'b1) ? 'b0          : 'bx );
  `endif
  `ifdef COL_RED
    assign CRAL_INT        = (ISOL_N===1'b0) ? {1'b0}      : ( (ISOL_N===1'b1) ? CRAL         : 'bx );
    assign CRAR_INT        = (ISOL_N===1'b0) ? {1'b0}      : ( (ISOL_N===1'b1) ? CRAR         : 'bx );
  `else
    assign CRAL_INT        = (ISOL_N===1'b0) ? {1'b0}      : ( (ISOL_N===1'b1) ? 'b0          : 'bx );
    assign CRAR_INT        = (ISOL_N===1'b0) ? {1'b0}      : ( (ISOL_N===1'b1) ? 'b0          : 'bx );
  `endif
  `ifdef BIST_TEST
    assign T_BE_N_INT        = (ISOL_N===1'b0) ? 1'b1        : ( (ISOL_N===1'b1) ? T_BE_N       : 'bx );
    assign T_CE_N_A_INT      = (ISOL_N===1'b0) ? 1'b1        : ( (ISOL_N===1'b1) ? T_CE_N_A       : 'bx );
    assign T_CE_N_B_INT      = (ISOL_N===1'b0) ? 1'b1        : ( (ISOL_N===1'b1) ? T_CE_N_B       : 'bx );
    assign T_GWE_N_A_INT     = (ISOL_N===1'b0) ? 1'b1        : ( (ISOL_N===1'b1) ? T_GWE_N_A      : 'bx );
    assign T_GWE_N_B_INT     = (ISOL_N===1'b0) ? 1'b1        : ( (ISOL_N===1'b1) ? T_GWE_N_B      : 'bx );
    assign T_A_A_INT         = (ISOL_N===1'b0) ? {9{1'b0}}  : ( (ISOL_N===1'b1) ? T_A_A          : 'bx );
    assign T_A_B_INT         = (ISOL_N===1'b0) ? {9{1'b0}}  : ( (ISOL_N===1'b1) ? T_A_B          : 'bx );
    assign T_DI_A_INT        = (ISOL_N===1'b0) ? {18{1'b0}}  : ( (ISOL_N===1'b1) ? T_DI_A         : 'bx );
    assign T_DI_B_INT        = (ISOL_N===1'b0) ? {18{1'b0}}  : ( (ISOL_N===1'b1) ? T_DI_B         : 'bx );
    `ifdef BIT_WRITE
      assign T_BWE_N_A_INT     = (ISOL_N===1'b0) ? {18{1'b1}}  : ( (ISOL_N===1'b1) ? T_BWE_N_A        : 'bx );
      assign T_BWE_N_B_INT     = (ISOL_N===1'b0) ? {18{1'b1}}  : ( (ISOL_N===1'b1) ? T_BWE_N_B        : 'bx );
    `else
      `ifdef BYTE_WRITE
        assign T_BWE_N_A_INT[0]  =  T_BYWE_N_A[0] ;
        assign T_BWE_N_B_INT[0]  =  T_BYWE_N_B[0] ;
        assign T_BWE_N_A_INT[1]  =  T_BYWE_N_A[0] ;
        assign T_BWE_N_B_INT[1]  =  T_BYWE_N_B[0] ;
        assign T_BWE_N_A_INT[2]  =  T_BYWE_N_A[0] ;
        assign T_BWE_N_B_INT[2]  =  T_BYWE_N_B[0] ;
        assign T_BWE_N_A_INT[3]  =  T_BYWE_N_A[0] ;
        assign T_BWE_N_B_INT[3]  =  T_BYWE_N_B[0] ;
        assign T_BWE_N_A_INT[4]  =  T_BYWE_N_A[0] ;
        assign T_BWE_N_B_INT[4]  =  T_BYWE_N_B[0] ;
        assign T_BWE_N_A_INT[5]  =  T_BYWE_N_A[0] ;
        assign T_BWE_N_B_INT[5]  =  T_BYWE_N_B[0] ;
        assign T_BWE_N_A_INT[6]  =  T_BYWE_N_A[0] ;
        assign T_BWE_N_B_INT[6]  =  T_BYWE_N_B[0] ;
        assign T_BWE_N_A_INT[7]  =  T_BYWE_N_A[0] ;
        assign T_BWE_N_B_INT[7]  =  T_BYWE_N_B[0] ;
        assign T_BWE_N_A_INT[8]  =  T_BYWE_N_A[1] ;
        assign T_BWE_N_B_INT[8]  =  T_BYWE_N_B[1] ;
        assign T_BWE_N_A_INT[9]  =  T_BYWE_N_A[1] ;
        assign T_BWE_N_B_INT[9]  =  T_BYWE_N_B[1] ;
        assign T_BWE_N_A_INT[10]  =  T_BYWE_N_A[1] ;
        assign T_BWE_N_B_INT[10]  =  T_BYWE_N_B[1] ;
        assign T_BWE_N_A_INT[11]  =  T_BYWE_N_A[1] ;
        assign T_BWE_N_B_INT[11]  =  T_BYWE_N_B[1] ;
        assign T_BWE_N_A_INT[12]  =  T_BYWE_N_A[1] ;
        assign T_BWE_N_B_INT[12]  =  T_BYWE_N_B[1] ;
        assign T_BWE_N_A_INT[13]  =  T_BYWE_N_A[1] ;
        assign T_BWE_N_B_INT[13]  =  T_BYWE_N_B[1] ;
        assign T_BWE_N_A_INT[14]  =  T_BYWE_N_A[1] ;
        assign T_BWE_N_B_INT[14]  =  T_BYWE_N_B[1] ;
        assign T_BWE_N_A_INT[15]  =  T_BYWE_N_A[1] ;
        assign T_BWE_N_B_INT[15]  =  T_BYWE_N_B[1] ;
        assign T_BWE_N_A_INT[16]  =  T_BYWE_N_A[2] ;
        assign T_BWE_N_B_INT[16]  =  T_BYWE_N_B[2] ;
        assign T_BWE_N_A_INT[17]  =  T_BYWE_N_A[2] ;
        assign T_BWE_N_B_INT[17]  =  T_BYWE_N_B[2] ;
      `else
        assign T_BWE_N_B_INT       = (ISOL_N===1'b0) ? {18{1'b1}}  : ( (ISOL_N===1'b1) ? 'b0          : 'bx );
        assign T_BWE_N_B_INT       = (ISOL_N===1'b0) ? {18{1'b1}}  : ( (ISOL_N===1'b1) ? 'b0          : 'bx );
      `endif
    `endif
    `ifdef OUTPUT_ENABLE
      assign T_OE_N_A_INT    = (ISOL_N===1'b0) ? 1'b1        : ( (ISOL_N===1'b1) ? T_OE_N_A       : 'bx );
      assign T_OE_N_B_INT    = (ISOL_N===1'b0) ? 1'b1        : ( (ISOL_N===1'b1) ? T_OE_N_B       : 'bx );
    `else
      assign T_OE_N_A_INT    = 'b0;
      assign T_OE_N_B_INT    = 'b0;
    `endif
  `else
    assign T_BE_N_INT        = (ISOL_N===1'b0) ? 1'b1        : ( (ISOL_N===1'b1) ? 'b1          : 'bx );
    assign T_CE_N_A_INT      = (ISOL_N===1'b0) ? 1'b1        : ( (ISOL_N===1'b1) ? 'b1          : 'bx );
    assign T_CE_N_B_INT      = (ISOL_N===1'b0) ? 1'b1        : ( (ISOL_N===1'b1) ? 'b1          : 'bx );
    assign T_GWE_N_A_INT     = (ISOL_N===1'b0) ? 1'b1        : ( (ISOL_N===1'b1) ? 'b1          : 'bx );
    assign T_GWE_N_B_INT     = (ISOL_N===1'b0) ? 1'b1        : ( (ISOL_N===1'b1) ? 'b1          : 'bx );
    assign T_A_A_INT         = (ISOL_N===1'b0) ? {9{1'b0}}  : ( (ISOL_N===1'b1) ? 'b0          : 'bx );
    assign T_A_B_INT         = (ISOL_N===1'b0) ? {9{1'b0}}  : ( (ISOL_N===1'b1) ? 'b0          : 'bx );
    assign T_DI_A_INT        = (ISOL_N===1'b0) ? {18{1'b0}}  : ( (ISOL_N===1'b1) ? 'b0          : 'bx );
    assign T_DI_B_INT        = (ISOL_N===1'b0) ? {18{1'b0}}  : ( (ISOL_N===1'b1) ? 'b0          : 'bx );
    assign T_BWE_N_A_INT     = (ISOL_N===1'b0) ? 'b1         : ( (ISOL_N===1'b1) ? 'b0          : 'bx );
    assign T_BWE_N_B_INT     = (ISOL_N===1'b0) ? 'b1         : ( (ISOL_N===1'b1) ? 'b0          : 'bx );
    assign T_OE_N_A_INT      = 'b0;
    assign T_OE_N_B_INT      = 'b0;
  `endif
`else
  `ifdef CLKINV
    assign CLK_A_INT           = (condition_pwr2 || condition_pwr3) ? 0: ~CLK_A;
    assign CLK_B_INT           = (condition_pwr2 || condition_pwr3) ? 0: ~CLK_B;
  `else
    assign CLK_A_INT           = (condition_pwr2 || condition_pwr3) ? 0:  CLK_A;
    assign CLK_B_INT           = (condition_pwr2 || condition_pwr3) ? 0:  CLK_B;
  `endif
  assign CE_N_A_INT          = CE_N_A;
  assign CE_N_B_INT          = CE_N_B;
  assign GWE_N_A_INT         = GWE_N_A;
  assign GWE_N_B_INT         = GWE_N_B;
  assign A_A_INT             = A_A;
  assign A_B_INT             = A_B;
  assign DI_A_INT            = DI_A;
  assign DI_B_INT            = DI_B;
  `ifdef BIT_WRITE
    assign BWE_N_A_INT       = BWE_N_A;
    assign BWE_N_B_INT       = BWE_N_B;
  `else
    `ifdef BYTE_WRITE
      assign BWE_N_A_INT[0]  =  BYWE_N_A[0] ;
      assign BWE_N_B_INT[0]  =  BYWE_N_B[0] ;
      assign BWE_N_A_INT[1]  =  BYWE_N_A[0] ;
      assign BWE_N_B_INT[1]  =  BYWE_N_B[0] ;
      assign BWE_N_A_INT[2]  =  BYWE_N_A[0] ;
      assign BWE_N_B_INT[2]  =  BYWE_N_B[0] ;
      assign BWE_N_A_INT[3]  =  BYWE_N_A[0] ;
      assign BWE_N_B_INT[3]  =  BYWE_N_B[0] ;
      assign BWE_N_A_INT[4]  =  BYWE_N_A[0] ;
      assign BWE_N_B_INT[4]  =  BYWE_N_B[0] ;
      assign BWE_N_A_INT[5]  =  BYWE_N_A[0] ;
      assign BWE_N_B_INT[5]  =  BYWE_N_B[0] ;
      assign BWE_N_A_INT[6]  =  BYWE_N_A[0] ;
      assign BWE_N_B_INT[6]  =  BYWE_N_B[0] ;
      assign BWE_N_A_INT[7]  =  BYWE_N_A[0] ;
      assign BWE_N_B_INT[7]  =  BYWE_N_B[0] ;
      assign BWE_N_A_INT[8]  =  BYWE_N_A[1] ;
      assign BWE_N_B_INT[8]  =  BYWE_N_B[1] ;
      assign BWE_N_A_INT[9]  =  BYWE_N_A[1] ;
      assign BWE_N_B_INT[9]  =  BYWE_N_B[1] ;
      assign BWE_N_A_INT[10]  =  BYWE_N_A[1] ;
      assign BWE_N_B_INT[10]  =  BYWE_N_B[1] ;
      assign BWE_N_A_INT[11]  =  BYWE_N_A[1] ;
      assign BWE_N_B_INT[11]  =  BYWE_N_B[1] ;
      assign BWE_N_A_INT[12]  =  BYWE_N_A[1] ;
      assign BWE_N_B_INT[12]  =  BYWE_N_B[1] ;
      assign BWE_N_A_INT[13]  =  BYWE_N_A[1] ;
      assign BWE_N_B_INT[13]  =  BYWE_N_B[1] ;
      assign BWE_N_A_INT[14]  =  BYWE_N_A[1] ;
      assign BWE_N_B_INT[14]  =  BYWE_N_B[1] ;
      assign BWE_N_A_INT[15]  =  BYWE_N_A[1] ;
      assign BWE_N_B_INT[15]  =  BYWE_N_B[1] ;
      assign BWE_N_A_INT[16]  =  BYWE_N_A[2] ;
      assign BWE_N_B_INT[16]  =  BYWE_N_B[2] ;
      assign BWE_N_A_INT[17]  =  BYWE_N_A[2] ;
      assign BWE_N_B_INT[17]  =  BYWE_N_B[2] ;
    `else
      assign BWE_N_A_INT       = 'b0;
      assign BWE_N_B_INT       = 'b0;
    `endif
  `endif
  `ifdef OUTPUT_ENABLE
    assign OE_N_A_INT        = OE_N_A;
    assign OE_N_B_INT        = OE_N_B;
  `else
    assign OE_N_A_INT        = 1'b0;
    assign OE_N_B_INT        = 1'b0;
  `endif
  `ifdef ASYNCHRONOUS_WRITE
    assign T_AWT_N_A_INT     = T_AWT_N_A;
    assign T_AWT_N_B_INT     = T_AWT_N_B;
  `else
    assign T_AWT_N_A_INT     = 1'b1;
    assign T_AWT_N_B_INT     = 1'b1;
  `endif
  assign T_RWM_A_INT         = T_RWM_A;
  assign T_RWM_B_INT         = T_RWM_B;
  `ifdef WRITE_ASSIST
    assign T_DLY_A_INT       = T_DLY_A;
    assign T_DLY_B_INT       = T_DLY_B;
  `else
    assign T_DLY_A_INT       = 'b0;
    assign T_DLY_B_INT       = 'b0;
  `endif
  `ifdef LOW_LEAK1
    assign LOLEAK_N_INT    = LOLEAK_N;
    assign LKRB_N_INT      = LKRB_N;
    assign DS_INT[0]       = DS[0];
    assign DS_INT[1]       = DS[1];
  `else
    `ifdef LOW_LEAK2
      assign LOLEAK_N_INT    = LOLEAK_N;
      assign LKRB_N_INT      = 'b1;
      assign DS_INT[0]       = DS[0];
      assign DS_INT[1]       = DS[1];
    `else
      assign LOLEAK_N_INT    = 'b1;
      assign LKRB_N_INT      = 'b1;
    `endif
  `endif
  `ifdef PWR_GATE1
    assign COREPWS_N_INT   = COREPWS_N;
    assign P_PWS_N_INT     = P_PWS_N;
  `else
    `ifdef PWR_GATE2
      assign COREPWS_N_INT   = 'b1;
      assign P_PWS_N_INT     = P_PWS_N;
    `else
      assign COREPWS_N_INT   = 'b1;
      assign P_PWS_N_INT     = 'b1;
    `endif
  `endif
  `ifdef ROW_RED
    assign RENF_INT        = RENF;
    assign RENS_INT        = RENS;
    assign RRAF_INT        = RRAF;
    assign RRAS_INT        = RRAS;
  `else
    assign RENF_INT        = 'b0;
    assign RENS_INT        = 'b0;
    assign RRAF_INT        = 'b0;
    assign RRAS_INT        = 'b0;
  `endif
  `ifdef COL_RED
    assign CRAL_INT        = CRAL;
    assign CRAR_INT        = CRAR;
  `else
    assign CRAL_INT        = 'b0;
    assign CRAR_INT        = 'b0;
  `endif
  `ifdef BIST_TEST
    assign T_BE_N_INT      = T_BE_N;
    assign T_CE_N_A_INT      = T_CE_N_A;
    assign T_CE_N_B_INT      = T_CE_N_B;
    assign T_GWE_N_A_INT     = T_GWE_N_A;
    assign T_GWE_N_B_INT     = T_GWE_N_B;
    assign T_A_A_INT         = T_A_A;
    assign T_A_B_INT         = T_A_B;
    assign T_DI_A_INT        = T_DI_A;
    assign T_DI_B_INT        = T_DI_B;
    `ifdef BIT_WRITE
      assign T_BWE_N_A_INT       = T_BWE_N_A;
      assign T_BWE_N_B_INT       = T_BWE_N_B;
    `else
      `ifdef BYTE_WRITE
        assign T_BWE_N_A_INT[0]  =  T_BYWE_N_A[0] ;
        assign T_BWE_N_B_INT[0]  =  T_BYWE_N_B[0] ;
        assign T_BWE_N_A_INT[1]  =  T_BYWE_N_A[0] ;
        assign T_BWE_N_B_INT[1]  =  T_BYWE_N_B[0] ;
        assign T_BWE_N_A_INT[2]  =  T_BYWE_N_A[0] ;
        assign T_BWE_N_B_INT[2]  =  T_BYWE_N_B[0] ;
        assign T_BWE_N_A_INT[3]  =  T_BYWE_N_A[0] ;
        assign T_BWE_N_B_INT[3]  =  T_BYWE_N_B[0] ;
        assign T_BWE_N_A_INT[4]  =  T_BYWE_N_A[0] ;
        assign T_BWE_N_B_INT[4]  =  T_BYWE_N_B[0] ;
        assign T_BWE_N_A_INT[5]  =  T_BYWE_N_A[0] ;
        assign T_BWE_N_B_INT[5]  =  T_BYWE_N_B[0] ;
        assign T_BWE_N_A_INT[6]  =  T_BYWE_N_A[0] ;
        assign T_BWE_N_B_INT[6]  =  T_BYWE_N_B[0] ;
        assign T_BWE_N_A_INT[7]  =  T_BYWE_N_A[0] ;
        assign T_BWE_N_B_INT[7]  =  T_BYWE_N_B[0] ;
        assign T_BWE_N_A_INT[8]  =  T_BYWE_N_A[1] ;
        assign T_BWE_N_B_INT[8]  =  T_BYWE_N_B[1] ;
        assign T_BWE_N_A_INT[9]  =  T_BYWE_N_A[1] ;
        assign T_BWE_N_B_INT[9]  =  T_BYWE_N_B[1] ;
        assign T_BWE_N_A_INT[10]  =  T_BYWE_N_A[1] ;
        assign T_BWE_N_B_INT[10]  =  T_BYWE_N_B[1] ;
        assign T_BWE_N_A_INT[11]  =  T_BYWE_N_A[1] ;
        assign T_BWE_N_B_INT[11]  =  T_BYWE_N_B[1] ;
        assign T_BWE_N_A_INT[12]  =  T_BYWE_N_A[1] ;
        assign T_BWE_N_B_INT[12]  =  T_BYWE_N_B[1] ;
        assign T_BWE_N_A_INT[13]  =  T_BYWE_N_A[1] ;
        assign T_BWE_N_B_INT[13]  =  T_BYWE_N_B[1] ;
        assign T_BWE_N_A_INT[14]  =  T_BYWE_N_A[1] ;
        assign T_BWE_N_B_INT[14]  =  T_BYWE_N_B[1] ;
        assign T_BWE_N_A_INT[15]  =  T_BYWE_N_A[1] ;
        assign T_BWE_N_B_INT[15]  =  T_BYWE_N_B[1] ;
        assign T_BWE_N_A_INT[16]  =  T_BYWE_N_A[2] ;
        assign T_BWE_N_B_INT[16]  =  T_BYWE_N_B[2] ;
        assign T_BWE_N_A_INT[17]  =  T_BYWE_N_A[2] ;
        assign T_BWE_N_B_INT[17]  =  T_BYWE_N_B[2] ;
      `else
        assign T_BWE_N_A_INT       = 'b0;
        assign T_BWE_N_B_INT       = 'b0;
      `endif
    `endif
    `ifdef OUTPUT_ENABLE
      assign T_OE_N_A_INT    = T_OE_N_A;
      assign T_OE_N_B_INT    = T_OE_N_B;
    `else
      assign T_OE_N_A_INT    = 'b0;
      assign T_OE_N_B_INT    = 'b0;
    `endif
  `else
    assign T_BE_N_INT      = 1'b1;
    assign T_CE_N_A_INT      = 'b1;
    assign T_CE_N_B_INT      = 'b1;
    assign T_GWE_N_A_INT     = 'b0;
    assign T_GWE_N_B_INT     = 'b0;
    assign T_A_A_INT         = 'b0;
    assign T_A_B_INT         = 'b0;
    assign T_DI_A_INT        = 'b0;
    assign T_DI_B_INT        = 'b0;
    assign T_BWE_N_A_INT     = 'b0;
    assign T_BWE_N_B_INT     = 'b0;
    assign T_OE_N_A_INT      = 'b0;
    assign T_OE_N_B_INT      = 'b0;
  `endif
`endif
// Isolation block

always @(CLK_A_INT)
  begin 
    if((T_RWM_A_INT[0] == 0 && T_RWM_A_INT[1] == 0 && T_RWM_A_INT[2] == 0))  begin 
      $display("The Read Write Margin is set to %b%b%b which is NOT RECOMMENDED. This may cause Functional and Speed problems - Please Verify your settings", T_RWM_A[2], T_RWM_A[1], T_RWM_A[0]);
      $display("The DEFAULT and RECOMMENDED SETTING for Read Write Margin is 011");
    end 
    if((T_RWM_A_INT[0] == 1 && T_RWM_A_INT[1] == 0 && T_RWM_A_INT[2] == 0))  begin 
      $display("The Read Write Margin is set to %b%b%b which is NOT RECOMMENDED. This may cause Functional and Speed problems - Please Verify your settings", T_RWM_A[2], T_RWM_A[1], T_RWM_A[0]);
      $display("The DEFAULT and RECOMMENDED SETTING for Read Write Margin is 011");
    end 
    if((T_RWM_A_INT[0] == 0 && T_RWM_A_INT[1] == 1 && T_RWM_A_INT[2] == 0))  begin 
      $display("The Read Write Margin is set to %b%b%b which is NOT RECOMMENDED. This may cause Functional and Speed problems - Please Verify your settings", T_RWM_A[2], T_RWM_A[1], T_RWM_A[0]);
      $display("The DEFAULT and RECOMMENDED SETTING for Read Write Margin is 011");
    end 
  end 

always @(CLK_B_INT)
  begin 
    if((T_RWM_B_INT[0] == 0 && T_RWM_B_INT[1] == 0 && T_RWM_B_INT[2] == 0))  begin 
      $display("The Read Write Margin is set to %b%b%b which is NOT RECOMMENDED. This may cause Functional and Speed problems - Please Verify your settings", T_RWM_B[2], T_RWM_B[1], T_RWM_B[0]);
      $display("The DEFAULT and RECOMMENDED SETTING for Read Write Margin is 011");
    end 
    if((T_RWM_B_INT[0] == 1 && T_RWM_B_INT[1] == 0 && T_RWM_B_INT[2] == 0))  begin 
      $display("The Read Write Margin is set to %b%b%b which is NOT RECOMMENDED. This may cause Functional and Speed problems - Please Verify your settings", T_RWM_B[2], T_RWM_B[1], T_RWM_B[0]);
      $display("The DEFAULT and RECOMMENDED SETTING for Read Write Margin is 011");
    end 
    if((T_RWM_B_INT[0] == 0 && T_RWM_B_INT[1] == 1 && T_RWM_B_INT[2] == 0))  begin 
      $display("The Read Write Margin is set to %b%b%b which is NOT RECOMMENDED. This may cause Functional and Speed problems - Please Verify your settings", T_RWM_B[2], T_RWM_B[1], T_RWM_B[0]);
      $display("The DEFAULT and RECOMMENDED SETTING for Read Write Margin is 011");
    end 
  end 
assign condition_pwr0 = !LOLEAK_N_INT &&  LKRB_N_INT &&  P_PWS_N_INT &&  COREPWS_N_INT;
assign condition_pwr1 = !LOLEAK_N_INT && !LKRB_N_INT &&  P_PWS_N_INT &&  COREPWS_N_INT;
assign condition_pwr2 = !LOLEAK_N_INT && !LKRB_N_INT && !P_PWS_N_INT &&  COREPWS_N_INT;
assign condition_pwr3 = !LOLEAK_N_INT && !LKRB_N_INT && !P_PWS_N_INT && !COREPWS_N_INT;

initial 
  begin 
  // Initializing Memory Array to x 
    for (m=0; m<1024; m=m+1) begin 
      memArray[m] = 18'bx; 
    end 
  end 

initial 
  begin 
    ok_a = 1;
  end 
 
initial 
  begin 
    ok_b = 1;
  end 
 
always @(posedge CLK_A_INT) 
  begin
    if(ok_a) begin
      `ifdef COLLISION
        begin
          in_A <= T_BE_N_INT ? (!CE_N_A_INT) : (!T_CE_N_A_INT);
          #0.3629 in_A <= 1'd0;
        end
      `else
        in_A <= 1'b1;
      `endif
    end
  end
always @(posedge CLK_B_INT) 
  begin
    if(ok_b) begin
      `ifdef COLLISION
        begin
          in_B <= T_BE_N_INT ? (!CE_N_B_INT) : (!T_CE_N_B_INT);
          #0.3629 in_B <= 1'd0;
        end
      `else
        in_B <= 1'b1;
      `endif
    end
  end
always @(posedge CLK_A_INT) 
  flag_wr_portA <= #0.001 1'b1;
always @(posedge CLK_A_INT) 
  flag_rd_portA <= #0.001 1'b1;
always @(posedge CLK_B_INT) 
  flag_wr_portB <= #0.001 1'b1;
always @(posedge CLK_B_INT) 
  flag_rd_portB <= #0.001 1'b1;


initial begin 
  CE_N_R_A = 1'b1; 
  CE_N_R_B = 1'b1; 
  GWe_R_A = 1'b0; 
  GWe_R_B = 1'b0; 
  We_R_A = 18'b0; 
  We_R_B = 18'b0; 
end 

always @(posedge CLK_A_INT) begin
  if(COREPWS_N_INT && P_PWS_N_INT) begin
  GWe_R_A      <=  T_BE_N_INT ? ~GWE_N_A_INT : ~T_GWE_N_A_INT;
  CE_N_R_A     <=  T_BE_N_INT ?  CE_N_A_INT  :  T_CE_N_A_INT;
  We_R_A       <=  T_BE_N_INT ? ~BWE_N_A_INT : ~T_BWE_N_A_INT;
  Address_R_A <=  (condition_pwr0 || condition_pwr1) ? 11'bx : T_BE_N_INT ? A_A_INT  : T_A_A_INT;
  if(!(T_BE_N_INT ? CE_N_A_INT : T_CE_N_A_INT)) begin 
    if(T_BE_N_INT && A_A_INT >= 1024)
      $display("%m Address for PORT A:%h  is out of bounds",A_A_INT);
    if(!T_BE_N_INT && T_A_A_INT >= 1024)
      $display("%m Bist Address for PORT A:%h  is out of bounds",T_A_A_INT);
  end 
  DataIn_R_A  <=  T_BE_N_INT ? DI_A_INT : T_DI_A_INT;
end
end

always @(posedge CLK_B_INT) begin
  if(COREPWS_N_INT && P_PWS_N_INT) begin
  GWe_R_B      <=  T_BE_N_INT ? ~GWE_N_B_INT : ~T_GWE_N_B_INT;
  CE_N_R_B     <=  T_BE_N_INT ?  CE_N_B_INT  :  T_CE_N_B_INT;
  We_R_B       <=  T_BE_N_INT ? ~BWE_N_B_INT : ~T_BWE_N_B_INT;
  Address_R_B <=  (condition_pwr0 || condition_pwr1) ? 11'bx : T_BE_N_INT ? A_B_INT  : T_A_B_INT;
  if(!(T_BE_N_INT ? CE_N_B_INT : T_CE_N_B_INT)) begin 
    if(T_BE_N_INT && A_B_INT >= 1024)
      $display("%m Address for PORT A:%h  is out of bounds",A_B_INT);
    if(!T_BE_N_INT && T_A_B_INT >= 1024)
      $display("%m Bist Address for PORT A:%h  is out of bounds",T_A_B_INT);
  end 
  DataIn_R_B  <=  T_BE_N_INT ? DI_B_INT : T_DI_B_INT;
end
end

always @(DataIn_R_A or Address_R_A or GWe_R_A or We_R_A or CE_N_R_A or COREPWS_N_INT or P_PWS_N_INT or flag_wr_portA or in_A or in_B) begin
  if(COREPWS_N_INT && P_PWS_N_INT) begin
    coll_w_a = in_A & in_B;
    if(ok_a) begin 
      if (GWe_R_A && !CE_N_R_A && flag_wr_portA) begin 
        tmpDataIn_R_A = memArray[Address_R_A];
        for (i_A=0; i_A<18; i_A=i_A+1) begin 
          if (We_R_A[i_A]) begin
            if(^Address_R_A === 1'bx) begin
              for (j_A=0; j_A<1024; j_A=j_A+1) begin 
                tmpDataIn_R_A = memArray[j_A];
                tmpDataIn_R_A[i_A] = 1'bx;
                if (GWe_R_A && !CE_N_R_A) 
                  memArray[j_A]=tmpDataIn_R_A;
              end
            end
            else 
              tmpDataIn_R_A[i_A] = DataIn_R_A[i_A];
          end
          else begin 
            if(^Address_R_A === 1'bx) begin
              for (j_A=0; j_A<1024; j_A=j_A+1) begin 
                tmpDataIn_R_A = memArray[j_A];
                if(We_R_A[i_A] === 1'bx)
                  tmpDataIn_R_A[i_A] = 1'bx;
                if (GWe_R_A && !CE_N_R_A) 
                  memArray[j_A]=tmpDataIn_R_A;
              end
            end
            else 
              if(We_R_A[i_A] === 1'bx)
                tmpDataIn_R_A[i_A] = 1'bx;
          end
        end
      end
      if (GWe_R_A && !CE_N_R_A && flag_wr_portA) begin
        if (!CE_N_R_B && GWe_R_B && ((Address_R_A == Address_R_B) && coll_w_a)) begin
          for (i_A=0; i_A<18; i_A=i_A+1) begin 
            if (We_R_A[i_A]) begin 
              if (We_R_B[i_A]) 
                DataIn_Reg[i_A] = 1'bx;
              else 
                DataIn_Reg[i_A] = tmpDataIn_R_A[i_A];
            end
            else 
              if(!We_R_A[i_A] && !We_R_B[i_A]) 
                DataIn_Reg[i_A] = tmpDataIn_R_A[i_A];
          end
          memArray[Address_R_A] = DataIn_Reg;
        end
        else 
          if(^Address_R_A !== 1'bx && ((^Address_R_B !== 1'bx) || CE_N_R_B || !GWe_R_B || !coll_w_a))
            memArray[Address_R_A]=tmpDataIn_R_A;
      end
      else begin 
        if (GWe_R_A === 1'bx && !CE_N_R_A) begin 
          if(^Address_R_A === 1'bx) 
            for (j_A=0; j_A<1024; j_A=j_A+1)
              memArray[j_A] = 18'bx; 
          else 
            memArray[Address_R_A] = 18'bx;
        end
      end
    end
    else begin
      if (GWe_R_A && !CE_N_R_A && flag_wr_portA) begin 
        tmpDataIn_R_A = memArray[Address_R_A];
        for (i_A=0; i_A<18; i_A=i_A+1) begin 
          if (We_R_A[i_A]) begin
            if(^Address_R_A === 1'bx) begin
              for (j_A=0; j_A<1024; j_A=j_A+1) begin 
                tmpDataIn_R_A = memArray[j_A];
                tmpDataIn_R_A[i_A] = 1'bx;
                if (GWe_R_A && !CE_N_R_A) begin 
                  memArray[j_A]= 18'bx;
                end
              end
            end
            else 
              tmpDataIn_R_A[i_A] = DataIn_R_A[i_A];
          end
          else begin 
            if(^Address_R_A === 1'bx) begin
              for (j_A=0; j_A<1024; j_A=j_A+1) begin 
                tmpDataIn_R_A = memArray[j_A];
                if(We_R_A[i_A] === 1'bx)
                  tmpDataIn_R_A[i_A] = 1'bx;
                if (GWe_R_A && !CE_N_R_A)  begin
                  memArray[j_A] = 18'bx;
                end
              end
            end
            else 
              if(We_R_A[i_A] === 1'bx)
                tmpDataIn_R_A[i_A] = 1'bx;
          end
        end
      end
      if (GWe_R_A && !CE_N_R_A && flag_wr_portA) begin
        if (!CE_N_R_B && GWe_R_B && ((Address_R_A == Address_R_B) && coll_w_a)) begin
          for (i_A=0; i_A<18; i_A=i_A+1) begin 
            if (We_R_A[i_A]) begin 
              if (We_R_B[i_A]) 
                DataIn_Reg[i_A] = 1'bx;
              else 
                DataIn_Reg[i_A] = tmpDataIn_R_A[i_A];
            end
            else 
              if(!We_R_A[i_A] && !We_R_B[i_A]) 
                DataIn_Reg[i_A] = tmpDataIn_R_A[i_A];
          end
          memArray[Address_R_A] = DataIn_Reg;
        end
        else 
          if(^Address_R_A !== 1'bx && ((^Address_R_B !== 1'bx) || CE_N_R_B || !GWe_R_B || !coll_w_a)) begin 
            memArray[Address_R_A] = 18'bx;
        end
      end
      else begin 
        if (GWe_R_A === 1'bx && !CE_N_R_A) begin 
          if(^Address_R_A === 1'bx) 
            for (j_A=0; j_A<1024; j_A=j_A+1) begin 
              memArray[j_A] = 18'bx; 
          end
          else begin 
            memArray[Address_R_A] = 18'bx;
          end
        end
      end
    end
    flag_wr_portA = 1'b0;
  end
  else 
  if(!COREPWS_N_INT) begin
    for (m=0; m<1024; m=m+1) begin 
      memArray[m] = 18'bx; 
    end 
  end 
end 

always @(DataIn_R_B or Address_R_B or GWe_R_B or We_R_B or CE_N_R_B or COREPWS_N_INT or P_PWS_N_INT or flag_wr_portB or in_A or in_B) begin
  if(COREPWS_N_INT && P_PWS_N_INT) begin
    coll_w_b = in_A & in_B;
    if(ok_b) begin 
      if (GWe_R_B && !CE_N_R_B && flag_wr_portB) begin 
        tmpDataIn_R_B = memArray[Address_R_B];
        for (i_B=0; i_B<18; i_B=i_B+1) begin 
          if (We_R_B[i_B]) begin
            if(^Address_R_B === 1'bx) begin
              for (j_B=0; j_B<1024; j_B=j_B+1) begin 
                tmpDataIn_R_B = memArray[j_B];
                tmpDataIn_R_B[i_B] = 1'bx;
                if (GWe_R_B && !CE_N_R_B) 
                  memArray[j_B]=tmpDataIn_R_B;
              end
            end
            else 
              tmpDataIn_R_B[i_B] = DataIn_R_B[i_B];
          end
          else begin 
            if(^Address_R_B === 1'bx) begin
              for (j_B=0; j_B<1024; j_B=j_B+1) begin 
                tmpDataIn_R_B = memArray[j_B];
                if(We_R_B[i_B] === 1'bx)
                  tmpDataIn_R_B[i_B] = 1'bx;
                if (GWe_R_B && !CE_N_R_B) 
                  memArray[j_B]=tmpDataIn_R_B;
              end
            end
            else 
              if(We_R_B[i_B] === 1'bx)
                tmpDataIn_R_B[i_B] = 1'bx;
          end
        end
      end
      if (GWe_R_B && !CE_N_R_B && flag_wr_portB) begin
        if (!CE_N_R_A && GWe_R_A && ((Address_R_A == Address_R_B) && coll_w_b)) begin
          for (i_B=0; i_B<18; i_B=i_B+1) begin 
            if (We_R_B[i_B]) begin 
              if (We_R_A[i_B]) 
                DataIn_Reg[i_B] = 1'bx;
              else 
                DataIn_Reg[i_B] = tmpDataIn_R_B[i_B];
            end
            else 
              if(!We_R_B[i_B] && !We_R_A[i_B]) 
                DataIn_Reg[i_B] = tmpDataIn_R_B[i_B];
          end
          memArray[Address_R_B] = DataIn_Reg;
        end
        else 
          if(^Address_R_B !== 1'bx && ((^Address_R_A !== 1'bx) || CE_N_R_A || !GWe_R_A || !coll_w_b))
            memArray[Address_R_B]=tmpDataIn_R_B;
      end
      else begin 
        if (GWe_R_B === 1'bx && !CE_N_R_B) begin 
          if(^Address_R_B === 1'bx) 
            for (j_B=0; j_B<1024; j_B=j_B+1)
              memArray[j_B] = 18'bx; 
          else 
            memArray[Address_R_B] = 18'bx;
        end
      end
    end
    else begin
      if (GWe_R_B && !CE_N_R_B && flag_wr_portB) begin 
        tmpDataIn_R_B = memArray[Address_R_B];
        for (i_B=0; i_B<18; i_B=i_B+1) begin 
          if (We_R_B[i_B]) begin
            if(^Address_R_B === 1'bx) begin
              for (j_B=0; j_B<1024; j_B=j_B+1) begin 
                tmpDataIn_R_B = memArray[j_B];
                tmpDataIn_R_B[i_B] = 1'bx;
                if (GWe_R_B && !CE_N_R_B) begin 
                  memArray[j_B]= 18'bx;
                end
              end
            end
            else 
              tmpDataIn_R_B[i_B] = DataIn_R_B[i_B];
          end
          else begin 
            if(^Address_R_B === 1'bx) begin
              for (j_B=0; j_B<1024; j_B=j_B+1) begin 
                tmpDataIn_R_B = memArray[j_B];
                if(We_R_B[i_B] === 1'bx)
                  tmpDataIn_R_B[i_B] = 1'bx;
                if (GWe_R_B && !CE_N_R_B)  begin
                  memArray[j_B] = 18'bx;
                end
              end
            end
            else 
              if(We_R_B[i_B] === 1'bx)
                tmpDataIn_R_B[i_B] = 1'bx;
          end
        end
      end
      if (GWe_R_B && !CE_N_R_B && flag_wr_portB) begin
        if (!CE_N_R_A && GWe_R_A && ((Address_R_A == Address_R_B) && coll_w_b)) begin
          for (i_B=0; i_B<18; i_B=i_B+1) begin 
            if (We_R_B[i_B]) begin 
              if (We_R_A[i_B]) 
                DataIn_Reg[i_B] = 1'bx;
              else 
                DataIn_Reg[i_B] = tmpDataIn_R_B[i_B];
            end
            else 
              if(!We_R_B[i_B] && !We_R_A[i_B]) 
                DataIn_Reg[i_B] = tmpDataIn_R_B[i_B];
          end
          memArray[Address_R_B] = DataIn_Reg;
        end
        else 
          if(^Address_R_B !== 1'bx && ((^Address_R_A !== 1'bx) || CE_N_R_A || !GWe_R_A || !coll_w_b)) begin 
            memArray[Address_R_B] = 18'bx;
        end
      end
      else begin 
        if (GWe_R_B === 1'bx && !CE_N_R_B) begin 
          if(^Address_R_B === 1'bx) 
            for (j_B=0; j_B<1024; j_B=j_B+1) begin 
              memArray[j_B] = 18'bx; 
          end
          else begin 
            memArray[Address_R_B] = 18'bx;
          end
        end
      end
    end
    flag_wr_portB = 1'b0;
  end
  else 
  if(!COREPWS_N_INT) begin
    for (m=0; m<1024; m=m+1) begin 
      memArray[m] = 18'bx; 
    end 
  end 
end 

always @(Address_R_A or Address_R_B or GWe_R_B or GWe_R_A or CE_N_R_A or CE_N_R_B or We_R_A or We_R_B or memArray[Address_R_A] or posedge in_A or posedge in_B or posedge flag_rd_portA or COREPWS_N_INT) begin 
  if(COREPWS_N_INT && P_PWS_N_INT) begin
    coll_r_a = in_A & in_B;
    if(ok_a) begin 
      if (!CE_N_R_A && GWe_R_B && !CE_N_R_B && ((Address_R_A == Address_R_B) && coll_r_a)) begin
        tmpDataIn_R_W_A = memArray[Address_R_A]; 
        for (i_A=0; i_A<18; i_A=i_A+1) begin 
          if(GWe_R_A) begin 
            if(!We_R_A[i_A] && We_R_B[i_A])
              Dout_TR_A[i_A] = 1'bx;
            else 
              Dout_TR_A[i_A] = tmpDataIn_R_W_A[i_A];
          end
          else begin 
            if(GWe_R_A === 1'bx && !CE_N_R_A) 
              Dout_TR_A[i_A] = 1'bx; 
            else begin 
              if(We_R_B[i_A]) 
                Dout_TR_A[i_A] = 1'bx;
              else
                Dout_TR_A[i_A] = tmpDataIn_R_W_A[i_A];
            end
          end
        end
      end
      else begin 
        if(!CE_N_R_A && flag_rd_portA)
          Dout_TR_A = memArray[Address_R_A];
        else if (CE_N_R_A === 1'bx)
          Dout_TR_A = 18'bx;
      end 
    end
    else begin
      if(!CE_N_R_A) begin
        Dout_TR_A = 18'bx;
      end
    end
    flag_rd_portA = 1'b0;
  end
  else 
    if(!COREPWS_N_INT) begin
      for (m=0; m<1024; m=m+1) begin
        memArray[m] = 18'bx;
      end
    end
end


always @(Address_R_A or Address_R_B or GWe_R_B or GWe_R_A or CE_N_R_A or CE_N_R_B or We_R_A or We_R_B or memArray[Address_R_B] or posedge in_A or posedge in_B or posedge flag_rd_portB or COREPWS_N_INT) begin 
  if(COREPWS_N_INT && P_PWS_N_INT) begin
    coll_r_b = in_A & in_B;
    if(ok_b) begin 
      if (!CE_N_R_A && GWe_R_A && !CE_N_R_B && ((Address_R_A == Address_R_B) && coll_r_b)) begin
        tmpDataIn_R_W_B = memArray[Address_R_B]; 
        for (i_B=0; i_B<18; i_B=i_B+1) begin 
          if(GWe_R_B) begin 
            if(!We_R_B[i_B] && We_R_A[i_B])
              Dout_TR_B[i_B] = 1'bx;
            else 
              Dout_TR_B[i_B] = tmpDataIn_R_W_B[i_B];
          end
          else begin 
            if(GWe_R_B === 1'bx && !CE_N_R_B) 
              Dout_TR_B[i_B] = 1'bx; 
            else begin 
              if(We_R_A[i_B]) 
                Dout_TR_B[i_B] = 1'bx;
              else
                Dout_TR_B[i_B] = tmpDataIn_R_W_B[i_B];
            end
          end
        end
      end
      else begin 
        if(!CE_N_R_B && flag_rd_portB)
          Dout_TR_B = memArray[Address_R_B];
        else if (CE_N_R_B === 1'bx)
          Dout_TR_B = 18'bx;
      end 
    end
    else  begin
      if(!CE_N_R_B) begin
        Dout_TR_B = 18'bx;
      end
    end
    flag_rd_portB = 1'b0;
  end
  else 
    if(!COREPWS_N_INT) begin
      for (m=0; m<1024; m=m+1) begin 
        memArray[m] = 18'bx; 
      end 
    end 
end 

assign Dout_A = (COREPWS_N_INT && P_PWS_N_INT) ? ( (CE_N_R_A === 1'bx) ? 18'bx : (CE_N_R_A ? (GWe_R_A ? Dout_A : Dout_A) : (GWe_R_A ? Dout_A : Dout_TR_A))) : 18'bx;
assign Dout_B = (COREPWS_N_INT && P_PWS_N_INT) ? ( (CE_N_R_B === 1'bx) ? 18'bx : (CE_N_R_B ? (GWe_R_B ? Dout_B : Dout_B) : (GWe_R_B ? Dout_B : Dout_TR_B))) : 18'bx;
assign Dout_A_INT = ((COREPWS_N_INT===1'bx) || (P_PWS_N_INT===1'bx)) ? 18'bx : (((COREPWS_N_INT===1'b0) || (P_PWS_N_INT===1'b0)) ? 18'b0 : Dout_A);
assign Dout_B_INT = ((COREPWS_N_INT===1'bx) || (P_PWS_N_INT===1'bx)) ? 18'bx : (((COREPWS_N_INT===1'b0) || (P_PWS_N_INT===1'b0)) ? 18'b0 : Dout_B);

assign DO_temp_A = T_AWT_N_A_INT ? Dout_A_INT : (T_BE_N_INT ? DI_A_INT : T_DI_A_INT);
assign DO_temp_B = T_AWT_N_B_INT ? Dout_B_INT : (T_BE_N_INT ? DI_B_INT : T_DI_B_INT);
assign #0 cntrlA =  1'b0;
assign #0 cntrlB =  1'b0;
bufif0(DO_A[0], DO_temp_A[0], cntrlA);
bufif0(DO_B[0], DO_temp_B[0], cntrlB);
bufif0(DO_A[1], DO_temp_A[1], cntrlA);
bufif0(DO_B[1], DO_temp_B[1], cntrlB);
bufif0(DO_A[2], DO_temp_A[2], cntrlA);
bufif0(DO_B[2], DO_temp_B[2], cntrlB);
bufif0(DO_A[3], DO_temp_A[3], cntrlA);
bufif0(DO_B[3], DO_temp_B[3], cntrlB);
bufif0(DO_A[4], DO_temp_A[4], cntrlA);
bufif0(DO_B[4], DO_temp_B[4], cntrlB);
bufif0(DO_A[5], DO_temp_A[5], cntrlA);
bufif0(DO_B[5], DO_temp_B[5], cntrlB);
bufif0(DO_A[6], DO_temp_A[6], cntrlA);
bufif0(DO_B[6], DO_temp_B[6], cntrlB);
bufif0(DO_A[7], DO_temp_A[7], cntrlA);
bufif0(DO_B[7], DO_temp_B[7], cntrlB);
bufif0(DO_A[8], DO_temp_A[8], cntrlA);
bufif0(DO_B[8], DO_temp_B[8], cntrlB);
bufif0(DO_A[9], DO_temp_A[9], cntrlA);
bufif0(DO_B[9], DO_temp_B[9], cntrlB);
bufif0(DO_A[10], DO_temp_A[10], cntrlA);
bufif0(DO_B[10], DO_temp_B[10], cntrlB);
bufif0(DO_A[11], DO_temp_A[11], cntrlA);
bufif0(DO_B[11], DO_temp_B[11], cntrlB);
bufif0(DO_A[12], DO_temp_A[12], cntrlA);
bufif0(DO_B[12], DO_temp_B[12], cntrlB);
bufif0(DO_A[13], DO_temp_A[13], cntrlA);
bufif0(DO_B[13], DO_temp_B[13], cntrlB);
bufif0(DO_A[14], DO_temp_A[14], cntrlA);
bufif0(DO_B[14], DO_temp_B[14], cntrlB);
bufif0(DO_A[15], DO_temp_A[15], cntrlA);
bufif0(DO_B[15], DO_temp_B[15], cntrlB);
bufif0(DO_A[16], DO_temp_A[16], cntrlA);
bufif0(DO_B[16], DO_temp_B[16], cntrlB);
bufif0(DO_A[17], DO_temp_A[17], cntrlA);
bufif0(DO_B[17], DO_temp_B[17], cntrlB);

wire condition5_a ;
wire condition5_b ;
wire condition1_a ;
wire condition1_b ;
wire condition2_a ;
wire condition2_b ;
wire condition3_a ;
wire condition3_b ;
wire condition4_a ;
wire condition4_b ;
assign condition1_a = ((!CE_N_A && T_BE_N) || (!T_CE_N_A && !T_BE_N)) ;
assign condition1_b = ((!CE_N_B && T_BE_N) || (!T_CE_N_B && !T_BE_N)) ;
assign condition2_a = (!T_CE_N_A && !T_BE_N) ;
assign condition2_b = (!T_CE_N_B && !T_BE_N) ;
assign condition3_a = (!CE_N_A && T_BE_N) ;
assign condition3_b = (!CE_N_B && T_BE_N) ;
assign condition4_a = (!T_BE_N) ;
assign condition4_b = (!T_BE_N) ;
assign condition5_a = (!CE_N_A) ;
assign condition5_b = (!CE_N_B) ;
wire timing_condition_margin_0_a ;
wire timing_condition_margin_1_a ;
wire timing_condition_margin_2_a ;
wire timing_condition_margin_3_a ;
wire timing_condition_margin_4_a ;
wire timing_condition_margin_5_a ;
wire timing_condition_margin_6_a ;
wire timing_condition_margin_7_a ;
wire timing_condition_margin_0_b ;
wire timing_condition_margin_1_b ;
wire timing_condition_margin_2_b ;
wire timing_condition_margin_3_b ;
wire timing_condition_margin_4_b ;
wire timing_condition_margin_5_b ;
wire timing_condition_margin_6_b ;
wire timing_condition_margin_7_b ;
assign timing_condition_margin_0_a = (T_RWM_A[0] == 0 && T_RWM_A[1] == 0 && T_RWM_A[2] == 0) ;
assign timing_condition_margin_1_a = (T_RWM_A[0] == 1 && T_RWM_A[1] == 0 && T_RWM_A[2] == 0) ;
assign timing_condition_margin_2_a = (T_RWM_A[0] == 0 && T_RWM_A[1] == 1 && T_RWM_A[2] == 0) ;
assign timing_condition_margin_3_a = (T_RWM_A[0] == 1 && T_RWM_A[1] == 1 && T_RWM_A[2] == 0) ;
assign timing_condition_margin_4_a = (T_RWM_A[0] == 0 && T_RWM_A[1] == 0 && T_RWM_A[2] == 1) ;
assign timing_condition_margin_5_a = (T_RWM_A[0] == 1 && T_RWM_A[1] == 0 && T_RWM_A[2] == 1) ;
assign timing_condition_margin_6_a = (T_RWM_A[0] == 0 && T_RWM_A[1] == 1 && T_RWM_A[2] == 1) ;
assign timing_condition_margin_7_a = (T_RWM_A[0] == 1 && T_RWM_A[1] == 1 && T_RWM_A[2] == 1) ;
assign timing_condition_margin_0_b = (T_RWM_B[0] == 0 && T_RWM_B[1] == 0 && T_RWM_B[2] == 0) ;
assign timing_condition_margin_1_b = (T_RWM_B[0] == 1 && T_RWM_B[1] == 0 && T_RWM_B[2] == 0) ;
assign timing_condition_margin_2_b = (T_RWM_B[0] == 0 && T_RWM_B[1] == 1 && T_RWM_B[2] == 0) ;
assign timing_condition_margin_3_b = (T_RWM_B[0] == 1 && T_RWM_B[1] == 1 && T_RWM_B[2] == 0) ;
assign timing_condition_margin_4_b = (T_RWM_B[0] == 0 && T_RWM_B[1] == 0 && T_RWM_B[2] == 1) ;
assign timing_condition_margin_5_b = (T_RWM_B[0] == 1 && T_RWM_B[1] == 0 && T_RWM_B[2] == 1) ;
assign timing_condition_margin_6_b = (T_RWM_B[0] == 0 && T_RWM_B[1] == 1 && T_RWM_B[2] == 1) ;
assign timing_condition_margin_7_b = (T_RWM_B[0] == 1 && T_RWM_B[1] == 1 && T_RWM_B[2] == 1) ;

specify
 specparam
  period_param_0 = 0.3057,
  period_param_1 = 0.3226,
  period_param_2 = 0.3462,
  period_param_3 = 0.3629,
  period_param_4 = 0.3913,
  period_param_5 = 0.4082,
  period_param_6 = 0.4319,
  period_param_7 = 0.4488,
  trise_CLK_A_DO_A_worst = 0.3289, 
  tfall_CLK_A_DO_A_worst = 0.3260, 
  trise_CLK_B_DO_B_worst = 0.3289, 
  tfall_CLK_B_DO_B_worst = 0.3260, 
  trise_CLK_DO_A_worst_0 = 0.2716, 
  tfall_CLK_DO_A_worst_0 = 0.2687, 
  trise_CLK_DO_B_worst_0 = 0.2716, 
  tfall_CLK_DO_B_worst_0 = 0.2687, 
  trise_CLK_DO_A_worst_1 = 0.2885, 
  tfall_CLK_DO_A_worst_1 = 0.2856, 
  trise_CLK_DO_B_worst_1 = 0.2885, 
  tfall_CLK_DO_B_worst_1 = 0.2856, 
  trise_CLK_DO_A_worst_2 = 0.3121, 
  tfall_CLK_DO_A_worst_2 = 0.3092, 
  trise_CLK_DO_B_worst_2 = 0.3121, 
  tfall_CLK_DO_B_worst_2 = 0.3092, 
  trise_CLK_DO_A_worst_3 = 0.3289, 
  tfall_CLK_DO_A_worst_3 = 0.3260, 
  trise_CLK_DO_B_worst_3 = 0.3289, 
  tfall_CLK_DO_B_worst_3 = 0.3260, 
  trise_CLK_DO_A_worst_4 = 0.3572, 
  tfall_CLK_DO_A_worst_4 = 0.3543, 
  trise_CLK_DO_B_worst_4 = 0.3572, 
  tfall_CLK_DO_B_worst_4 = 0.3543, 
  trise_CLK_DO_A_worst_5 = 0.3741, 
  tfall_CLK_DO_A_worst_5 = 0.3712, 
  trise_CLK_DO_B_worst_5 = 0.3741, 
  tfall_CLK_DO_B_worst_5 = 0.3712, 
  trise_CLK_DO_A_worst_6 = 0.3979, 
  tfall_CLK_DO_A_worst_6 = 0.3950, 
  trise_CLK_DO_B_worst_6 = 0.3979, 
  tfall_CLK_DO_B_worst_6 = 0.3950, 
  trise_CLK_DO_A_worst_7 = 0.4147, 
  tfall_CLK_DO_A_worst_7 = 0.4118, 
  trise_CLK_DO_B_worst_7 = 0.4147, 
  tfall_CLK_DO_B_worst_7 = 0.4118, 
  t_T_RWM_A_setup_worst = 0.0898,
  t_T_RWM_B_setup_worst = 0.0898,
  t_T_RWM_A_hold_worst  = 0.3629,
  t_T_RWM_B_hold_worst  = 0.3629,
  t_A_A_setup_worst = 0.1196,
  t_A_B_setup_worst = 0.1196,
  t_A_A_hold_worst  = 0.0612,
  t_A_B_hold_worst  = 0.0612,
  t_DI_A_setup_worst = 0.0974,
  t_DI_B_setup_worst = 0.0974,
  t_DI_A_hold_worst  = 0.1434,
  t_DI_B_hold_worst  = 0.1434,
  t_CE_N_A_setup_worst = 0.1178,
  t_CE_N_B_setup_worst = 0.1178,
  t_CE_N_A_hold_worst  = 0.0488,
  t_CE_N_B_hold_worst  = 0.0488,
  t_BWE_N_A_setup_worst = 0.0974,
  t_BWE_N_B_setup_worst = 0.0974,
  t_BWE_N_A_hold_worst  = 0.1434,
  t_BWE_N_B_hold_worst  = 0.1434,
  t_T_BE_N_setup_worst = 1.3960,
  t_T_BE_N_hold_worst  = 0.3629,
  t_T_A_A_setup_worst = 0.1508,
  t_T_A_B_setup_worst = 0.1508,
  t_T_A_A_hold_worst  = 0.0413,
  t_T_A_B_hold_worst  = 0.0413,
  t_T_DI_A_setup_worst = 0.1317,
  t_T_DI_B_setup_worst = 0.1317,
  t_T_DI_A_hold_worst  = 0.1114,
  t_T_DI_B_hold_worst  = 0.1114,
  t_T_CE_N_A_setup_worst = 0.1578,
  t_T_CE_N_B_setup_worst = 0.1578,
  t_T_CE_N_A_hold_worst  = 0.0424,
  t_T_CE_N_B_hold_worst  = 0.0424,
  t_T_GWE_N_A_setup_worst = 0.2098,
  t_T_GWE_N_B_setup_worst = 0.2098,
  t_T_GWE_N_A_hold_worst  = -0.0685,
  t_T_GWE_N_B_hold_worst  = -0.0685,
  t_T_BWE_N_A_setup_worst = 0.1317,
  t_T_BWE_N_B_setup_worst = 0.1317,
  t_T_BWE_N_A_hold_worst  = 0.1114,
  t_T_BWE_N_B_hold_worst  = 0.1114,
  t_GWE_N_A_setup_worst = 0.1574,
  t_GWE_N_B_setup_worst = 0.1574,
  t_GWE_N_A_hold_worst  = -0.0178,
  t_GWE_N_B_hold_worst  = -0.0178;
  $period(posedge CLK_A &&& timing_condition_margin_0_a, period_param_0);
  $period(posedge CLK_A &&& timing_condition_margin_1_a, period_param_1);
  $period(posedge CLK_A &&& timing_condition_margin_2_a, period_param_2);
  $period(posedge CLK_A &&& timing_condition_margin_3_a, period_param_3);
  $period(posedge CLK_A &&& timing_condition_margin_4_a, period_param_4);
  $period(posedge CLK_A &&& timing_condition_margin_5_a, period_param_5);
  $period(posedge CLK_A &&& timing_condition_margin_6_a, period_param_6);
  $period(posedge CLK_A &&& timing_condition_margin_7_a, period_param_7);
  $period(posedge CLK_B &&& timing_condition_margin_0_b, period_param_0);
  $period(posedge CLK_B &&& timing_condition_margin_1_b, period_param_1);
  $period(posedge CLK_B &&& timing_condition_margin_2_b, period_param_2);
  $period(posedge CLK_B &&& timing_condition_margin_3_b, period_param_3);
  $period(posedge CLK_B &&& timing_condition_margin_4_b, period_param_4);
  $period(posedge CLK_B &&& timing_condition_margin_5_b, period_param_5);
  $period(posedge CLK_B &&& timing_condition_margin_6_b, period_param_6);
  $period(posedge CLK_B &&& timing_condition_margin_7_b, period_param_7);
  $width(posedge CLK_A, 0.082531);
  $width(posedge CLK_B, 0.082531);
  $width(negedge CLK_A, 0.200960);
  $width(negedge CLK_B, 0.200960);
  /*The default setting corresponds to T_RWM == 3'd3*/
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 0 && T_RWM_A[2] == 0 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[0] +: T_DI_A[0])) = (trise_CLK_DO_A_worst_0, tfall_CLK_DO_A_worst_0);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 0 && T_RWM_A[2] == 0 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[1] +: T_DI_A[1])) = (trise_CLK_DO_A_worst_0, tfall_CLK_DO_A_worst_0);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 0 && T_RWM_A[2] == 0 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[2] +: T_DI_A[2])) = (trise_CLK_DO_A_worst_0, tfall_CLK_DO_A_worst_0);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 0 && T_RWM_A[2] == 0 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[3] +: T_DI_A[3])) = (trise_CLK_DO_A_worst_0, tfall_CLK_DO_A_worst_0);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 0 && T_RWM_A[2] == 0 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[4] +: T_DI_A[4])) = (trise_CLK_DO_A_worst_0, tfall_CLK_DO_A_worst_0);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 0 && T_RWM_A[2] == 0 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[5] +: T_DI_A[5])) = (trise_CLK_DO_A_worst_0, tfall_CLK_DO_A_worst_0);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 0 && T_RWM_A[2] == 0 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[6] +: T_DI_A[6])) = (trise_CLK_DO_A_worst_0, tfall_CLK_DO_A_worst_0);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 0 && T_RWM_A[2] == 0 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[7] +: T_DI_A[7])) = (trise_CLK_DO_A_worst_0, tfall_CLK_DO_A_worst_0);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 0 && T_RWM_A[2] == 0 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[8] +: T_DI_A[8])) = (trise_CLK_DO_A_worst_0, tfall_CLK_DO_A_worst_0);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 0 && T_RWM_A[2] == 0 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[9] +: T_DI_A[9])) = (trise_CLK_DO_A_worst_0, tfall_CLK_DO_A_worst_0);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 0 && T_RWM_A[2] == 0 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[10] +: T_DI_A[10])) = (trise_CLK_DO_A_worst_0, tfall_CLK_DO_A_worst_0);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 0 && T_RWM_A[2] == 0 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[11] +: T_DI_A[11])) = (trise_CLK_DO_A_worst_0, tfall_CLK_DO_A_worst_0);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 0 && T_RWM_A[2] == 0 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[12] +: T_DI_A[12])) = (trise_CLK_DO_A_worst_0, tfall_CLK_DO_A_worst_0);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 0 && T_RWM_A[2] == 0 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[13] +: T_DI_A[13])) = (trise_CLK_DO_A_worst_0, tfall_CLK_DO_A_worst_0);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 0 && T_RWM_A[2] == 0 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[14] +: T_DI_A[14])) = (trise_CLK_DO_A_worst_0, tfall_CLK_DO_A_worst_0);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 0 && T_RWM_A[2] == 0 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[15] +: T_DI_A[15])) = (trise_CLK_DO_A_worst_0, tfall_CLK_DO_A_worst_0);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 0 && T_RWM_A[2] == 0 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[16] +: T_DI_A[16])) = (trise_CLK_DO_A_worst_0, tfall_CLK_DO_A_worst_0);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 0 && T_RWM_A[2] == 0 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[17] +: T_DI_A[17])) = (trise_CLK_DO_A_worst_0, tfall_CLK_DO_A_worst_0);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 0 && T_RWM_A[2] == 0 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[0] +: T_DI_A[0])) = (trise_CLK_DO_A_worst_1, tfall_CLK_DO_A_worst_1);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 0 && T_RWM_A[2] == 0 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[1] +: T_DI_A[1])) = (trise_CLK_DO_A_worst_1, tfall_CLK_DO_A_worst_1);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 0 && T_RWM_A[2] == 0 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[2] +: T_DI_A[2])) = (trise_CLK_DO_A_worst_1, tfall_CLK_DO_A_worst_1);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 0 && T_RWM_A[2] == 0 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[3] +: T_DI_A[3])) = (trise_CLK_DO_A_worst_1, tfall_CLK_DO_A_worst_1);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 0 && T_RWM_A[2] == 0 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[4] +: T_DI_A[4])) = (trise_CLK_DO_A_worst_1, tfall_CLK_DO_A_worst_1);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 0 && T_RWM_A[2] == 0 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[5] +: T_DI_A[5])) = (trise_CLK_DO_A_worst_1, tfall_CLK_DO_A_worst_1);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 0 && T_RWM_A[2] == 0 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[6] +: T_DI_A[6])) = (trise_CLK_DO_A_worst_1, tfall_CLK_DO_A_worst_1);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 0 && T_RWM_A[2] == 0 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[7] +: T_DI_A[7])) = (trise_CLK_DO_A_worst_1, tfall_CLK_DO_A_worst_1);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 0 && T_RWM_A[2] == 0 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[8] +: T_DI_A[8])) = (trise_CLK_DO_A_worst_1, tfall_CLK_DO_A_worst_1);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 0 && T_RWM_A[2] == 0 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[9] +: T_DI_A[9])) = (trise_CLK_DO_A_worst_1, tfall_CLK_DO_A_worst_1);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 0 && T_RWM_A[2] == 0 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[10] +: T_DI_A[10])) = (trise_CLK_DO_A_worst_1, tfall_CLK_DO_A_worst_1);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 0 && T_RWM_A[2] == 0 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[11] +: T_DI_A[11])) = (trise_CLK_DO_A_worst_1, tfall_CLK_DO_A_worst_1);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 0 && T_RWM_A[2] == 0 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[12] +: T_DI_A[12])) = (trise_CLK_DO_A_worst_1, tfall_CLK_DO_A_worst_1);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 0 && T_RWM_A[2] == 0 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[13] +: T_DI_A[13])) = (trise_CLK_DO_A_worst_1, tfall_CLK_DO_A_worst_1);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 0 && T_RWM_A[2] == 0 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[14] +: T_DI_A[14])) = (trise_CLK_DO_A_worst_1, tfall_CLK_DO_A_worst_1);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 0 && T_RWM_A[2] == 0 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[15] +: T_DI_A[15])) = (trise_CLK_DO_A_worst_1, tfall_CLK_DO_A_worst_1);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 0 && T_RWM_A[2] == 0 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[16] +: T_DI_A[16])) = (trise_CLK_DO_A_worst_1, tfall_CLK_DO_A_worst_1);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 0 && T_RWM_A[2] == 0 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[17] +: T_DI_A[17])) = (trise_CLK_DO_A_worst_1, tfall_CLK_DO_A_worst_1);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 1 && T_RWM_A[2] == 0 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[0] +: T_DI_A[0])) = (trise_CLK_DO_A_worst_2, tfall_CLK_DO_A_worst_2);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 1 && T_RWM_A[2] == 0 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[1] +: T_DI_A[1])) = (trise_CLK_DO_A_worst_2, tfall_CLK_DO_A_worst_2);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 1 && T_RWM_A[2] == 0 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[2] +: T_DI_A[2])) = (trise_CLK_DO_A_worst_2, tfall_CLK_DO_A_worst_2);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 1 && T_RWM_A[2] == 0 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[3] +: T_DI_A[3])) = (trise_CLK_DO_A_worst_2, tfall_CLK_DO_A_worst_2);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 1 && T_RWM_A[2] == 0 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[4] +: T_DI_A[4])) = (trise_CLK_DO_A_worst_2, tfall_CLK_DO_A_worst_2);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 1 && T_RWM_A[2] == 0 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[5] +: T_DI_A[5])) = (trise_CLK_DO_A_worst_2, tfall_CLK_DO_A_worst_2);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 1 && T_RWM_A[2] == 0 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[6] +: T_DI_A[6])) = (trise_CLK_DO_A_worst_2, tfall_CLK_DO_A_worst_2);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 1 && T_RWM_A[2] == 0 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[7] +: T_DI_A[7])) = (trise_CLK_DO_A_worst_2, tfall_CLK_DO_A_worst_2);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 1 && T_RWM_A[2] == 0 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[8] +: T_DI_A[8])) = (trise_CLK_DO_A_worst_2, tfall_CLK_DO_A_worst_2);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 1 && T_RWM_A[2] == 0 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[9] +: T_DI_A[9])) = (trise_CLK_DO_A_worst_2, tfall_CLK_DO_A_worst_2);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 1 && T_RWM_A[2] == 0 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[10] +: T_DI_A[10])) = (trise_CLK_DO_A_worst_2, tfall_CLK_DO_A_worst_2);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 1 && T_RWM_A[2] == 0 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[11] +: T_DI_A[11])) = (trise_CLK_DO_A_worst_2, tfall_CLK_DO_A_worst_2);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 1 && T_RWM_A[2] == 0 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[12] +: T_DI_A[12])) = (trise_CLK_DO_A_worst_2, tfall_CLK_DO_A_worst_2);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 1 && T_RWM_A[2] == 0 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[13] +: T_DI_A[13])) = (trise_CLK_DO_A_worst_2, tfall_CLK_DO_A_worst_2);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 1 && T_RWM_A[2] == 0 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[14] +: T_DI_A[14])) = (trise_CLK_DO_A_worst_2, tfall_CLK_DO_A_worst_2);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 1 && T_RWM_A[2] == 0 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[15] +: T_DI_A[15])) = (trise_CLK_DO_A_worst_2, tfall_CLK_DO_A_worst_2);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 1 && T_RWM_A[2] == 0 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[16] +: T_DI_A[16])) = (trise_CLK_DO_A_worst_2, tfall_CLK_DO_A_worst_2);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 1 && T_RWM_A[2] == 0 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[17] +: T_DI_A[17])) = (trise_CLK_DO_A_worst_2, tfall_CLK_DO_A_worst_2);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 1 && T_RWM_A[2] == 0 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[0] +: T_DI_A[0])) = (trise_CLK_DO_A_worst_3, tfall_CLK_DO_A_worst_3);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 1 && T_RWM_A[2] == 0 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[1] +: T_DI_A[1])) = (trise_CLK_DO_A_worst_3, tfall_CLK_DO_A_worst_3);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 1 && T_RWM_A[2] == 0 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[2] +: T_DI_A[2])) = (trise_CLK_DO_A_worst_3, tfall_CLK_DO_A_worst_3);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 1 && T_RWM_A[2] == 0 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[3] +: T_DI_A[3])) = (trise_CLK_DO_A_worst_3, tfall_CLK_DO_A_worst_3);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 1 && T_RWM_A[2] == 0 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[4] +: T_DI_A[4])) = (trise_CLK_DO_A_worst_3, tfall_CLK_DO_A_worst_3);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 1 && T_RWM_A[2] == 0 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[5] +: T_DI_A[5])) = (trise_CLK_DO_A_worst_3, tfall_CLK_DO_A_worst_3);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 1 && T_RWM_A[2] == 0 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[6] +: T_DI_A[6])) = (trise_CLK_DO_A_worst_3, tfall_CLK_DO_A_worst_3);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 1 && T_RWM_A[2] == 0 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[7] +: T_DI_A[7])) = (trise_CLK_DO_A_worst_3, tfall_CLK_DO_A_worst_3);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 1 && T_RWM_A[2] == 0 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[8] +: T_DI_A[8])) = (trise_CLK_DO_A_worst_3, tfall_CLK_DO_A_worst_3);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 1 && T_RWM_A[2] == 0 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[9] +: T_DI_A[9])) = (trise_CLK_DO_A_worst_3, tfall_CLK_DO_A_worst_3);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 1 && T_RWM_A[2] == 0 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[10] +: T_DI_A[10])) = (trise_CLK_DO_A_worst_3, tfall_CLK_DO_A_worst_3);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 1 && T_RWM_A[2] == 0 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[11] +: T_DI_A[11])) = (trise_CLK_DO_A_worst_3, tfall_CLK_DO_A_worst_3);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 1 && T_RWM_A[2] == 0 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[12] +: T_DI_A[12])) = (trise_CLK_DO_A_worst_3, tfall_CLK_DO_A_worst_3);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 1 && T_RWM_A[2] == 0 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[13] +: T_DI_A[13])) = (trise_CLK_DO_A_worst_3, tfall_CLK_DO_A_worst_3);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 1 && T_RWM_A[2] == 0 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[14] +: T_DI_A[14])) = (trise_CLK_DO_A_worst_3, tfall_CLK_DO_A_worst_3);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 1 && T_RWM_A[2] == 0 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[15] +: T_DI_A[15])) = (trise_CLK_DO_A_worst_3, tfall_CLK_DO_A_worst_3);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 1 && T_RWM_A[2] == 0 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[16] +: T_DI_A[16])) = (trise_CLK_DO_A_worst_3, tfall_CLK_DO_A_worst_3);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 1 && T_RWM_A[2] == 0 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[17] +: T_DI_A[17])) = (trise_CLK_DO_A_worst_3, tfall_CLK_DO_A_worst_3);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 0 && T_RWM_A[2] == 1 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[0] +: T_DI_A[0])) = (trise_CLK_DO_A_worst_4, tfall_CLK_DO_A_worst_4);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 0 && T_RWM_A[2] == 1 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[1] +: T_DI_A[1])) = (trise_CLK_DO_A_worst_4, tfall_CLK_DO_A_worst_4);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 0 && T_RWM_A[2] == 1 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[2] +: T_DI_A[2])) = (trise_CLK_DO_A_worst_4, tfall_CLK_DO_A_worst_4);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 0 && T_RWM_A[2] == 1 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[3] +: T_DI_A[3])) = (trise_CLK_DO_A_worst_4, tfall_CLK_DO_A_worst_4);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 0 && T_RWM_A[2] == 1 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[4] +: T_DI_A[4])) = (trise_CLK_DO_A_worst_4, tfall_CLK_DO_A_worst_4);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 0 && T_RWM_A[2] == 1 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[5] +: T_DI_A[5])) = (trise_CLK_DO_A_worst_4, tfall_CLK_DO_A_worst_4);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 0 && T_RWM_A[2] == 1 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[6] +: T_DI_A[6])) = (trise_CLK_DO_A_worst_4, tfall_CLK_DO_A_worst_4);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 0 && T_RWM_A[2] == 1 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[7] +: T_DI_A[7])) = (trise_CLK_DO_A_worst_4, tfall_CLK_DO_A_worst_4);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 0 && T_RWM_A[2] == 1 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[8] +: T_DI_A[8])) = (trise_CLK_DO_A_worst_4, tfall_CLK_DO_A_worst_4);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 0 && T_RWM_A[2] == 1 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[9] +: T_DI_A[9])) = (trise_CLK_DO_A_worst_4, tfall_CLK_DO_A_worst_4);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 0 && T_RWM_A[2] == 1 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[10] +: T_DI_A[10])) = (trise_CLK_DO_A_worst_4, tfall_CLK_DO_A_worst_4);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 0 && T_RWM_A[2] == 1 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[11] +: T_DI_A[11])) = (trise_CLK_DO_A_worst_4, tfall_CLK_DO_A_worst_4);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 0 && T_RWM_A[2] == 1 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[12] +: T_DI_A[12])) = (trise_CLK_DO_A_worst_4, tfall_CLK_DO_A_worst_4);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 0 && T_RWM_A[2] == 1 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[13] +: T_DI_A[13])) = (trise_CLK_DO_A_worst_4, tfall_CLK_DO_A_worst_4);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 0 && T_RWM_A[2] == 1 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[14] +: T_DI_A[14])) = (trise_CLK_DO_A_worst_4, tfall_CLK_DO_A_worst_4);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 0 && T_RWM_A[2] == 1 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[15] +: T_DI_A[15])) = (trise_CLK_DO_A_worst_4, tfall_CLK_DO_A_worst_4);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 0 && T_RWM_A[2] == 1 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[16] +: T_DI_A[16])) = (trise_CLK_DO_A_worst_4, tfall_CLK_DO_A_worst_4);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 0 && T_RWM_A[2] == 1 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[17] +: T_DI_A[17])) = (trise_CLK_DO_A_worst_4, tfall_CLK_DO_A_worst_4);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 0 && T_RWM_A[2] == 1 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[0] +: T_DI_A[0])) = (trise_CLK_DO_A_worst_5, tfall_CLK_DO_A_worst_5);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 0 && T_RWM_A[2] == 1 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[1] +: T_DI_A[1])) = (trise_CLK_DO_A_worst_5, tfall_CLK_DO_A_worst_5);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 0 && T_RWM_A[2] == 1 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[2] +: T_DI_A[2])) = (trise_CLK_DO_A_worst_5, tfall_CLK_DO_A_worst_5);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 0 && T_RWM_A[2] == 1 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[3] +: T_DI_A[3])) = (trise_CLK_DO_A_worst_5, tfall_CLK_DO_A_worst_5);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 0 && T_RWM_A[2] == 1 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[4] +: T_DI_A[4])) = (trise_CLK_DO_A_worst_5, tfall_CLK_DO_A_worst_5);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 0 && T_RWM_A[2] == 1 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[5] +: T_DI_A[5])) = (trise_CLK_DO_A_worst_5, tfall_CLK_DO_A_worst_5);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 0 && T_RWM_A[2] == 1 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[6] +: T_DI_A[6])) = (trise_CLK_DO_A_worst_5, tfall_CLK_DO_A_worst_5);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 0 && T_RWM_A[2] == 1 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[7] +: T_DI_A[7])) = (trise_CLK_DO_A_worst_5, tfall_CLK_DO_A_worst_5);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 0 && T_RWM_A[2] == 1 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[8] +: T_DI_A[8])) = (trise_CLK_DO_A_worst_5, tfall_CLK_DO_A_worst_5);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 0 && T_RWM_A[2] == 1 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[9] +: T_DI_A[9])) = (trise_CLK_DO_A_worst_5, tfall_CLK_DO_A_worst_5);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 0 && T_RWM_A[2] == 1 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[10] +: T_DI_A[10])) = (trise_CLK_DO_A_worst_5, tfall_CLK_DO_A_worst_5);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 0 && T_RWM_A[2] == 1 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[11] +: T_DI_A[11])) = (trise_CLK_DO_A_worst_5, tfall_CLK_DO_A_worst_5);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 0 && T_RWM_A[2] == 1 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[12] +: T_DI_A[12])) = (trise_CLK_DO_A_worst_5, tfall_CLK_DO_A_worst_5);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 0 && T_RWM_A[2] == 1 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[13] +: T_DI_A[13])) = (trise_CLK_DO_A_worst_5, tfall_CLK_DO_A_worst_5);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 0 && T_RWM_A[2] == 1 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[14] +: T_DI_A[14])) = (trise_CLK_DO_A_worst_5, tfall_CLK_DO_A_worst_5);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 0 && T_RWM_A[2] == 1 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[15] +: T_DI_A[15])) = (trise_CLK_DO_A_worst_5, tfall_CLK_DO_A_worst_5);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 0 && T_RWM_A[2] == 1 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[16] +: T_DI_A[16])) = (trise_CLK_DO_A_worst_5, tfall_CLK_DO_A_worst_5);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 0 && T_RWM_A[2] == 1 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[17] +: T_DI_A[17])) = (trise_CLK_DO_A_worst_5, tfall_CLK_DO_A_worst_5);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 1 && T_RWM_A[2] == 1 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[0] +: T_DI_A[0])) = (trise_CLK_DO_A_worst_6, tfall_CLK_DO_A_worst_6);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 1 && T_RWM_A[2] == 1 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[1] +: T_DI_A[1])) = (trise_CLK_DO_A_worst_6, tfall_CLK_DO_A_worst_6);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 1 && T_RWM_A[2] == 1 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[2] +: T_DI_A[2])) = (trise_CLK_DO_A_worst_6, tfall_CLK_DO_A_worst_6);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 1 && T_RWM_A[2] == 1 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[3] +: T_DI_A[3])) = (trise_CLK_DO_A_worst_6, tfall_CLK_DO_A_worst_6);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 1 && T_RWM_A[2] == 1 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[4] +: T_DI_A[4])) = (trise_CLK_DO_A_worst_6, tfall_CLK_DO_A_worst_6);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 1 && T_RWM_A[2] == 1 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[5] +: T_DI_A[5])) = (trise_CLK_DO_A_worst_6, tfall_CLK_DO_A_worst_6);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 1 && T_RWM_A[2] == 1 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[6] +: T_DI_A[6])) = (trise_CLK_DO_A_worst_6, tfall_CLK_DO_A_worst_6);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 1 && T_RWM_A[2] == 1 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[7] +: T_DI_A[7])) = (trise_CLK_DO_A_worst_6, tfall_CLK_DO_A_worst_6);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 1 && T_RWM_A[2] == 1 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[8] +: T_DI_A[8])) = (trise_CLK_DO_A_worst_6, tfall_CLK_DO_A_worst_6);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 1 && T_RWM_A[2] == 1 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[9] +: T_DI_A[9])) = (trise_CLK_DO_A_worst_6, tfall_CLK_DO_A_worst_6);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 1 && T_RWM_A[2] == 1 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[10] +: T_DI_A[10])) = (trise_CLK_DO_A_worst_6, tfall_CLK_DO_A_worst_6);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 1 && T_RWM_A[2] == 1 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[11] +: T_DI_A[11])) = (trise_CLK_DO_A_worst_6, tfall_CLK_DO_A_worst_6);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 1 && T_RWM_A[2] == 1 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[12] +: T_DI_A[12])) = (trise_CLK_DO_A_worst_6, tfall_CLK_DO_A_worst_6);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 1 && T_RWM_A[2] == 1 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[13] +: T_DI_A[13])) = (trise_CLK_DO_A_worst_6, tfall_CLK_DO_A_worst_6);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 1 && T_RWM_A[2] == 1 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[14] +: T_DI_A[14])) = (trise_CLK_DO_A_worst_6, tfall_CLK_DO_A_worst_6);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 1 && T_RWM_A[2] == 1 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[15] +: T_DI_A[15])) = (trise_CLK_DO_A_worst_6, tfall_CLK_DO_A_worst_6);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 1 && T_RWM_A[2] == 1 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[16] +: T_DI_A[16])) = (trise_CLK_DO_A_worst_6, tfall_CLK_DO_A_worst_6);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 1 && T_RWM_A[2] == 1 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[17] +: T_DI_A[17])) = (trise_CLK_DO_A_worst_6, tfall_CLK_DO_A_worst_6);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 1 && T_RWM_A[2] == 1 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[0] +: T_DI_A[0])) = (trise_CLK_DO_A_worst_7, tfall_CLK_DO_A_worst_7);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 1 && T_RWM_A[2] == 1 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[1] +: T_DI_A[1])) = (trise_CLK_DO_A_worst_7, tfall_CLK_DO_A_worst_7);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 1 && T_RWM_A[2] == 1 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[2] +: T_DI_A[2])) = (trise_CLK_DO_A_worst_7, tfall_CLK_DO_A_worst_7);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 1 && T_RWM_A[2] == 1 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[3] +: T_DI_A[3])) = (trise_CLK_DO_A_worst_7, tfall_CLK_DO_A_worst_7);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 1 && T_RWM_A[2] == 1 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[4] +: T_DI_A[4])) = (trise_CLK_DO_A_worst_7, tfall_CLK_DO_A_worst_7);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 1 && T_RWM_A[2] == 1 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[5] +: T_DI_A[5])) = (trise_CLK_DO_A_worst_7, tfall_CLK_DO_A_worst_7);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 1 && T_RWM_A[2] == 1 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[6] +: T_DI_A[6])) = (trise_CLK_DO_A_worst_7, tfall_CLK_DO_A_worst_7);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 1 && T_RWM_A[2] == 1 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[7] +: T_DI_A[7])) = (trise_CLK_DO_A_worst_7, tfall_CLK_DO_A_worst_7);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 1 && T_RWM_A[2] == 1 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[8] +: T_DI_A[8])) = (trise_CLK_DO_A_worst_7, tfall_CLK_DO_A_worst_7);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 1 && T_RWM_A[2] == 1 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[9] +: T_DI_A[9])) = (trise_CLK_DO_A_worst_7, tfall_CLK_DO_A_worst_7);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 1 && T_RWM_A[2] == 1 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[10] +: T_DI_A[10])) = (trise_CLK_DO_A_worst_7, tfall_CLK_DO_A_worst_7);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 1 && T_RWM_A[2] == 1 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[11] +: T_DI_A[11])) = (trise_CLK_DO_A_worst_7, tfall_CLK_DO_A_worst_7);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 1 && T_RWM_A[2] == 1 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[12] +: T_DI_A[12])) = (trise_CLK_DO_A_worst_7, tfall_CLK_DO_A_worst_7);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 1 && T_RWM_A[2] == 1 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[13] +: T_DI_A[13])) = (trise_CLK_DO_A_worst_7, tfall_CLK_DO_A_worst_7);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 1 && T_RWM_A[2] == 1 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[14] +: T_DI_A[14])) = (trise_CLK_DO_A_worst_7, tfall_CLK_DO_A_worst_7);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 1 && T_RWM_A[2] == 1 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[15] +: T_DI_A[15])) = (trise_CLK_DO_A_worst_7, tfall_CLK_DO_A_worst_7);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 1 && T_RWM_A[2] == 1 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[16] +: T_DI_A[16])) = (trise_CLK_DO_A_worst_7, tfall_CLK_DO_A_worst_7);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 1 && T_RWM_A[2] == 1 && T_BE_N == 0) 
  (posedge CLK_A=>(DO_A[17] +: T_DI_A[17])) = (trise_CLK_DO_A_worst_7, tfall_CLK_DO_A_worst_7);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 0 && T_RWM_B[2] == 0 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[0] +: T_DI_B[0])) = (trise_CLK_DO_B_worst_0, tfall_CLK_DO_B_worst_0);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 0 && T_RWM_B[2] == 0 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[1] +: T_DI_B[1])) = (trise_CLK_DO_B_worst_0, tfall_CLK_DO_B_worst_0);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 0 && T_RWM_B[2] == 0 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[2] +: T_DI_B[2])) = (trise_CLK_DO_B_worst_0, tfall_CLK_DO_B_worst_0);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 0 && T_RWM_B[2] == 0 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[3] +: T_DI_B[3])) = (trise_CLK_DO_B_worst_0, tfall_CLK_DO_B_worst_0);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 0 && T_RWM_B[2] == 0 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[4] +: T_DI_B[4])) = (trise_CLK_DO_B_worst_0, tfall_CLK_DO_B_worst_0);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 0 && T_RWM_B[2] == 0 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[5] +: T_DI_B[5])) = (trise_CLK_DO_B_worst_0, tfall_CLK_DO_B_worst_0);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 0 && T_RWM_B[2] == 0 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[6] +: T_DI_B[6])) = (trise_CLK_DO_B_worst_0, tfall_CLK_DO_B_worst_0);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 0 && T_RWM_B[2] == 0 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[7] +: T_DI_B[7])) = (trise_CLK_DO_B_worst_0, tfall_CLK_DO_B_worst_0);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 0 && T_RWM_B[2] == 0 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[8] +: T_DI_B[8])) = (trise_CLK_DO_B_worst_0, tfall_CLK_DO_B_worst_0);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 0 && T_RWM_B[2] == 0 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[9] +: T_DI_B[9])) = (trise_CLK_DO_B_worst_0, tfall_CLK_DO_B_worst_0);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 0 && T_RWM_B[2] == 0 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[10] +: T_DI_B[10])) = (trise_CLK_DO_B_worst_0, tfall_CLK_DO_B_worst_0);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 0 && T_RWM_B[2] == 0 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[11] +: T_DI_B[11])) = (trise_CLK_DO_B_worst_0, tfall_CLK_DO_B_worst_0);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 0 && T_RWM_B[2] == 0 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[12] +: T_DI_B[12])) = (trise_CLK_DO_B_worst_0, tfall_CLK_DO_B_worst_0);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 0 && T_RWM_B[2] == 0 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[13] +: T_DI_B[13])) = (trise_CLK_DO_B_worst_0, tfall_CLK_DO_B_worst_0);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 0 && T_RWM_B[2] == 0 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[14] +: T_DI_B[14])) = (trise_CLK_DO_B_worst_0, tfall_CLK_DO_B_worst_0);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 0 && T_RWM_B[2] == 0 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[15] +: T_DI_B[15])) = (trise_CLK_DO_B_worst_0, tfall_CLK_DO_B_worst_0);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 0 && T_RWM_B[2] == 0 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[16] +: T_DI_B[16])) = (trise_CLK_DO_B_worst_0, tfall_CLK_DO_B_worst_0);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 0 && T_RWM_B[2] == 0 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[17] +: T_DI_B[17])) = (trise_CLK_DO_B_worst_0, tfall_CLK_DO_B_worst_0);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 0 && T_RWM_B[2] == 0 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[0] +: T_DI_B[0])) = (trise_CLK_DO_B_worst_1, tfall_CLK_DO_B_worst_1);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 0 && T_RWM_B[2] == 0 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[1] +: T_DI_B[1])) = (trise_CLK_DO_B_worst_1, tfall_CLK_DO_B_worst_1);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 0 && T_RWM_B[2] == 0 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[2] +: T_DI_B[2])) = (trise_CLK_DO_B_worst_1, tfall_CLK_DO_B_worst_1);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 0 && T_RWM_B[2] == 0 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[3] +: T_DI_B[3])) = (trise_CLK_DO_B_worst_1, tfall_CLK_DO_B_worst_1);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 0 && T_RWM_B[2] == 0 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[4] +: T_DI_B[4])) = (trise_CLK_DO_B_worst_1, tfall_CLK_DO_B_worst_1);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 0 && T_RWM_B[2] == 0 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[5] +: T_DI_B[5])) = (trise_CLK_DO_B_worst_1, tfall_CLK_DO_B_worst_1);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 0 && T_RWM_B[2] == 0 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[6] +: T_DI_B[6])) = (trise_CLK_DO_B_worst_1, tfall_CLK_DO_B_worst_1);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 0 && T_RWM_B[2] == 0 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[7] +: T_DI_B[7])) = (trise_CLK_DO_B_worst_1, tfall_CLK_DO_B_worst_1);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 0 && T_RWM_B[2] == 0 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[8] +: T_DI_B[8])) = (trise_CLK_DO_B_worst_1, tfall_CLK_DO_B_worst_1);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 0 && T_RWM_B[2] == 0 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[9] +: T_DI_B[9])) = (trise_CLK_DO_B_worst_1, tfall_CLK_DO_B_worst_1);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 0 && T_RWM_B[2] == 0 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[10] +: T_DI_B[10])) = (trise_CLK_DO_B_worst_1, tfall_CLK_DO_B_worst_1);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 0 && T_RWM_B[2] == 0 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[11] +: T_DI_B[11])) = (trise_CLK_DO_B_worst_1, tfall_CLK_DO_B_worst_1);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 0 && T_RWM_B[2] == 0 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[12] +: T_DI_B[12])) = (trise_CLK_DO_B_worst_1, tfall_CLK_DO_B_worst_1);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 0 && T_RWM_B[2] == 0 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[13] +: T_DI_B[13])) = (trise_CLK_DO_B_worst_1, tfall_CLK_DO_B_worst_1);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 0 && T_RWM_B[2] == 0 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[14] +: T_DI_B[14])) = (trise_CLK_DO_B_worst_1, tfall_CLK_DO_B_worst_1);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 0 && T_RWM_B[2] == 0 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[15] +: T_DI_B[15])) = (trise_CLK_DO_B_worst_1, tfall_CLK_DO_B_worst_1);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 0 && T_RWM_B[2] == 0 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[16] +: T_DI_B[16])) = (trise_CLK_DO_B_worst_1, tfall_CLK_DO_B_worst_1);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 0 && T_RWM_B[2] == 0 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[17] +: T_DI_B[17])) = (trise_CLK_DO_B_worst_1, tfall_CLK_DO_B_worst_1);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 1 && T_RWM_B[2] == 0 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[0] +: T_DI_B[0])) = (trise_CLK_DO_B_worst_2, tfall_CLK_DO_B_worst_2);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 1 && T_RWM_B[2] == 0 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[1] +: T_DI_B[1])) = (trise_CLK_DO_B_worst_2, tfall_CLK_DO_B_worst_2);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 1 && T_RWM_B[2] == 0 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[2] +: T_DI_B[2])) = (trise_CLK_DO_B_worst_2, tfall_CLK_DO_B_worst_2);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 1 && T_RWM_B[2] == 0 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[3] +: T_DI_B[3])) = (trise_CLK_DO_B_worst_2, tfall_CLK_DO_B_worst_2);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 1 && T_RWM_B[2] == 0 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[4] +: T_DI_B[4])) = (trise_CLK_DO_B_worst_2, tfall_CLK_DO_B_worst_2);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 1 && T_RWM_B[2] == 0 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[5] +: T_DI_B[5])) = (trise_CLK_DO_B_worst_2, tfall_CLK_DO_B_worst_2);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 1 && T_RWM_B[2] == 0 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[6] +: T_DI_B[6])) = (trise_CLK_DO_B_worst_2, tfall_CLK_DO_B_worst_2);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 1 && T_RWM_B[2] == 0 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[7] +: T_DI_B[7])) = (trise_CLK_DO_B_worst_2, tfall_CLK_DO_B_worst_2);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 1 && T_RWM_B[2] == 0 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[8] +: T_DI_B[8])) = (trise_CLK_DO_B_worst_2, tfall_CLK_DO_B_worst_2);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 1 && T_RWM_B[2] == 0 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[9] +: T_DI_B[9])) = (trise_CLK_DO_B_worst_2, tfall_CLK_DO_B_worst_2);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 1 && T_RWM_B[2] == 0 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[10] +: T_DI_B[10])) = (trise_CLK_DO_B_worst_2, tfall_CLK_DO_B_worst_2);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 1 && T_RWM_B[2] == 0 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[11] +: T_DI_B[11])) = (trise_CLK_DO_B_worst_2, tfall_CLK_DO_B_worst_2);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 1 && T_RWM_B[2] == 0 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[12] +: T_DI_B[12])) = (trise_CLK_DO_B_worst_2, tfall_CLK_DO_B_worst_2);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 1 && T_RWM_B[2] == 0 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[13] +: T_DI_B[13])) = (trise_CLK_DO_B_worst_2, tfall_CLK_DO_B_worst_2);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 1 && T_RWM_B[2] == 0 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[14] +: T_DI_B[14])) = (trise_CLK_DO_B_worst_2, tfall_CLK_DO_B_worst_2);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 1 && T_RWM_B[2] == 0 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[15] +: T_DI_B[15])) = (trise_CLK_DO_B_worst_2, tfall_CLK_DO_B_worst_2);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 1 && T_RWM_B[2] == 0 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[16] +: T_DI_B[16])) = (trise_CLK_DO_B_worst_2, tfall_CLK_DO_B_worst_2);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 1 && T_RWM_B[2] == 0 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[17] +: T_DI_B[17])) = (trise_CLK_DO_B_worst_2, tfall_CLK_DO_B_worst_2);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 1 && T_RWM_B[2] == 0 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[0] +: T_DI_B[0])) = (trise_CLK_DO_B_worst_3, tfall_CLK_DO_B_worst_3);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 1 && T_RWM_B[2] == 0 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[1] +: T_DI_B[1])) = (trise_CLK_DO_B_worst_3, tfall_CLK_DO_B_worst_3);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 1 && T_RWM_B[2] == 0 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[2] +: T_DI_B[2])) = (trise_CLK_DO_B_worst_3, tfall_CLK_DO_B_worst_3);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 1 && T_RWM_B[2] == 0 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[3] +: T_DI_B[3])) = (trise_CLK_DO_B_worst_3, tfall_CLK_DO_B_worst_3);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 1 && T_RWM_B[2] == 0 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[4] +: T_DI_B[4])) = (trise_CLK_DO_B_worst_3, tfall_CLK_DO_B_worst_3);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 1 && T_RWM_B[2] == 0 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[5] +: T_DI_B[5])) = (trise_CLK_DO_B_worst_3, tfall_CLK_DO_B_worst_3);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 1 && T_RWM_B[2] == 0 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[6] +: T_DI_B[6])) = (trise_CLK_DO_B_worst_3, tfall_CLK_DO_B_worst_3);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 1 && T_RWM_B[2] == 0 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[7] +: T_DI_B[7])) = (trise_CLK_DO_B_worst_3, tfall_CLK_DO_B_worst_3);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 1 && T_RWM_B[2] == 0 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[8] +: T_DI_B[8])) = (trise_CLK_DO_B_worst_3, tfall_CLK_DO_B_worst_3);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 1 && T_RWM_B[2] == 0 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[9] +: T_DI_B[9])) = (trise_CLK_DO_B_worst_3, tfall_CLK_DO_B_worst_3);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 1 && T_RWM_B[2] == 0 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[10] +: T_DI_B[10])) = (trise_CLK_DO_B_worst_3, tfall_CLK_DO_B_worst_3);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 1 && T_RWM_B[2] == 0 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[11] +: T_DI_B[11])) = (trise_CLK_DO_B_worst_3, tfall_CLK_DO_B_worst_3);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 1 && T_RWM_B[2] == 0 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[12] +: T_DI_B[12])) = (trise_CLK_DO_B_worst_3, tfall_CLK_DO_B_worst_3);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 1 && T_RWM_B[2] == 0 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[13] +: T_DI_B[13])) = (trise_CLK_DO_B_worst_3, tfall_CLK_DO_B_worst_3);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 1 && T_RWM_B[2] == 0 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[14] +: T_DI_B[14])) = (trise_CLK_DO_B_worst_3, tfall_CLK_DO_B_worst_3);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 1 && T_RWM_B[2] == 0 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[15] +: T_DI_B[15])) = (trise_CLK_DO_B_worst_3, tfall_CLK_DO_B_worst_3);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 1 && T_RWM_B[2] == 0 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[16] +: T_DI_B[16])) = (trise_CLK_DO_B_worst_3, tfall_CLK_DO_B_worst_3);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 1 && T_RWM_B[2] == 0 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[17] +: T_DI_B[17])) = (trise_CLK_DO_B_worst_3, tfall_CLK_DO_B_worst_3);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 0 && T_RWM_B[2] == 1 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[0] +: T_DI_B[0])) = (trise_CLK_DO_B_worst_4, tfall_CLK_DO_B_worst_4);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 0 && T_RWM_B[2] == 1 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[1] +: T_DI_B[1])) = (trise_CLK_DO_B_worst_4, tfall_CLK_DO_B_worst_4);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 0 && T_RWM_B[2] == 1 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[2] +: T_DI_B[2])) = (trise_CLK_DO_B_worst_4, tfall_CLK_DO_B_worst_4);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 0 && T_RWM_B[2] == 1 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[3] +: T_DI_B[3])) = (trise_CLK_DO_B_worst_4, tfall_CLK_DO_B_worst_4);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 0 && T_RWM_B[2] == 1 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[4] +: T_DI_B[4])) = (trise_CLK_DO_B_worst_4, tfall_CLK_DO_B_worst_4);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 0 && T_RWM_B[2] == 1 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[5] +: T_DI_B[5])) = (trise_CLK_DO_B_worst_4, tfall_CLK_DO_B_worst_4);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 0 && T_RWM_B[2] == 1 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[6] +: T_DI_B[6])) = (trise_CLK_DO_B_worst_4, tfall_CLK_DO_B_worst_4);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 0 && T_RWM_B[2] == 1 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[7] +: T_DI_B[7])) = (trise_CLK_DO_B_worst_4, tfall_CLK_DO_B_worst_4);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 0 && T_RWM_B[2] == 1 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[8] +: T_DI_B[8])) = (trise_CLK_DO_B_worst_4, tfall_CLK_DO_B_worst_4);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 0 && T_RWM_B[2] == 1 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[9] +: T_DI_B[9])) = (trise_CLK_DO_B_worst_4, tfall_CLK_DO_B_worst_4);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 0 && T_RWM_B[2] == 1 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[10] +: T_DI_B[10])) = (trise_CLK_DO_B_worst_4, tfall_CLK_DO_B_worst_4);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 0 && T_RWM_B[2] == 1 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[11] +: T_DI_B[11])) = (trise_CLK_DO_B_worst_4, tfall_CLK_DO_B_worst_4);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 0 && T_RWM_B[2] == 1 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[12] +: T_DI_B[12])) = (trise_CLK_DO_B_worst_4, tfall_CLK_DO_B_worst_4);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 0 && T_RWM_B[2] == 1 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[13] +: T_DI_B[13])) = (trise_CLK_DO_B_worst_4, tfall_CLK_DO_B_worst_4);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 0 && T_RWM_B[2] == 1 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[14] +: T_DI_B[14])) = (trise_CLK_DO_B_worst_4, tfall_CLK_DO_B_worst_4);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 0 && T_RWM_B[2] == 1 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[15] +: T_DI_B[15])) = (trise_CLK_DO_B_worst_4, tfall_CLK_DO_B_worst_4);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 0 && T_RWM_B[2] == 1 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[16] +: T_DI_B[16])) = (trise_CLK_DO_B_worst_4, tfall_CLK_DO_B_worst_4);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 0 && T_RWM_B[2] == 1 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[17] +: T_DI_B[17])) = (trise_CLK_DO_B_worst_4, tfall_CLK_DO_B_worst_4);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 0 && T_RWM_B[2] == 1 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[0] +: T_DI_B[0])) = (trise_CLK_DO_B_worst_5, tfall_CLK_DO_B_worst_5);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 0 && T_RWM_B[2] == 1 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[1] +: T_DI_B[1])) = (trise_CLK_DO_B_worst_5, tfall_CLK_DO_B_worst_5);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 0 && T_RWM_B[2] == 1 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[2] +: T_DI_B[2])) = (trise_CLK_DO_B_worst_5, tfall_CLK_DO_B_worst_5);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 0 && T_RWM_B[2] == 1 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[3] +: T_DI_B[3])) = (trise_CLK_DO_B_worst_5, tfall_CLK_DO_B_worst_5);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 0 && T_RWM_B[2] == 1 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[4] +: T_DI_B[4])) = (trise_CLK_DO_B_worst_5, tfall_CLK_DO_B_worst_5);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 0 && T_RWM_B[2] == 1 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[5] +: T_DI_B[5])) = (trise_CLK_DO_B_worst_5, tfall_CLK_DO_B_worst_5);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 0 && T_RWM_B[2] == 1 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[6] +: T_DI_B[6])) = (trise_CLK_DO_B_worst_5, tfall_CLK_DO_B_worst_5);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 0 && T_RWM_B[2] == 1 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[7] +: T_DI_B[7])) = (trise_CLK_DO_B_worst_5, tfall_CLK_DO_B_worst_5);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 0 && T_RWM_B[2] == 1 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[8] +: T_DI_B[8])) = (trise_CLK_DO_B_worst_5, tfall_CLK_DO_B_worst_5);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 0 && T_RWM_B[2] == 1 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[9] +: T_DI_B[9])) = (trise_CLK_DO_B_worst_5, tfall_CLK_DO_B_worst_5);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 0 && T_RWM_B[2] == 1 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[10] +: T_DI_B[10])) = (trise_CLK_DO_B_worst_5, tfall_CLK_DO_B_worst_5);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 0 && T_RWM_B[2] == 1 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[11] +: T_DI_B[11])) = (trise_CLK_DO_B_worst_5, tfall_CLK_DO_B_worst_5);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 0 && T_RWM_B[2] == 1 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[12] +: T_DI_B[12])) = (trise_CLK_DO_B_worst_5, tfall_CLK_DO_B_worst_5);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 0 && T_RWM_B[2] == 1 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[13] +: T_DI_B[13])) = (trise_CLK_DO_B_worst_5, tfall_CLK_DO_B_worst_5);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 0 && T_RWM_B[2] == 1 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[14] +: T_DI_B[14])) = (trise_CLK_DO_B_worst_5, tfall_CLK_DO_B_worst_5);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 0 && T_RWM_B[2] == 1 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[15] +: T_DI_B[15])) = (trise_CLK_DO_B_worst_5, tfall_CLK_DO_B_worst_5);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 0 && T_RWM_B[2] == 1 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[16] +: T_DI_B[16])) = (trise_CLK_DO_B_worst_5, tfall_CLK_DO_B_worst_5);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 0 && T_RWM_B[2] == 1 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[17] +: T_DI_B[17])) = (trise_CLK_DO_B_worst_5, tfall_CLK_DO_B_worst_5);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 1 && T_RWM_B[2] == 1 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[0] +: T_DI_B[0])) = (trise_CLK_DO_B_worst_6, tfall_CLK_DO_B_worst_6);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 1 && T_RWM_B[2] == 1 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[1] +: T_DI_B[1])) = (trise_CLK_DO_B_worst_6, tfall_CLK_DO_B_worst_6);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 1 && T_RWM_B[2] == 1 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[2] +: T_DI_B[2])) = (trise_CLK_DO_B_worst_6, tfall_CLK_DO_B_worst_6);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 1 && T_RWM_B[2] == 1 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[3] +: T_DI_B[3])) = (trise_CLK_DO_B_worst_6, tfall_CLK_DO_B_worst_6);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 1 && T_RWM_B[2] == 1 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[4] +: T_DI_B[4])) = (trise_CLK_DO_B_worst_6, tfall_CLK_DO_B_worst_6);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 1 && T_RWM_B[2] == 1 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[5] +: T_DI_B[5])) = (trise_CLK_DO_B_worst_6, tfall_CLK_DO_B_worst_6);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 1 && T_RWM_B[2] == 1 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[6] +: T_DI_B[6])) = (trise_CLK_DO_B_worst_6, tfall_CLK_DO_B_worst_6);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 1 && T_RWM_B[2] == 1 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[7] +: T_DI_B[7])) = (trise_CLK_DO_B_worst_6, tfall_CLK_DO_B_worst_6);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 1 && T_RWM_B[2] == 1 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[8] +: T_DI_B[8])) = (trise_CLK_DO_B_worst_6, tfall_CLK_DO_B_worst_6);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 1 && T_RWM_B[2] == 1 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[9] +: T_DI_B[9])) = (trise_CLK_DO_B_worst_6, tfall_CLK_DO_B_worst_6);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 1 && T_RWM_B[2] == 1 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[10] +: T_DI_B[10])) = (trise_CLK_DO_B_worst_6, tfall_CLK_DO_B_worst_6);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 1 && T_RWM_B[2] == 1 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[11] +: T_DI_B[11])) = (trise_CLK_DO_B_worst_6, tfall_CLK_DO_B_worst_6);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 1 && T_RWM_B[2] == 1 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[12] +: T_DI_B[12])) = (trise_CLK_DO_B_worst_6, tfall_CLK_DO_B_worst_6);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 1 && T_RWM_B[2] == 1 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[13] +: T_DI_B[13])) = (trise_CLK_DO_B_worst_6, tfall_CLK_DO_B_worst_6);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 1 && T_RWM_B[2] == 1 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[14] +: T_DI_B[14])) = (trise_CLK_DO_B_worst_6, tfall_CLK_DO_B_worst_6);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 1 && T_RWM_B[2] == 1 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[15] +: T_DI_B[15])) = (trise_CLK_DO_B_worst_6, tfall_CLK_DO_B_worst_6);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 1 && T_RWM_B[2] == 1 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[16] +: T_DI_B[16])) = (trise_CLK_DO_B_worst_6, tfall_CLK_DO_B_worst_6);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 1 && T_RWM_B[2] == 1 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[17] +: T_DI_B[17])) = (trise_CLK_DO_B_worst_6, tfall_CLK_DO_B_worst_6);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 1 && T_RWM_B[2] == 1 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[0] +: T_DI_B[0])) = (trise_CLK_DO_B_worst_7, tfall_CLK_DO_B_worst_7);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 1 && T_RWM_B[2] == 1 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[1] +: T_DI_B[1])) = (trise_CLK_DO_B_worst_7, tfall_CLK_DO_B_worst_7);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 1 && T_RWM_B[2] == 1 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[2] +: T_DI_B[2])) = (trise_CLK_DO_B_worst_7, tfall_CLK_DO_B_worst_7);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 1 && T_RWM_B[2] == 1 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[3] +: T_DI_B[3])) = (trise_CLK_DO_B_worst_7, tfall_CLK_DO_B_worst_7);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 1 && T_RWM_B[2] == 1 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[4] +: T_DI_B[4])) = (trise_CLK_DO_B_worst_7, tfall_CLK_DO_B_worst_7);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 1 && T_RWM_B[2] == 1 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[5] +: T_DI_B[5])) = (trise_CLK_DO_B_worst_7, tfall_CLK_DO_B_worst_7);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 1 && T_RWM_B[2] == 1 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[6] +: T_DI_B[6])) = (trise_CLK_DO_B_worst_7, tfall_CLK_DO_B_worst_7);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 1 && T_RWM_B[2] == 1 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[7] +: T_DI_B[7])) = (trise_CLK_DO_B_worst_7, tfall_CLK_DO_B_worst_7);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 1 && T_RWM_B[2] == 1 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[8] +: T_DI_B[8])) = (trise_CLK_DO_B_worst_7, tfall_CLK_DO_B_worst_7);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 1 && T_RWM_B[2] == 1 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[9] +: T_DI_B[9])) = (trise_CLK_DO_B_worst_7, tfall_CLK_DO_B_worst_7);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 1 && T_RWM_B[2] == 1 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[10] +: T_DI_B[10])) = (trise_CLK_DO_B_worst_7, tfall_CLK_DO_B_worst_7);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 1 && T_RWM_B[2] == 1 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[11] +: T_DI_B[11])) = (trise_CLK_DO_B_worst_7, tfall_CLK_DO_B_worst_7);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 1 && T_RWM_B[2] == 1 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[12] +: T_DI_B[12])) = (trise_CLK_DO_B_worst_7, tfall_CLK_DO_B_worst_7);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 1 && T_RWM_B[2] == 1 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[13] +: T_DI_B[13])) = (trise_CLK_DO_B_worst_7, tfall_CLK_DO_B_worst_7);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 1 && T_RWM_B[2] == 1 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[14] +: T_DI_B[14])) = (trise_CLK_DO_B_worst_7, tfall_CLK_DO_B_worst_7);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 1 && T_RWM_B[2] == 1 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[15] +: T_DI_B[15])) = (trise_CLK_DO_B_worst_7, tfall_CLK_DO_B_worst_7);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 1 && T_RWM_B[2] == 1 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[16] +: T_DI_B[16])) = (trise_CLK_DO_B_worst_7, tfall_CLK_DO_B_worst_7);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 1 && T_RWM_B[2] == 1 && T_BE_N == 0) 
  (posedge CLK_B=>(DO_B[17] +: T_DI_B[17])) = (trise_CLK_DO_B_worst_7, tfall_CLK_DO_B_worst_7);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 0 && T_RWM_A[2] == 0 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[0] +: DI_A[0])) = (trise_CLK_DO_A_worst_0, tfall_CLK_DO_A_worst_0);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 0 && T_RWM_A[2] == 0 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[1] +: DI_A[1])) = (trise_CLK_DO_A_worst_0, tfall_CLK_DO_A_worst_0);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 0 && T_RWM_A[2] == 0 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[2] +: DI_A[2])) = (trise_CLK_DO_A_worst_0, tfall_CLK_DO_A_worst_0);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 0 && T_RWM_A[2] == 0 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[3] +: DI_A[3])) = (trise_CLK_DO_A_worst_0, tfall_CLK_DO_A_worst_0);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 0 && T_RWM_A[2] == 0 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[4] +: DI_A[4])) = (trise_CLK_DO_A_worst_0, tfall_CLK_DO_A_worst_0);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 0 && T_RWM_A[2] == 0 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[5] +: DI_A[5])) = (trise_CLK_DO_A_worst_0, tfall_CLK_DO_A_worst_0);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 0 && T_RWM_A[2] == 0 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[6] +: DI_A[6])) = (trise_CLK_DO_A_worst_0, tfall_CLK_DO_A_worst_0);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 0 && T_RWM_A[2] == 0 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[7] +: DI_A[7])) = (trise_CLK_DO_A_worst_0, tfall_CLK_DO_A_worst_0);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 0 && T_RWM_A[2] == 0 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[8] +: DI_A[8])) = (trise_CLK_DO_A_worst_0, tfall_CLK_DO_A_worst_0);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 0 && T_RWM_A[2] == 0 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[9] +: DI_A[9])) = (trise_CLK_DO_A_worst_0, tfall_CLK_DO_A_worst_0);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 0 && T_RWM_A[2] == 0 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[10] +: DI_A[10])) = (trise_CLK_DO_A_worst_0, tfall_CLK_DO_A_worst_0);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 0 && T_RWM_A[2] == 0 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[11] +: DI_A[11])) = (trise_CLK_DO_A_worst_0, tfall_CLK_DO_A_worst_0);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 0 && T_RWM_A[2] == 0 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[12] +: DI_A[12])) = (trise_CLK_DO_A_worst_0, tfall_CLK_DO_A_worst_0);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 0 && T_RWM_A[2] == 0 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[13] +: DI_A[13])) = (trise_CLK_DO_A_worst_0, tfall_CLK_DO_A_worst_0);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 0 && T_RWM_A[2] == 0 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[14] +: DI_A[14])) = (trise_CLK_DO_A_worst_0, tfall_CLK_DO_A_worst_0);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 0 && T_RWM_A[2] == 0 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[15] +: DI_A[15])) = (trise_CLK_DO_A_worst_0, tfall_CLK_DO_A_worst_0);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 0 && T_RWM_A[2] == 0 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[16] +: DI_A[16])) = (trise_CLK_DO_A_worst_0, tfall_CLK_DO_A_worst_0);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 0 && T_RWM_A[2] == 0 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[17] +: DI_A[17])) = (trise_CLK_DO_A_worst_0, tfall_CLK_DO_A_worst_0);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 0 && T_RWM_A[2] == 0 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[0] +: DI_A[0])) = (trise_CLK_DO_A_worst_1, tfall_CLK_DO_A_worst_1);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 0 && T_RWM_A[2] == 0 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[1] +: DI_A[1])) = (trise_CLK_DO_A_worst_1, tfall_CLK_DO_A_worst_1);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 0 && T_RWM_A[2] == 0 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[2] +: DI_A[2])) = (trise_CLK_DO_A_worst_1, tfall_CLK_DO_A_worst_1);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 0 && T_RWM_A[2] == 0 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[3] +: DI_A[3])) = (trise_CLK_DO_A_worst_1, tfall_CLK_DO_A_worst_1);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 0 && T_RWM_A[2] == 0 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[4] +: DI_A[4])) = (trise_CLK_DO_A_worst_1, tfall_CLK_DO_A_worst_1);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 0 && T_RWM_A[2] == 0 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[5] +: DI_A[5])) = (trise_CLK_DO_A_worst_1, tfall_CLK_DO_A_worst_1);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 0 && T_RWM_A[2] == 0 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[6] +: DI_A[6])) = (trise_CLK_DO_A_worst_1, tfall_CLK_DO_A_worst_1);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 0 && T_RWM_A[2] == 0 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[7] +: DI_A[7])) = (trise_CLK_DO_A_worst_1, tfall_CLK_DO_A_worst_1);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 0 && T_RWM_A[2] == 0 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[8] +: DI_A[8])) = (trise_CLK_DO_A_worst_1, tfall_CLK_DO_A_worst_1);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 0 && T_RWM_A[2] == 0 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[9] +: DI_A[9])) = (trise_CLK_DO_A_worst_1, tfall_CLK_DO_A_worst_1);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 0 && T_RWM_A[2] == 0 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[10] +: DI_A[10])) = (trise_CLK_DO_A_worst_1, tfall_CLK_DO_A_worst_1);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 0 && T_RWM_A[2] == 0 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[11] +: DI_A[11])) = (trise_CLK_DO_A_worst_1, tfall_CLK_DO_A_worst_1);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 0 && T_RWM_A[2] == 0 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[12] +: DI_A[12])) = (trise_CLK_DO_A_worst_1, tfall_CLK_DO_A_worst_1);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 0 && T_RWM_A[2] == 0 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[13] +: DI_A[13])) = (trise_CLK_DO_A_worst_1, tfall_CLK_DO_A_worst_1);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 0 && T_RWM_A[2] == 0 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[14] +: DI_A[14])) = (trise_CLK_DO_A_worst_1, tfall_CLK_DO_A_worst_1);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 0 && T_RWM_A[2] == 0 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[15] +: DI_A[15])) = (trise_CLK_DO_A_worst_1, tfall_CLK_DO_A_worst_1);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 0 && T_RWM_A[2] == 0 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[16] +: DI_A[16])) = (trise_CLK_DO_A_worst_1, tfall_CLK_DO_A_worst_1);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 0 && T_RWM_A[2] == 0 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[17] +: DI_A[17])) = (trise_CLK_DO_A_worst_1, tfall_CLK_DO_A_worst_1);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 1 && T_RWM_A[2] == 0 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[0] +: DI_A[0])) = (trise_CLK_DO_A_worst_2, tfall_CLK_DO_A_worst_2);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 1 && T_RWM_A[2] == 0 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[1] +: DI_A[1])) = (trise_CLK_DO_A_worst_2, tfall_CLK_DO_A_worst_2);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 1 && T_RWM_A[2] == 0 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[2] +: DI_A[2])) = (trise_CLK_DO_A_worst_2, tfall_CLK_DO_A_worst_2);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 1 && T_RWM_A[2] == 0 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[3] +: DI_A[3])) = (trise_CLK_DO_A_worst_2, tfall_CLK_DO_A_worst_2);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 1 && T_RWM_A[2] == 0 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[4] +: DI_A[4])) = (trise_CLK_DO_A_worst_2, tfall_CLK_DO_A_worst_2);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 1 && T_RWM_A[2] == 0 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[5] +: DI_A[5])) = (trise_CLK_DO_A_worst_2, tfall_CLK_DO_A_worst_2);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 1 && T_RWM_A[2] == 0 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[6] +: DI_A[6])) = (trise_CLK_DO_A_worst_2, tfall_CLK_DO_A_worst_2);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 1 && T_RWM_A[2] == 0 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[7] +: DI_A[7])) = (trise_CLK_DO_A_worst_2, tfall_CLK_DO_A_worst_2);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 1 && T_RWM_A[2] == 0 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[8] +: DI_A[8])) = (trise_CLK_DO_A_worst_2, tfall_CLK_DO_A_worst_2);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 1 && T_RWM_A[2] == 0 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[9] +: DI_A[9])) = (trise_CLK_DO_A_worst_2, tfall_CLK_DO_A_worst_2);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 1 && T_RWM_A[2] == 0 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[10] +: DI_A[10])) = (trise_CLK_DO_A_worst_2, tfall_CLK_DO_A_worst_2);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 1 && T_RWM_A[2] == 0 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[11] +: DI_A[11])) = (trise_CLK_DO_A_worst_2, tfall_CLK_DO_A_worst_2);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 1 && T_RWM_A[2] == 0 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[12] +: DI_A[12])) = (trise_CLK_DO_A_worst_2, tfall_CLK_DO_A_worst_2);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 1 && T_RWM_A[2] == 0 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[13] +: DI_A[13])) = (trise_CLK_DO_A_worst_2, tfall_CLK_DO_A_worst_2);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 1 && T_RWM_A[2] == 0 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[14] +: DI_A[14])) = (trise_CLK_DO_A_worst_2, tfall_CLK_DO_A_worst_2);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 1 && T_RWM_A[2] == 0 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[15] +: DI_A[15])) = (trise_CLK_DO_A_worst_2, tfall_CLK_DO_A_worst_2);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 1 && T_RWM_A[2] == 0 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[16] +: DI_A[16])) = (trise_CLK_DO_A_worst_2, tfall_CLK_DO_A_worst_2);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 1 && T_RWM_A[2] == 0 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[17] +: DI_A[17])) = (trise_CLK_DO_A_worst_2, tfall_CLK_DO_A_worst_2);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 1 && T_RWM_A[2] == 0 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[0] +: DI_A[0])) = (trise_CLK_DO_A_worst_3, tfall_CLK_DO_A_worst_3);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 1 && T_RWM_A[2] == 0 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[1] +: DI_A[1])) = (trise_CLK_DO_A_worst_3, tfall_CLK_DO_A_worst_3);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 1 && T_RWM_A[2] == 0 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[2] +: DI_A[2])) = (trise_CLK_DO_A_worst_3, tfall_CLK_DO_A_worst_3);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 1 && T_RWM_A[2] == 0 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[3] +: DI_A[3])) = (trise_CLK_DO_A_worst_3, tfall_CLK_DO_A_worst_3);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 1 && T_RWM_A[2] == 0 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[4] +: DI_A[4])) = (trise_CLK_DO_A_worst_3, tfall_CLK_DO_A_worst_3);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 1 && T_RWM_A[2] == 0 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[5] +: DI_A[5])) = (trise_CLK_DO_A_worst_3, tfall_CLK_DO_A_worst_3);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 1 && T_RWM_A[2] == 0 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[6] +: DI_A[6])) = (trise_CLK_DO_A_worst_3, tfall_CLK_DO_A_worst_3);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 1 && T_RWM_A[2] == 0 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[7] +: DI_A[7])) = (trise_CLK_DO_A_worst_3, tfall_CLK_DO_A_worst_3);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 1 && T_RWM_A[2] == 0 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[8] +: DI_A[8])) = (trise_CLK_DO_A_worst_3, tfall_CLK_DO_A_worst_3);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 1 && T_RWM_A[2] == 0 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[9] +: DI_A[9])) = (trise_CLK_DO_A_worst_3, tfall_CLK_DO_A_worst_3);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 1 && T_RWM_A[2] == 0 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[10] +: DI_A[10])) = (trise_CLK_DO_A_worst_3, tfall_CLK_DO_A_worst_3);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 1 && T_RWM_A[2] == 0 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[11] +: DI_A[11])) = (trise_CLK_DO_A_worst_3, tfall_CLK_DO_A_worst_3);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 1 && T_RWM_A[2] == 0 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[12] +: DI_A[12])) = (trise_CLK_DO_A_worst_3, tfall_CLK_DO_A_worst_3);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 1 && T_RWM_A[2] == 0 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[13] +: DI_A[13])) = (trise_CLK_DO_A_worst_3, tfall_CLK_DO_A_worst_3);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 1 && T_RWM_A[2] == 0 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[14] +: DI_A[14])) = (trise_CLK_DO_A_worst_3, tfall_CLK_DO_A_worst_3);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 1 && T_RWM_A[2] == 0 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[15] +: DI_A[15])) = (trise_CLK_DO_A_worst_3, tfall_CLK_DO_A_worst_3);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 1 && T_RWM_A[2] == 0 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[16] +: DI_A[16])) = (trise_CLK_DO_A_worst_3, tfall_CLK_DO_A_worst_3);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 1 && T_RWM_A[2] == 0 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[17] +: DI_A[17])) = (trise_CLK_DO_A_worst_3, tfall_CLK_DO_A_worst_3);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 0 && T_RWM_A[2] == 1 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[0] +: DI_A[0])) = (trise_CLK_DO_A_worst_4, tfall_CLK_DO_A_worst_4);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 0 && T_RWM_A[2] == 1 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[1] +: DI_A[1])) = (trise_CLK_DO_A_worst_4, tfall_CLK_DO_A_worst_4);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 0 && T_RWM_A[2] == 1 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[2] +: DI_A[2])) = (trise_CLK_DO_A_worst_4, tfall_CLK_DO_A_worst_4);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 0 && T_RWM_A[2] == 1 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[3] +: DI_A[3])) = (trise_CLK_DO_A_worst_4, tfall_CLK_DO_A_worst_4);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 0 && T_RWM_A[2] == 1 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[4] +: DI_A[4])) = (trise_CLK_DO_A_worst_4, tfall_CLK_DO_A_worst_4);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 0 && T_RWM_A[2] == 1 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[5] +: DI_A[5])) = (trise_CLK_DO_A_worst_4, tfall_CLK_DO_A_worst_4);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 0 && T_RWM_A[2] == 1 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[6] +: DI_A[6])) = (trise_CLK_DO_A_worst_4, tfall_CLK_DO_A_worst_4);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 0 && T_RWM_A[2] == 1 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[7] +: DI_A[7])) = (trise_CLK_DO_A_worst_4, tfall_CLK_DO_A_worst_4);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 0 && T_RWM_A[2] == 1 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[8] +: DI_A[8])) = (trise_CLK_DO_A_worst_4, tfall_CLK_DO_A_worst_4);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 0 && T_RWM_A[2] == 1 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[9] +: DI_A[9])) = (trise_CLK_DO_A_worst_4, tfall_CLK_DO_A_worst_4);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 0 && T_RWM_A[2] == 1 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[10] +: DI_A[10])) = (trise_CLK_DO_A_worst_4, tfall_CLK_DO_A_worst_4);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 0 && T_RWM_A[2] == 1 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[11] +: DI_A[11])) = (trise_CLK_DO_A_worst_4, tfall_CLK_DO_A_worst_4);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 0 && T_RWM_A[2] == 1 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[12] +: DI_A[12])) = (trise_CLK_DO_A_worst_4, tfall_CLK_DO_A_worst_4);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 0 && T_RWM_A[2] == 1 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[13] +: DI_A[13])) = (trise_CLK_DO_A_worst_4, tfall_CLK_DO_A_worst_4);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 0 && T_RWM_A[2] == 1 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[14] +: DI_A[14])) = (trise_CLK_DO_A_worst_4, tfall_CLK_DO_A_worst_4);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 0 && T_RWM_A[2] == 1 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[15] +: DI_A[15])) = (trise_CLK_DO_A_worst_4, tfall_CLK_DO_A_worst_4);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 0 && T_RWM_A[2] == 1 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[16] +: DI_A[16])) = (trise_CLK_DO_A_worst_4, tfall_CLK_DO_A_worst_4);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 0 && T_RWM_A[2] == 1 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[17] +: DI_A[17])) = (trise_CLK_DO_A_worst_4, tfall_CLK_DO_A_worst_4);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 0 && T_RWM_A[2] == 1 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[0] +: DI_A[0])) = (trise_CLK_DO_A_worst_5, tfall_CLK_DO_A_worst_5);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 0 && T_RWM_A[2] == 1 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[1] +: DI_A[1])) = (trise_CLK_DO_A_worst_5, tfall_CLK_DO_A_worst_5);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 0 && T_RWM_A[2] == 1 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[2] +: DI_A[2])) = (trise_CLK_DO_A_worst_5, tfall_CLK_DO_A_worst_5);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 0 && T_RWM_A[2] == 1 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[3] +: DI_A[3])) = (trise_CLK_DO_A_worst_5, tfall_CLK_DO_A_worst_5);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 0 && T_RWM_A[2] == 1 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[4] +: DI_A[4])) = (trise_CLK_DO_A_worst_5, tfall_CLK_DO_A_worst_5);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 0 && T_RWM_A[2] == 1 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[5] +: DI_A[5])) = (trise_CLK_DO_A_worst_5, tfall_CLK_DO_A_worst_5);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 0 && T_RWM_A[2] == 1 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[6] +: DI_A[6])) = (trise_CLK_DO_A_worst_5, tfall_CLK_DO_A_worst_5);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 0 && T_RWM_A[2] == 1 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[7] +: DI_A[7])) = (trise_CLK_DO_A_worst_5, tfall_CLK_DO_A_worst_5);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 0 && T_RWM_A[2] == 1 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[8] +: DI_A[8])) = (trise_CLK_DO_A_worst_5, tfall_CLK_DO_A_worst_5);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 0 && T_RWM_A[2] == 1 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[9] +: DI_A[9])) = (trise_CLK_DO_A_worst_5, tfall_CLK_DO_A_worst_5);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 0 && T_RWM_A[2] == 1 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[10] +: DI_A[10])) = (trise_CLK_DO_A_worst_5, tfall_CLK_DO_A_worst_5);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 0 && T_RWM_A[2] == 1 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[11] +: DI_A[11])) = (trise_CLK_DO_A_worst_5, tfall_CLK_DO_A_worst_5);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 0 && T_RWM_A[2] == 1 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[12] +: DI_A[12])) = (trise_CLK_DO_A_worst_5, tfall_CLK_DO_A_worst_5);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 0 && T_RWM_A[2] == 1 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[13] +: DI_A[13])) = (trise_CLK_DO_A_worst_5, tfall_CLK_DO_A_worst_5);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 0 && T_RWM_A[2] == 1 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[14] +: DI_A[14])) = (trise_CLK_DO_A_worst_5, tfall_CLK_DO_A_worst_5);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 0 && T_RWM_A[2] == 1 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[15] +: DI_A[15])) = (trise_CLK_DO_A_worst_5, tfall_CLK_DO_A_worst_5);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 0 && T_RWM_A[2] == 1 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[16] +: DI_A[16])) = (trise_CLK_DO_A_worst_5, tfall_CLK_DO_A_worst_5);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 0 && T_RWM_A[2] == 1 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[17] +: DI_A[17])) = (trise_CLK_DO_A_worst_5, tfall_CLK_DO_A_worst_5);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 1 && T_RWM_A[2] == 1 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[0] +: DI_A[0])) = (trise_CLK_DO_A_worst_6, tfall_CLK_DO_A_worst_6);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 1 && T_RWM_A[2] == 1 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[1] +: DI_A[1])) = (trise_CLK_DO_A_worst_6, tfall_CLK_DO_A_worst_6);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 1 && T_RWM_A[2] == 1 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[2] +: DI_A[2])) = (trise_CLK_DO_A_worst_6, tfall_CLK_DO_A_worst_6);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 1 && T_RWM_A[2] == 1 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[3] +: DI_A[3])) = (trise_CLK_DO_A_worst_6, tfall_CLK_DO_A_worst_6);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 1 && T_RWM_A[2] == 1 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[4] +: DI_A[4])) = (trise_CLK_DO_A_worst_6, tfall_CLK_DO_A_worst_6);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 1 && T_RWM_A[2] == 1 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[5] +: DI_A[5])) = (trise_CLK_DO_A_worst_6, tfall_CLK_DO_A_worst_6);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 1 && T_RWM_A[2] == 1 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[6] +: DI_A[6])) = (trise_CLK_DO_A_worst_6, tfall_CLK_DO_A_worst_6);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 1 && T_RWM_A[2] == 1 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[7] +: DI_A[7])) = (trise_CLK_DO_A_worst_6, tfall_CLK_DO_A_worst_6);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 1 && T_RWM_A[2] == 1 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[8] +: DI_A[8])) = (trise_CLK_DO_A_worst_6, tfall_CLK_DO_A_worst_6);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 1 && T_RWM_A[2] == 1 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[9] +: DI_A[9])) = (trise_CLK_DO_A_worst_6, tfall_CLK_DO_A_worst_6);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 1 && T_RWM_A[2] == 1 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[10] +: DI_A[10])) = (trise_CLK_DO_A_worst_6, tfall_CLK_DO_A_worst_6);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 1 && T_RWM_A[2] == 1 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[11] +: DI_A[11])) = (trise_CLK_DO_A_worst_6, tfall_CLK_DO_A_worst_6);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 1 && T_RWM_A[2] == 1 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[12] +: DI_A[12])) = (trise_CLK_DO_A_worst_6, tfall_CLK_DO_A_worst_6);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 1 && T_RWM_A[2] == 1 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[13] +: DI_A[13])) = (trise_CLK_DO_A_worst_6, tfall_CLK_DO_A_worst_6);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 1 && T_RWM_A[2] == 1 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[14] +: DI_A[14])) = (trise_CLK_DO_A_worst_6, tfall_CLK_DO_A_worst_6);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 1 && T_RWM_A[2] == 1 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[15] +: DI_A[15])) = (trise_CLK_DO_A_worst_6, tfall_CLK_DO_A_worst_6);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 1 && T_RWM_A[2] == 1 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[16] +: DI_A[16])) = (trise_CLK_DO_A_worst_6, tfall_CLK_DO_A_worst_6);
 if(T_RWM_A[0] == 0 && T_RWM_A[1] == 1 && T_RWM_A[2] == 1 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[17] +: DI_A[17])) = (trise_CLK_DO_A_worst_6, tfall_CLK_DO_A_worst_6);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 1 && T_RWM_A[2] == 1 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[0] +: DI_A[0])) = (trise_CLK_DO_A_worst_7, tfall_CLK_DO_A_worst_7);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 1 && T_RWM_A[2] == 1 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[1] +: DI_A[1])) = (trise_CLK_DO_A_worst_7, tfall_CLK_DO_A_worst_7);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 1 && T_RWM_A[2] == 1 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[2] +: DI_A[2])) = (trise_CLK_DO_A_worst_7, tfall_CLK_DO_A_worst_7);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 1 && T_RWM_A[2] == 1 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[3] +: DI_A[3])) = (trise_CLK_DO_A_worst_7, tfall_CLK_DO_A_worst_7);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 1 && T_RWM_A[2] == 1 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[4] +: DI_A[4])) = (trise_CLK_DO_A_worst_7, tfall_CLK_DO_A_worst_7);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 1 && T_RWM_A[2] == 1 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[5] +: DI_A[5])) = (trise_CLK_DO_A_worst_7, tfall_CLK_DO_A_worst_7);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 1 && T_RWM_A[2] == 1 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[6] +: DI_A[6])) = (trise_CLK_DO_A_worst_7, tfall_CLK_DO_A_worst_7);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 1 && T_RWM_A[2] == 1 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[7] +: DI_A[7])) = (trise_CLK_DO_A_worst_7, tfall_CLK_DO_A_worst_7);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 1 && T_RWM_A[2] == 1 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[8] +: DI_A[8])) = (trise_CLK_DO_A_worst_7, tfall_CLK_DO_A_worst_7);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 1 && T_RWM_A[2] == 1 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[9] +: DI_A[9])) = (trise_CLK_DO_A_worst_7, tfall_CLK_DO_A_worst_7);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 1 && T_RWM_A[2] == 1 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[10] +: DI_A[10])) = (trise_CLK_DO_A_worst_7, tfall_CLK_DO_A_worst_7);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 1 && T_RWM_A[2] == 1 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[11] +: DI_A[11])) = (trise_CLK_DO_A_worst_7, tfall_CLK_DO_A_worst_7);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 1 && T_RWM_A[2] == 1 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[12] +: DI_A[12])) = (trise_CLK_DO_A_worst_7, tfall_CLK_DO_A_worst_7);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 1 && T_RWM_A[2] == 1 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[13] +: DI_A[13])) = (trise_CLK_DO_A_worst_7, tfall_CLK_DO_A_worst_7);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 1 && T_RWM_A[2] == 1 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[14] +: DI_A[14])) = (trise_CLK_DO_A_worst_7, tfall_CLK_DO_A_worst_7);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 1 && T_RWM_A[2] == 1 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[15] +: DI_A[15])) = (trise_CLK_DO_A_worst_7, tfall_CLK_DO_A_worst_7);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 1 && T_RWM_A[2] == 1 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[16] +: DI_A[16])) = (trise_CLK_DO_A_worst_7, tfall_CLK_DO_A_worst_7);
 if(T_RWM_A[0] == 1 && T_RWM_A[1] == 1 && T_RWM_A[2] == 1 && T_BE_N == 1) 
  (posedge CLK_A=>(DO_A[17] +: DI_A[17])) = (trise_CLK_DO_A_worst_7, tfall_CLK_DO_A_worst_7);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 0 && T_RWM_B[2] == 0 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[0] +: DI_B[0])) = (trise_CLK_DO_B_worst_0, tfall_CLK_DO_B_worst_0);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 0 && T_RWM_B[2] == 0 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[1] +: DI_B[1])) = (trise_CLK_DO_B_worst_0, tfall_CLK_DO_B_worst_0);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 0 && T_RWM_B[2] == 0 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[2] +: DI_B[2])) = (trise_CLK_DO_B_worst_0, tfall_CLK_DO_B_worst_0);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 0 && T_RWM_B[2] == 0 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[3] +: DI_B[3])) = (trise_CLK_DO_B_worst_0, tfall_CLK_DO_B_worst_0);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 0 && T_RWM_B[2] == 0 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[4] +: DI_B[4])) = (trise_CLK_DO_B_worst_0, tfall_CLK_DO_B_worst_0);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 0 && T_RWM_B[2] == 0 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[5] +: DI_B[5])) = (trise_CLK_DO_B_worst_0, tfall_CLK_DO_B_worst_0);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 0 && T_RWM_B[2] == 0 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[6] +: DI_B[6])) = (trise_CLK_DO_B_worst_0, tfall_CLK_DO_B_worst_0);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 0 && T_RWM_B[2] == 0 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[7] +: DI_B[7])) = (trise_CLK_DO_B_worst_0, tfall_CLK_DO_B_worst_0);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 0 && T_RWM_B[2] == 0 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[8] +: DI_B[8])) = (trise_CLK_DO_B_worst_0, tfall_CLK_DO_B_worst_0);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 0 && T_RWM_B[2] == 0 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[9] +: DI_B[9])) = (trise_CLK_DO_B_worst_0, tfall_CLK_DO_B_worst_0);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 0 && T_RWM_B[2] == 0 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[10] +: DI_B[10])) = (trise_CLK_DO_B_worst_0, tfall_CLK_DO_B_worst_0);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 0 && T_RWM_B[2] == 0 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[11] +: DI_B[11])) = (trise_CLK_DO_B_worst_0, tfall_CLK_DO_B_worst_0);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 0 && T_RWM_B[2] == 0 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[12] +: DI_B[12])) = (trise_CLK_DO_B_worst_0, tfall_CLK_DO_B_worst_0);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 0 && T_RWM_B[2] == 0 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[13] +: DI_B[13])) = (trise_CLK_DO_B_worst_0, tfall_CLK_DO_B_worst_0);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 0 && T_RWM_B[2] == 0 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[14] +: DI_B[14])) = (trise_CLK_DO_B_worst_0, tfall_CLK_DO_B_worst_0);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 0 && T_RWM_B[2] == 0 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[15] +: DI_B[15])) = (trise_CLK_DO_B_worst_0, tfall_CLK_DO_B_worst_0);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 0 && T_RWM_B[2] == 0 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[16] +: DI_B[16])) = (trise_CLK_DO_B_worst_0, tfall_CLK_DO_B_worst_0);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 0 && T_RWM_B[2] == 0 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[17] +: DI_B[17])) = (trise_CLK_DO_B_worst_0, tfall_CLK_DO_B_worst_0);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 0 && T_RWM_B[2] == 0 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[0] +: DI_B[0])) = (trise_CLK_DO_B_worst_1, tfall_CLK_DO_B_worst_1);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 0 && T_RWM_B[2] == 0 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[1] +: DI_B[1])) = (trise_CLK_DO_B_worst_1, tfall_CLK_DO_B_worst_1);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 0 && T_RWM_B[2] == 0 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[2] +: DI_B[2])) = (trise_CLK_DO_B_worst_1, tfall_CLK_DO_B_worst_1);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 0 && T_RWM_B[2] == 0 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[3] +: DI_B[3])) = (trise_CLK_DO_B_worst_1, tfall_CLK_DO_B_worst_1);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 0 && T_RWM_B[2] == 0 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[4] +: DI_B[4])) = (trise_CLK_DO_B_worst_1, tfall_CLK_DO_B_worst_1);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 0 && T_RWM_B[2] == 0 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[5] +: DI_B[5])) = (trise_CLK_DO_B_worst_1, tfall_CLK_DO_B_worst_1);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 0 && T_RWM_B[2] == 0 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[6] +: DI_B[6])) = (trise_CLK_DO_B_worst_1, tfall_CLK_DO_B_worst_1);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 0 && T_RWM_B[2] == 0 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[7] +: DI_B[7])) = (trise_CLK_DO_B_worst_1, tfall_CLK_DO_B_worst_1);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 0 && T_RWM_B[2] == 0 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[8] +: DI_B[8])) = (trise_CLK_DO_B_worst_1, tfall_CLK_DO_B_worst_1);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 0 && T_RWM_B[2] == 0 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[9] +: DI_B[9])) = (trise_CLK_DO_B_worst_1, tfall_CLK_DO_B_worst_1);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 0 && T_RWM_B[2] == 0 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[10] +: DI_B[10])) = (trise_CLK_DO_B_worst_1, tfall_CLK_DO_B_worst_1);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 0 && T_RWM_B[2] == 0 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[11] +: DI_B[11])) = (trise_CLK_DO_B_worst_1, tfall_CLK_DO_B_worst_1);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 0 && T_RWM_B[2] == 0 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[12] +: DI_B[12])) = (trise_CLK_DO_B_worst_1, tfall_CLK_DO_B_worst_1);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 0 && T_RWM_B[2] == 0 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[13] +: DI_B[13])) = (trise_CLK_DO_B_worst_1, tfall_CLK_DO_B_worst_1);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 0 && T_RWM_B[2] == 0 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[14] +: DI_B[14])) = (trise_CLK_DO_B_worst_1, tfall_CLK_DO_B_worst_1);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 0 && T_RWM_B[2] == 0 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[15] +: DI_B[15])) = (trise_CLK_DO_B_worst_1, tfall_CLK_DO_B_worst_1);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 0 && T_RWM_B[2] == 0 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[16] +: DI_B[16])) = (trise_CLK_DO_B_worst_1, tfall_CLK_DO_B_worst_1);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 0 && T_RWM_B[2] == 0 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[17] +: DI_B[17])) = (trise_CLK_DO_B_worst_1, tfall_CLK_DO_B_worst_1);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 1 && T_RWM_B[2] == 0 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[0] +: DI_B[0])) = (trise_CLK_DO_B_worst_2, tfall_CLK_DO_B_worst_2);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 1 && T_RWM_B[2] == 0 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[1] +: DI_B[1])) = (trise_CLK_DO_B_worst_2, tfall_CLK_DO_B_worst_2);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 1 && T_RWM_B[2] == 0 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[2] +: DI_B[2])) = (trise_CLK_DO_B_worst_2, tfall_CLK_DO_B_worst_2);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 1 && T_RWM_B[2] == 0 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[3] +: DI_B[3])) = (trise_CLK_DO_B_worst_2, tfall_CLK_DO_B_worst_2);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 1 && T_RWM_B[2] == 0 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[4] +: DI_B[4])) = (trise_CLK_DO_B_worst_2, tfall_CLK_DO_B_worst_2);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 1 && T_RWM_B[2] == 0 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[5] +: DI_B[5])) = (trise_CLK_DO_B_worst_2, tfall_CLK_DO_B_worst_2);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 1 && T_RWM_B[2] == 0 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[6] +: DI_B[6])) = (trise_CLK_DO_B_worst_2, tfall_CLK_DO_B_worst_2);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 1 && T_RWM_B[2] == 0 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[7] +: DI_B[7])) = (trise_CLK_DO_B_worst_2, tfall_CLK_DO_B_worst_2);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 1 && T_RWM_B[2] == 0 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[8] +: DI_B[8])) = (trise_CLK_DO_B_worst_2, tfall_CLK_DO_B_worst_2);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 1 && T_RWM_B[2] == 0 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[9] +: DI_B[9])) = (trise_CLK_DO_B_worst_2, tfall_CLK_DO_B_worst_2);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 1 && T_RWM_B[2] == 0 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[10] +: DI_B[10])) = (trise_CLK_DO_B_worst_2, tfall_CLK_DO_B_worst_2);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 1 && T_RWM_B[2] == 0 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[11] +: DI_B[11])) = (trise_CLK_DO_B_worst_2, tfall_CLK_DO_B_worst_2);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 1 && T_RWM_B[2] == 0 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[12] +: DI_B[12])) = (trise_CLK_DO_B_worst_2, tfall_CLK_DO_B_worst_2);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 1 && T_RWM_B[2] == 0 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[13] +: DI_B[13])) = (trise_CLK_DO_B_worst_2, tfall_CLK_DO_B_worst_2);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 1 && T_RWM_B[2] == 0 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[14] +: DI_B[14])) = (trise_CLK_DO_B_worst_2, tfall_CLK_DO_B_worst_2);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 1 && T_RWM_B[2] == 0 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[15] +: DI_B[15])) = (trise_CLK_DO_B_worst_2, tfall_CLK_DO_B_worst_2);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 1 && T_RWM_B[2] == 0 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[16] +: DI_B[16])) = (trise_CLK_DO_B_worst_2, tfall_CLK_DO_B_worst_2);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 1 && T_RWM_B[2] == 0 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[17] +: DI_B[17])) = (trise_CLK_DO_B_worst_2, tfall_CLK_DO_B_worst_2);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 1 && T_RWM_B[2] == 0 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[0] +: DI_B[0])) = (trise_CLK_DO_B_worst_3, tfall_CLK_DO_B_worst_3);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 1 && T_RWM_B[2] == 0 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[1] +: DI_B[1])) = (trise_CLK_DO_B_worst_3, tfall_CLK_DO_B_worst_3);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 1 && T_RWM_B[2] == 0 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[2] +: DI_B[2])) = (trise_CLK_DO_B_worst_3, tfall_CLK_DO_B_worst_3);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 1 && T_RWM_B[2] == 0 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[3] +: DI_B[3])) = (trise_CLK_DO_B_worst_3, tfall_CLK_DO_B_worst_3);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 1 && T_RWM_B[2] == 0 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[4] +: DI_B[4])) = (trise_CLK_DO_B_worst_3, tfall_CLK_DO_B_worst_3);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 1 && T_RWM_B[2] == 0 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[5] +: DI_B[5])) = (trise_CLK_DO_B_worst_3, tfall_CLK_DO_B_worst_3);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 1 && T_RWM_B[2] == 0 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[6] +: DI_B[6])) = (trise_CLK_DO_B_worst_3, tfall_CLK_DO_B_worst_3);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 1 && T_RWM_B[2] == 0 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[7] +: DI_B[7])) = (trise_CLK_DO_B_worst_3, tfall_CLK_DO_B_worst_3);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 1 && T_RWM_B[2] == 0 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[8] +: DI_B[8])) = (trise_CLK_DO_B_worst_3, tfall_CLK_DO_B_worst_3);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 1 && T_RWM_B[2] == 0 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[9] +: DI_B[9])) = (trise_CLK_DO_B_worst_3, tfall_CLK_DO_B_worst_3);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 1 && T_RWM_B[2] == 0 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[10] +: DI_B[10])) = (trise_CLK_DO_B_worst_3, tfall_CLK_DO_B_worst_3);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 1 && T_RWM_B[2] == 0 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[11] +: DI_B[11])) = (trise_CLK_DO_B_worst_3, tfall_CLK_DO_B_worst_3);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 1 && T_RWM_B[2] == 0 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[12] +: DI_B[12])) = (trise_CLK_DO_B_worst_3, tfall_CLK_DO_B_worst_3);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 1 && T_RWM_B[2] == 0 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[13] +: DI_B[13])) = (trise_CLK_DO_B_worst_3, tfall_CLK_DO_B_worst_3);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 1 && T_RWM_B[2] == 0 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[14] +: DI_B[14])) = (trise_CLK_DO_B_worst_3, tfall_CLK_DO_B_worst_3);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 1 && T_RWM_B[2] == 0 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[15] +: DI_B[15])) = (trise_CLK_DO_B_worst_3, tfall_CLK_DO_B_worst_3);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 1 && T_RWM_B[2] == 0 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[16] +: DI_B[16])) = (trise_CLK_DO_B_worst_3, tfall_CLK_DO_B_worst_3);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 1 && T_RWM_B[2] == 0 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[17] +: DI_B[17])) = (trise_CLK_DO_B_worst_3, tfall_CLK_DO_B_worst_3);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 0 && T_RWM_B[2] == 1 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[0] +: DI_B[0])) = (trise_CLK_DO_B_worst_4, tfall_CLK_DO_B_worst_4);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 0 && T_RWM_B[2] == 1 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[1] +: DI_B[1])) = (trise_CLK_DO_B_worst_4, tfall_CLK_DO_B_worst_4);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 0 && T_RWM_B[2] == 1 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[2] +: DI_B[2])) = (trise_CLK_DO_B_worst_4, tfall_CLK_DO_B_worst_4);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 0 && T_RWM_B[2] == 1 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[3] +: DI_B[3])) = (trise_CLK_DO_B_worst_4, tfall_CLK_DO_B_worst_4);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 0 && T_RWM_B[2] == 1 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[4] +: DI_B[4])) = (trise_CLK_DO_B_worst_4, tfall_CLK_DO_B_worst_4);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 0 && T_RWM_B[2] == 1 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[5] +: DI_B[5])) = (trise_CLK_DO_B_worst_4, tfall_CLK_DO_B_worst_4);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 0 && T_RWM_B[2] == 1 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[6] +: DI_B[6])) = (trise_CLK_DO_B_worst_4, tfall_CLK_DO_B_worst_4);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 0 && T_RWM_B[2] == 1 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[7] +: DI_B[7])) = (trise_CLK_DO_B_worst_4, tfall_CLK_DO_B_worst_4);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 0 && T_RWM_B[2] == 1 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[8] +: DI_B[8])) = (trise_CLK_DO_B_worst_4, tfall_CLK_DO_B_worst_4);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 0 && T_RWM_B[2] == 1 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[9] +: DI_B[9])) = (trise_CLK_DO_B_worst_4, tfall_CLK_DO_B_worst_4);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 0 && T_RWM_B[2] == 1 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[10] +: DI_B[10])) = (trise_CLK_DO_B_worst_4, tfall_CLK_DO_B_worst_4);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 0 && T_RWM_B[2] == 1 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[11] +: DI_B[11])) = (trise_CLK_DO_B_worst_4, tfall_CLK_DO_B_worst_4);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 0 && T_RWM_B[2] == 1 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[12] +: DI_B[12])) = (trise_CLK_DO_B_worst_4, tfall_CLK_DO_B_worst_4);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 0 && T_RWM_B[2] == 1 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[13] +: DI_B[13])) = (trise_CLK_DO_B_worst_4, tfall_CLK_DO_B_worst_4);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 0 && T_RWM_B[2] == 1 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[14] +: DI_B[14])) = (trise_CLK_DO_B_worst_4, tfall_CLK_DO_B_worst_4);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 0 && T_RWM_B[2] == 1 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[15] +: DI_B[15])) = (trise_CLK_DO_B_worst_4, tfall_CLK_DO_B_worst_4);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 0 && T_RWM_B[2] == 1 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[16] +: DI_B[16])) = (trise_CLK_DO_B_worst_4, tfall_CLK_DO_B_worst_4);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 0 && T_RWM_B[2] == 1 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[17] +: DI_B[17])) = (trise_CLK_DO_B_worst_4, tfall_CLK_DO_B_worst_4);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 0 && T_RWM_B[2] == 1 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[0] +: DI_B[0])) = (trise_CLK_DO_B_worst_5, tfall_CLK_DO_B_worst_5);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 0 && T_RWM_B[2] == 1 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[1] +: DI_B[1])) = (trise_CLK_DO_B_worst_5, tfall_CLK_DO_B_worst_5);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 0 && T_RWM_B[2] == 1 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[2] +: DI_B[2])) = (trise_CLK_DO_B_worst_5, tfall_CLK_DO_B_worst_5);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 0 && T_RWM_B[2] == 1 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[3] +: DI_B[3])) = (trise_CLK_DO_B_worst_5, tfall_CLK_DO_B_worst_5);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 0 && T_RWM_B[2] == 1 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[4] +: DI_B[4])) = (trise_CLK_DO_B_worst_5, tfall_CLK_DO_B_worst_5);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 0 && T_RWM_B[2] == 1 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[5] +: DI_B[5])) = (trise_CLK_DO_B_worst_5, tfall_CLK_DO_B_worst_5);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 0 && T_RWM_B[2] == 1 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[6] +: DI_B[6])) = (trise_CLK_DO_B_worst_5, tfall_CLK_DO_B_worst_5);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 0 && T_RWM_B[2] == 1 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[7] +: DI_B[7])) = (trise_CLK_DO_B_worst_5, tfall_CLK_DO_B_worst_5);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 0 && T_RWM_B[2] == 1 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[8] +: DI_B[8])) = (trise_CLK_DO_B_worst_5, tfall_CLK_DO_B_worst_5);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 0 && T_RWM_B[2] == 1 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[9] +: DI_B[9])) = (trise_CLK_DO_B_worst_5, tfall_CLK_DO_B_worst_5);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 0 && T_RWM_B[2] == 1 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[10] +: DI_B[10])) = (trise_CLK_DO_B_worst_5, tfall_CLK_DO_B_worst_5);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 0 && T_RWM_B[2] == 1 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[11] +: DI_B[11])) = (trise_CLK_DO_B_worst_5, tfall_CLK_DO_B_worst_5);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 0 && T_RWM_B[2] == 1 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[12] +: DI_B[12])) = (trise_CLK_DO_B_worst_5, tfall_CLK_DO_B_worst_5);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 0 && T_RWM_B[2] == 1 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[13] +: DI_B[13])) = (trise_CLK_DO_B_worst_5, tfall_CLK_DO_B_worst_5);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 0 && T_RWM_B[2] == 1 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[14] +: DI_B[14])) = (trise_CLK_DO_B_worst_5, tfall_CLK_DO_B_worst_5);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 0 && T_RWM_B[2] == 1 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[15] +: DI_B[15])) = (trise_CLK_DO_B_worst_5, tfall_CLK_DO_B_worst_5);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 0 && T_RWM_B[2] == 1 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[16] +: DI_B[16])) = (trise_CLK_DO_B_worst_5, tfall_CLK_DO_B_worst_5);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 0 && T_RWM_B[2] == 1 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[17] +: DI_B[17])) = (trise_CLK_DO_B_worst_5, tfall_CLK_DO_B_worst_5);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 1 && T_RWM_B[2] == 1 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[0] +: DI_B[0])) = (trise_CLK_DO_B_worst_6, tfall_CLK_DO_B_worst_6);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 1 && T_RWM_B[2] == 1 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[1] +: DI_B[1])) = (trise_CLK_DO_B_worst_6, tfall_CLK_DO_B_worst_6);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 1 && T_RWM_B[2] == 1 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[2] +: DI_B[2])) = (trise_CLK_DO_B_worst_6, tfall_CLK_DO_B_worst_6);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 1 && T_RWM_B[2] == 1 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[3] +: DI_B[3])) = (trise_CLK_DO_B_worst_6, tfall_CLK_DO_B_worst_6);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 1 && T_RWM_B[2] == 1 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[4] +: DI_B[4])) = (trise_CLK_DO_B_worst_6, tfall_CLK_DO_B_worst_6);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 1 && T_RWM_B[2] == 1 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[5] +: DI_B[5])) = (trise_CLK_DO_B_worst_6, tfall_CLK_DO_B_worst_6);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 1 && T_RWM_B[2] == 1 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[6] +: DI_B[6])) = (trise_CLK_DO_B_worst_6, tfall_CLK_DO_B_worst_6);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 1 && T_RWM_B[2] == 1 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[7] +: DI_B[7])) = (trise_CLK_DO_B_worst_6, tfall_CLK_DO_B_worst_6);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 1 && T_RWM_B[2] == 1 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[8] +: DI_B[8])) = (trise_CLK_DO_B_worst_6, tfall_CLK_DO_B_worst_6);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 1 && T_RWM_B[2] == 1 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[9] +: DI_B[9])) = (trise_CLK_DO_B_worst_6, tfall_CLK_DO_B_worst_6);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 1 && T_RWM_B[2] == 1 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[10] +: DI_B[10])) = (trise_CLK_DO_B_worst_6, tfall_CLK_DO_B_worst_6);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 1 && T_RWM_B[2] == 1 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[11] +: DI_B[11])) = (trise_CLK_DO_B_worst_6, tfall_CLK_DO_B_worst_6);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 1 && T_RWM_B[2] == 1 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[12] +: DI_B[12])) = (trise_CLK_DO_B_worst_6, tfall_CLK_DO_B_worst_6);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 1 && T_RWM_B[2] == 1 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[13] +: DI_B[13])) = (trise_CLK_DO_B_worst_6, tfall_CLK_DO_B_worst_6);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 1 && T_RWM_B[2] == 1 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[14] +: DI_B[14])) = (trise_CLK_DO_B_worst_6, tfall_CLK_DO_B_worst_6);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 1 && T_RWM_B[2] == 1 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[15] +: DI_B[15])) = (trise_CLK_DO_B_worst_6, tfall_CLK_DO_B_worst_6);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 1 && T_RWM_B[2] == 1 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[16] +: DI_B[16])) = (trise_CLK_DO_B_worst_6, tfall_CLK_DO_B_worst_6);
 if(T_RWM_B[0] == 0 && T_RWM_B[1] == 1 && T_RWM_B[2] == 1 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[17] +: DI_B[17])) = (trise_CLK_DO_B_worst_6, tfall_CLK_DO_B_worst_6);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 1 && T_RWM_B[2] == 1 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[0] +: DI_B[0])) = (trise_CLK_DO_B_worst_7, tfall_CLK_DO_B_worst_7);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 1 && T_RWM_B[2] == 1 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[1] +: DI_B[1])) = (trise_CLK_DO_B_worst_7, tfall_CLK_DO_B_worst_7);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 1 && T_RWM_B[2] == 1 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[2] +: DI_B[2])) = (trise_CLK_DO_B_worst_7, tfall_CLK_DO_B_worst_7);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 1 && T_RWM_B[2] == 1 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[3] +: DI_B[3])) = (trise_CLK_DO_B_worst_7, tfall_CLK_DO_B_worst_7);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 1 && T_RWM_B[2] == 1 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[4] +: DI_B[4])) = (trise_CLK_DO_B_worst_7, tfall_CLK_DO_B_worst_7);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 1 && T_RWM_B[2] == 1 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[5] +: DI_B[5])) = (trise_CLK_DO_B_worst_7, tfall_CLK_DO_B_worst_7);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 1 && T_RWM_B[2] == 1 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[6] +: DI_B[6])) = (trise_CLK_DO_B_worst_7, tfall_CLK_DO_B_worst_7);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 1 && T_RWM_B[2] == 1 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[7] +: DI_B[7])) = (trise_CLK_DO_B_worst_7, tfall_CLK_DO_B_worst_7);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 1 && T_RWM_B[2] == 1 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[8] +: DI_B[8])) = (trise_CLK_DO_B_worst_7, tfall_CLK_DO_B_worst_7);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 1 && T_RWM_B[2] == 1 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[9] +: DI_B[9])) = (trise_CLK_DO_B_worst_7, tfall_CLK_DO_B_worst_7);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 1 && T_RWM_B[2] == 1 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[10] +: DI_B[10])) = (trise_CLK_DO_B_worst_7, tfall_CLK_DO_B_worst_7);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 1 && T_RWM_B[2] == 1 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[11] +: DI_B[11])) = (trise_CLK_DO_B_worst_7, tfall_CLK_DO_B_worst_7);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 1 && T_RWM_B[2] == 1 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[12] +: DI_B[12])) = (trise_CLK_DO_B_worst_7, tfall_CLK_DO_B_worst_7);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 1 && T_RWM_B[2] == 1 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[13] +: DI_B[13])) = (trise_CLK_DO_B_worst_7, tfall_CLK_DO_B_worst_7);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 1 && T_RWM_B[2] == 1 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[14] +: DI_B[14])) = (trise_CLK_DO_B_worst_7, tfall_CLK_DO_B_worst_7);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 1 && T_RWM_B[2] == 1 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[15] +: DI_B[15])) = (trise_CLK_DO_B_worst_7, tfall_CLK_DO_B_worst_7);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 1 && T_RWM_B[2] == 1 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[16] +: DI_B[16])) = (trise_CLK_DO_B_worst_7, tfall_CLK_DO_B_worst_7);
 if(T_RWM_B[0] == 1 && T_RWM_B[1] == 1 && T_RWM_B[2] == 1 && T_BE_N == 1) 
  (posedge CLK_B=>(DO_B[17] +: DI_B[17])) = (trise_CLK_DO_B_worst_7, tfall_CLK_DO_B_worst_7);
 `ifdef SDFVERSION_2
  $setup(posedge T_RWM_A[0] &&& condition1_a, posedge CLK_A &&& condition1_a, t_T_RWM_A_setup_worst);
  $setup(negedge T_RWM_A[0] &&& condition1_a, posedge CLK_A &&& condition1_a, t_T_RWM_A_setup_worst);
  $hold(posedge CLK_A &&& condition1_a, posedge T_RWM_A[0] &&& condition1_a, t_T_RWM_A_hold_worst);
  $hold(posedge CLK_A &&& condition1_a, negedge T_RWM_A[0] &&& condition1_a, t_T_RWM_A_hold_worst);
  $setup(posedge T_RWM_A[1] &&& condition1_a, posedge CLK_A &&& condition1_a, t_T_RWM_A_setup_worst);
  $setup(negedge T_RWM_A[1] &&& condition1_a, posedge CLK_A &&& condition1_a, t_T_RWM_A_setup_worst);
  $hold(posedge CLK_A &&& condition1_a, posedge T_RWM_A[1] &&& condition1_a, t_T_RWM_A_hold_worst);
  $hold(posedge CLK_A &&& condition1_a, negedge T_RWM_A[1] &&& condition1_a, t_T_RWM_A_hold_worst);
  $setup(posedge T_RWM_A[2] &&& condition1_a, posedge CLK_A &&& condition1_a, t_T_RWM_A_setup_worst);
  $setup(negedge T_RWM_A[2] &&& condition1_a, posedge CLK_A &&& condition1_a, t_T_RWM_A_setup_worst);
  $hold(posedge CLK_A &&& condition1_a, posedge T_RWM_A[2] &&& condition1_a, t_T_RWM_A_hold_worst);
  $hold(posedge CLK_A &&& condition1_a, negedge T_RWM_A[2] &&& condition1_a, t_T_RWM_A_hold_worst);
  $setup(posedge T_RWM_B[0] &&& condition1_b, posedge CLK_B &&& condition1_b, t_T_RWM_B_setup_worst);
  $setup(negedge T_RWM_B[0] &&& condition1_b, posedge CLK_B &&& condition1_b, t_T_RWM_B_setup_worst);
  $hold(posedge CLK_B &&& condition1_b, posedge T_RWM_B[0] &&& condition1_b, t_T_RWM_B_hold_worst);
  $hold(posedge CLK_B &&& condition1_b, negedge T_RWM_B[0] &&& condition1_b, t_T_RWM_B_hold_worst);
  $setup(posedge T_RWM_B[1] &&& condition1_b, posedge CLK_B &&& condition1_b, t_T_RWM_B_setup_worst);
  $setup(negedge T_RWM_B[1] &&& condition1_b, posedge CLK_B &&& condition1_b, t_T_RWM_B_setup_worst);
  $hold(posedge CLK_B &&& condition1_b, posedge T_RWM_B[1] &&& condition1_b, t_T_RWM_B_hold_worst);
  $hold(posedge CLK_B &&& condition1_b, negedge T_RWM_B[1] &&& condition1_b, t_T_RWM_B_hold_worst);
  $setup(posedge T_RWM_B[2] &&& condition1_b, posedge CLK_B &&& condition1_b, t_T_RWM_B_setup_worst);
  $setup(negedge T_RWM_B[2] &&& condition1_b, posedge CLK_B &&& condition1_b, t_T_RWM_B_setup_worst);
  $hold(posedge CLK_B &&& condition1_b, posedge T_RWM_B[2] &&& condition1_b, t_T_RWM_B_hold_worst);
  $hold(posedge CLK_B &&& condition1_b, negedge T_RWM_B[2] &&& condition1_b, t_T_RWM_B_hold_worst);
  $setup(posedge DI_A[0] &&& condition3_a, posedge CLK_A &&& condition3_a, t_DI_A_setup_worst);
  $setup(negedge DI_A[0] &&& condition3_a, posedge CLK_A &&& condition3_a, t_DI_A_setup_worst);
  $hold(posedge CLK_A &&& condition3_a, posedge DI_A[0] &&& condition3_a, t_DI_A_hold_worst);
  $hold(posedge CLK_A &&& condition3_a, negedge DI_A[0] &&& condition3_a, t_DI_A_hold_worst);
  $setup(posedge DI_A[1] &&& condition3_a, posedge CLK_A &&& condition3_a, t_DI_A_setup_worst);
  $setup(negedge DI_A[1] &&& condition3_a, posedge CLK_A &&& condition3_a, t_DI_A_setup_worst);
  $hold(posedge CLK_A &&& condition3_a, posedge DI_A[1] &&& condition3_a, t_DI_A_hold_worst);
  $hold(posedge CLK_A &&& condition3_a, negedge DI_A[1] &&& condition3_a, t_DI_A_hold_worst);
  $setup(posedge DI_A[2] &&& condition3_a, posedge CLK_A &&& condition3_a, t_DI_A_setup_worst);
  $setup(negedge DI_A[2] &&& condition3_a, posedge CLK_A &&& condition3_a, t_DI_A_setup_worst);
  $hold(posedge CLK_A &&& condition3_a, posedge DI_A[2] &&& condition3_a, t_DI_A_hold_worst);
  $hold(posedge CLK_A &&& condition3_a, negedge DI_A[2] &&& condition3_a, t_DI_A_hold_worst);
  $setup(posedge DI_A[3] &&& condition3_a, posedge CLK_A &&& condition3_a, t_DI_A_setup_worst);
  $setup(negedge DI_A[3] &&& condition3_a, posedge CLK_A &&& condition3_a, t_DI_A_setup_worst);
  $hold(posedge CLK_A &&& condition3_a, posedge DI_A[3] &&& condition3_a, t_DI_A_hold_worst);
  $hold(posedge CLK_A &&& condition3_a, negedge DI_A[3] &&& condition3_a, t_DI_A_hold_worst);
  $setup(posedge DI_A[4] &&& condition3_a, posedge CLK_A &&& condition3_a, t_DI_A_setup_worst);
  $setup(negedge DI_A[4] &&& condition3_a, posedge CLK_A &&& condition3_a, t_DI_A_setup_worst);
  $hold(posedge CLK_A &&& condition3_a, posedge DI_A[4] &&& condition3_a, t_DI_A_hold_worst);
  $hold(posedge CLK_A &&& condition3_a, negedge DI_A[4] &&& condition3_a, t_DI_A_hold_worst);
  $setup(posedge DI_A[5] &&& condition3_a, posedge CLK_A &&& condition3_a, t_DI_A_setup_worst);
  $setup(negedge DI_A[5] &&& condition3_a, posedge CLK_A &&& condition3_a, t_DI_A_setup_worst);
  $hold(posedge CLK_A &&& condition3_a, posedge DI_A[5] &&& condition3_a, t_DI_A_hold_worst);
  $hold(posedge CLK_A &&& condition3_a, negedge DI_A[5] &&& condition3_a, t_DI_A_hold_worst);
  $setup(posedge DI_A[6] &&& condition3_a, posedge CLK_A &&& condition3_a, t_DI_A_setup_worst);
  $setup(negedge DI_A[6] &&& condition3_a, posedge CLK_A &&& condition3_a, t_DI_A_setup_worst);
  $hold(posedge CLK_A &&& condition3_a, posedge DI_A[6] &&& condition3_a, t_DI_A_hold_worst);
  $hold(posedge CLK_A &&& condition3_a, negedge DI_A[6] &&& condition3_a, t_DI_A_hold_worst);
  $setup(posedge DI_A[7] &&& condition3_a, posedge CLK_A &&& condition3_a, t_DI_A_setup_worst);
  $setup(negedge DI_A[7] &&& condition3_a, posedge CLK_A &&& condition3_a, t_DI_A_setup_worst);
  $hold(posedge CLK_A &&& condition3_a, posedge DI_A[7] &&& condition3_a, t_DI_A_hold_worst);
  $hold(posedge CLK_A &&& condition3_a, negedge DI_A[7] &&& condition3_a, t_DI_A_hold_worst);
  $setup(posedge DI_A[8] &&& condition3_a, posedge CLK_A &&& condition3_a, t_DI_A_setup_worst);
  $setup(negedge DI_A[8] &&& condition3_a, posedge CLK_A &&& condition3_a, t_DI_A_setup_worst);
  $hold(posedge CLK_A &&& condition3_a, posedge DI_A[8] &&& condition3_a, t_DI_A_hold_worst);
  $hold(posedge CLK_A &&& condition3_a, negedge DI_A[8] &&& condition3_a, t_DI_A_hold_worst);
  $setup(posedge DI_A[9] &&& condition3_a, posedge CLK_A &&& condition3_a, t_DI_A_setup_worst);
  $setup(negedge DI_A[9] &&& condition3_a, posedge CLK_A &&& condition3_a, t_DI_A_setup_worst);
  $hold(posedge CLK_A &&& condition3_a, posedge DI_A[9] &&& condition3_a, t_DI_A_hold_worst);
  $hold(posedge CLK_A &&& condition3_a, negedge DI_A[9] &&& condition3_a, t_DI_A_hold_worst);
  $setup(posedge DI_A[10] &&& condition3_a, posedge CLK_A &&& condition3_a, t_DI_A_setup_worst);
  $setup(negedge DI_A[10] &&& condition3_a, posedge CLK_A &&& condition3_a, t_DI_A_setup_worst);
  $hold(posedge CLK_A &&& condition3_a, posedge DI_A[10] &&& condition3_a, t_DI_A_hold_worst);
  $hold(posedge CLK_A &&& condition3_a, negedge DI_A[10] &&& condition3_a, t_DI_A_hold_worst);
  $setup(posedge DI_A[11] &&& condition3_a, posedge CLK_A &&& condition3_a, t_DI_A_setup_worst);
  $setup(negedge DI_A[11] &&& condition3_a, posedge CLK_A &&& condition3_a, t_DI_A_setup_worst);
  $hold(posedge CLK_A &&& condition3_a, posedge DI_A[11] &&& condition3_a, t_DI_A_hold_worst);
  $hold(posedge CLK_A &&& condition3_a, negedge DI_A[11] &&& condition3_a, t_DI_A_hold_worst);
  $setup(posedge DI_A[12] &&& condition3_a, posedge CLK_A &&& condition3_a, t_DI_A_setup_worst);
  $setup(negedge DI_A[12] &&& condition3_a, posedge CLK_A &&& condition3_a, t_DI_A_setup_worst);
  $hold(posedge CLK_A &&& condition3_a, posedge DI_A[12] &&& condition3_a, t_DI_A_hold_worst);
  $hold(posedge CLK_A &&& condition3_a, negedge DI_A[12] &&& condition3_a, t_DI_A_hold_worst);
  $setup(posedge DI_A[13] &&& condition3_a, posedge CLK_A &&& condition3_a, t_DI_A_setup_worst);
  $setup(negedge DI_A[13] &&& condition3_a, posedge CLK_A &&& condition3_a, t_DI_A_setup_worst);
  $hold(posedge CLK_A &&& condition3_a, posedge DI_A[13] &&& condition3_a, t_DI_A_hold_worst);
  $hold(posedge CLK_A &&& condition3_a, negedge DI_A[13] &&& condition3_a, t_DI_A_hold_worst);
  $setup(posedge DI_A[14] &&& condition3_a, posedge CLK_A &&& condition3_a, t_DI_A_setup_worst);
  $setup(negedge DI_A[14] &&& condition3_a, posedge CLK_A &&& condition3_a, t_DI_A_setup_worst);
  $hold(posedge CLK_A &&& condition3_a, posedge DI_A[14] &&& condition3_a, t_DI_A_hold_worst);
  $hold(posedge CLK_A &&& condition3_a, negedge DI_A[14] &&& condition3_a, t_DI_A_hold_worst);
  $setup(posedge DI_A[15] &&& condition3_a, posedge CLK_A &&& condition3_a, t_DI_A_setup_worst);
  $setup(negedge DI_A[15] &&& condition3_a, posedge CLK_A &&& condition3_a, t_DI_A_setup_worst);
  $hold(posedge CLK_A &&& condition3_a, posedge DI_A[15] &&& condition3_a, t_DI_A_hold_worst);
  $hold(posedge CLK_A &&& condition3_a, negedge DI_A[15] &&& condition3_a, t_DI_A_hold_worst);
  $setup(posedge DI_A[16] &&& condition3_a, posedge CLK_A &&& condition3_a, t_DI_A_setup_worst);
  $setup(negedge DI_A[16] &&& condition3_a, posedge CLK_A &&& condition3_a, t_DI_A_setup_worst);
  $hold(posedge CLK_A &&& condition3_a, posedge DI_A[16] &&& condition3_a, t_DI_A_hold_worst);
  $hold(posedge CLK_A &&& condition3_a, negedge DI_A[16] &&& condition3_a, t_DI_A_hold_worst);
  $setup(posedge DI_A[17] &&& condition3_a, posedge CLK_A &&& condition3_a, t_DI_A_setup_worst);
  $setup(negedge DI_A[17] &&& condition3_a, posedge CLK_A &&& condition3_a, t_DI_A_setup_worst);
  $hold(posedge CLK_A &&& condition3_a, posedge DI_A[17] &&& condition3_a, t_DI_A_hold_worst);
  $hold(posedge CLK_A &&& condition3_a, negedge DI_A[17] &&& condition3_a, t_DI_A_hold_worst);
  $setup(posedge DI_B[0] &&& condition3_b, posedge CLK_B &&& condition3_b, t_DI_B_setup_worst);
  $setup(negedge DI_B[0] &&& condition3_b, posedge CLK_B &&& condition3_b, t_DI_B_setup_worst);
  $hold(posedge CLK_B &&& condition3_b, posedge DI_B[0] &&& condition3_b, t_DI_B_hold_worst);
  $hold(posedge CLK_B &&& condition3_b, negedge DI_B[0] &&& condition3_b, t_DI_B_hold_worst);
  $setup(posedge DI_B[1] &&& condition3_b, posedge CLK_B &&& condition3_b, t_DI_B_setup_worst);
  $setup(negedge DI_B[1] &&& condition3_b, posedge CLK_B &&& condition3_b, t_DI_B_setup_worst);
  $hold(posedge CLK_B &&& condition3_b, posedge DI_B[1] &&& condition3_b, t_DI_B_hold_worst);
  $hold(posedge CLK_B &&& condition3_b, negedge DI_B[1] &&& condition3_b, t_DI_B_hold_worst);
  $setup(posedge DI_B[2] &&& condition3_b, posedge CLK_B &&& condition3_b, t_DI_B_setup_worst);
  $setup(negedge DI_B[2] &&& condition3_b, posedge CLK_B &&& condition3_b, t_DI_B_setup_worst);
  $hold(posedge CLK_B &&& condition3_b, posedge DI_B[2] &&& condition3_b, t_DI_B_hold_worst);
  $hold(posedge CLK_B &&& condition3_b, negedge DI_B[2] &&& condition3_b, t_DI_B_hold_worst);
  $setup(posedge DI_B[3] &&& condition3_b, posedge CLK_B &&& condition3_b, t_DI_B_setup_worst);
  $setup(negedge DI_B[3] &&& condition3_b, posedge CLK_B &&& condition3_b, t_DI_B_setup_worst);
  $hold(posedge CLK_B &&& condition3_b, posedge DI_B[3] &&& condition3_b, t_DI_B_hold_worst);
  $hold(posedge CLK_B &&& condition3_b, negedge DI_B[3] &&& condition3_b, t_DI_B_hold_worst);
  $setup(posedge DI_B[4] &&& condition3_b, posedge CLK_B &&& condition3_b, t_DI_B_setup_worst);
  $setup(negedge DI_B[4] &&& condition3_b, posedge CLK_B &&& condition3_b, t_DI_B_setup_worst);
  $hold(posedge CLK_B &&& condition3_b, posedge DI_B[4] &&& condition3_b, t_DI_B_hold_worst);
  $hold(posedge CLK_B &&& condition3_b, negedge DI_B[4] &&& condition3_b, t_DI_B_hold_worst);
  $setup(posedge DI_B[5] &&& condition3_b, posedge CLK_B &&& condition3_b, t_DI_B_setup_worst);
  $setup(negedge DI_B[5] &&& condition3_b, posedge CLK_B &&& condition3_b, t_DI_B_setup_worst);
  $hold(posedge CLK_B &&& condition3_b, posedge DI_B[5] &&& condition3_b, t_DI_B_hold_worst);
  $hold(posedge CLK_B &&& condition3_b, negedge DI_B[5] &&& condition3_b, t_DI_B_hold_worst);
  $setup(posedge DI_B[6] &&& condition3_b, posedge CLK_B &&& condition3_b, t_DI_B_setup_worst);
  $setup(negedge DI_B[6] &&& condition3_b, posedge CLK_B &&& condition3_b, t_DI_B_setup_worst);
  $hold(posedge CLK_B &&& condition3_b, posedge DI_B[6] &&& condition3_b, t_DI_B_hold_worst);
  $hold(posedge CLK_B &&& condition3_b, negedge DI_B[6] &&& condition3_b, t_DI_B_hold_worst);
  $setup(posedge DI_B[7] &&& condition3_b, posedge CLK_B &&& condition3_b, t_DI_B_setup_worst);
  $setup(negedge DI_B[7] &&& condition3_b, posedge CLK_B &&& condition3_b, t_DI_B_setup_worst);
  $hold(posedge CLK_B &&& condition3_b, posedge DI_B[7] &&& condition3_b, t_DI_B_hold_worst);
  $hold(posedge CLK_B &&& condition3_b, negedge DI_B[7] &&& condition3_b, t_DI_B_hold_worst);
  $setup(posedge DI_B[8] &&& condition3_b, posedge CLK_B &&& condition3_b, t_DI_B_setup_worst);
  $setup(negedge DI_B[8] &&& condition3_b, posedge CLK_B &&& condition3_b, t_DI_B_setup_worst);
  $hold(posedge CLK_B &&& condition3_b, posedge DI_B[8] &&& condition3_b, t_DI_B_hold_worst);
  $hold(posedge CLK_B &&& condition3_b, negedge DI_B[8] &&& condition3_b, t_DI_B_hold_worst);
  $setup(posedge DI_B[9] &&& condition3_b, posedge CLK_B &&& condition3_b, t_DI_B_setup_worst);
  $setup(negedge DI_B[9] &&& condition3_b, posedge CLK_B &&& condition3_b, t_DI_B_setup_worst);
  $hold(posedge CLK_B &&& condition3_b, posedge DI_B[9] &&& condition3_b, t_DI_B_hold_worst);
  $hold(posedge CLK_B &&& condition3_b, negedge DI_B[9] &&& condition3_b, t_DI_B_hold_worst);
  $setup(posedge DI_B[10] &&& condition3_b, posedge CLK_B &&& condition3_b, t_DI_B_setup_worst);
  $setup(negedge DI_B[10] &&& condition3_b, posedge CLK_B &&& condition3_b, t_DI_B_setup_worst);
  $hold(posedge CLK_B &&& condition3_b, posedge DI_B[10] &&& condition3_b, t_DI_B_hold_worst);
  $hold(posedge CLK_B &&& condition3_b, negedge DI_B[10] &&& condition3_b, t_DI_B_hold_worst);
  $setup(posedge DI_B[11] &&& condition3_b, posedge CLK_B &&& condition3_b, t_DI_B_setup_worst);
  $setup(negedge DI_B[11] &&& condition3_b, posedge CLK_B &&& condition3_b, t_DI_B_setup_worst);
  $hold(posedge CLK_B &&& condition3_b, posedge DI_B[11] &&& condition3_b, t_DI_B_hold_worst);
  $hold(posedge CLK_B &&& condition3_b, negedge DI_B[11] &&& condition3_b, t_DI_B_hold_worst);
  $setup(posedge DI_B[12] &&& condition3_b, posedge CLK_B &&& condition3_b, t_DI_B_setup_worst);
  $setup(negedge DI_B[12] &&& condition3_b, posedge CLK_B &&& condition3_b, t_DI_B_setup_worst);
  $hold(posedge CLK_B &&& condition3_b, posedge DI_B[12] &&& condition3_b, t_DI_B_hold_worst);
  $hold(posedge CLK_B &&& condition3_b, negedge DI_B[12] &&& condition3_b, t_DI_B_hold_worst);
  $setup(posedge DI_B[13] &&& condition3_b, posedge CLK_B &&& condition3_b, t_DI_B_setup_worst);
  $setup(negedge DI_B[13] &&& condition3_b, posedge CLK_B &&& condition3_b, t_DI_B_setup_worst);
  $hold(posedge CLK_B &&& condition3_b, posedge DI_B[13] &&& condition3_b, t_DI_B_hold_worst);
  $hold(posedge CLK_B &&& condition3_b, negedge DI_B[13] &&& condition3_b, t_DI_B_hold_worst);
  $setup(posedge DI_B[14] &&& condition3_b, posedge CLK_B &&& condition3_b, t_DI_B_setup_worst);
  $setup(negedge DI_B[14] &&& condition3_b, posedge CLK_B &&& condition3_b, t_DI_B_setup_worst);
  $hold(posedge CLK_B &&& condition3_b, posedge DI_B[14] &&& condition3_b, t_DI_B_hold_worst);
  $hold(posedge CLK_B &&& condition3_b, negedge DI_B[14] &&& condition3_b, t_DI_B_hold_worst);
  $setup(posedge DI_B[15] &&& condition3_b, posedge CLK_B &&& condition3_b, t_DI_B_setup_worst);
  $setup(negedge DI_B[15] &&& condition3_b, posedge CLK_B &&& condition3_b, t_DI_B_setup_worst);
  $hold(posedge CLK_B &&& condition3_b, posedge DI_B[15] &&& condition3_b, t_DI_B_hold_worst);
  $hold(posedge CLK_B &&& condition3_b, negedge DI_B[15] &&& condition3_b, t_DI_B_hold_worst);
  $setup(posedge DI_B[16] &&& condition3_b, posedge CLK_B &&& condition3_b, t_DI_B_setup_worst);
  $setup(negedge DI_B[16] &&& condition3_b, posedge CLK_B &&& condition3_b, t_DI_B_setup_worst);
  $hold(posedge CLK_B &&& condition3_b, posedge DI_B[16] &&& condition3_b, t_DI_B_hold_worst);
  $hold(posedge CLK_B &&& condition3_b, negedge DI_B[16] &&& condition3_b, t_DI_B_hold_worst);
  $setup(posedge DI_B[17] &&& condition3_b, posedge CLK_B &&& condition3_b, t_DI_B_setup_worst);
  $setup(negedge DI_B[17] &&& condition3_b, posedge CLK_B &&& condition3_b, t_DI_B_setup_worst);
  $hold(posedge CLK_B &&& condition3_b, posedge DI_B[17] &&& condition3_b, t_DI_B_hold_worst);
  $hold(posedge CLK_B &&& condition3_b, negedge DI_B[17] &&& condition3_b, t_DI_B_hold_worst);
  $setup(posedge T_BE_N, posedge CLK_A, t_T_BE_N_setup_worst);
  $setup(negedge T_BE_N, posedge CLK_A, t_T_BE_N_setup_worst);
  $hold(posedge CLK_A, posedge T_BE_N, t_T_BE_N_hold_worst);
  $hold(posedge CLK_A, negedge T_BE_N, t_T_BE_N_hold_worst);
  $setup(posedge T_BE_N, posedge CLK_B, t_T_BE_N_setup_worst);
  $setup(negedge T_BE_N, posedge CLK_B, t_T_BE_N_setup_worst);
  $hold(posedge CLK_B, posedge T_BE_N, t_T_BE_N_hold_worst);
  $hold(posedge CLK_B, negedge T_BE_N, t_T_BE_N_hold_worst);
  $setup(posedge T_DI_A[0] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_DI_A_setup_worst);
  $setup(negedge T_DI_A[0] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_DI_A_setup_worst);
  $hold(posedge CLK_A &&& condition2_a, posedge T_DI_A[0] &&& condition2_a, t_T_DI_A_hold_worst);
  $hold(posedge CLK_A &&& condition2_a, negedge T_DI_A[0] &&& condition2_a, t_T_DI_A_hold_worst);
  $setup(posedge T_DI_A[1] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_DI_A_setup_worst);
  $setup(negedge T_DI_A[1] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_DI_A_setup_worst);
  $hold(posedge CLK_A &&& condition2_a, posedge T_DI_A[1] &&& condition2_a, t_T_DI_A_hold_worst);
  $hold(posedge CLK_A &&& condition2_a, negedge T_DI_A[1] &&& condition2_a, t_T_DI_A_hold_worst);
  $setup(posedge T_DI_A[2] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_DI_A_setup_worst);
  $setup(negedge T_DI_A[2] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_DI_A_setup_worst);
  $hold(posedge CLK_A &&& condition2_a, posedge T_DI_A[2] &&& condition2_a, t_T_DI_A_hold_worst);
  $hold(posedge CLK_A &&& condition2_a, negedge T_DI_A[2] &&& condition2_a, t_T_DI_A_hold_worst);
  $setup(posedge T_DI_A[3] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_DI_A_setup_worst);
  $setup(negedge T_DI_A[3] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_DI_A_setup_worst);
  $hold(posedge CLK_A &&& condition2_a, posedge T_DI_A[3] &&& condition2_a, t_T_DI_A_hold_worst);
  $hold(posedge CLK_A &&& condition2_a, negedge T_DI_A[3] &&& condition2_a, t_T_DI_A_hold_worst);
  $setup(posedge T_DI_A[4] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_DI_A_setup_worst);
  $setup(negedge T_DI_A[4] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_DI_A_setup_worst);
  $hold(posedge CLK_A &&& condition2_a, posedge T_DI_A[4] &&& condition2_a, t_T_DI_A_hold_worst);
  $hold(posedge CLK_A &&& condition2_a, negedge T_DI_A[4] &&& condition2_a, t_T_DI_A_hold_worst);
  $setup(posedge T_DI_A[5] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_DI_A_setup_worst);
  $setup(negedge T_DI_A[5] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_DI_A_setup_worst);
  $hold(posedge CLK_A &&& condition2_a, posedge T_DI_A[5] &&& condition2_a, t_T_DI_A_hold_worst);
  $hold(posedge CLK_A &&& condition2_a, negedge T_DI_A[5] &&& condition2_a, t_T_DI_A_hold_worst);
  $setup(posedge T_DI_A[6] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_DI_A_setup_worst);
  $setup(negedge T_DI_A[6] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_DI_A_setup_worst);
  $hold(posedge CLK_A &&& condition2_a, posedge T_DI_A[6] &&& condition2_a, t_T_DI_A_hold_worst);
  $hold(posedge CLK_A &&& condition2_a, negedge T_DI_A[6] &&& condition2_a, t_T_DI_A_hold_worst);
  $setup(posedge T_DI_A[7] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_DI_A_setup_worst);
  $setup(negedge T_DI_A[7] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_DI_A_setup_worst);
  $hold(posedge CLK_A &&& condition2_a, posedge T_DI_A[7] &&& condition2_a, t_T_DI_A_hold_worst);
  $hold(posedge CLK_A &&& condition2_a, negedge T_DI_A[7] &&& condition2_a, t_T_DI_A_hold_worst);
  $setup(posedge T_DI_A[8] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_DI_A_setup_worst);
  $setup(negedge T_DI_A[8] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_DI_A_setup_worst);
  $hold(posedge CLK_A &&& condition2_a, posedge T_DI_A[8] &&& condition2_a, t_T_DI_A_hold_worst);
  $hold(posedge CLK_A &&& condition2_a, negedge T_DI_A[8] &&& condition2_a, t_T_DI_A_hold_worst);
  $setup(posedge T_DI_A[9] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_DI_A_setup_worst);
  $setup(negedge T_DI_A[9] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_DI_A_setup_worst);
  $hold(posedge CLK_A &&& condition2_a, posedge T_DI_A[9] &&& condition2_a, t_T_DI_A_hold_worst);
  $hold(posedge CLK_A &&& condition2_a, negedge T_DI_A[9] &&& condition2_a, t_T_DI_A_hold_worst);
  $setup(posedge T_DI_A[10] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_DI_A_setup_worst);
  $setup(negedge T_DI_A[10] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_DI_A_setup_worst);
  $hold(posedge CLK_A &&& condition2_a, posedge T_DI_A[10] &&& condition2_a, t_T_DI_A_hold_worst);
  $hold(posedge CLK_A &&& condition2_a, negedge T_DI_A[10] &&& condition2_a, t_T_DI_A_hold_worst);
  $setup(posedge T_DI_A[11] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_DI_A_setup_worst);
  $setup(negedge T_DI_A[11] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_DI_A_setup_worst);
  $hold(posedge CLK_A &&& condition2_a, posedge T_DI_A[11] &&& condition2_a, t_T_DI_A_hold_worst);
  $hold(posedge CLK_A &&& condition2_a, negedge T_DI_A[11] &&& condition2_a, t_T_DI_A_hold_worst);
  $setup(posedge T_DI_A[12] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_DI_A_setup_worst);
  $setup(negedge T_DI_A[12] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_DI_A_setup_worst);
  $hold(posedge CLK_A &&& condition2_a, posedge T_DI_A[12] &&& condition2_a, t_T_DI_A_hold_worst);
  $hold(posedge CLK_A &&& condition2_a, negedge T_DI_A[12] &&& condition2_a, t_T_DI_A_hold_worst);
  $setup(posedge T_DI_A[13] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_DI_A_setup_worst);
  $setup(negedge T_DI_A[13] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_DI_A_setup_worst);
  $hold(posedge CLK_A &&& condition2_a, posedge T_DI_A[13] &&& condition2_a, t_T_DI_A_hold_worst);
  $hold(posedge CLK_A &&& condition2_a, negedge T_DI_A[13] &&& condition2_a, t_T_DI_A_hold_worst);
  $setup(posedge T_DI_A[14] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_DI_A_setup_worst);
  $setup(negedge T_DI_A[14] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_DI_A_setup_worst);
  $hold(posedge CLK_A &&& condition2_a, posedge T_DI_A[14] &&& condition2_a, t_T_DI_A_hold_worst);
  $hold(posedge CLK_A &&& condition2_a, negedge T_DI_A[14] &&& condition2_a, t_T_DI_A_hold_worst);
  $setup(posedge T_DI_A[15] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_DI_A_setup_worst);
  $setup(negedge T_DI_A[15] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_DI_A_setup_worst);
  $hold(posedge CLK_A &&& condition2_a, posedge T_DI_A[15] &&& condition2_a, t_T_DI_A_hold_worst);
  $hold(posedge CLK_A &&& condition2_a, negedge T_DI_A[15] &&& condition2_a, t_T_DI_A_hold_worst);
  $setup(posedge T_DI_A[16] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_DI_A_setup_worst);
  $setup(negedge T_DI_A[16] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_DI_A_setup_worst);
  $hold(posedge CLK_A &&& condition2_a, posedge T_DI_A[16] &&& condition2_a, t_T_DI_A_hold_worst);
  $hold(posedge CLK_A &&& condition2_a, negedge T_DI_A[16] &&& condition2_a, t_T_DI_A_hold_worst);
  $setup(posedge T_DI_A[17] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_DI_A_setup_worst);
  $setup(negedge T_DI_A[17] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_DI_A_setup_worst);
  $hold(posedge CLK_A &&& condition2_a, posedge T_DI_A[17] &&& condition2_a, t_T_DI_A_hold_worst);
  $hold(posedge CLK_A &&& condition2_a, negedge T_DI_A[17] &&& condition2_a, t_T_DI_A_hold_worst);
  $setup(posedge T_DI_B[0] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_DI_B_setup_worst);
  $setup(negedge T_DI_B[0] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_DI_B_setup_worst);
  $hold(posedge CLK_B &&& condition2_b, posedge T_DI_B[0] &&& condition2_b, t_T_DI_B_hold_worst);
  $hold(posedge CLK_B &&& condition2_b, negedge T_DI_B[0] &&& condition2_b, t_T_DI_B_hold_worst);
  $setup(posedge T_DI_B[1] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_DI_B_setup_worst);
  $setup(negedge T_DI_B[1] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_DI_B_setup_worst);
  $hold(posedge CLK_B &&& condition2_b, posedge T_DI_B[1] &&& condition2_b, t_T_DI_B_hold_worst);
  $hold(posedge CLK_B &&& condition2_b, negedge T_DI_B[1] &&& condition2_b, t_T_DI_B_hold_worst);
  $setup(posedge T_DI_B[2] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_DI_B_setup_worst);
  $setup(negedge T_DI_B[2] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_DI_B_setup_worst);
  $hold(posedge CLK_B &&& condition2_b, posedge T_DI_B[2] &&& condition2_b, t_T_DI_B_hold_worst);
  $hold(posedge CLK_B &&& condition2_b, negedge T_DI_B[2] &&& condition2_b, t_T_DI_B_hold_worst);
  $setup(posedge T_DI_B[3] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_DI_B_setup_worst);
  $setup(negedge T_DI_B[3] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_DI_B_setup_worst);
  $hold(posedge CLK_B &&& condition2_b, posedge T_DI_B[3] &&& condition2_b, t_T_DI_B_hold_worst);
  $hold(posedge CLK_B &&& condition2_b, negedge T_DI_B[3] &&& condition2_b, t_T_DI_B_hold_worst);
  $setup(posedge T_DI_B[4] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_DI_B_setup_worst);
  $setup(negedge T_DI_B[4] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_DI_B_setup_worst);
  $hold(posedge CLK_B &&& condition2_b, posedge T_DI_B[4] &&& condition2_b, t_T_DI_B_hold_worst);
  $hold(posedge CLK_B &&& condition2_b, negedge T_DI_B[4] &&& condition2_b, t_T_DI_B_hold_worst);
  $setup(posedge T_DI_B[5] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_DI_B_setup_worst);
  $setup(negedge T_DI_B[5] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_DI_B_setup_worst);
  $hold(posedge CLK_B &&& condition2_b, posedge T_DI_B[5] &&& condition2_b, t_T_DI_B_hold_worst);
  $hold(posedge CLK_B &&& condition2_b, negedge T_DI_B[5] &&& condition2_b, t_T_DI_B_hold_worst);
  $setup(posedge T_DI_B[6] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_DI_B_setup_worst);
  $setup(negedge T_DI_B[6] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_DI_B_setup_worst);
  $hold(posedge CLK_B &&& condition2_b, posedge T_DI_B[6] &&& condition2_b, t_T_DI_B_hold_worst);
  $hold(posedge CLK_B &&& condition2_b, negedge T_DI_B[6] &&& condition2_b, t_T_DI_B_hold_worst);
  $setup(posedge T_DI_B[7] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_DI_B_setup_worst);
  $setup(negedge T_DI_B[7] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_DI_B_setup_worst);
  $hold(posedge CLK_B &&& condition2_b, posedge T_DI_B[7] &&& condition2_b, t_T_DI_B_hold_worst);
  $hold(posedge CLK_B &&& condition2_b, negedge T_DI_B[7] &&& condition2_b, t_T_DI_B_hold_worst);
  $setup(posedge T_DI_B[8] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_DI_B_setup_worst);
  $setup(negedge T_DI_B[8] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_DI_B_setup_worst);
  $hold(posedge CLK_B &&& condition2_b, posedge T_DI_B[8] &&& condition2_b, t_T_DI_B_hold_worst);
  $hold(posedge CLK_B &&& condition2_b, negedge T_DI_B[8] &&& condition2_b, t_T_DI_B_hold_worst);
  $setup(posedge T_DI_B[9] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_DI_B_setup_worst);
  $setup(negedge T_DI_B[9] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_DI_B_setup_worst);
  $hold(posedge CLK_B &&& condition2_b, posedge T_DI_B[9] &&& condition2_b, t_T_DI_B_hold_worst);
  $hold(posedge CLK_B &&& condition2_b, negedge T_DI_B[9] &&& condition2_b, t_T_DI_B_hold_worst);
  $setup(posedge T_DI_B[10] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_DI_B_setup_worst);
  $setup(negedge T_DI_B[10] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_DI_B_setup_worst);
  $hold(posedge CLK_B &&& condition2_b, posedge T_DI_B[10] &&& condition2_b, t_T_DI_B_hold_worst);
  $hold(posedge CLK_B &&& condition2_b, negedge T_DI_B[10] &&& condition2_b, t_T_DI_B_hold_worst);
  $setup(posedge T_DI_B[11] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_DI_B_setup_worst);
  $setup(negedge T_DI_B[11] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_DI_B_setup_worst);
  $hold(posedge CLK_B &&& condition2_b, posedge T_DI_B[11] &&& condition2_b, t_T_DI_B_hold_worst);
  $hold(posedge CLK_B &&& condition2_b, negedge T_DI_B[11] &&& condition2_b, t_T_DI_B_hold_worst);
  $setup(posedge T_DI_B[12] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_DI_B_setup_worst);
  $setup(negedge T_DI_B[12] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_DI_B_setup_worst);
  $hold(posedge CLK_B &&& condition2_b, posedge T_DI_B[12] &&& condition2_b, t_T_DI_B_hold_worst);
  $hold(posedge CLK_B &&& condition2_b, negedge T_DI_B[12] &&& condition2_b, t_T_DI_B_hold_worst);
  $setup(posedge T_DI_B[13] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_DI_B_setup_worst);
  $setup(negedge T_DI_B[13] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_DI_B_setup_worst);
  $hold(posedge CLK_B &&& condition2_b, posedge T_DI_B[13] &&& condition2_b, t_T_DI_B_hold_worst);
  $hold(posedge CLK_B &&& condition2_b, negedge T_DI_B[13] &&& condition2_b, t_T_DI_B_hold_worst);
  $setup(posedge T_DI_B[14] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_DI_B_setup_worst);
  $setup(negedge T_DI_B[14] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_DI_B_setup_worst);
  $hold(posedge CLK_B &&& condition2_b, posedge T_DI_B[14] &&& condition2_b, t_T_DI_B_hold_worst);
  $hold(posedge CLK_B &&& condition2_b, negedge T_DI_B[14] &&& condition2_b, t_T_DI_B_hold_worst);
  $setup(posedge T_DI_B[15] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_DI_B_setup_worst);
  $setup(negedge T_DI_B[15] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_DI_B_setup_worst);
  $hold(posedge CLK_B &&& condition2_b, posedge T_DI_B[15] &&& condition2_b, t_T_DI_B_hold_worst);
  $hold(posedge CLK_B &&& condition2_b, negedge T_DI_B[15] &&& condition2_b, t_T_DI_B_hold_worst);
  $setup(posedge T_DI_B[16] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_DI_B_setup_worst);
  $setup(negedge T_DI_B[16] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_DI_B_setup_worst);
  $hold(posedge CLK_B &&& condition2_b, posedge T_DI_B[16] &&& condition2_b, t_T_DI_B_hold_worst);
  $hold(posedge CLK_B &&& condition2_b, negedge T_DI_B[16] &&& condition2_b, t_T_DI_B_hold_worst);
  $setup(posedge T_DI_B[17] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_DI_B_setup_worst);
  $setup(negedge T_DI_B[17] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_DI_B_setup_worst);
  $hold(posedge CLK_B &&& condition2_b, posedge T_DI_B[17] &&& condition2_b, t_T_DI_B_hold_worst);
  $hold(posedge CLK_B &&& condition2_b, negedge T_DI_B[17] &&& condition2_b, t_T_DI_B_hold_worst);
  $setup(posedge A_A[0] &&& condition3_a, posedge CLK_A &&& condition3_a, t_A_A_setup_worst);
  $setup(negedge A_A[0] &&& condition3_a, posedge CLK_A &&& condition3_a, t_A_A_setup_worst);
  $hold(posedge CLK_A &&& condition3_a, posedge A_A[0] &&& condition3_a, t_A_A_hold_worst);
  $hold(posedge CLK_A &&& condition3_a, negedge A_A[0] &&& condition3_a, t_A_A_hold_worst);
  $setup(posedge A_A[1] &&& condition3_a, posedge CLK_A &&& condition3_a, t_A_A_setup_worst);
  $setup(negedge A_A[1] &&& condition3_a, posedge CLK_A &&& condition3_a, t_A_A_setup_worst);
  $hold(posedge CLK_A &&& condition3_a, posedge A_A[1] &&& condition3_a, t_A_A_hold_worst);
  $hold(posedge CLK_A &&& condition3_a, negedge A_A[1] &&& condition3_a, t_A_A_hold_worst);
  $setup(posedge A_A[2] &&& condition3_a, posedge CLK_A &&& condition3_a, t_A_A_setup_worst);
  $setup(negedge A_A[2] &&& condition3_a, posedge CLK_A &&& condition3_a, t_A_A_setup_worst);
  $hold(posedge CLK_A &&& condition3_a, posedge A_A[2] &&& condition3_a, t_A_A_hold_worst);
  $hold(posedge CLK_A &&& condition3_a, negedge A_A[2] &&& condition3_a, t_A_A_hold_worst);
  $setup(posedge A_A[3] &&& condition3_a, posedge CLK_A &&& condition3_a, t_A_A_setup_worst);
  $setup(negedge A_A[3] &&& condition3_a, posedge CLK_A &&& condition3_a, t_A_A_setup_worst);
  $hold(posedge CLK_A &&& condition3_a, posedge A_A[3] &&& condition3_a, t_A_A_hold_worst);
  $hold(posedge CLK_A &&& condition3_a, negedge A_A[3] &&& condition3_a, t_A_A_hold_worst);
  $setup(posedge A_A[4] &&& condition3_a, posedge CLK_A &&& condition3_a, t_A_A_setup_worst);
  $setup(negedge A_A[4] &&& condition3_a, posedge CLK_A &&& condition3_a, t_A_A_setup_worst);
  $hold(posedge CLK_A &&& condition3_a, posedge A_A[4] &&& condition3_a, t_A_A_hold_worst);
  $hold(posedge CLK_A &&& condition3_a, negedge A_A[4] &&& condition3_a, t_A_A_hold_worst);
  $setup(posedge A_A[5] &&& condition3_a, posedge CLK_A &&& condition3_a, t_A_A_setup_worst);
  $setup(negedge A_A[5] &&& condition3_a, posedge CLK_A &&& condition3_a, t_A_A_setup_worst);
  $hold(posedge CLK_A &&& condition3_a, posedge A_A[5] &&& condition3_a, t_A_A_hold_worst);
  $hold(posedge CLK_A &&& condition3_a, negedge A_A[5] &&& condition3_a, t_A_A_hold_worst);
  $setup(posedge A_A[6] &&& condition3_a, posedge CLK_A &&& condition3_a, t_A_A_setup_worst);
  $setup(negedge A_A[6] &&& condition3_a, posedge CLK_A &&& condition3_a, t_A_A_setup_worst);
  $hold(posedge CLK_A &&& condition3_a, posedge A_A[6] &&& condition3_a, t_A_A_hold_worst);
  $hold(posedge CLK_A &&& condition3_a, negedge A_A[6] &&& condition3_a, t_A_A_hold_worst);
  $setup(posedge A_A[7] &&& condition3_a, posedge CLK_A &&& condition3_a, t_A_A_setup_worst);
  $setup(negedge A_A[7] &&& condition3_a, posedge CLK_A &&& condition3_a, t_A_A_setup_worst);
  $hold(posedge CLK_A &&& condition3_a, posedge A_A[7] &&& condition3_a, t_A_A_hold_worst);
  $hold(posedge CLK_A &&& condition3_a, negedge A_A[7] &&& condition3_a, t_A_A_hold_worst);
  $setup(posedge A_A[8] &&& condition3_a, posedge CLK_A &&& condition3_a, t_A_A_setup_worst);
  $setup(negedge A_A[8] &&& condition3_a, posedge CLK_A &&& condition3_a, t_A_A_setup_worst);
  $hold(posedge CLK_A &&& condition3_a, posedge A_A[8] &&& condition3_a, t_A_A_hold_worst);
  $hold(posedge CLK_A &&& condition3_a, negedge A_A[8] &&& condition3_a, t_A_A_hold_worst);
  $setup(posedge A_A[9] &&& condition3_a, posedge CLK_A &&& condition3_a, t_A_A_setup_worst);
  $setup(negedge A_A[9] &&& condition3_a, posedge CLK_A &&& condition3_a, t_A_A_setup_worst);
  $hold(posedge CLK_A &&& condition3_a, posedge A_A[9] &&& condition3_a, t_A_A_hold_worst);
  $hold(posedge CLK_A &&& condition3_a, negedge A_A[9] &&& condition3_a, t_A_A_hold_worst);
  $setup(posedge A_B[0] &&& condition3_b, posedge CLK_B &&& condition3_b, t_A_B_setup_worst);
  $setup(negedge A_B[0] &&& condition3_b, posedge CLK_B &&& condition3_b, t_A_B_setup_worst);
  $hold(posedge CLK_B &&& condition3_b, posedge A_B[0] &&& condition3_b, t_A_B_hold_worst);
  $hold(posedge CLK_B &&& condition3_b, negedge A_B[0] &&& condition3_b, t_A_B_hold_worst);
  $setup(posedge A_B[1] &&& condition3_b, posedge CLK_B &&& condition3_b, t_A_B_setup_worst);
  $setup(negedge A_B[1] &&& condition3_b, posedge CLK_B &&& condition3_b, t_A_B_setup_worst);
  $hold(posedge CLK_B &&& condition3_b, posedge A_B[1] &&& condition3_b, t_A_B_hold_worst);
  $hold(posedge CLK_B &&& condition3_b, negedge A_B[1] &&& condition3_b, t_A_B_hold_worst);
  $setup(posedge A_B[2] &&& condition3_b, posedge CLK_B &&& condition3_b, t_A_B_setup_worst);
  $setup(negedge A_B[2] &&& condition3_b, posedge CLK_B &&& condition3_b, t_A_B_setup_worst);
  $hold(posedge CLK_B &&& condition3_b, posedge A_B[2] &&& condition3_b, t_A_B_hold_worst);
  $hold(posedge CLK_B &&& condition3_b, negedge A_B[2] &&& condition3_b, t_A_B_hold_worst);
  $setup(posedge A_B[3] &&& condition3_b, posedge CLK_B &&& condition3_b, t_A_B_setup_worst);
  $setup(negedge A_B[3] &&& condition3_b, posedge CLK_B &&& condition3_b, t_A_B_setup_worst);
  $hold(posedge CLK_B &&& condition3_b, posedge A_B[3] &&& condition3_b, t_A_B_hold_worst);
  $hold(posedge CLK_B &&& condition3_b, negedge A_B[3] &&& condition3_b, t_A_B_hold_worst);
  $setup(posedge A_B[4] &&& condition3_b, posedge CLK_B &&& condition3_b, t_A_B_setup_worst);
  $setup(negedge A_B[4] &&& condition3_b, posedge CLK_B &&& condition3_b, t_A_B_setup_worst);
  $hold(posedge CLK_B &&& condition3_b, posedge A_B[4] &&& condition3_b, t_A_B_hold_worst);
  $hold(posedge CLK_B &&& condition3_b, negedge A_B[4] &&& condition3_b, t_A_B_hold_worst);
  $setup(posedge A_B[5] &&& condition3_b, posedge CLK_B &&& condition3_b, t_A_B_setup_worst);
  $setup(negedge A_B[5] &&& condition3_b, posedge CLK_B &&& condition3_b, t_A_B_setup_worst);
  $hold(posedge CLK_B &&& condition3_b, posedge A_B[5] &&& condition3_b, t_A_B_hold_worst);
  $hold(posedge CLK_B &&& condition3_b, negedge A_B[5] &&& condition3_b, t_A_B_hold_worst);
  $setup(posedge A_B[6] &&& condition3_b, posedge CLK_B &&& condition3_b, t_A_B_setup_worst);
  $setup(negedge A_B[6] &&& condition3_b, posedge CLK_B &&& condition3_b, t_A_B_setup_worst);
  $hold(posedge CLK_B &&& condition3_b, posedge A_B[6] &&& condition3_b, t_A_B_hold_worst);
  $hold(posedge CLK_B &&& condition3_b, negedge A_B[6] &&& condition3_b, t_A_B_hold_worst);
  $setup(posedge A_B[7] &&& condition3_b, posedge CLK_B &&& condition3_b, t_A_B_setup_worst);
  $setup(negedge A_B[7] &&& condition3_b, posedge CLK_B &&& condition3_b, t_A_B_setup_worst);
  $hold(posedge CLK_B &&& condition3_b, posedge A_B[7] &&& condition3_b, t_A_B_hold_worst);
  $hold(posedge CLK_B &&& condition3_b, negedge A_B[7] &&& condition3_b, t_A_B_hold_worst);
  $setup(posedge A_B[8] &&& condition3_b, posedge CLK_B &&& condition3_b, t_A_B_setup_worst);
  $setup(negedge A_B[8] &&& condition3_b, posedge CLK_B &&& condition3_b, t_A_B_setup_worst);
  $hold(posedge CLK_B &&& condition3_b, posedge A_B[8] &&& condition3_b, t_A_B_hold_worst);
  $hold(posedge CLK_B &&& condition3_b, negedge A_B[8] &&& condition3_b, t_A_B_hold_worst);
  $setup(posedge A_B[9] &&& condition3_b, posedge CLK_B &&& condition3_b, t_A_B_setup_worst);
  $setup(negedge A_B[9] &&& condition3_b, posedge CLK_B &&& condition3_b, t_A_B_setup_worst);
  $hold(posedge CLK_B &&& condition3_b, posedge A_B[9] &&& condition3_b, t_A_B_hold_worst);
  $hold(posedge CLK_B &&& condition3_b, negedge A_B[9] &&& condition3_b, t_A_B_hold_worst);
  $setup(posedge T_A_A[0] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_A_A_setup_worst);
  $setup(negedge T_A_A[0] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_A_A_setup_worst);
  $hold(posedge CLK_A &&& condition2_a, posedge T_A_A[0] &&& condition2_a, t_T_A_A_hold_worst);
  $hold(posedge CLK_A &&& condition2_a, negedge T_A_A[0] &&& condition2_a, t_T_A_A_hold_worst);
  $setup(posedge T_A_A[1] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_A_A_setup_worst);
  $setup(negedge T_A_A[1] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_A_A_setup_worst);
  $hold(posedge CLK_A &&& condition2_a, posedge T_A_A[1] &&& condition2_a, t_T_A_A_hold_worst);
  $hold(posedge CLK_A &&& condition2_a, negedge T_A_A[1] &&& condition2_a, t_T_A_A_hold_worst);
  $setup(posedge T_A_A[2] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_A_A_setup_worst);
  $setup(negedge T_A_A[2] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_A_A_setup_worst);
  $hold(posedge CLK_A &&& condition2_a, posedge T_A_A[2] &&& condition2_a, t_T_A_A_hold_worst);
  $hold(posedge CLK_A &&& condition2_a, negedge T_A_A[2] &&& condition2_a, t_T_A_A_hold_worst);
  $setup(posedge T_A_A[3] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_A_A_setup_worst);
  $setup(negedge T_A_A[3] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_A_A_setup_worst);
  $hold(posedge CLK_A &&& condition2_a, posedge T_A_A[3] &&& condition2_a, t_T_A_A_hold_worst);
  $hold(posedge CLK_A &&& condition2_a, negedge T_A_A[3] &&& condition2_a, t_T_A_A_hold_worst);
  $setup(posedge T_A_A[4] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_A_A_setup_worst);
  $setup(negedge T_A_A[4] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_A_A_setup_worst);
  $hold(posedge CLK_A &&& condition2_a, posedge T_A_A[4] &&& condition2_a, t_T_A_A_hold_worst);
  $hold(posedge CLK_A &&& condition2_a, negedge T_A_A[4] &&& condition2_a, t_T_A_A_hold_worst);
  $setup(posedge T_A_A[5] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_A_A_setup_worst);
  $setup(negedge T_A_A[5] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_A_A_setup_worst);
  $hold(posedge CLK_A &&& condition2_a, posedge T_A_A[5] &&& condition2_a, t_T_A_A_hold_worst);
  $hold(posedge CLK_A &&& condition2_a, negedge T_A_A[5] &&& condition2_a, t_T_A_A_hold_worst);
  $setup(posedge T_A_A[6] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_A_A_setup_worst);
  $setup(negedge T_A_A[6] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_A_A_setup_worst);
  $hold(posedge CLK_A &&& condition2_a, posedge T_A_A[6] &&& condition2_a, t_T_A_A_hold_worst);
  $hold(posedge CLK_A &&& condition2_a, negedge T_A_A[6] &&& condition2_a, t_T_A_A_hold_worst);
  $setup(posedge T_A_A[7] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_A_A_setup_worst);
  $setup(negedge T_A_A[7] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_A_A_setup_worst);
  $hold(posedge CLK_A &&& condition2_a, posedge T_A_A[7] &&& condition2_a, t_T_A_A_hold_worst);
  $hold(posedge CLK_A &&& condition2_a, negedge T_A_A[7] &&& condition2_a, t_T_A_A_hold_worst);
  $setup(posedge T_A_A[8] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_A_A_setup_worst);
  $setup(negedge T_A_A[8] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_A_A_setup_worst);
  $hold(posedge CLK_A &&& condition2_a, posedge T_A_A[8] &&& condition2_a, t_T_A_A_hold_worst);
  $hold(posedge CLK_A &&& condition2_a, negedge T_A_A[8] &&& condition2_a, t_T_A_A_hold_worst);
  $setup(posedge T_A_A[9] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_A_A_setup_worst);
  $setup(negedge T_A_A[9] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_A_A_setup_worst);
  $hold(posedge CLK_A &&& condition2_a, posedge T_A_A[9] &&& condition2_a, t_T_A_A_hold_worst);
  $hold(posedge CLK_A &&& condition2_a, negedge T_A_A[9] &&& condition2_a, t_T_A_A_hold_worst);
  $setup(posedge T_A_B[0] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_A_B_setup_worst);
  $setup(negedge T_A_B[0] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_A_B_setup_worst);
  $hold(posedge CLK_B &&& condition2_b, posedge T_A_B[0] &&& condition2_b, t_T_A_B_hold_worst);
  $hold(posedge CLK_B &&& condition2_b, negedge T_A_B[0] &&& condition2_b, t_T_A_B_hold_worst);
  $setup(posedge T_A_B[1] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_A_B_setup_worst);
  $setup(negedge T_A_B[1] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_A_B_setup_worst);
  $hold(posedge CLK_B &&& condition2_b, posedge T_A_B[1] &&& condition2_b, t_T_A_B_hold_worst);
  $hold(posedge CLK_B &&& condition2_b, negedge T_A_B[1] &&& condition2_b, t_T_A_B_hold_worst);
  $setup(posedge T_A_B[2] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_A_B_setup_worst);
  $setup(negedge T_A_B[2] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_A_B_setup_worst);
  $hold(posedge CLK_B &&& condition2_b, posedge T_A_B[2] &&& condition2_b, t_T_A_B_hold_worst);
  $hold(posedge CLK_B &&& condition2_b, negedge T_A_B[2] &&& condition2_b, t_T_A_B_hold_worst);
  $setup(posedge T_A_B[3] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_A_B_setup_worst);
  $setup(negedge T_A_B[3] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_A_B_setup_worst);
  $hold(posedge CLK_B &&& condition2_b, posedge T_A_B[3] &&& condition2_b, t_T_A_B_hold_worst);
  $hold(posedge CLK_B &&& condition2_b, negedge T_A_B[3] &&& condition2_b, t_T_A_B_hold_worst);
  $setup(posedge T_A_B[4] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_A_B_setup_worst);
  $setup(negedge T_A_B[4] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_A_B_setup_worst);
  $hold(posedge CLK_B &&& condition2_b, posedge T_A_B[4] &&& condition2_b, t_T_A_B_hold_worst);
  $hold(posedge CLK_B &&& condition2_b, negedge T_A_B[4] &&& condition2_b, t_T_A_B_hold_worst);
  $setup(posedge T_A_B[5] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_A_B_setup_worst);
  $setup(negedge T_A_B[5] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_A_B_setup_worst);
  $hold(posedge CLK_B &&& condition2_b, posedge T_A_B[5] &&& condition2_b, t_T_A_B_hold_worst);
  $hold(posedge CLK_B &&& condition2_b, negedge T_A_B[5] &&& condition2_b, t_T_A_B_hold_worst);
  $setup(posedge T_A_B[6] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_A_B_setup_worst);
  $setup(negedge T_A_B[6] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_A_B_setup_worst);
  $hold(posedge CLK_B &&& condition2_b, posedge T_A_B[6] &&& condition2_b, t_T_A_B_hold_worst);
  $hold(posedge CLK_B &&& condition2_b, negedge T_A_B[6] &&& condition2_b, t_T_A_B_hold_worst);
  $setup(posedge T_A_B[7] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_A_B_setup_worst);
  $setup(negedge T_A_B[7] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_A_B_setup_worst);
  $hold(posedge CLK_B &&& condition2_b, posedge T_A_B[7] &&& condition2_b, t_T_A_B_hold_worst);
  $hold(posedge CLK_B &&& condition2_b, negedge T_A_B[7] &&& condition2_b, t_T_A_B_hold_worst);
  $setup(posedge T_A_B[8] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_A_B_setup_worst);
  $setup(negedge T_A_B[8] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_A_B_setup_worst);
  $hold(posedge CLK_B &&& condition2_b, posedge T_A_B[8] &&& condition2_b, t_T_A_B_hold_worst);
  $hold(posedge CLK_B &&& condition2_b, negedge T_A_B[8] &&& condition2_b, t_T_A_B_hold_worst);
  $setup(posedge T_A_B[9] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_A_B_setup_worst);
  $setup(negedge T_A_B[9] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_A_B_setup_worst);
  $hold(posedge CLK_B &&& condition2_b, posedge T_A_B[9] &&& condition2_b, t_T_A_B_hold_worst);
  $hold(posedge CLK_B &&& condition2_b, negedge T_A_B[9] &&& condition2_b, t_T_A_B_hold_worst);
  $setup(posedge CE_N_A &&& T_BE_N, posedge CLK_A &&& T_BE_N, t_CE_N_A_setup_worst);
  $setup(negedge CE_N_A &&& T_BE_N, posedge CLK_A &&& T_BE_N, t_CE_N_A_setup_worst);
  $hold(posedge CLK_A &&& T_BE_N, posedge CE_N_A &&& T_BE_N, t_CE_N_A_hold_worst);
  $hold(posedge CLK_A &&& T_BE_N, negedge CE_N_A &&& T_BE_N, t_CE_N_A_hold_worst);
  $setup(posedge CE_N_B &&& T_BE_N, posedge CLK_B &&& T_BE_N, t_CE_N_B_setup_worst);
  $setup(negedge CE_N_B &&& T_BE_N, posedge CLK_B &&& T_BE_N, t_CE_N_B_setup_worst);
  $hold(posedge CLK_B &&& T_BE_N, posedge CE_N_B &&& T_BE_N, t_CE_N_B_hold_worst);
  $hold(posedge CLK_B &&& T_BE_N, negedge CE_N_B &&& T_BE_N, t_CE_N_B_hold_worst);
  $setup(posedge T_CE_N_A &&& condition4_a, posedge CLK_A &&& condition4_a, t_T_CE_N_A_setup_worst);
  $setup(negedge T_CE_N_A &&& condition4_a, posedge CLK_A &&& condition4_a, t_T_CE_N_A_setup_worst);
  $hold(posedge CLK_A &&& condition4_a, posedge T_CE_N_A &&& condition4_a, t_T_CE_N_A_hold_worst);
  $hold(posedge CLK_A &&& condition4_a, negedge T_CE_N_A &&& condition4_a, t_T_CE_N_A_hold_worst);
  $setup(posedge T_CE_N_B &&& condition4_b, posedge CLK_B &&& condition4_b, t_T_CE_N_B_setup_worst);
  $setup(negedge T_CE_N_B &&& condition4_b, posedge CLK_B &&& condition4_b, t_T_CE_N_B_setup_worst);
  $hold(posedge CLK_B &&& condition4_b, posedge T_CE_N_B &&& condition4_b, t_T_CE_N_B_hold_worst);
  $hold(posedge CLK_B &&& condition4_b, negedge T_CE_N_B &&& condition4_b, t_T_CE_N_B_hold_worst);
  $setup(posedge GWE_N_A &&& condition3_a, posedge CLK_A &&& condition3_a, t_GWE_N_A_setup_worst);
  $setup(negedge GWE_N_A &&& condition3_a, posedge CLK_A &&& condition3_a, t_GWE_N_A_setup_worst);
  $hold(posedge CLK_A &&& condition3_a, posedge GWE_N_A &&& condition3_a, t_GWE_N_A_hold_worst);
  $hold(posedge CLK_A &&& condition3_a, negedge GWE_N_A &&& condition3_a, t_GWE_N_A_hold_worst);
  $setup(posedge GWE_N_B &&& condition3_b, posedge CLK_B &&& condition3_b, t_GWE_N_B_setup_worst);
  $setup(negedge GWE_N_B &&& condition3_b, posedge CLK_B &&& condition3_b, t_GWE_N_B_setup_worst);
  $hold(posedge CLK_B &&& condition3_b, posedge GWE_N_B &&& condition3_b, t_GWE_N_B_hold_worst);
  $hold(posedge CLK_B &&& condition3_b, negedge GWE_N_B &&& condition3_b, t_GWE_N_B_hold_worst);
  $setup(posedge T_GWE_N_A &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_GWE_N_A_setup_worst);
  $setup(negedge T_GWE_N_A &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_GWE_N_A_setup_worst);
  $hold(posedge CLK_A &&& condition2_a, posedge T_GWE_N_A &&& condition2_a, t_T_GWE_N_A_hold_worst);
  $hold(posedge CLK_A &&& condition2_a, negedge T_GWE_N_A &&& condition2_a, t_T_GWE_N_A_hold_worst);
  $setup(posedge T_GWE_N_B &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_GWE_N_B_setup_worst);
  $setup(negedge T_GWE_N_B &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_GWE_N_B_setup_worst);
  $hold(posedge CLK_B &&& condition2_b, posedge T_GWE_N_B &&& condition2_b, t_T_GWE_N_B_hold_worst);
  $hold(posedge CLK_B &&& condition2_b, negedge T_GWE_N_B &&& condition2_b, t_T_GWE_N_B_hold_worst);
  $setup(posedge BWE_N_A[0] &&& condition3_a, posedge CLK_A &&& condition3_a, t_BWE_N_A_setup_worst);
  $setup(negedge BWE_N_A[0] &&& condition3_a, posedge CLK_A &&& condition3_a, t_BWE_N_A_setup_worst);
  $hold(posedge CLK_A &&& condition3_a, posedge BWE_N_A[0] &&& condition3_a, t_BWE_N_A_hold_worst);
  $hold(posedge CLK_A &&& condition3_a, negedge BWE_N_A[0] &&& condition3_a, t_BWE_N_A_hold_worst);
  $setup(posedge BWE_N_A[1] &&& condition3_a, posedge CLK_A &&& condition3_a, t_BWE_N_A_setup_worst);
  $setup(negedge BWE_N_A[1] &&& condition3_a, posedge CLK_A &&& condition3_a, t_BWE_N_A_setup_worst);
  $hold(posedge CLK_A &&& condition3_a, posedge BWE_N_A[1] &&& condition3_a, t_BWE_N_A_hold_worst);
  $hold(posedge CLK_A &&& condition3_a, negedge BWE_N_A[1] &&& condition3_a, t_BWE_N_A_hold_worst);
  $setup(posedge BWE_N_A[2] &&& condition3_a, posedge CLK_A &&& condition3_a, t_BWE_N_A_setup_worst);
  $setup(negedge BWE_N_A[2] &&& condition3_a, posedge CLK_A &&& condition3_a, t_BWE_N_A_setup_worst);
  $hold(posedge CLK_A &&& condition3_a, posedge BWE_N_A[2] &&& condition3_a, t_BWE_N_A_hold_worst);
  $hold(posedge CLK_A &&& condition3_a, negedge BWE_N_A[2] &&& condition3_a, t_BWE_N_A_hold_worst);
  $setup(posedge BWE_N_A[3] &&& condition3_a, posedge CLK_A &&& condition3_a, t_BWE_N_A_setup_worst);
  $setup(negedge BWE_N_A[3] &&& condition3_a, posedge CLK_A &&& condition3_a, t_BWE_N_A_setup_worst);
  $hold(posedge CLK_A &&& condition3_a, posedge BWE_N_A[3] &&& condition3_a, t_BWE_N_A_hold_worst);
  $hold(posedge CLK_A &&& condition3_a, negedge BWE_N_A[3] &&& condition3_a, t_BWE_N_A_hold_worst);
  $setup(posedge BWE_N_A[4] &&& condition3_a, posedge CLK_A &&& condition3_a, t_BWE_N_A_setup_worst);
  $setup(negedge BWE_N_A[4] &&& condition3_a, posedge CLK_A &&& condition3_a, t_BWE_N_A_setup_worst);
  $hold(posedge CLK_A &&& condition3_a, posedge BWE_N_A[4] &&& condition3_a, t_BWE_N_A_hold_worst);
  $hold(posedge CLK_A &&& condition3_a, negedge BWE_N_A[4] &&& condition3_a, t_BWE_N_A_hold_worst);
  $setup(posedge BWE_N_A[5] &&& condition3_a, posedge CLK_A &&& condition3_a, t_BWE_N_A_setup_worst);
  $setup(negedge BWE_N_A[5] &&& condition3_a, posedge CLK_A &&& condition3_a, t_BWE_N_A_setup_worst);
  $hold(posedge CLK_A &&& condition3_a, posedge BWE_N_A[5] &&& condition3_a, t_BWE_N_A_hold_worst);
  $hold(posedge CLK_A &&& condition3_a, negedge BWE_N_A[5] &&& condition3_a, t_BWE_N_A_hold_worst);
  $setup(posedge BWE_N_A[6] &&& condition3_a, posedge CLK_A &&& condition3_a, t_BWE_N_A_setup_worst);
  $setup(negedge BWE_N_A[6] &&& condition3_a, posedge CLK_A &&& condition3_a, t_BWE_N_A_setup_worst);
  $hold(posedge CLK_A &&& condition3_a, posedge BWE_N_A[6] &&& condition3_a, t_BWE_N_A_hold_worst);
  $hold(posedge CLK_A &&& condition3_a, negedge BWE_N_A[6] &&& condition3_a, t_BWE_N_A_hold_worst);
  $setup(posedge BWE_N_A[7] &&& condition3_a, posedge CLK_A &&& condition3_a, t_BWE_N_A_setup_worst);
  $setup(negedge BWE_N_A[7] &&& condition3_a, posedge CLK_A &&& condition3_a, t_BWE_N_A_setup_worst);
  $hold(posedge CLK_A &&& condition3_a, posedge BWE_N_A[7] &&& condition3_a, t_BWE_N_A_hold_worst);
  $hold(posedge CLK_A &&& condition3_a, negedge BWE_N_A[7] &&& condition3_a, t_BWE_N_A_hold_worst);
  $setup(posedge BWE_N_A[8] &&& condition3_a, posedge CLK_A &&& condition3_a, t_BWE_N_A_setup_worst);
  $setup(negedge BWE_N_A[8] &&& condition3_a, posedge CLK_A &&& condition3_a, t_BWE_N_A_setup_worst);
  $hold(posedge CLK_A &&& condition3_a, posedge BWE_N_A[8] &&& condition3_a, t_BWE_N_A_hold_worst);
  $hold(posedge CLK_A &&& condition3_a, negedge BWE_N_A[8] &&& condition3_a, t_BWE_N_A_hold_worst);
  $setup(posedge BWE_N_A[9] &&& condition3_a, posedge CLK_A &&& condition3_a, t_BWE_N_A_setup_worst);
  $setup(negedge BWE_N_A[9] &&& condition3_a, posedge CLK_A &&& condition3_a, t_BWE_N_A_setup_worst);
  $hold(posedge CLK_A &&& condition3_a, posedge BWE_N_A[9] &&& condition3_a, t_BWE_N_A_hold_worst);
  $hold(posedge CLK_A &&& condition3_a, negedge BWE_N_A[9] &&& condition3_a, t_BWE_N_A_hold_worst);
  $setup(posedge BWE_N_A[10] &&& condition3_a, posedge CLK_A &&& condition3_a, t_BWE_N_A_setup_worst);
  $setup(negedge BWE_N_A[10] &&& condition3_a, posedge CLK_A &&& condition3_a, t_BWE_N_A_setup_worst);
  $hold(posedge CLK_A &&& condition3_a, posedge BWE_N_A[10] &&& condition3_a, t_BWE_N_A_hold_worst);
  $hold(posedge CLK_A &&& condition3_a, negedge BWE_N_A[10] &&& condition3_a, t_BWE_N_A_hold_worst);
  $setup(posedge BWE_N_A[11] &&& condition3_a, posedge CLK_A &&& condition3_a, t_BWE_N_A_setup_worst);
  $setup(negedge BWE_N_A[11] &&& condition3_a, posedge CLK_A &&& condition3_a, t_BWE_N_A_setup_worst);
  $hold(posedge CLK_A &&& condition3_a, posedge BWE_N_A[11] &&& condition3_a, t_BWE_N_A_hold_worst);
  $hold(posedge CLK_A &&& condition3_a, negedge BWE_N_A[11] &&& condition3_a, t_BWE_N_A_hold_worst);
  $setup(posedge BWE_N_A[12] &&& condition3_a, posedge CLK_A &&& condition3_a, t_BWE_N_A_setup_worst);
  $setup(negedge BWE_N_A[12] &&& condition3_a, posedge CLK_A &&& condition3_a, t_BWE_N_A_setup_worst);
  $hold(posedge CLK_A &&& condition3_a, posedge BWE_N_A[12] &&& condition3_a, t_BWE_N_A_hold_worst);
  $hold(posedge CLK_A &&& condition3_a, negedge BWE_N_A[12] &&& condition3_a, t_BWE_N_A_hold_worst);
  $setup(posedge BWE_N_A[13] &&& condition3_a, posedge CLK_A &&& condition3_a, t_BWE_N_A_setup_worst);
  $setup(negedge BWE_N_A[13] &&& condition3_a, posedge CLK_A &&& condition3_a, t_BWE_N_A_setup_worst);
  $hold(posedge CLK_A &&& condition3_a, posedge BWE_N_A[13] &&& condition3_a, t_BWE_N_A_hold_worst);
  $hold(posedge CLK_A &&& condition3_a, negedge BWE_N_A[13] &&& condition3_a, t_BWE_N_A_hold_worst);
  $setup(posedge BWE_N_A[14] &&& condition3_a, posedge CLK_A &&& condition3_a, t_BWE_N_A_setup_worst);
  $setup(negedge BWE_N_A[14] &&& condition3_a, posedge CLK_A &&& condition3_a, t_BWE_N_A_setup_worst);
  $hold(posedge CLK_A &&& condition3_a, posedge BWE_N_A[14] &&& condition3_a, t_BWE_N_A_hold_worst);
  $hold(posedge CLK_A &&& condition3_a, negedge BWE_N_A[14] &&& condition3_a, t_BWE_N_A_hold_worst);
  $setup(posedge BWE_N_A[15] &&& condition3_a, posedge CLK_A &&& condition3_a, t_BWE_N_A_setup_worst);
  $setup(negedge BWE_N_A[15] &&& condition3_a, posedge CLK_A &&& condition3_a, t_BWE_N_A_setup_worst);
  $hold(posedge CLK_A &&& condition3_a, posedge BWE_N_A[15] &&& condition3_a, t_BWE_N_A_hold_worst);
  $hold(posedge CLK_A &&& condition3_a, negedge BWE_N_A[15] &&& condition3_a, t_BWE_N_A_hold_worst);
  $setup(posedge BWE_N_A[16] &&& condition3_a, posedge CLK_A &&& condition3_a, t_BWE_N_A_setup_worst);
  $setup(negedge BWE_N_A[16] &&& condition3_a, posedge CLK_A &&& condition3_a, t_BWE_N_A_setup_worst);
  $hold(posedge CLK_A &&& condition3_a, posedge BWE_N_A[16] &&& condition3_a, t_BWE_N_A_hold_worst);
  $hold(posedge CLK_A &&& condition3_a, negedge BWE_N_A[16] &&& condition3_a, t_BWE_N_A_hold_worst);
  $setup(posedge BWE_N_A[17] &&& condition3_a, posedge CLK_A &&& condition3_a, t_BWE_N_A_setup_worst);
  $setup(negedge BWE_N_A[17] &&& condition3_a, posedge CLK_A &&& condition3_a, t_BWE_N_A_setup_worst);
  $hold(posedge CLK_A &&& condition3_a, posedge BWE_N_A[17] &&& condition3_a, t_BWE_N_A_hold_worst);
  $hold(posedge CLK_A &&& condition3_a, negedge BWE_N_A[17] &&& condition3_a, t_BWE_N_A_hold_worst);
  $setup(posedge BWE_N_B[0] &&& condition3_b, posedge CLK_B &&& condition3_b, t_BWE_N_B_setup_worst);
  $setup(negedge BWE_N_B[0] &&& condition3_b, posedge CLK_B &&& condition3_b, t_BWE_N_B_setup_worst);
  $hold(posedge CLK_B &&& condition3_b, posedge BWE_N_B[0] &&& condition3_b, t_BWE_N_B_hold_worst);
  $hold(posedge CLK_B &&& condition3_b, negedge BWE_N_B[0] &&& condition3_b, t_BWE_N_B_hold_worst);
  $setup(posedge BWE_N_B[1] &&& condition3_b, posedge CLK_B &&& condition3_b, t_BWE_N_B_setup_worst);
  $setup(negedge BWE_N_B[1] &&& condition3_b, posedge CLK_B &&& condition3_b, t_BWE_N_B_setup_worst);
  $hold(posedge CLK_B &&& condition3_b, posedge BWE_N_B[1] &&& condition3_b, t_BWE_N_B_hold_worst);
  $hold(posedge CLK_B &&& condition3_b, negedge BWE_N_B[1] &&& condition3_b, t_BWE_N_B_hold_worst);
  $setup(posedge BWE_N_B[2] &&& condition3_b, posedge CLK_B &&& condition3_b, t_BWE_N_B_setup_worst);
  $setup(negedge BWE_N_B[2] &&& condition3_b, posedge CLK_B &&& condition3_b, t_BWE_N_B_setup_worst);
  $hold(posedge CLK_B &&& condition3_b, posedge BWE_N_B[2] &&& condition3_b, t_BWE_N_B_hold_worst);
  $hold(posedge CLK_B &&& condition3_b, negedge BWE_N_B[2] &&& condition3_b, t_BWE_N_B_hold_worst);
  $setup(posedge BWE_N_B[3] &&& condition3_b, posedge CLK_B &&& condition3_b, t_BWE_N_B_setup_worst);
  $setup(negedge BWE_N_B[3] &&& condition3_b, posedge CLK_B &&& condition3_b, t_BWE_N_B_setup_worst);
  $hold(posedge CLK_B &&& condition3_b, posedge BWE_N_B[3] &&& condition3_b, t_BWE_N_B_hold_worst);
  $hold(posedge CLK_B &&& condition3_b, negedge BWE_N_B[3] &&& condition3_b, t_BWE_N_B_hold_worst);
  $setup(posedge BWE_N_B[4] &&& condition3_b, posedge CLK_B &&& condition3_b, t_BWE_N_B_setup_worst);
  $setup(negedge BWE_N_B[4] &&& condition3_b, posedge CLK_B &&& condition3_b, t_BWE_N_B_setup_worst);
  $hold(posedge CLK_B &&& condition3_b, posedge BWE_N_B[4] &&& condition3_b, t_BWE_N_B_hold_worst);
  $hold(posedge CLK_B &&& condition3_b, negedge BWE_N_B[4] &&& condition3_b, t_BWE_N_B_hold_worst);
  $setup(posedge BWE_N_B[5] &&& condition3_b, posedge CLK_B &&& condition3_b, t_BWE_N_B_setup_worst);
  $setup(negedge BWE_N_B[5] &&& condition3_b, posedge CLK_B &&& condition3_b, t_BWE_N_B_setup_worst);
  $hold(posedge CLK_B &&& condition3_b, posedge BWE_N_B[5] &&& condition3_b, t_BWE_N_B_hold_worst);
  $hold(posedge CLK_B &&& condition3_b, negedge BWE_N_B[5] &&& condition3_b, t_BWE_N_B_hold_worst);
  $setup(posedge BWE_N_B[6] &&& condition3_b, posedge CLK_B &&& condition3_b, t_BWE_N_B_setup_worst);
  $setup(negedge BWE_N_B[6] &&& condition3_b, posedge CLK_B &&& condition3_b, t_BWE_N_B_setup_worst);
  $hold(posedge CLK_B &&& condition3_b, posedge BWE_N_B[6] &&& condition3_b, t_BWE_N_B_hold_worst);
  $hold(posedge CLK_B &&& condition3_b, negedge BWE_N_B[6] &&& condition3_b, t_BWE_N_B_hold_worst);
  $setup(posedge BWE_N_B[7] &&& condition3_b, posedge CLK_B &&& condition3_b, t_BWE_N_B_setup_worst);
  $setup(negedge BWE_N_B[7] &&& condition3_b, posedge CLK_B &&& condition3_b, t_BWE_N_B_setup_worst);
  $hold(posedge CLK_B &&& condition3_b, posedge BWE_N_B[7] &&& condition3_b, t_BWE_N_B_hold_worst);
  $hold(posedge CLK_B &&& condition3_b, negedge BWE_N_B[7] &&& condition3_b, t_BWE_N_B_hold_worst);
  $setup(posedge BWE_N_B[8] &&& condition3_b, posedge CLK_B &&& condition3_b, t_BWE_N_B_setup_worst);
  $setup(negedge BWE_N_B[8] &&& condition3_b, posedge CLK_B &&& condition3_b, t_BWE_N_B_setup_worst);
  $hold(posedge CLK_B &&& condition3_b, posedge BWE_N_B[8] &&& condition3_b, t_BWE_N_B_hold_worst);
  $hold(posedge CLK_B &&& condition3_b, negedge BWE_N_B[8] &&& condition3_b, t_BWE_N_B_hold_worst);
  $setup(posedge BWE_N_B[9] &&& condition3_b, posedge CLK_B &&& condition3_b, t_BWE_N_B_setup_worst);
  $setup(negedge BWE_N_B[9] &&& condition3_b, posedge CLK_B &&& condition3_b, t_BWE_N_B_setup_worst);
  $hold(posedge CLK_B &&& condition3_b, posedge BWE_N_B[9] &&& condition3_b, t_BWE_N_B_hold_worst);
  $hold(posedge CLK_B &&& condition3_b, negedge BWE_N_B[9] &&& condition3_b, t_BWE_N_B_hold_worst);
  $setup(posedge BWE_N_B[10] &&& condition3_b, posedge CLK_B &&& condition3_b, t_BWE_N_B_setup_worst);
  $setup(negedge BWE_N_B[10] &&& condition3_b, posedge CLK_B &&& condition3_b, t_BWE_N_B_setup_worst);
  $hold(posedge CLK_B &&& condition3_b, posedge BWE_N_B[10] &&& condition3_b, t_BWE_N_B_hold_worst);
  $hold(posedge CLK_B &&& condition3_b, negedge BWE_N_B[10] &&& condition3_b, t_BWE_N_B_hold_worst);
  $setup(posedge BWE_N_B[11] &&& condition3_b, posedge CLK_B &&& condition3_b, t_BWE_N_B_setup_worst);
  $setup(negedge BWE_N_B[11] &&& condition3_b, posedge CLK_B &&& condition3_b, t_BWE_N_B_setup_worst);
  $hold(posedge CLK_B &&& condition3_b, posedge BWE_N_B[11] &&& condition3_b, t_BWE_N_B_hold_worst);
  $hold(posedge CLK_B &&& condition3_b, negedge BWE_N_B[11] &&& condition3_b, t_BWE_N_B_hold_worst);
  $setup(posedge BWE_N_B[12] &&& condition3_b, posedge CLK_B &&& condition3_b, t_BWE_N_B_setup_worst);
  $setup(negedge BWE_N_B[12] &&& condition3_b, posedge CLK_B &&& condition3_b, t_BWE_N_B_setup_worst);
  $hold(posedge CLK_B &&& condition3_b, posedge BWE_N_B[12] &&& condition3_b, t_BWE_N_B_hold_worst);
  $hold(posedge CLK_B &&& condition3_b, negedge BWE_N_B[12] &&& condition3_b, t_BWE_N_B_hold_worst);
  $setup(posedge BWE_N_B[13] &&& condition3_b, posedge CLK_B &&& condition3_b, t_BWE_N_B_setup_worst);
  $setup(negedge BWE_N_B[13] &&& condition3_b, posedge CLK_B &&& condition3_b, t_BWE_N_B_setup_worst);
  $hold(posedge CLK_B &&& condition3_b, posedge BWE_N_B[13] &&& condition3_b, t_BWE_N_B_hold_worst);
  $hold(posedge CLK_B &&& condition3_b, negedge BWE_N_B[13] &&& condition3_b, t_BWE_N_B_hold_worst);
  $setup(posedge BWE_N_B[14] &&& condition3_b, posedge CLK_B &&& condition3_b, t_BWE_N_B_setup_worst);
  $setup(negedge BWE_N_B[14] &&& condition3_b, posedge CLK_B &&& condition3_b, t_BWE_N_B_setup_worst);
  $hold(posedge CLK_B &&& condition3_b, posedge BWE_N_B[14] &&& condition3_b, t_BWE_N_B_hold_worst);
  $hold(posedge CLK_B &&& condition3_b, negedge BWE_N_B[14] &&& condition3_b, t_BWE_N_B_hold_worst);
  $setup(posedge BWE_N_B[15] &&& condition3_b, posedge CLK_B &&& condition3_b, t_BWE_N_B_setup_worst);
  $setup(negedge BWE_N_B[15] &&& condition3_b, posedge CLK_B &&& condition3_b, t_BWE_N_B_setup_worst);
  $hold(posedge CLK_B &&& condition3_b, posedge BWE_N_B[15] &&& condition3_b, t_BWE_N_B_hold_worst);
  $hold(posedge CLK_B &&& condition3_b, negedge BWE_N_B[15] &&& condition3_b, t_BWE_N_B_hold_worst);
  $setup(posedge BWE_N_B[16] &&& condition3_b, posedge CLK_B &&& condition3_b, t_BWE_N_B_setup_worst);
  $setup(negedge BWE_N_B[16] &&& condition3_b, posedge CLK_B &&& condition3_b, t_BWE_N_B_setup_worst);
  $hold(posedge CLK_B &&& condition3_b, posedge BWE_N_B[16] &&& condition3_b, t_BWE_N_B_hold_worst);
  $hold(posedge CLK_B &&& condition3_b, negedge BWE_N_B[16] &&& condition3_b, t_BWE_N_B_hold_worst);
  $setup(posedge BWE_N_B[17] &&& condition3_b, posedge CLK_B &&& condition3_b, t_BWE_N_B_setup_worst);
  $setup(negedge BWE_N_B[17] &&& condition3_b, posedge CLK_B &&& condition3_b, t_BWE_N_B_setup_worst);
  $hold(posedge CLK_B &&& condition3_b, posedge BWE_N_B[17] &&& condition3_b, t_BWE_N_B_hold_worst);
  $hold(posedge CLK_B &&& condition3_b, negedge BWE_N_B[17] &&& condition3_b, t_BWE_N_B_hold_worst);
  $setup(posedge T_BWE_N_A[0] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_BWE_N_A_setup_worst);
  $setup(negedge T_BWE_N_A[0] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_BWE_N_A_setup_worst);
  $hold(posedge CLK_A &&& condition2_a, posedge T_BWE_N_A[0] &&& condition2_a, t_T_BWE_N_A_hold_worst);
  $hold(posedge CLK_A &&& condition2_a, negedge T_BWE_N_A[0] &&& condition2_a, t_T_BWE_N_A_hold_worst);
  $setup(posedge T_BWE_N_A[1] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_BWE_N_A_setup_worst);
  $setup(negedge T_BWE_N_A[1] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_BWE_N_A_setup_worst);
  $hold(posedge CLK_A &&& condition2_a, posedge T_BWE_N_A[1] &&& condition2_a, t_T_BWE_N_A_hold_worst);
  $hold(posedge CLK_A &&& condition2_a, negedge T_BWE_N_A[1] &&& condition2_a, t_T_BWE_N_A_hold_worst);
  $setup(posedge T_BWE_N_A[2] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_BWE_N_A_setup_worst);
  $setup(negedge T_BWE_N_A[2] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_BWE_N_A_setup_worst);
  $hold(posedge CLK_A &&& condition2_a, posedge T_BWE_N_A[2] &&& condition2_a, t_T_BWE_N_A_hold_worst);
  $hold(posedge CLK_A &&& condition2_a, negedge T_BWE_N_A[2] &&& condition2_a, t_T_BWE_N_A_hold_worst);
  $setup(posedge T_BWE_N_A[3] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_BWE_N_A_setup_worst);
  $setup(negedge T_BWE_N_A[3] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_BWE_N_A_setup_worst);
  $hold(posedge CLK_A &&& condition2_a, posedge T_BWE_N_A[3] &&& condition2_a, t_T_BWE_N_A_hold_worst);
  $hold(posedge CLK_A &&& condition2_a, negedge T_BWE_N_A[3] &&& condition2_a, t_T_BWE_N_A_hold_worst);
  $setup(posedge T_BWE_N_A[4] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_BWE_N_A_setup_worst);
  $setup(negedge T_BWE_N_A[4] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_BWE_N_A_setup_worst);
  $hold(posedge CLK_A &&& condition2_a, posedge T_BWE_N_A[4] &&& condition2_a, t_T_BWE_N_A_hold_worst);
  $hold(posedge CLK_A &&& condition2_a, negedge T_BWE_N_A[4] &&& condition2_a, t_T_BWE_N_A_hold_worst);
  $setup(posedge T_BWE_N_A[5] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_BWE_N_A_setup_worst);
  $setup(negedge T_BWE_N_A[5] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_BWE_N_A_setup_worst);
  $hold(posedge CLK_A &&& condition2_a, posedge T_BWE_N_A[5] &&& condition2_a, t_T_BWE_N_A_hold_worst);
  $hold(posedge CLK_A &&& condition2_a, negedge T_BWE_N_A[5] &&& condition2_a, t_T_BWE_N_A_hold_worst);
  $setup(posedge T_BWE_N_A[6] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_BWE_N_A_setup_worst);
  $setup(negedge T_BWE_N_A[6] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_BWE_N_A_setup_worst);
  $hold(posedge CLK_A &&& condition2_a, posedge T_BWE_N_A[6] &&& condition2_a, t_T_BWE_N_A_hold_worst);
  $hold(posedge CLK_A &&& condition2_a, negedge T_BWE_N_A[6] &&& condition2_a, t_T_BWE_N_A_hold_worst);
  $setup(posedge T_BWE_N_A[7] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_BWE_N_A_setup_worst);
  $setup(negedge T_BWE_N_A[7] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_BWE_N_A_setup_worst);
  $hold(posedge CLK_A &&& condition2_a, posedge T_BWE_N_A[7] &&& condition2_a, t_T_BWE_N_A_hold_worst);
  $hold(posedge CLK_A &&& condition2_a, negedge T_BWE_N_A[7] &&& condition2_a, t_T_BWE_N_A_hold_worst);
  $setup(posedge T_BWE_N_A[8] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_BWE_N_A_setup_worst);
  $setup(negedge T_BWE_N_A[8] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_BWE_N_A_setup_worst);
  $hold(posedge CLK_A &&& condition2_a, posedge T_BWE_N_A[8] &&& condition2_a, t_T_BWE_N_A_hold_worst);
  $hold(posedge CLK_A &&& condition2_a, negedge T_BWE_N_A[8] &&& condition2_a, t_T_BWE_N_A_hold_worst);
  $setup(posedge T_BWE_N_A[9] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_BWE_N_A_setup_worst);
  $setup(negedge T_BWE_N_A[9] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_BWE_N_A_setup_worst);
  $hold(posedge CLK_A &&& condition2_a, posedge T_BWE_N_A[9] &&& condition2_a, t_T_BWE_N_A_hold_worst);
  $hold(posedge CLK_A &&& condition2_a, negedge T_BWE_N_A[9] &&& condition2_a, t_T_BWE_N_A_hold_worst);
  $setup(posedge T_BWE_N_A[10] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_BWE_N_A_setup_worst);
  $setup(negedge T_BWE_N_A[10] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_BWE_N_A_setup_worst);
  $hold(posedge CLK_A &&& condition2_a, posedge T_BWE_N_A[10] &&& condition2_a, t_T_BWE_N_A_hold_worst);
  $hold(posedge CLK_A &&& condition2_a, negedge T_BWE_N_A[10] &&& condition2_a, t_T_BWE_N_A_hold_worst);
  $setup(posedge T_BWE_N_A[11] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_BWE_N_A_setup_worst);
  $setup(negedge T_BWE_N_A[11] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_BWE_N_A_setup_worst);
  $hold(posedge CLK_A &&& condition2_a, posedge T_BWE_N_A[11] &&& condition2_a, t_T_BWE_N_A_hold_worst);
  $hold(posedge CLK_A &&& condition2_a, negedge T_BWE_N_A[11] &&& condition2_a, t_T_BWE_N_A_hold_worst);
  $setup(posedge T_BWE_N_A[12] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_BWE_N_A_setup_worst);
  $setup(negedge T_BWE_N_A[12] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_BWE_N_A_setup_worst);
  $hold(posedge CLK_A &&& condition2_a, posedge T_BWE_N_A[12] &&& condition2_a, t_T_BWE_N_A_hold_worst);
  $hold(posedge CLK_A &&& condition2_a, negedge T_BWE_N_A[12] &&& condition2_a, t_T_BWE_N_A_hold_worst);
  $setup(posedge T_BWE_N_A[13] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_BWE_N_A_setup_worst);
  $setup(negedge T_BWE_N_A[13] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_BWE_N_A_setup_worst);
  $hold(posedge CLK_A &&& condition2_a, posedge T_BWE_N_A[13] &&& condition2_a, t_T_BWE_N_A_hold_worst);
  $hold(posedge CLK_A &&& condition2_a, negedge T_BWE_N_A[13] &&& condition2_a, t_T_BWE_N_A_hold_worst);
  $setup(posedge T_BWE_N_A[14] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_BWE_N_A_setup_worst);
  $setup(negedge T_BWE_N_A[14] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_BWE_N_A_setup_worst);
  $hold(posedge CLK_A &&& condition2_a, posedge T_BWE_N_A[14] &&& condition2_a, t_T_BWE_N_A_hold_worst);
  $hold(posedge CLK_A &&& condition2_a, negedge T_BWE_N_A[14] &&& condition2_a, t_T_BWE_N_A_hold_worst);
  $setup(posedge T_BWE_N_A[15] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_BWE_N_A_setup_worst);
  $setup(negedge T_BWE_N_A[15] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_BWE_N_A_setup_worst);
  $hold(posedge CLK_A &&& condition2_a, posedge T_BWE_N_A[15] &&& condition2_a, t_T_BWE_N_A_hold_worst);
  $hold(posedge CLK_A &&& condition2_a, negedge T_BWE_N_A[15] &&& condition2_a, t_T_BWE_N_A_hold_worst);
  $setup(posedge T_BWE_N_A[16] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_BWE_N_A_setup_worst);
  $setup(negedge T_BWE_N_A[16] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_BWE_N_A_setup_worst);
  $hold(posedge CLK_A &&& condition2_a, posedge T_BWE_N_A[16] &&& condition2_a, t_T_BWE_N_A_hold_worst);
  $hold(posedge CLK_A &&& condition2_a, negedge T_BWE_N_A[16] &&& condition2_a, t_T_BWE_N_A_hold_worst);
  $setup(posedge T_BWE_N_A[17] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_BWE_N_A_setup_worst);
  $setup(negedge T_BWE_N_A[17] &&& condition2_a, posedge CLK_A &&& condition2_a, t_T_BWE_N_A_setup_worst);
  $hold(posedge CLK_A &&& condition2_a, posedge T_BWE_N_A[17] &&& condition2_a, t_T_BWE_N_A_hold_worst);
  $hold(posedge CLK_A &&& condition2_a, negedge T_BWE_N_A[17] &&& condition2_a, t_T_BWE_N_A_hold_worst);
  $setup(posedge T_BWE_N_B[0] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_BWE_N_B_setup_worst);
  $setup(negedge T_BWE_N_B[0] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_BWE_N_B_setup_worst);
  $hold(posedge CLK_B &&& condition2_b, posedge T_BWE_N_B[0] &&& condition2_b, t_T_BWE_N_B_hold_worst);
  $hold(posedge CLK_B &&& condition2_b, negedge T_BWE_N_B[0] &&& condition2_b, t_T_BWE_N_B_hold_worst);
  $setup(posedge T_BWE_N_B[1] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_BWE_N_B_setup_worst);
  $setup(negedge T_BWE_N_B[1] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_BWE_N_B_setup_worst);
  $hold(posedge CLK_B &&& condition2_b, posedge T_BWE_N_B[1] &&& condition2_b, t_T_BWE_N_B_hold_worst);
  $hold(posedge CLK_B &&& condition2_b, negedge T_BWE_N_B[1] &&& condition2_b, t_T_BWE_N_B_hold_worst);
  $setup(posedge T_BWE_N_B[2] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_BWE_N_B_setup_worst);
  $setup(negedge T_BWE_N_B[2] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_BWE_N_B_setup_worst);
  $hold(posedge CLK_B &&& condition2_b, posedge T_BWE_N_B[2] &&& condition2_b, t_T_BWE_N_B_hold_worst);
  $hold(posedge CLK_B &&& condition2_b, negedge T_BWE_N_B[2] &&& condition2_b, t_T_BWE_N_B_hold_worst);
  $setup(posedge T_BWE_N_B[3] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_BWE_N_B_setup_worst);
  $setup(negedge T_BWE_N_B[3] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_BWE_N_B_setup_worst);
  $hold(posedge CLK_B &&& condition2_b, posedge T_BWE_N_B[3] &&& condition2_b, t_T_BWE_N_B_hold_worst);
  $hold(posedge CLK_B &&& condition2_b, negedge T_BWE_N_B[3] &&& condition2_b, t_T_BWE_N_B_hold_worst);
  $setup(posedge T_BWE_N_B[4] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_BWE_N_B_setup_worst);
  $setup(negedge T_BWE_N_B[4] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_BWE_N_B_setup_worst);
  $hold(posedge CLK_B &&& condition2_b, posedge T_BWE_N_B[4] &&& condition2_b, t_T_BWE_N_B_hold_worst);
  $hold(posedge CLK_B &&& condition2_b, negedge T_BWE_N_B[4] &&& condition2_b, t_T_BWE_N_B_hold_worst);
  $setup(posedge T_BWE_N_B[5] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_BWE_N_B_setup_worst);
  $setup(negedge T_BWE_N_B[5] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_BWE_N_B_setup_worst);
  $hold(posedge CLK_B &&& condition2_b, posedge T_BWE_N_B[5] &&& condition2_b, t_T_BWE_N_B_hold_worst);
  $hold(posedge CLK_B &&& condition2_b, negedge T_BWE_N_B[5] &&& condition2_b, t_T_BWE_N_B_hold_worst);
  $setup(posedge T_BWE_N_B[6] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_BWE_N_B_setup_worst);
  $setup(negedge T_BWE_N_B[6] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_BWE_N_B_setup_worst);
  $hold(posedge CLK_B &&& condition2_b, posedge T_BWE_N_B[6] &&& condition2_b, t_T_BWE_N_B_hold_worst);
  $hold(posedge CLK_B &&& condition2_b, negedge T_BWE_N_B[6] &&& condition2_b, t_T_BWE_N_B_hold_worst);
  $setup(posedge T_BWE_N_B[7] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_BWE_N_B_setup_worst);
  $setup(negedge T_BWE_N_B[7] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_BWE_N_B_setup_worst);
  $hold(posedge CLK_B &&& condition2_b, posedge T_BWE_N_B[7] &&& condition2_b, t_T_BWE_N_B_hold_worst);
  $hold(posedge CLK_B &&& condition2_b, negedge T_BWE_N_B[7] &&& condition2_b, t_T_BWE_N_B_hold_worst);
  $setup(posedge T_BWE_N_B[8] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_BWE_N_B_setup_worst);
  $setup(negedge T_BWE_N_B[8] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_BWE_N_B_setup_worst);
  $hold(posedge CLK_B &&& condition2_b, posedge T_BWE_N_B[8] &&& condition2_b, t_T_BWE_N_B_hold_worst);
  $hold(posedge CLK_B &&& condition2_b, negedge T_BWE_N_B[8] &&& condition2_b, t_T_BWE_N_B_hold_worst);
  $setup(posedge T_BWE_N_B[9] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_BWE_N_B_setup_worst);
  $setup(negedge T_BWE_N_B[9] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_BWE_N_B_setup_worst);
  $hold(posedge CLK_B &&& condition2_b, posedge T_BWE_N_B[9] &&& condition2_b, t_T_BWE_N_B_hold_worst);
  $hold(posedge CLK_B &&& condition2_b, negedge T_BWE_N_B[9] &&& condition2_b, t_T_BWE_N_B_hold_worst);
  $setup(posedge T_BWE_N_B[10] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_BWE_N_B_setup_worst);
  $setup(negedge T_BWE_N_B[10] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_BWE_N_B_setup_worst);
  $hold(posedge CLK_B &&& condition2_b, posedge T_BWE_N_B[10] &&& condition2_b, t_T_BWE_N_B_hold_worst);
  $hold(posedge CLK_B &&& condition2_b, negedge T_BWE_N_B[10] &&& condition2_b, t_T_BWE_N_B_hold_worst);
  $setup(posedge T_BWE_N_B[11] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_BWE_N_B_setup_worst);
  $setup(negedge T_BWE_N_B[11] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_BWE_N_B_setup_worst);
  $hold(posedge CLK_B &&& condition2_b, posedge T_BWE_N_B[11] &&& condition2_b, t_T_BWE_N_B_hold_worst);
  $hold(posedge CLK_B &&& condition2_b, negedge T_BWE_N_B[11] &&& condition2_b, t_T_BWE_N_B_hold_worst);
  $setup(posedge T_BWE_N_B[12] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_BWE_N_B_setup_worst);
  $setup(negedge T_BWE_N_B[12] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_BWE_N_B_setup_worst);
  $hold(posedge CLK_B &&& condition2_b, posedge T_BWE_N_B[12] &&& condition2_b, t_T_BWE_N_B_hold_worst);
  $hold(posedge CLK_B &&& condition2_b, negedge T_BWE_N_B[12] &&& condition2_b, t_T_BWE_N_B_hold_worst);
  $setup(posedge T_BWE_N_B[13] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_BWE_N_B_setup_worst);
  $setup(negedge T_BWE_N_B[13] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_BWE_N_B_setup_worst);
  $hold(posedge CLK_B &&& condition2_b, posedge T_BWE_N_B[13] &&& condition2_b, t_T_BWE_N_B_hold_worst);
  $hold(posedge CLK_B &&& condition2_b, negedge T_BWE_N_B[13] &&& condition2_b, t_T_BWE_N_B_hold_worst);
  $setup(posedge T_BWE_N_B[14] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_BWE_N_B_setup_worst);
  $setup(negedge T_BWE_N_B[14] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_BWE_N_B_setup_worst);
  $hold(posedge CLK_B &&& condition2_b, posedge T_BWE_N_B[14] &&& condition2_b, t_T_BWE_N_B_hold_worst);
  $hold(posedge CLK_B &&& condition2_b, negedge T_BWE_N_B[14] &&& condition2_b, t_T_BWE_N_B_hold_worst);
  $setup(posedge T_BWE_N_B[15] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_BWE_N_B_setup_worst);
  $setup(negedge T_BWE_N_B[15] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_BWE_N_B_setup_worst);
  $hold(posedge CLK_B &&& condition2_b, posedge T_BWE_N_B[15] &&& condition2_b, t_T_BWE_N_B_hold_worst);
  $hold(posedge CLK_B &&& condition2_b, negedge T_BWE_N_B[15] &&& condition2_b, t_T_BWE_N_B_hold_worst);
  $setup(posedge T_BWE_N_B[16] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_BWE_N_B_setup_worst);
  $setup(negedge T_BWE_N_B[16] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_BWE_N_B_setup_worst);
  $hold(posedge CLK_B &&& condition2_b, posedge T_BWE_N_B[16] &&& condition2_b, t_T_BWE_N_B_hold_worst);
  $hold(posedge CLK_B &&& condition2_b, negedge T_BWE_N_B[16] &&& condition2_b, t_T_BWE_N_B_hold_worst);
  $setup(posedge T_BWE_N_B[17] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_BWE_N_B_setup_worst);
  $setup(negedge T_BWE_N_B[17] &&& condition2_b, posedge CLK_B &&& condition2_b, t_T_BWE_N_B_setup_worst);
  $hold(posedge CLK_B &&& condition2_b, posedge T_BWE_N_B[17] &&& condition2_b, t_T_BWE_N_B_hold_worst);
  $hold(posedge CLK_B &&& condition2_b, negedge T_BWE_N_B[17] &&& condition2_b, t_T_BWE_N_B_hold_worst);
 `endif
 `ifdef SDFVERSION_3
  $setuphold(posedge CLK_A &&& condition1_a, posedge T_RWM_A[0] &&& condition1_a, t_T_RWM_A_setup_worst, t_T_RWM_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition1_a, negedge T_RWM_A[0] &&& condition1_a, t_T_RWM_A_setup_worst, t_T_RWM_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition1_a, posedge T_RWM_A[1] &&& condition1_a, t_T_RWM_A_setup_worst, t_T_RWM_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition1_a, negedge T_RWM_A[1] &&& condition1_a, t_T_RWM_A_setup_worst, t_T_RWM_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition1_a, posedge T_RWM_A[2] &&& condition1_a, t_T_RWM_A_setup_worst, t_T_RWM_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition1_a, negedge T_RWM_A[2] &&& condition1_a, t_T_RWM_A_setup_worst, t_T_RWM_A_hold_worst);
  $setuphold(posedge CLK_B &&& condition1_b, posedge T_RWM_B[0] &&& condition1_b, t_T_RWM_B_setup_worst, t_T_RWM_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition1_b, negedge T_RWM_B[0] &&& condition1_b, t_T_RWM_B_setup_worst, t_T_RWM_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition1_b, posedge T_RWM_B[1] &&& condition1_b, t_T_RWM_B_setup_worst, t_T_RWM_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition1_b, negedge T_RWM_B[1] &&& condition1_b, t_T_RWM_B_setup_worst, t_T_RWM_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition1_b, posedge T_RWM_B[2] &&& condition1_b, t_T_RWM_B_setup_worst, t_T_RWM_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition1_b, negedge T_RWM_B[2] &&& condition1_b, t_T_RWM_B_setup_worst, t_T_RWM_B_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, posedge DI_A[0] &&& condition3_a, t_DI_A_setup_worst, t_DI_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, negedge DI_A[0] &&& condition3_a, t_DI_A_setup_worst, t_DI_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, posedge DI_A[1] &&& condition3_a, t_DI_A_setup_worst, t_DI_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, negedge DI_A[1] &&& condition3_a, t_DI_A_setup_worst, t_DI_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, posedge DI_A[2] &&& condition3_a, t_DI_A_setup_worst, t_DI_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, negedge DI_A[2] &&& condition3_a, t_DI_A_setup_worst, t_DI_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, posedge DI_A[3] &&& condition3_a, t_DI_A_setup_worst, t_DI_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, negedge DI_A[3] &&& condition3_a, t_DI_A_setup_worst, t_DI_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, posedge DI_A[4] &&& condition3_a, t_DI_A_setup_worst, t_DI_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, negedge DI_A[4] &&& condition3_a, t_DI_A_setup_worst, t_DI_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, posedge DI_A[5] &&& condition3_a, t_DI_A_setup_worst, t_DI_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, negedge DI_A[5] &&& condition3_a, t_DI_A_setup_worst, t_DI_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, posedge DI_A[6] &&& condition3_a, t_DI_A_setup_worst, t_DI_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, negedge DI_A[6] &&& condition3_a, t_DI_A_setup_worst, t_DI_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, posedge DI_A[7] &&& condition3_a, t_DI_A_setup_worst, t_DI_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, negedge DI_A[7] &&& condition3_a, t_DI_A_setup_worst, t_DI_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, posedge DI_A[8] &&& condition3_a, t_DI_A_setup_worst, t_DI_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, negedge DI_A[8] &&& condition3_a, t_DI_A_setup_worst, t_DI_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, posedge DI_A[9] &&& condition3_a, t_DI_A_setup_worst, t_DI_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, negedge DI_A[9] &&& condition3_a, t_DI_A_setup_worst, t_DI_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, posedge DI_A[10] &&& condition3_a, t_DI_A_setup_worst, t_DI_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, negedge DI_A[10] &&& condition3_a, t_DI_A_setup_worst, t_DI_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, posedge DI_A[11] &&& condition3_a, t_DI_A_setup_worst, t_DI_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, negedge DI_A[11] &&& condition3_a, t_DI_A_setup_worst, t_DI_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, posedge DI_A[12] &&& condition3_a, t_DI_A_setup_worst, t_DI_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, negedge DI_A[12] &&& condition3_a, t_DI_A_setup_worst, t_DI_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, posedge DI_A[13] &&& condition3_a, t_DI_A_setup_worst, t_DI_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, negedge DI_A[13] &&& condition3_a, t_DI_A_setup_worst, t_DI_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, posedge DI_A[14] &&& condition3_a, t_DI_A_setup_worst, t_DI_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, negedge DI_A[14] &&& condition3_a, t_DI_A_setup_worst, t_DI_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, posedge DI_A[15] &&& condition3_a, t_DI_A_setup_worst, t_DI_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, negedge DI_A[15] &&& condition3_a, t_DI_A_setup_worst, t_DI_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, posedge DI_A[16] &&& condition3_a, t_DI_A_setup_worst, t_DI_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, negedge DI_A[16] &&& condition3_a, t_DI_A_setup_worst, t_DI_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, posedge DI_A[17] &&& condition3_a, t_DI_A_setup_worst, t_DI_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, negedge DI_A[17] &&& condition3_a, t_DI_A_setup_worst, t_DI_A_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, posedge DI_B[0] &&& condition3_b, t_DI_B_setup_worst, t_DI_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, negedge DI_B[0] &&& condition3_b, t_DI_B_setup_worst, t_DI_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, posedge DI_B[1] &&& condition3_b, t_DI_B_setup_worst, t_DI_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, negedge DI_B[1] &&& condition3_b, t_DI_B_setup_worst, t_DI_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, posedge DI_B[2] &&& condition3_b, t_DI_B_setup_worst, t_DI_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, negedge DI_B[2] &&& condition3_b, t_DI_B_setup_worst, t_DI_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, posedge DI_B[3] &&& condition3_b, t_DI_B_setup_worst, t_DI_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, negedge DI_B[3] &&& condition3_b, t_DI_B_setup_worst, t_DI_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, posedge DI_B[4] &&& condition3_b, t_DI_B_setup_worst, t_DI_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, negedge DI_B[4] &&& condition3_b, t_DI_B_setup_worst, t_DI_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, posedge DI_B[5] &&& condition3_b, t_DI_B_setup_worst, t_DI_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, negedge DI_B[5] &&& condition3_b, t_DI_B_setup_worst, t_DI_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, posedge DI_B[6] &&& condition3_b, t_DI_B_setup_worst, t_DI_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, negedge DI_B[6] &&& condition3_b, t_DI_B_setup_worst, t_DI_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, posedge DI_B[7] &&& condition3_b, t_DI_B_setup_worst, t_DI_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, negedge DI_B[7] &&& condition3_b, t_DI_B_setup_worst, t_DI_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, posedge DI_B[8] &&& condition3_b, t_DI_B_setup_worst, t_DI_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, negedge DI_B[8] &&& condition3_b, t_DI_B_setup_worst, t_DI_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, posedge DI_B[9] &&& condition3_b, t_DI_B_setup_worst, t_DI_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, negedge DI_B[9] &&& condition3_b, t_DI_B_setup_worst, t_DI_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, posedge DI_B[10] &&& condition3_b, t_DI_B_setup_worst, t_DI_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, negedge DI_B[10] &&& condition3_b, t_DI_B_setup_worst, t_DI_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, posedge DI_B[11] &&& condition3_b, t_DI_B_setup_worst, t_DI_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, negedge DI_B[11] &&& condition3_b, t_DI_B_setup_worst, t_DI_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, posedge DI_B[12] &&& condition3_b, t_DI_B_setup_worst, t_DI_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, negedge DI_B[12] &&& condition3_b, t_DI_B_setup_worst, t_DI_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, posedge DI_B[13] &&& condition3_b, t_DI_B_setup_worst, t_DI_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, negedge DI_B[13] &&& condition3_b, t_DI_B_setup_worst, t_DI_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, posedge DI_B[14] &&& condition3_b, t_DI_B_setup_worst, t_DI_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, negedge DI_B[14] &&& condition3_b, t_DI_B_setup_worst, t_DI_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, posedge DI_B[15] &&& condition3_b, t_DI_B_setup_worst, t_DI_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, negedge DI_B[15] &&& condition3_b, t_DI_B_setup_worst, t_DI_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, posedge DI_B[16] &&& condition3_b, t_DI_B_setup_worst, t_DI_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, negedge DI_B[16] &&& condition3_b, t_DI_B_setup_worst, t_DI_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, posedge DI_B[17] &&& condition3_b, t_DI_B_setup_worst, t_DI_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, negedge DI_B[17] &&& condition3_b, t_DI_B_setup_worst, t_DI_B_hold_worst);
  $setuphold(posedge CLK_A, posedge T_BE_N, t_T_BE_N_setup_worst, t_T_BE_N_hold_worst);
  $setuphold(posedge CLK_A, negedge T_BE_N, t_T_BE_N_setup_worst, t_T_BE_N_hold_worst);
  $setuphold(posedge CLK_B, posedge T_BE_N, t_T_BE_N_setup_worst, t_T_BE_N_hold_worst);
  $setuphold(posedge CLK_B, negedge T_BE_N, t_T_BE_N_setup_worst, t_T_BE_N_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, posedge T_DI_A[0] &&& condition2_a, t_T_DI_A_setup_worst, t_T_DI_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, negedge T_DI_A[0] &&& condition2_a, t_T_DI_A_setup_worst, t_T_DI_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, posedge T_DI_A[1] &&& condition2_a, t_T_DI_A_setup_worst, t_T_DI_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, negedge T_DI_A[1] &&& condition2_a, t_T_DI_A_setup_worst, t_T_DI_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, posedge T_DI_A[2] &&& condition2_a, t_T_DI_A_setup_worst, t_T_DI_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, negedge T_DI_A[2] &&& condition2_a, t_T_DI_A_setup_worst, t_T_DI_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, posedge T_DI_A[3] &&& condition2_a, t_T_DI_A_setup_worst, t_T_DI_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, negedge T_DI_A[3] &&& condition2_a, t_T_DI_A_setup_worst, t_T_DI_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, posedge T_DI_A[4] &&& condition2_a, t_T_DI_A_setup_worst, t_T_DI_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, negedge T_DI_A[4] &&& condition2_a, t_T_DI_A_setup_worst, t_T_DI_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, posedge T_DI_A[5] &&& condition2_a, t_T_DI_A_setup_worst, t_T_DI_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, negedge T_DI_A[5] &&& condition2_a, t_T_DI_A_setup_worst, t_T_DI_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, posedge T_DI_A[6] &&& condition2_a, t_T_DI_A_setup_worst, t_T_DI_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, negedge T_DI_A[6] &&& condition2_a, t_T_DI_A_setup_worst, t_T_DI_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, posedge T_DI_A[7] &&& condition2_a, t_T_DI_A_setup_worst, t_T_DI_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, negedge T_DI_A[7] &&& condition2_a, t_T_DI_A_setup_worst, t_T_DI_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, posedge T_DI_A[8] &&& condition2_a, t_T_DI_A_setup_worst, t_T_DI_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, negedge T_DI_A[8] &&& condition2_a, t_T_DI_A_setup_worst, t_T_DI_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, posedge T_DI_A[9] &&& condition2_a, t_T_DI_A_setup_worst, t_T_DI_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, negedge T_DI_A[9] &&& condition2_a, t_T_DI_A_setup_worst, t_T_DI_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, posedge T_DI_A[10] &&& condition2_a, t_T_DI_A_setup_worst, t_T_DI_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, negedge T_DI_A[10] &&& condition2_a, t_T_DI_A_setup_worst, t_T_DI_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, posedge T_DI_A[11] &&& condition2_a, t_T_DI_A_setup_worst, t_T_DI_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, negedge T_DI_A[11] &&& condition2_a, t_T_DI_A_setup_worst, t_T_DI_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, posedge T_DI_A[12] &&& condition2_a, t_T_DI_A_setup_worst, t_T_DI_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, negedge T_DI_A[12] &&& condition2_a, t_T_DI_A_setup_worst, t_T_DI_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, posedge T_DI_A[13] &&& condition2_a, t_T_DI_A_setup_worst, t_T_DI_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, negedge T_DI_A[13] &&& condition2_a, t_T_DI_A_setup_worst, t_T_DI_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, posedge T_DI_A[14] &&& condition2_a, t_T_DI_A_setup_worst, t_T_DI_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, negedge T_DI_A[14] &&& condition2_a, t_T_DI_A_setup_worst, t_T_DI_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, posedge T_DI_A[15] &&& condition2_a, t_T_DI_A_setup_worst, t_T_DI_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, negedge T_DI_A[15] &&& condition2_a, t_T_DI_A_setup_worst, t_T_DI_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, posedge T_DI_A[16] &&& condition2_a, t_T_DI_A_setup_worst, t_T_DI_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, negedge T_DI_A[16] &&& condition2_a, t_T_DI_A_setup_worst, t_T_DI_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, posedge T_DI_A[17] &&& condition2_a, t_T_DI_A_setup_worst, t_T_DI_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, negedge T_DI_A[17] &&& condition2_a, t_T_DI_A_setup_worst, t_T_DI_A_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, posedge T_DI_B[0] &&& condition2_b, t_T_DI_B_setup_worst, t_T_DI_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, negedge T_DI_B[0] &&& condition2_b, t_T_DI_B_setup_worst, t_T_DI_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, posedge T_DI_B[1] &&& condition2_b, t_T_DI_B_setup_worst, t_T_DI_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, negedge T_DI_B[1] &&& condition2_b, t_T_DI_B_setup_worst, t_T_DI_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, posedge T_DI_B[2] &&& condition2_b, t_T_DI_B_setup_worst, t_T_DI_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, negedge T_DI_B[2] &&& condition2_b, t_T_DI_B_setup_worst, t_T_DI_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, posedge T_DI_B[3] &&& condition2_b, t_T_DI_B_setup_worst, t_T_DI_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, negedge T_DI_B[3] &&& condition2_b, t_T_DI_B_setup_worst, t_T_DI_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, posedge T_DI_B[4] &&& condition2_b, t_T_DI_B_setup_worst, t_T_DI_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, negedge T_DI_B[4] &&& condition2_b, t_T_DI_B_setup_worst, t_T_DI_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, posedge T_DI_B[5] &&& condition2_b, t_T_DI_B_setup_worst, t_T_DI_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, negedge T_DI_B[5] &&& condition2_b, t_T_DI_B_setup_worst, t_T_DI_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, posedge T_DI_B[6] &&& condition2_b, t_T_DI_B_setup_worst, t_T_DI_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, negedge T_DI_B[6] &&& condition2_b, t_T_DI_B_setup_worst, t_T_DI_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, posedge T_DI_B[7] &&& condition2_b, t_T_DI_B_setup_worst, t_T_DI_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, negedge T_DI_B[7] &&& condition2_b, t_T_DI_B_setup_worst, t_T_DI_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, posedge T_DI_B[8] &&& condition2_b, t_T_DI_B_setup_worst, t_T_DI_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, negedge T_DI_B[8] &&& condition2_b, t_T_DI_B_setup_worst, t_T_DI_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, posedge T_DI_B[9] &&& condition2_b, t_T_DI_B_setup_worst, t_T_DI_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, negedge T_DI_B[9] &&& condition2_b, t_T_DI_B_setup_worst, t_T_DI_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, posedge T_DI_B[10] &&& condition2_b, t_T_DI_B_setup_worst, t_T_DI_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, negedge T_DI_B[10] &&& condition2_b, t_T_DI_B_setup_worst, t_T_DI_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, posedge T_DI_B[11] &&& condition2_b, t_T_DI_B_setup_worst, t_T_DI_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, negedge T_DI_B[11] &&& condition2_b, t_T_DI_B_setup_worst, t_T_DI_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, posedge T_DI_B[12] &&& condition2_b, t_T_DI_B_setup_worst, t_T_DI_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, negedge T_DI_B[12] &&& condition2_b, t_T_DI_B_setup_worst, t_T_DI_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, posedge T_DI_B[13] &&& condition2_b, t_T_DI_B_setup_worst, t_T_DI_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, negedge T_DI_B[13] &&& condition2_b, t_T_DI_B_setup_worst, t_T_DI_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, posedge T_DI_B[14] &&& condition2_b, t_T_DI_B_setup_worst, t_T_DI_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, negedge T_DI_B[14] &&& condition2_b, t_T_DI_B_setup_worst, t_T_DI_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, posedge T_DI_B[15] &&& condition2_b, t_T_DI_B_setup_worst, t_T_DI_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, negedge T_DI_B[15] &&& condition2_b, t_T_DI_B_setup_worst, t_T_DI_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, posedge T_DI_B[16] &&& condition2_b, t_T_DI_B_setup_worst, t_T_DI_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, negedge T_DI_B[16] &&& condition2_b, t_T_DI_B_setup_worst, t_T_DI_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, posedge T_DI_B[17] &&& condition2_b, t_T_DI_B_setup_worst, t_T_DI_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, negedge T_DI_B[17] &&& condition2_b, t_T_DI_B_setup_worst, t_T_DI_B_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, posedge A_A[0] &&& condition3_a, t_A_A_setup_worst, t_A_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, negedge A_A[0] &&& condition3_a, t_A_A_setup_worst, t_A_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, posedge A_A[1] &&& condition3_a, t_A_A_setup_worst, t_A_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, negedge A_A[1] &&& condition3_a, t_A_A_setup_worst, t_A_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, posedge A_A[2] &&& condition3_a, t_A_A_setup_worst, t_A_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, negedge A_A[2] &&& condition3_a, t_A_A_setup_worst, t_A_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, posedge A_A[3] &&& condition3_a, t_A_A_setup_worst, t_A_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, negedge A_A[3] &&& condition3_a, t_A_A_setup_worst, t_A_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, posedge A_A[4] &&& condition3_a, t_A_A_setup_worst, t_A_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, negedge A_A[4] &&& condition3_a, t_A_A_setup_worst, t_A_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, posedge A_A[5] &&& condition3_a, t_A_A_setup_worst, t_A_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, negedge A_A[5] &&& condition3_a, t_A_A_setup_worst, t_A_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, posedge A_A[6] &&& condition3_a, t_A_A_setup_worst, t_A_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, negedge A_A[6] &&& condition3_a, t_A_A_setup_worst, t_A_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, posedge A_A[7] &&& condition3_a, t_A_A_setup_worst, t_A_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, negedge A_A[7] &&& condition3_a, t_A_A_setup_worst, t_A_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, posedge A_A[8] &&& condition3_a, t_A_A_setup_worst, t_A_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, negedge A_A[8] &&& condition3_a, t_A_A_setup_worst, t_A_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, posedge A_A[9] &&& condition3_a, t_A_A_setup_worst, t_A_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, negedge A_A[9] &&& condition3_a, t_A_A_setup_worst, t_A_A_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, posedge A_B[0] &&& condition3_b, t_A_B_setup_worst, t_A_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, negedge A_B[0] &&& condition3_b, t_A_B_setup_worst, t_A_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, posedge A_B[1] &&& condition3_b, t_A_B_setup_worst, t_A_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, negedge A_B[1] &&& condition3_b, t_A_B_setup_worst, t_A_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, posedge A_B[2] &&& condition3_b, t_A_B_setup_worst, t_A_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, negedge A_B[2] &&& condition3_b, t_A_B_setup_worst, t_A_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, posedge A_B[3] &&& condition3_b, t_A_B_setup_worst, t_A_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, negedge A_B[3] &&& condition3_b, t_A_B_setup_worst, t_A_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, posedge A_B[4] &&& condition3_b, t_A_B_setup_worst, t_A_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, negedge A_B[4] &&& condition3_b, t_A_B_setup_worst, t_A_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, posedge A_B[5] &&& condition3_b, t_A_B_setup_worst, t_A_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, negedge A_B[5] &&& condition3_b, t_A_B_setup_worst, t_A_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, posedge A_B[6] &&& condition3_b, t_A_B_setup_worst, t_A_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, negedge A_B[6] &&& condition3_b, t_A_B_setup_worst, t_A_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, posedge A_B[7] &&& condition3_b, t_A_B_setup_worst, t_A_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, negedge A_B[7] &&& condition3_b, t_A_B_setup_worst, t_A_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, posedge A_B[8] &&& condition3_b, t_A_B_setup_worst, t_A_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, negedge A_B[8] &&& condition3_b, t_A_B_setup_worst, t_A_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, posedge A_B[9] &&& condition3_b, t_A_B_setup_worst, t_A_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, negedge A_B[9] &&& condition3_b, t_A_B_setup_worst, t_A_B_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, posedge T_A_A[0] &&& condition2_a, t_T_A_A_setup_worst, t_T_A_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, negedge T_A_A[0] &&& condition2_a, t_T_A_A_setup_worst, t_T_A_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, posedge T_A_A[1] &&& condition2_a, t_T_A_A_setup_worst, t_T_A_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, negedge T_A_A[1] &&& condition2_a, t_T_A_A_setup_worst, t_T_A_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, posedge T_A_A[2] &&& condition2_a, t_T_A_A_setup_worst, t_T_A_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, negedge T_A_A[2] &&& condition2_a, t_T_A_A_setup_worst, t_T_A_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, posedge T_A_A[3] &&& condition2_a, t_T_A_A_setup_worst, t_T_A_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, negedge T_A_A[3] &&& condition2_a, t_T_A_A_setup_worst, t_T_A_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, posedge T_A_A[4] &&& condition2_a, t_T_A_A_setup_worst, t_T_A_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, negedge T_A_A[4] &&& condition2_a, t_T_A_A_setup_worst, t_T_A_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, posedge T_A_A[5] &&& condition2_a, t_T_A_A_setup_worst, t_T_A_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, negedge T_A_A[5] &&& condition2_a, t_T_A_A_setup_worst, t_T_A_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, posedge T_A_A[6] &&& condition2_a, t_T_A_A_setup_worst, t_T_A_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, negedge T_A_A[6] &&& condition2_a, t_T_A_A_setup_worst, t_T_A_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, posedge T_A_A[7] &&& condition2_a, t_T_A_A_setup_worst, t_T_A_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, negedge T_A_A[7] &&& condition2_a, t_T_A_A_setup_worst, t_T_A_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, posedge T_A_A[8] &&& condition2_a, t_T_A_A_setup_worst, t_T_A_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, negedge T_A_A[8] &&& condition2_a, t_T_A_A_setup_worst, t_T_A_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, posedge T_A_A[9] &&& condition2_a, t_T_A_A_setup_worst, t_T_A_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, negedge T_A_A[9] &&& condition2_a, t_T_A_A_setup_worst, t_T_A_A_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, posedge T_A_B[0] &&& condition2_b, t_T_A_B_setup_worst, t_T_A_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, negedge T_A_B[0] &&& condition2_b, t_T_A_B_setup_worst, t_T_A_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, posedge T_A_B[1] &&& condition2_b, t_T_A_B_setup_worst, t_T_A_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, negedge T_A_B[1] &&& condition2_b, t_T_A_B_setup_worst, t_T_A_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, posedge T_A_B[2] &&& condition2_b, t_T_A_B_setup_worst, t_T_A_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, negedge T_A_B[2] &&& condition2_b, t_T_A_B_setup_worst, t_T_A_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, posedge T_A_B[3] &&& condition2_b, t_T_A_B_setup_worst, t_T_A_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, negedge T_A_B[3] &&& condition2_b, t_T_A_B_setup_worst, t_T_A_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, posedge T_A_B[4] &&& condition2_b, t_T_A_B_setup_worst, t_T_A_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, negedge T_A_B[4] &&& condition2_b, t_T_A_B_setup_worst, t_T_A_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, posedge T_A_B[5] &&& condition2_b, t_T_A_B_setup_worst, t_T_A_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, negedge T_A_B[5] &&& condition2_b, t_T_A_B_setup_worst, t_T_A_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, posedge T_A_B[6] &&& condition2_b, t_T_A_B_setup_worst, t_T_A_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, negedge T_A_B[6] &&& condition2_b, t_T_A_B_setup_worst, t_T_A_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, posedge T_A_B[7] &&& condition2_b, t_T_A_B_setup_worst, t_T_A_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, negedge T_A_B[7] &&& condition2_b, t_T_A_B_setup_worst, t_T_A_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, posedge T_A_B[8] &&& condition2_b, t_T_A_B_setup_worst, t_T_A_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, negedge T_A_B[8] &&& condition2_b, t_T_A_B_setup_worst, t_T_A_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, posedge T_A_B[9] &&& condition2_b, t_T_A_B_setup_worst, t_T_A_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, negedge T_A_B[9] &&& condition2_b, t_T_A_B_setup_worst, t_T_A_B_hold_worst);
  $setuphold(posedge CLK_A &&& T_BE_N, posedge CE_N_A &&& T_BE_N, t_CE_N_A_setup_worst, t_CE_N_A_hold_worst);
  $setuphold(posedge CLK_A &&& T_BE_N, negedge CE_N_A &&& T_BE_N, t_CE_N_A_setup_worst, t_CE_N_A_hold_worst);
  $setuphold(posedge CLK_B &&& T_BE_N, posedge CE_N_B &&& T_BE_N, t_CE_N_B_setup_worst, t_CE_N_B_hold_worst);
  $setuphold(posedge CLK_B &&& T_BE_N, negedge CE_N_B &&& T_BE_N, t_CE_N_B_setup_worst, t_CE_N_B_hold_worst);
  $setuphold(posedge CLK_A &&& condition4_a, posedge T_CE_N_A &&& condition4_a, t_T_CE_N_A_setup_worst, t_T_CE_N_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition4_a, negedge T_CE_N_A &&& condition4_a, t_T_CE_N_A_setup_worst, t_T_CE_N_A_hold_worst);
  $setuphold(posedge CLK_B &&& condition4_b, posedge T_CE_N_B &&& condition4_b, t_T_CE_N_B_setup_worst, t_T_CE_N_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition4_b, negedge T_CE_N_B &&& condition4_b, t_T_CE_N_B_setup_worst, t_T_CE_N_B_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, posedge GWE_N_A &&& condition3_a, t_GWE_N_A_setup_worst, t_GWE_N_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, negedge GWE_N_A &&& condition3_a, t_GWE_N_A_setup_worst, t_GWE_N_A_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, posedge GWE_N_B &&& condition3_b, t_GWE_N_B_setup_worst, t_GWE_N_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, negedge GWE_N_B &&& condition3_b, t_GWE_N_B_setup_worst, t_GWE_N_B_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, posedge T_GWE_N_A &&& condition2_a, t_T_GWE_N_A_setup_worst, t_T_GWE_N_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, negedge T_GWE_N_A &&& condition2_a, t_T_GWE_N_A_setup_worst, t_T_GWE_N_A_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, posedge T_GWE_N_B &&& condition2_b, t_T_GWE_N_B_setup_worst, t_T_GWE_N_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, negedge T_GWE_N_B &&& condition2_b, t_T_GWE_N_B_setup_worst, t_T_GWE_N_B_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, posedge BWE_N_A[0] &&& condition3_a, t_BWE_N_A_setup_worst, t_BWE_N_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, negedge BWE_N_A[0] &&& condition3_a, t_BWE_N_A_setup_worst, t_BWE_N_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, posedge BWE_N_A[1] &&& condition3_a, t_BWE_N_A_setup_worst, t_BWE_N_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, negedge BWE_N_A[1] &&& condition3_a, t_BWE_N_A_setup_worst, t_BWE_N_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, posedge BWE_N_A[2] &&& condition3_a, t_BWE_N_A_setup_worst, t_BWE_N_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, negedge BWE_N_A[2] &&& condition3_a, t_BWE_N_A_setup_worst, t_BWE_N_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, posedge BWE_N_A[3] &&& condition3_a, t_BWE_N_A_setup_worst, t_BWE_N_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, negedge BWE_N_A[3] &&& condition3_a, t_BWE_N_A_setup_worst, t_BWE_N_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, posedge BWE_N_A[4] &&& condition3_a, t_BWE_N_A_setup_worst, t_BWE_N_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, negedge BWE_N_A[4] &&& condition3_a, t_BWE_N_A_setup_worst, t_BWE_N_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, posedge BWE_N_A[5] &&& condition3_a, t_BWE_N_A_setup_worst, t_BWE_N_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, negedge BWE_N_A[5] &&& condition3_a, t_BWE_N_A_setup_worst, t_BWE_N_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, posedge BWE_N_A[6] &&& condition3_a, t_BWE_N_A_setup_worst, t_BWE_N_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, negedge BWE_N_A[6] &&& condition3_a, t_BWE_N_A_setup_worst, t_BWE_N_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, posedge BWE_N_A[7] &&& condition3_a, t_BWE_N_A_setup_worst, t_BWE_N_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, negedge BWE_N_A[7] &&& condition3_a, t_BWE_N_A_setup_worst, t_BWE_N_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, posedge BWE_N_A[8] &&& condition3_a, t_BWE_N_A_setup_worst, t_BWE_N_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, negedge BWE_N_A[8] &&& condition3_a, t_BWE_N_A_setup_worst, t_BWE_N_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, posedge BWE_N_A[9] &&& condition3_a, t_BWE_N_A_setup_worst, t_BWE_N_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, negedge BWE_N_A[9] &&& condition3_a, t_BWE_N_A_setup_worst, t_BWE_N_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, posedge BWE_N_A[10] &&& condition3_a, t_BWE_N_A_setup_worst, t_BWE_N_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, negedge BWE_N_A[10] &&& condition3_a, t_BWE_N_A_setup_worst, t_BWE_N_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, posedge BWE_N_A[11] &&& condition3_a, t_BWE_N_A_setup_worst, t_BWE_N_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, negedge BWE_N_A[11] &&& condition3_a, t_BWE_N_A_setup_worst, t_BWE_N_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, posedge BWE_N_A[12] &&& condition3_a, t_BWE_N_A_setup_worst, t_BWE_N_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, negedge BWE_N_A[12] &&& condition3_a, t_BWE_N_A_setup_worst, t_BWE_N_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, posedge BWE_N_A[13] &&& condition3_a, t_BWE_N_A_setup_worst, t_BWE_N_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, negedge BWE_N_A[13] &&& condition3_a, t_BWE_N_A_setup_worst, t_BWE_N_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, posedge BWE_N_A[14] &&& condition3_a, t_BWE_N_A_setup_worst, t_BWE_N_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, negedge BWE_N_A[14] &&& condition3_a, t_BWE_N_A_setup_worst, t_BWE_N_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, posedge BWE_N_A[15] &&& condition3_a, t_BWE_N_A_setup_worst, t_BWE_N_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, negedge BWE_N_A[15] &&& condition3_a, t_BWE_N_A_setup_worst, t_BWE_N_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, posedge BWE_N_A[16] &&& condition3_a, t_BWE_N_A_setup_worst, t_BWE_N_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, negedge BWE_N_A[16] &&& condition3_a, t_BWE_N_A_setup_worst, t_BWE_N_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, posedge BWE_N_A[17] &&& condition3_a, t_BWE_N_A_setup_worst, t_BWE_N_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition3_a, negedge BWE_N_A[17] &&& condition3_a, t_BWE_N_A_setup_worst, t_BWE_N_A_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, posedge BWE_N_B[0] &&& condition3_b, t_BWE_N_B_setup_worst, t_BWE_N_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, negedge BWE_N_B[0] &&& condition3_b, t_BWE_N_B_setup_worst, t_BWE_N_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, posedge BWE_N_B[1] &&& condition3_b, t_BWE_N_B_setup_worst, t_BWE_N_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, negedge BWE_N_B[1] &&& condition3_b, t_BWE_N_B_setup_worst, t_BWE_N_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, posedge BWE_N_B[2] &&& condition3_b, t_BWE_N_B_setup_worst, t_BWE_N_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, negedge BWE_N_B[2] &&& condition3_b, t_BWE_N_B_setup_worst, t_BWE_N_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, posedge BWE_N_B[3] &&& condition3_b, t_BWE_N_B_setup_worst, t_BWE_N_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, negedge BWE_N_B[3] &&& condition3_b, t_BWE_N_B_setup_worst, t_BWE_N_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, posedge BWE_N_B[4] &&& condition3_b, t_BWE_N_B_setup_worst, t_BWE_N_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, negedge BWE_N_B[4] &&& condition3_b, t_BWE_N_B_setup_worst, t_BWE_N_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, posedge BWE_N_B[5] &&& condition3_b, t_BWE_N_B_setup_worst, t_BWE_N_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, negedge BWE_N_B[5] &&& condition3_b, t_BWE_N_B_setup_worst, t_BWE_N_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, posedge BWE_N_B[6] &&& condition3_b, t_BWE_N_B_setup_worst, t_BWE_N_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, negedge BWE_N_B[6] &&& condition3_b, t_BWE_N_B_setup_worst, t_BWE_N_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, posedge BWE_N_B[7] &&& condition3_b, t_BWE_N_B_setup_worst, t_BWE_N_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, negedge BWE_N_B[7] &&& condition3_b, t_BWE_N_B_setup_worst, t_BWE_N_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, posedge BWE_N_B[8] &&& condition3_b, t_BWE_N_B_setup_worst, t_BWE_N_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, negedge BWE_N_B[8] &&& condition3_b, t_BWE_N_B_setup_worst, t_BWE_N_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, posedge BWE_N_B[9] &&& condition3_b, t_BWE_N_B_setup_worst, t_BWE_N_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, negedge BWE_N_B[9] &&& condition3_b, t_BWE_N_B_setup_worst, t_BWE_N_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, posedge BWE_N_B[10] &&& condition3_b, t_BWE_N_B_setup_worst, t_BWE_N_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, negedge BWE_N_B[10] &&& condition3_b, t_BWE_N_B_setup_worst, t_BWE_N_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, posedge BWE_N_B[11] &&& condition3_b, t_BWE_N_B_setup_worst, t_BWE_N_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, negedge BWE_N_B[11] &&& condition3_b, t_BWE_N_B_setup_worst, t_BWE_N_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, posedge BWE_N_B[12] &&& condition3_b, t_BWE_N_B_setup_worst, t_BWE_N_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, negedge BWE_N_B[12] &&& condition3_b, t_BWE_N_B_setup_worst, t_BWE_N_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, posedge BWE_N_B[13] &&& condition3_b, t_BWE_N_B_setup_worst, t_BWE_N_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, negedge BWE_N_B[13] &&& condition3_b, t_BWE_N_B_setup_worst, t_BWE_N_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, posedge BWE_N_B[14] &&& condition3_b, t_BWE_N_B_setup_worst, t_BWE_N_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, negedge BWE_N_B[14] &&& condition3_b, t_BWE_N_B_setup_worst, t_BWE_N_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, posedge BWE_N_B[15] &&& condition3_b, t_BWE_N_B_setup_worst, t_BWE_N_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, negedge BWE_N_B[15] &&& condition3_b, t_BWE_N_B_setup_worst, t_BWE_N_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, posedge BWE_N_B[16] &&& condition3_b, t_BWE_N_B_setup_worst, t_BWE_N_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, negedge BWE_N_B[16] &&& condition3_b, t_BWE_N_B_setup_worst, t_BWE_N_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, posedge BWE_N_B[17] &&& condition3_b, t_BWE_N_B_setup_worst, t_BWE_N_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition3_b, negedge BWE_N_B[17] &&& condition3_b, t_BWE_N_B_setup_worst, t_BWE_N_B_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, posedge T_BWE_N_A[0] &&& condition2_a, t_T_BWE_N_A_setup_worst, t_T_BWE_N_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, negedge T_BWE_N_A[0] &&& condition2_a, t_T_BWE_N_A_setup_worst, t_T_BWE_N_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, posedge T_BWE_N_A[1] &&& condition2_a, t_T_BWE_N_A_setup_worst, t_T_BWE_N_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, negedge T_BWE_N_A[1] &&& condition2_a, t_T_BWE_N_A_setup_worst, t_T_BWE_N_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, posedge T_BWE_N_A[2] &&& condition2_a, t_T_BWE_N_A_setup_worst, t_T_BWE_N_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, negedge T_BWE_N_A[2] &&& condition2_a, t_T_BWE_N_A_setup_worst, t_T_BWE_N_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, posedge T_BWE_N_A[3] &&& condition2_a, t_T_BWE_N_A_setup_worst, t_T_BWE_N_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, negedge T_BWE_N_A[3] &&& condition2_a, t_T_BWE_N_A_setup_worst, t_T_BWE_N_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, posedge T_BWE_N_A[4] &&& condition2_a, t_T_BWE_N_A_setup_worst, t_T_BWE_N_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, negedge T_BWE_N_A[4] &&& condition2_a, t_T_BWE_N_A_setup_worst, t_T_BWE_N_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, posedge T_BWE_N_A[5] &&& condition2_a, t_T_BWE_N_A_setup_worst, t_T_BWE_N_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, negedge T_BWE_N_A[5] &&& condition2_a, t_T_BWE_N_A_setup_worst, t_T_BWE_N_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, posedge T_BWE_N_A[6] &&& condition2_a, t_T_BWE_N_A_setup_worst, t_T_BWE_N_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, negedge T_BWE_N_A[6] &&& condition2_a, t_T_BWE_N_A_setup_worst, t_T_BWE_N_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, posedge T_BWE_N_A[7] &&& condition2_a, t_T_BWE_N_A_setup_worst, t_T_BWE_N_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, negedge T_BWE_N_A[7] &&& condition2_a, t_T_BWE_N_A_setup_worst, t_T_BWE_N_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, posedge T_BWE_N_A[8] &&& condition2_a, t_T_BWE_N_A_setup_worst, t_T_BWE_N_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, negedge T_BWE_N_A[8] &&& condition2_a, t_T_BWE_N_A_setup_worst, t_T_BWE_N_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, posedge T_BWE_N_A[9] &&& condition2_a, t_T_BWE_N_A_setup_worst, t_T_BWE_N_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, negedge T_BWE_N_A[9] &&& condition2_a, t_T_BWE_N_A_setup_worst, t_T_BWE_N_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, posedge T_BWE_N_A[10] &&& condition2_a, t_T_BWE_N_A_setup_worst, t_T_BWE_N_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, negedge T_BWE_N_A[10] &&& condition2_a, t_T_BWE_N_A_setup_worst, t_T_BWE_N_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, posedge T_BWE_N_A[11] &&& condition2_a, t_T_BWE_N_A_setup_worst, t_T_BWE_N_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, negedge T_BWE_N_A[11] &&& condition2_a, t_T_BWE_N_A_setup_worst, t_T_BWE_N_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, posedge T_BWE_N_A[12] &&& condition2_a, t_T_BWE_N_A_setup_worst, t_T_BWE_N_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, negedge T_BWE_N_A[12] &&& condition2_a, t_T_BWE_N_A_setup_worst, t_T_BWE_N_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, posedge T_BWE_N_A[13] &&& condition2_a, t_T_BWE_N_A_setup_worst, t_T_BWE_N_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, negedge T_BWE_N_A[13] &&& condition2_a, t_T_BWE_N_A_setup_worst, t_T_BWE_N_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, posedge T_BWE_N_A[14] &&& condition2_a, t_T_BWE_N_A_setup_worst, t_T_BWE_N_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, negedge T_BWE_N_A[14] &&& condition2_a, t_T_BWE_N_A_setup_worst, t_T_BWE_N_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, posedge T_BWE_N_A[15] &&& condition2_a, t_T_BWE_N_A_setup_worst, t_T_BWE_N_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, negedge T_BWE_N_A[15] &&& condition2_a, t_T_BWE_N_A_setup_worst, t_T_BWE_N_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, posedge T_BWE_N_A[16] &&& condition2_a, t_T_BWE_N_A_setup_worst, t_T_BWE_N_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, negedge T_BWE_N_A[16] &&& condition2_a, t_T_BWE_N_A_setup_worst, t_T_BWE_N_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, posedge T_BWE_N_A[17] &&& condition2_a, t_T_BWE_N_A_setup_worst, t_T_BWE_N_A_hold_worst);
  $setuphold(posedge CLK_A &&& condition2_a, negedge T_BWE_N_A[17] &&& condition2_a, t_T_BWE_N_A_setup_worst, t_T_BWE_N_A_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, posedge T_BWE_N_B[0] &&& condition2_b, t_T_BWE_N_B_setup_worst, t_T_BWE_N_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, negedge T_BWE_N_B[0] &&& condition2_b, t_T_BWE_N_B_setup_worst, t_T_BWE_N_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, posedge T_BWE_N_B[1] &&& condition2_b, t_T_BWE_N_B_setup_worst, t_T_BWE_N_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, negedge T_BWE_N_B[1] &&& condition2_b, t_T_BWE_N_B_setup_worst, t_T_BWE_N_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, posedge T_BWE_N_B[2] &&& condition2_b, t_T_BWE_N_B_setup_worst, t_T_BWE_N_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, negedge T_BWE_N_B[2] &&& condition2_b, t_T_BWE_N_B_setup_worst, t_T_BWE_N_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, posedge T_BWE_N_B[3] &&& condition2_b, t_T_BWE_N_B_setup_worst, t_T_BWE_N_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, negedge T_BWE_N_B[3] &&& condition2_b, t_T_BWE_N_B_setup_worst, t_T_BWE_N_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, posedge T_BWE_N_B[4] &&& condition2_b, t_T_BWE_N_B_setup_worst, t_T_BWE_N_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, negedge T_BWE_N_B[4] &&& condition2_b, t_T_BWE_N_B_setup_worst, t_T_BWE_N_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, posedge T_BWE_N_B[5] &&& condition2_b, t_T_BWE_N_B_setup_worst, t_T_BWE_N_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, negedge T_BWE_N_B[5] &&& condition2_b, t_T_BWE_N_B_setup_worst, t_T_BWE_N_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, posedge T_BWE_N_B[6] &&& condition2_b, t_T_BWE_N_B_setup_worst, t_T_BWE_N_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, negedge T_BWE_N_B[6] &&& condition2_b, t_T_BWE_N_B_setup_worst, t_T_BWE_N_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, posedge T_BWE_N_B[7] &&& condition2_b, t_T_BWE_N_B_setup_worst, t_T_BWE_N_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, negedge T_BWE_N_B[7] &&& condition2_b, t_T_BWE_N_B_setup_worst, t_T_BWE_N_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, posedge T_BWE_N_B[8] &&& condition2_b, t_T_BWE_N_B_setup_worst, t_T_BWE_N_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, negedge T_BWE_N_B[8] &&& condition2_b, t_T_BWE_N_B_setup_worst, t_T_BWE_N_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, posedge T_BWE_N_B[9] &&& condition2_b, t_T_BWE_N_B_setup_worst, t_T_BWE_N_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, negedge T_BWE_N_B[9] &&& condition2_b, t_T_BWE_N_B_setup_worst, t_T_BWE_N_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, posedge T_BWE_N_B[10] &&& condition2_b, t_T_BWE_N_B_setup_worst, t_T_BWE_N_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, negedge T_BWE_N_B[10] &&& condition2_b, t_T_BWE_N_B_setup_worst, t_T_BWE_N_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, posedge T_BWE_N_B[11] &&& condition2_b, t_T_BWE_N_B_setup_worst, t_T_BWE_N_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, negedge T_BWE_N_B[11] &&& condition2_b, t_T_BWE_N_B_setup_worst, t_T_BWE_N_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, posedge T_BWE_N_B[12] &&& condition2_b, t_T_BWE_N_B_setup_worst, t_T_BWE_N_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, negedge T_BWE_N_B[12] &&& condition2_b, t_T_BWE_N_B_setup_worst, t_T_BWE_N_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, posedge T_BWE_N_B[13] &&& condition2_b, t_T_BWE_N_B_setup_worst, t_T_BWE_N_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, negedge T_BWE_N_B[13] &&& condition2_b, t_T_BWE_N_B_setup_worst, t_T_BWE_N_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, posedge T_BWE_N_B[14] &&& condition2_b, t_T_BWE_N_B_setup_worst, t_T_BWE_N_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, negedge T_BWE_N_B[14] &&& condition2_b, t_T_BWE_N_B_setup_worst, t_T_BWE_N_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, posedge T_BWE_N_B[15] &&& condition2_b, t_T_BWE_N_B_setup_worst, t_T_BWE_N_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, negedge T_BWE_N_B[15] &&& condition2_b, t_T_BWE_N_B_setup_worst, t_T_BWE_N_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, posedge T_BWE_N_B[16] &&& condition2_b, t_T_BWE_N_B_setup_worst, t_T_BWE_N_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, negedge T_BWE_N_B[16] &&& condition2_b, t_T_BWE_N_B_setup_worst, t_T_BWE_N_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, posedge T_BWE_N_B[17] &&& condition2_b, t_T_BWE_N_B_setup_worst, t_T_BWE_N_B_hold_worst);
  $setuphold(posedge CLK_B &&& condition2_b, negedge T_BWE_N_B[17] &&& condition2_b, t_T_BWE_N_B_setup_worst, t_T_BWE_N_B_hold_worst);
 `endif
endspecify
endmodule

`endcelldefine

