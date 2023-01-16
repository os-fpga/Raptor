`celldefine
module RS_BRAM ( SCAN_EN_i, SCAN_MODE_i, SCAN_CLK_i, SCAN_RESET_i,
                 SCAN_i, SCAN_o, mode, GRESET_i, WEN_A1_i, WEN_B1_i,
                 BE_A1_i, BE_B1_i, REN_A1_i, REN_B1_i, CLK_A1_i,
                 CLK_B1_i, ADDR_A1_i, ADDR_B1_i, WDATA_A1_i,
                 WDATA_B1_i, RDATA_A1_o, RDATA_B1_o, FLUSH1_i,
                 BE_A2_i, BE_B2_i, WEN_A2_i, WEN_B2_i, REN_A2_i,
                 REN_B2_i, CLK_A2_i, CLK_B2_i, ADDR_A2_i, ADDR_B2_i,
                 WDATA_A2_i, WDATA_B2_i, RDATA_A2_o, RDATA_B2_o, FLUSH2_i,
                 RAM_ID_i, PL_INIT_i, PL_ENA_i, PL_REN_i, PL_CLK_i, PL_WEN_i,
                 PL_ADDR_i, PL_DATA_i, PL_INIT_o, PL_ENA_o, PL_REN_o, PL_CLK_o,
                 PL_WEN_o, PL_ADDR_o, PL_DATA_o );

  input SCAN_EN_i;
  input SCAN_MODE_i;
  input SCAN_CLK_i;
  input SCAN_RESET_i;
  input [0:27] SCAN_i;
  input [0:80] mode;
  input GRESET_i;
  input WEN_A1_i, WEN_B1_i;
  input [0:1] BE_A1_i, BE_B1_i;
  input REN_A1_i, REN_B1_i;
  input CLK_A1_i, CLK_B1_i;
  input [0:14] ADDR_A1_i, ADDR_B1_i;
  input [0:17] WDATA_A1_i, WDATA_B1_i;
  input FLUSH1_i;
  input WEN_A2_i, WEN_B2_i;
  input [0:1] BE_A2_i, BE_B2_i;
  input REN_A2_i, REN_B2_i;
  input CLK_A2_i, CLK_B2_i;
  input [0:13] ADDR_A2_i, ADDR_B2_i;
  input [0:17] WDATA_A2_i, WDATA_B2_i;
  input FLUSH2_i;
  input [0:19] RAM_ID_i;
  input PL_INIT_i, PL_ENA_i, PL_REN_i, PL_CLK_i;
  input [0:1] PL_WEN_i;
  input [0:31] PL_ADDR_i;
  input [0:35] PL_DATA_i;
  output PL_INIT_o, PL_ENA_o, PL_REN_o, PL_CLK_o;
  output [0:1] PL_WEN_o;
  output [0:31] PL_ADDR_o;
  output [0:35] PL_DATA_o;
  output [0:27] SCAN_o;
  output [0:17] RDATA_A1_o, RDATA_B1_o;
  output [0:17] RDATA_A2_o, RDATA_B2_o;

endmodule
`endcelldefine
