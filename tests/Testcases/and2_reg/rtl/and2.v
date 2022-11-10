/////////////////////////////////////////
//  Functionality: 2-input AND 
//  Author:        Xifan Tang
////////////////////////////////////////
// `timescale 1ns / 1ps

module ff(D,clk,Q);
  input logic D, clk;   
  output reg Q; 
  always @(posedge clk) 
    begin
      Q <= D; 
    end 
endmodule

module and2(a,b,c);
  input wire a, b;
  output wire c;

assign c = a & b;

endmodule


module reg_and2(reg_a,reg_b,clk,Q);
  input logic reg_a, reg_b, clk;   
  output reg Q;
  ff ff_a(reg_a, clk, a_ff);
  ff ff_b(reg_b, clk, b_ff);
  and2 a2 (a_ff, b_ff, c);
  ff ff_c(c, clk, Q);    
endmodule
