`timescale 1ns / 1ps
    /////////////////////////////////////////////////////////////
   // Rapid Silicon Raptor Example Design                      //
  // One flipflop example, with Verilator testbench            //
/////////////////////////////////////////////////////////////////

module dut  (input d,
              input rstn,
              input clock0,
              output reg q);
 
  always @ (posedge clock0 or negedge rstn) 
      begin
       if (!rstn)
          q <= 0;
       else
          q <= d;
      end    
endmodule
 
