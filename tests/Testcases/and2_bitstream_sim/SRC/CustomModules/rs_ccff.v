`timescale 1ns/1ps
// https://openfpga.readthedocs.io/en/master/manual/arch_lang/circuit_model_examples/#configuration-chain-flip-flop-with-configure-enable-signals

module RS_CCFF (
  CFG_EN, // Config enable active, 0=configuration mode
  Q,      // Q output for shift chain
  CK,     // Clock Input
  D,      // Data Input
  MEM,    // connected to the datapath
  MEMB    // connected to the datapath
);

input D;
input CK;
input CFG_EN;
output Q;
output MEMB;
output MEM;

  // A=W*0.48
  // Total W =2.208
  INVRTND1BWP7D5T16P96CPD CCFF_MEMB (.I(MEM), .ZN(MEMB));            // W=0.192 um^2
  DFQD1BWP7D5T16P96CPD CCFF (.Q(Q), .CP(CK), .D(D));            // W=1.728 um^2
  AN2D1BWP7D5T16P96CPD CCFF_CE (.Z(MEM), .A1(Q), .A2(CFG_EN));  // W=0.480 um^2

endmodule
