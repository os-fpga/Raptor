`timescale 1ns/1ps
//  This is an example RTL code to instantiate an output DDR register
//  In theory, this rgeistercould be inferred but no noe does that and suggest we only support instantiation

module output_ddr (
  input  [1:0] D,
  input CLK,
  output DDR_OUT
);

  reg [1:0] q_reg;
  
  always @(posedge CLK)
    q_reg <= D;

  O_DDR oddr_inst (
    .D(q_reg),         //  D is a 2-bit output
	.R(1'b0),
	.E(1'b1),
	.C(CLK),
	.Q(DDR_OUT));

endmodule