/////////////////////////////////////////
//  Functionality: 2-input AND 
//  Author:        Xifan Tang
////////////////////////////////////////
// `timescale 1ns / 1ps

module and2(
  one,
  sec,
  s);

input wire one;
input wire sec;
output wire s;

assign s = one & sec;

endmodule

