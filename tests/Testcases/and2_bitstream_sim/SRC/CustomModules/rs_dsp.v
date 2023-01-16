/////////////////////////////////////////////////////////////
// Created by: Synopsys DC Ultra(TM) in wire load mode
// Version   : S-2021.06-SP2
// Date      : Tue Sep 27 11:06:20 2022
/////////////////////////////////////////////////////////////


module RS_DSP ( greset, reset, clock, feedback, load_acc, unsigned_a,
        unsigned_b, a_i, acc_fir_i, b_i, z_o, dly_b_o, saturate_enable,
        shift_right, round, subtract, mode, scan_i, scan_mode, scan_o, scan_en );
  input [0:2] feedback;
  input [0:19] a_i;
  input [0:5] acc_fir_i;
  input [0:17] b_i;
  output [0:37] z_o;
  output [0:17] dly_b_o;
  input [0:5] shift_right;
  input [0:83] mode;
  input [0:19] scan_i;
  output [0:19] scan_o;
  input greset, reset, clock, load_acc, unsigned_a, unsigned_b,
         saturate_enable, round, subtract, scan_mode, scan_en;


endmodule

