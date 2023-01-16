`timescale 1ns/1ps
//-----------------------------------------------------
// Function    : LATCH || FF with Enable, Async, Set
//
//-----------------------------------------------------

module RS_IOFF ( D, SI, G_RESET, CK, MODE_SEL, SCAN_MODE, SCAN_ENABLE, Q );
  input D;
  input SI;
  input G_RESET;
  input CK;
  input MODE_SEL;
  input SCAN_MODE;
  input SCAN_ENABLE;
  output Q;

  wire clk;
  wire clk_out;

  assign clk = SCAN_MODE ? CK : clk_out;

  CKXOR2D1BWP7D5T16P96CPD XNOR2_CLKINV (
          .A1(MODE_SEL),        // MODE_SEL=1, clk_int = CK
          .A2(CK),              // MODE_SEL=0, clk_int = ~CK
          .Z(clk_out)
      );

  SDFCNQD1BWP7D5T16P96CPD DFF(
        .D(D),
        .SI(SI),
        .SE(SCAN_ENABLE),
        .CDN(G_RESET),
        .CP(clk),
        .Q(Q)
    );

endmodule

