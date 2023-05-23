`timescale 1ns / 1ps
////////////////////////////////////////////////////////
// Rapid Silicon Raptor Example Design                //
// and2_verilog                                       //
// and2.v - Top-level file of simple 2-input AND gate //
////////////////////////////////////////////////////////

module and2 (
  input a,
  input b,
  input clk,
  input reset,
  output reg c = 1'b0
);

  reg a_reg, b_reg  = 1'b0;

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


module and2x2  (
  input a,
  input b,
  input clk1,
  input clk2,
  input reset,
  output reg c1 = 1'b0
  output reg c2 = 1'b0
);
   
   and2 u1 (a,b, clk1, reset, c1);
   and2 u2 (a,b, clk2, reset, c2);


endmodule
