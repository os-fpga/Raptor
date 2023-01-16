/////////////////////////////////////////////////////////////
// Created by: Synopsys DC Ultra(TM) in wire load mode
// Version   : S-2021.06-SP2
// Date      : Tue Sep 27 10:50:25 2022
/////////////////////////////////////////////////////////////


module RS_BRAM ( SCAN_EN_i, SCAN_MODE_i, SCAN_i,
        SCAN_o, mode, GRESET_i, WEN_A1_i, WEN_B1_i, BE_A1_i, BE_B1_i, REN_A1_i,
        REN_B1_i, CLK_A1_i, CLK_B1_i, ADDR_A1_i, ADDR_B1_i, WDATA_A1_i,
        WDATA_B1_i, RDATA_A1_o, RDATA_B1_o, FLUSH1_i, BE_A2_i, BE_B2_i,
        WEN_A2_i, WEN_B2_i, REN_A2_i, REN_B2_i, CLK_A2_i, CLK_B2_i, ADDR_A2_i,
        ADDR_B2_i, WDATA_A2_i, WDATA_B2_i, RDATA_A2_o, RDATA_B2_o, FLUSH2_i,
        RAM_ID_i, PL_INIT_i, PL_ENA_i, PL_REN_i, PL_CLK_i, PL_WEN_i, PL_ADDR_i,
        PL_DATA_i, PL_INIT_o, PL_ENA_o, PL_REN_o, PL_CLK_o, PL_WEN_o,
        PL_ADDR_o, PL_DATA_o, rwm );
  input [0:43] SCAN_i; // THERE ARE 48 scan chains at the tile level : 44 to RS_BRAM, 4 to MMFF
  output [0:43] SCAN_o;
  input [0:80] mode;
  input [0:1] BE_A1_i;
  input [0:1] BE_B1_i;
  input [0:14] ADDR_A1_i;
  input [0:14] ADDR_B1_i;
  input [0:17] WDATA_A1_i;
  input [0:17] WDATA_B1_i;
  output [0:17] RDATA_A1_o;
  output [0:17] RDATA_B1_o;
  input [0:1] BE_A2_i;
  input [0:1] BE_B2_i;
  input [0:13] ADDR_A2_i;
  input [0:13] ADDR_B2_i;
  input [0:17] WDATA_A2_i;
  input [0:17] WDATA_B2_i;
  output [0:17] RDATA_A2_o;
  output [0:17] RDATA_B2_o;
  input [0:19] RAM_ID_i;
  input [0:1] PL_WEN_i;
  input [0:31] PL_ADDR_i;
  input [0:35] PL_DATA_i;
  output [0:1] PL_WEN_o;
  output [0:31] PL_ADDR_o;
  output [0:35] PL_DATA_o;
  input SCAN_EN_i, SCAN_MODE_i, GRESET_i, WEN_A1_i,
         WEN_B1_i, REN_A1_i, REN_B1_i, CLK_A1_i, CLK_B1_i, FLUSH1_i, WEN_A2_i,
         WEN_B2_i, REN_A2_i, REN_B2_i, CLK_A2_i, CLK_B2_i, FLUSH2_i, PL_INIT_i,
         PL_ENA_i, PL_REN_i, PL_CLK_i;
  output PL_INIT_o, PL_ENA_o, PL_REN_o, PL_CLK_o;
  input [0:2] rwm;

 
endmodule

