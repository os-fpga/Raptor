`timescale 1ns / 1ps
    /////////////////////////////////////////////////////////////
   // Rapid Silicon Raptor Example Design                      //
  // counter_verilog                                           //
 // counter.v - Top-level file of paramatizable 32-bit counter //
/////////////////////////////////////////////////////////////////

module counter #(
  parameter counter_width = 32
) (
  input clk,
  input reset,
  output reg [counter_width-1:0] result = {counter_width{1'b0}}
);

  always @(posedge clk)
    if (reset) 
      result = {counter_width{1'b0}};		
    else 
      result = result + 1'b1;

endmodule		
