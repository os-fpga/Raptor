
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
 
