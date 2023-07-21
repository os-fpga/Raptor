 `timescale 1ns/1ps
//  This is an example RTL code to infer a bi-directional I/O

module bidirectional_io (
  input BIDIR_IN,
  input CLK,
  input OE,
  output reg BIDIR_OUT = 1'b0,
  inout BIDIR
);

  reg q_reg = 1'b0;

  always @(posedge CLK) begin
    q_reg <= BIDIR_IN;
    BIDIR_OUT <= BIDIR;
  end
  
  assign BIDIR = OE ? q_reg : 1'bz;
	
endmodule