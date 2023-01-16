`celldefine
module RS_DSP ( greset, reset, clock, feedback, load_acc, unsigned_a,
                unsigned_b, a_i, acc_fir_i, b_i, z_o, dly_b_o,
                saturate_enable, shift_right, round, subtract,
                mode, scan_i, scan_clk, scan_mode,
                scan_reset, scan_en, scan_o );

  output [0:37] z_o;
  output [0:17] dly_b_o;
  output [0:9] scan_o;
  input scan_en;
  input scan_mode;
  input scan_clk;
  input [0:19] a_i;
  input [0:5] acc_fir_i;
  input [0:17] b_i;
  input greset;
  input scan_reset;
  input reset;
  input [0:2] feedback;
  input load_acc;
  input unsigned_a;
  input unsigned_b;
  input saturate_enable;
  input [0:5] shift_right;
  input round;
  input subtract;
  input [0:9] scan_i;
  input clock;
  input [0:83] mode;

endmodule
`endcelldefine
