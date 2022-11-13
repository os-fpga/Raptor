/////////////////////////////////////////
//  Functionality: 2-input AND 
//  Author:        Xifan Tang
////////////////////////////////////////
// `timescale 1ns / 1ps

module and2(
  a,
  b,
  c,
  clk,
  reset);

input wire a;
input wire b;
input wire clk;
input wire reset;
output c;

wire d;
reg c;

assign d = a & b;

always@(posedge clk or posedge reset) begin 
  if (reset)
    c = 0;
  else
    c = d;
end

endmodule
