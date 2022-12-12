module counter (clock0, reset, result, increment);

	input clock0;
	input reset;
        input [31:0] increment;
   
	output [31:0] result;

	reg [31:0] result;

    initial begin
      result <= 0;
    end

	always @(posedge clock0)
	begin
		if (reset) 
			result = 0;		
		else 
			result = result + increment;
	end
endmodule		
