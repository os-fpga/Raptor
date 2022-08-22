module top (
  a,
  b,
  c,
  o);

input wire a;
input wire b;
input wire c;
output wire o;
   
wire internal;

assign o = (c) ? internal : a;
assign internal = (c) ? b : a;
   
endmodule
