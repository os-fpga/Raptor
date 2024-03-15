`timescale 1ns / 1ps
////////////////////////////////////////////////////////
// Rapid Silicon Raptor Example Design                //
// and2_verilog                                       //
// and2.v - Top-level file of simple 2-input AND gate //
////////////////////////////////////////////////////////

module and2 (
  input [1:0]      a,
  input [1:0]      b,
  input            clk,
  input            reset,
  output reg [1:0] c = 2'b0
);

  reg [1:0] a_reg = 2'b0;
  reg [1:0] b_reg = 2'b0;

  always@(posedge clk)
    if (reset) begin
      a_reg <= 2'b0;
      b_reg <= 2'b0;
      c     <= 2'b0;
    end else begin
      a_reg <= a;
      b_reg <= b;
      c     <= a_reg & b_reg;
    end

endmodule
