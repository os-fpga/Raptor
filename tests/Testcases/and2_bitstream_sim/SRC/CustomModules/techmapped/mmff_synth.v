/////////////////////////////////////////////////////////////
// Created by: Synopsys DC Expert(TM) in wire load mode
/////////////////////////////////////////////////////////////


module MMFF ( din, scan_in, clock, reset, enable, scan_enable, scan_mode, mode,
        global_reset, out, scan_out );
  input [4:0] mode;
  input din, scan_in, clock, reset, enable, scan_enable, scan_mode,
         global_reset;
  output out, scan_out;
  wire   out, latch_q, dff_q, clock_in, dff_d, async_reset_n, n7, n8, n9;
  assign scan_out = out;

  LHCNQD1BWP7D5T16P96CPD latch ( .E(clock_in), .D(din), .CDN(async_reset_n),
        .Q(latch_q) );
  SDFCNQD1BWP7D5T16P96CPD dff ( .D(dff_d), .SI(scan_in), .SE(scan_enable),
        .CP(clock_in), .CDN(async_reset_n), .Q(dff_q) );
  CKMUX2D1BWP7D5T16P96CPD U13 ( .I0(dff_q), .I1(latch_q), .S(mode[0]), .Z(out)
         );
  IAOI21D1BWP7D5T16P96CPD U14 ( .A2(mode[4]), .A1(n7), .B(n8), .ZN(dff_d) );
  MUX2ND1BWP7D5T16P96CPD U15 ( .I0(din), .I1(dff_q), .S(n9), .ZN(n8) );
  NR2RTND1BWP7D5T16P96CPD U16 ( .A1(enable), .A2(mode[2]), .ZN(n9) );
  CKXOR2D1BWP7D5T16P96CPD U17 ( .A1(mode[1]), .A2(clock), .Z(clock_in) );
  AOI21D1BWP7D5T16P96CPD U18 ( .A1(mode[4]), .A2(n7), .B(global_reset), .ZN(
        async_reset_n) );
  INR2D1BWP7D5T16P96CPD U19 ( .A1(reset), .B1(mode[3]), .ZN(n7) );
endmodule

