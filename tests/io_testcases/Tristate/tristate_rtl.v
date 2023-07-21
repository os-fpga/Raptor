 `timescale 1ns/1ps
//  This is an example RTL code to infer a 3-state output

module tristate (
  input D,
  input CLK,
  input OE,
  output T_OUT
);

  reg q_reg = 1'b0;

  always @(posedge CLK)
    q_reg <= D;
  
  assign T_OUT = OE ? q_reg : 1'bz;
	
endmodule