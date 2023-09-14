module counter (input wire clock0, input wire reset, output wire [3:0] out);

   
   UP_COUNTER inst (clock0, reset, out);
   
   
endmodule
