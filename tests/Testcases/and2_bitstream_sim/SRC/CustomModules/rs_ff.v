`timescale 1ns/1ps
//-----------------------------------------------------
// Function    : LATCH || FF with Enable, Async, Set
//       
//-----------------------------------------------------
`default_nettype wire
module RS_FF ( cfg_done, D, SI, SE, R, RS, E, CK, SCAN_MODE, MODE_SEL, SO, Q ); 
  input cfg_done;
  //input LAT_R;  // latch reset (why?)
  input D;
  input SI;
  input SE;  
  //input S;
  input RS;     // sync reset
  input R;      // async reset
  input E;
  input CK;     // clock already inverted if necessary
  input SCAN_MODE;
  //input SCAN_CLK;
  input MODE_SEL; // 1= LATCH mode
  output SO;
  output Q;

  wire din, DMUX1;
  //wire clk_int;
  //wire clk;
  wire LATQ;
  wire mode_sel0;

//  XNR2D1BWP7D5T16P96CPD XNOR2_CLKINV (
//          .A1(MODE_SEL[1]),     // MODE_SEL=1, clk_int = CK
//          .A2(CK),              // MODE_SEL=0, clk_int = ~CK
//          .ZN(clk_int)
//      );
//  CKMUX2D1BWP7D5T16P96CPD MUX2_DFT (
//          .I0(clk_int),
//          .I1(SCAN_CLK),
//          .S(SCAN_MODE),
//          .Z(clk)
//      );

//  MUX2D1BWP30P140LVT MUX2_TESTMUX(
//              .I0(DMUX1),
//              .I1(SI),
//              .S(SE),
//              .Z(DMUX2)
//      );

  AN2D12BWP7D5T16P96CPD SYNC_RST (
        .A1(D),
        .A2(RS), 
        .Z(din)
        );

  MUX2D1BWP7D5T16P96CPD MUX2_EN(
                .I0(Q),
                .I1(din),
                .S(E),
                .Z(DMUX1)
        );

// DFCSNQD1BWP30P140LVT DFF(
//              .D(DMUX2),
//              .SDN(S),
//              .CDN(R),
//              .CP(clk),
//              .Q(DFFQ)
//      );

  SDFCNQD1BWP7D5T16P96CPD DFF(
                .D(DMUX1),
                .SI(SI),
                .SE(SE),
                .CP(CK),
                .CDN(R),
//                .SDN(S),
                .Q(SO)
        );

  LHCNQD1BWP7D5T16P96CPD  LAT(
                .D(DMUX1),
                //.SDN(S),
                //.CDN(LAT_R),  // why?
                .CDN(R),
                .E(CK),
                .Q(LATQ)
        );

  AN2D0BWP7D5T16P96CPD AND_CFG_DONE (
          .A1(MODE_SEL),
          .A2(cfg_done),             
          .Z(mode_sel0)
      );

  MUX2D1BWP7D5T16P96CPD MUX2_LATFFQ(
                .I0(SO),
                .I1(LATQ),
                .S(mode_sel0),
                .Z(Q)
        );

endmodule

