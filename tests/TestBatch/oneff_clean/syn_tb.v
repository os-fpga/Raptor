module syn_tb(input logic rstn,
              input logic clk, 
              output logic rtl_q); 

  logic d;
  integer state = 0;

  // Stimulus + Model Checking
   always @ (posedge clk) begin
      if (state==0) begin       
         d <= 0;
	 state <= state+1; 
     end
      else if (state==1) begin
         d <= 1;
         state <= state+1; 	 
      end
      else if (state==2) begin
         d <= 1;
         state <= state+1;
      end
      else if (state==3) begin
	 d <= 0;
	 state <= state+1; 
      end 
      else if (state==4) begin
	 d <= 0;
	 state <= state+1; 
      end
      else if (state==5) begin
	 d <= 1;
	 state <= state+1; 
      end
      else if (state==6) begin
	 d <= 0;
	 state <= state+1; 
      end
      else if (state==7) begin
	 d <= 0;
	 state <= state+1; 
      end
      else if (state==8) begin
	 d <= 0;
	 state <= state+1; 
      end  
      $display("d = %0d, q = %0d", d, rtl_q);

   end

   dut rtl_model(d, rstn, clk, rtl_q);

 
endmodule
