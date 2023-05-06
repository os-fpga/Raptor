module tb();
   wire out;
   
   add__a_to_output dut(out);
   initial begin 
   #1
     $display("out=%d", out);
      
   end
endmodule
