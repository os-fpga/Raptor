`timescale 1ns/1ps
//  This is an example RTL code to instantiate an input DDR register
//  In theory, this rgeistercould be inferred but no noe does that and suggest we only support instantiation

module input_ddr (
  input  D,
  input CLK,
  output reg [1:0] Q = 2'b00
);

  wire [1:0] ddr_out;
  
  always @(posedge CLK)
    Q <= ddr_out;

  I_DDR iddr_inst (
    .D(D),
	.R(1'b0),
	.E(1'b1),
	.C(CLK),
	.Q(ddr_out));  //  Q is a 2-bit output

endmodule