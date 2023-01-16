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

  TDP36K_top i_bram ( .SCAN_i(SCAN_i),
        .SCAN_EN_i(SCAN_EN_i),
        .SCAN_MODE_i(SCAN_MODE_i), .SCAN_o(SCAN_o),
        .GRESET_i(GRESET_i), .WEN_A1_i(WEN_A1_i), .WEN_B1_i(
        WEN_B1_i), .BE_A1_i({BE_A1_i[1], BE_A1_i[0]}), .BE_B1_i({BE_B1_i[1],
        BE_B1_i[0]}), .REN_A1_i(REN_A1_i), .REN_B1_i(REN_B1_i), .CLK_A1_i(
        CLK_A1_i), .CLK_B1_i(CLK_B1_i), .ADDR_A1_i({ADDR_A1_i[14],
        ADDR_A1_i[13], ADDR_A1_i[12], ADDR_A1_i[11], ADDR_A1_i[10],
        ADDR_A1_i[9], ADDR_A1_i[8], ADDR_A1_i[7], ADDR_A1_i[6], ADDR_A1_i[5],
        ADDR_A1_i[4], ADDR_A1_i[3], ADDR_A1_i[2], ADDR_A1_i[1], ADDR_A1_i[0]}),
        .ADDR_B1_i({ADDR_B1_i[14], ADDR_B1_i[13], ADDR_B1_i[12], ADDR_B1_i[11],
        ADDR_B1_i[10], ADDR_B1_i[9], ADDR_B1_i[8], ADDR_B1_i[7], ADDR_B1_i[6],
        ADDR_B1_i[5], ADDR_B1_i[4], ADDR_B1_i[3], ADDR_B1_i[2], ADDR_B1_i[1],
        ADDR_B1_i[0]}), .WDATA_A1_i({WDATA_A1_i[17], WDATA_A1_i[16],
        WDATA_A1_i[15], WDATA_A1_i[14], WDATA_A1_i[13], WDATA_A1_i[12],
        WDATA_A1_i[11], WDATA_A1_i[10], WDATA_A1_i[9], WDATA_A1_i[8],
        WDATA_A1_i[7], WDATA_A1_i[6], WDATA_A1_i[5], WDATA_A1_i[4],
        WDATA_A1_i[3], WDATA_A1_i[2], WDATA_A1_i[1], WDATA_A1_i[0]}),
        .WDATA_B1_i({WDATA_B1_i[17], WDATA_B1_i[16], WDATA_B1_i[15],
        WDATA_B1_i[14], WDATA_B1_i[13], WDATA_B1_i[12], WDATA_B1_i[11],
        WDATA_B1_i[10], WDATA_B1_i[9], WDATA_B1_i[8], WDATA_B1_i[7],
        WDATA_B1_i[6], WDATA_B1_i[5], WDATA_B1_i[4], WDATA_B1_i[3],
        WDATA_B1_i[2], WDATA_B1_i[1], WDATA_B1_i[0]}), .RDATA_A1_o({
        RDATA_A1_o[17], RDATA_A1_o[16], RDATA_A1_o[15], RDATA_A1_o[14],
        RDATA_A1_o[13], RDATA_A1_o[12], RDATA_A1_o[11], RDATA_A1_o[10],
        RDATA_A1_o[9], RDATA_A1_o[8], RDATA_A1_o[7], RDATA_A1_o[6],
        RDATA_A1_o[5], RDATA_A1_o[4], RDATA_A1_o[3], RDATA_A1_o[2],
        RDATA_A1_o[1], RDATA_A1_o[0]}), .RDATA_B1_o({RDATA_B1_o[17],
        RDATA_B1_o[16], RDATA_B1_o[15], RDATA_B1_o[14], RDATA_B1_o[13],
        RDATA_B1_o[12], RDATA_B1_o[11], RDATA_B1_o[10], RDATA_B1_o[9],
        RDATA_B1_o[8], RDATA_B1_o[7], RDATA_B1_o[6], RDATA_B1_o[5],
        RDATA_B1_o[4], RDATA_B1_o[3], RDATA_B1_o[2], RDATA_B1_o[1],
        RDATA_B1_o[0]}), .FLUSH1_i(FLUSH1_i), .SYNC_FIFO1_i(mode[0]),
        .RMODE_A1_i({mode[3], mode[2], mode[1]}), .RMODE_B1_i({mode[6],
        mode[5], mode[4]}), .WMODE_A1_i({mode[9], mode[8], mode[7]}),
        .WMODE_B1_i({mode[12], mode[11], mode[10]}), .FMODE1_i(mode[13]),
        .POWERDN1_i(mode[14]), .SLEEP1_i(mode[15]), .PROTECT1_i(mode[16]),
        .UPAE1_i({mode[28], mode[27], mode[26], mode[25], mode[24], mode[23],
        mode[22], mode[21], mode[20], mode[19], mode[18], mode[17]}),
        .UPAF1_i({mode[40], mode[39], mode[38], mode[37], mode[36], mode[35],
        mode[34], mode[33], mode[32], mode[31], mode[30], mode[29]}),
        .WEN_A2_i(WEN_A2_i), .WEN_B2_i(WEN_B2_i), .BE_A2_i({BE_A2_i[1],
        BE_A2_i[0]}), .BE_B2_i({BE_B2_i[1], BE_B2_i[0]}), .REN_A2_i(REN_A2_i),
        .REN_B2_i(REN_B2_i), .CLK_A2_i(CLK_A2_i), .CLK_B2_i(CLK_B2_i),
        .ADDR_A2_i({ADDR_A2_i[13], ADDR_A2_i[12], ADDR_A2_i[11], ADDR_A2_i[10],
        ADDR_A2_i[9], ADDR_A2_i[8], ADDR_A2_i[7], ADDR_A2_i[6], ADDR_A2_i[5],
        ADDR_A2_i[4], ADDR_A2_i[3], ADDR_A2_i[2], ADDR_A2_i[1], ADDR_A2_i[0]}),
        .ADDR_B2_i({ADDR_B2_i[13], ADDR_B2_i[12], ADDR_B2_i[11], ADDR_B2_i[10],
        ADDR_B2_i[9], ADDR_B2_i[8], ADDR_B2_i[7], ADDR_B2_i[6], ADDR_B2_i[5],
        ADDR_B2_i[4], ADDR_B2_i[3], ADDR_B2_i[2], ADDR_B2_i[1], ADDR_B2_i[0]}),
        .WDATA_A2_i({WDATA_A2_i[17], WDATA_A2_i[16], WDATA_A2_i[15],
        WDATA_A2_i[14], WDATA_A2_i[13], WDATA_A2_i[12], WDATA_A2_i[11],
        WDATA_A2_i[10], WDATA_A2_i[9], WDATA_A2_i[8], WDATA_A2_i[7],
        WDATA_A2_i[6], WDATA_A2_i[5], WDATA_A2_i[4], WDATA_A2_i[3],
        WDATA_A2_i[2], WDATA_A2_i[1], WDATA_A2_i[0]}), .WDATA_B2_i({
        WDATA_B2_i[17], WDATA_B2_i[16], WDATA_B2_i[15], WDATA_B2_i[14],
        WDATA_B2_i[13], WDATA_B2_i[12], WDATA_B2_i[11], WDATA_B2_i[10],
        WDATA_B2_i[9], WDATA_B2_i[8], WDATA_B2_i[7], WDATA_B2_i[6],
        WDATA_B2_i[5], WDATA_B2_i[4], WDATA_B2_i[3], WDATA_B2_i[2],
        WDATA_B2_i[1], WDATA_B2_i[0]}), .RDATA_A2_o({RDATA_A2_o[17],
        RDATA_A2_o[16], RDATA_A2_o[15], RDATA_A2_o[14], RDATA_A2_o[13],
        RDATA_A2_o[12], RDATA_A2_o[11], RDATA_A2_o[10], RDATA_A2_o[9],
        RDATA_A2_o[8], RDATA_A2_o[7], RDATA_A2_o[6], RDATA_A2_o[5],
        RDATA_A2_o[4], RDATA_A2_o[3], RDATA_A2_o[2], RDATA_A2_o[1],
        RDATA_A2_o[0]}), .RDATA_B2_o({RDATA_B2_o[17], RDATA_B2_o[16],
        RDATA_B2_o[15], RDATA_B2_o[14], RDATA_B2_o[13], RDATA_B2_o[12],
        RDATA_B2_o[11], RDATA_B2_o[10], RDATA_B2_o[9], RDATA_B2_o[8],
        RDATA_B2_o[7], RDATA_B2_o[6], RDATA_B2_o[5], RDATA_B2_o[4],
        RDATA_B2_o[3], RDATA_B2_o[2], RDATA_B2_o[1], RDATA_B2_o[0]}),
        .FLUSH2_i(FLUSH2_i), .SYNC_FIFO2_i(mode[41]), .RMODE_A2_i({mode[44],
        mode[43], mode[42]}), .RMODE_B2_i({mode[47], mode[46], mode[45]}),
        .WMODE_A2_i({mode[50], mode[49], mode[48]}), .WMODE_B2_i({mode[53],
        mode[52], mode[51]}), .FMODE2_i(mode[54]), .POWERDN2_i(mode[55]),
        .SLEEP2_i(mode[56]), .PROTECT2_i(mode[57]), .UPAE2_i({mode[68],
        mode[67], mode[66], mode[65], mode[64], mode[63], mode[62], mode[61],
        mode[60], mode[59], mode[58]}), .UPAF2_i({mode[79], mode[78], mode[77],
        mode[76], mode[75], mode[74], mode[73], mode[72], mode[71], mode[70],
        mode[69]}), .SPLIT_i(mode[80]), .RAM_ID_i({RAM_ID_i[19], RAM_ID_i[18],
        RAM_ID_i[17], RAM_ID_i[16], RAM_ID_i[15], RAM_ID_i[14], RAM_ID_i[13],
        RAM_ID_i[12], RAM_ID_i[11], RAM_ID_i[10], RAM_ID_i[9], RAM_ID_i[8],
        RAM_ID_i[7], RAM_ID_i[6], RAM_ID_i[5], RAM_ID_i[4], RAM_ID_i[3],
        RAM_ID_i[2], RAM_ID_i[1], RAM_ID_i[0]}), .PL_INIT_i(PL_INIT_i),
        .PL_ENA_i(PL_ENA_i), .PL_REN_i(PL_REN_i), .PL_CLK_i(PL_CLK_i),
        .PL_WEN_i({PL_WEN_i[1], PL_WEN_i[0]}), .PL_ADDR_i({PL_ADDR_i[31],
        PL_ADDR_i[30], PL_ADDR_i[29], PL_ADDR_i[28], PL_ADDR_i[27],
        PL_ADDR_i[26], PL_ADDR_i[25], PL_ADDR_i[24], PL_ADDR_i[23],
        PL_ADDR_i[22], PL_ADDR_i[21], PL_ADDR_i[20], PL_ADDR_i[19],
        PL_ADDR_i[18], PL_ADDR_i[17], PL_ADDR_i[16], PL_ADDR_i[15],
        PL_ADDR_i[14], PL_ADDR_i[13], PL_ADDR_i[12], PL_ADDR_i[11],
        PL_ADDR_i[10], PL_ADDR_i[9], PL_ADDR_i[8], PL_ADDR_i[7], PL_ADDR_i[6],
        PL_ADDR_i[5], PL_ADDR_i[4], PL_ADDR_i[3], PL_ADDR_i[2], PL_ADDR_i[1],
        PL_ADDR_i[0]}), .PL_DATA_i({PL_DATA_i[35], PL_DATA_i[34],
        PL_DATA_i[33], PL_DATA_i[32], PL_DATA_i[31], PL_DATA_i[30],
        PL_DATA_i[29], PL_DATA_i[28], PL_DATA_i[27], PL_DATA_i[26],
        PL_DATA_i[25], PL_DATA_i[24], PL_DATA_i[23], PL_DATA_i[22],
        PL_DATA_i[21], PL_DATA_i[20], PL_DATA_i[19], PL_DATA_i[18],
        PL_DATA_i[17], PL_DATA_i[16], PL_DATA_i[15], PL_DATA_i[14],
        PL_DATA_i[13], PL_DATA_i[12], PL_DATA_i[11], PL_DATA_i[10],
        PL_DATA_i[9], PL_DATA_i[8], PL_DATA_i[7], PL_DATA_i[6], PL_DATA_i[5],
        PL_DATA_i[4], PL_DATA_i[3], PL_DATA_i[2], PL_DATA_i[1], PL_DATA_i[0]}),
        .PL_INIT_o(PL_INIT_o), .PL_ENA_o(PL_ENA_o), .PL_REN_o(PL_REN_o),
        .PL_CLK_o(PL_CLK_o), .PL_WEN_o({PL_WEN_o[1], PL_WEN_o[0]}),
        .PL_ADDR_o({PL_ADDR_o[31], PL_ADDR_o[30], PL_ADDR_o[29], PL_ADDR_o[28],
        PL_ADDR_o[27], PL_ADDR_o[26], PL_ADDR_o[25], PL_ADDR_o[24],
        PL_ADDR_o[23], PL_ADDR_o[22], PL_ADDR_o[21], PL_ADDR_o[20],
        PL_ADDR_o[19], PL_ADDR_o[18], PL_ADDR_o[17], PL_ADDR_o[16],
        PL_ADDR_o[15], PL_ADDR_o[14], PL_ADDR_o[13], PL_ADDR_o[12],
        PL_ADDR_o[11], PL_ADDR_o[10], PL_ADDR_o[9], PL_ADDR_o[8], PL_ADDR_o[7],
        PL_ADDR_o[6], PL_ADDR_o[5], PL_ADDR_o[4], PL_ADDR_o[3], PL_ADDR_o[2],
        PL_ADDR_o[1], PL_ADDR_o[0]}), .PL_DATA_o({PL_DATA_o[35], PL_DATA_o[34],
        PL_DATA_o[33], PL_DATA_o[32], PL_DATA_o[31], PL_DATA_o[30],
        PL_DATA_o[29], PL_DATA_o[28], PL_DATA_o[27], PL_DATA_o[26],
        PL_DATA_o[25], PL_DATA_o[24], PL_DATA_o[23], PL_DATA_o[22],
        PL_DATA_o[21], PL_DATA_o[20], PL_DATA_o[19], PL_DATA_o[18],
        PL_DATA_o[17], PL_DATA_o[16], PL_DATA_o[15], PL_DATA_o[14],
        PL_DATA_o[13], PL_DATA_o[12], PL_DATA_o[11], PL_DATA_o[10],
        PL_DATA_o[9], PL_DATA_o[8], PL_DATA_o[7], PL_DATA_o[6], PL_DATA_o[5],
        PL_DATA_o[4], PL_DATA_o[3], PL_DATA_o[2], PL_DATA_o[1], PL_DATA_o[0]}),
        .rwm(rwm)
);
endmodule

