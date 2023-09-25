`timescale 1ns / 1ps
////////////////////////////////////////////////////////
// Rapid Silicon Raptor Example Design                //
// and2_verilog                                       //
// and2.v - Top-level file of simple 2-input AND gate //
////////////////////////////////////////////////////////

module Llatch( input D, input G, output reg Q);

always @*
  if (G == 1'b1)
    Q <= D;
  else
    Q<=Q;
   
endmodule

module and2 (
  input a,
  input b,
  input clk,
  input reset,
  output reg c = 1'b0,
  output wire Q
);

  reg a_reg, b_reg  = 1'b0;

   Llatch my (a,b, Q);
   
   
   
  always@(posedge clk)
    if (reset) begin
      a_reg <= 1'b0;
      b_reg <= 1'b0;
      c     <= 1'b0;
    end else begin
      a_reg <= a;
      b_reg <= b;
      c     <= a_reg & b_reg;
    end

endmodule
