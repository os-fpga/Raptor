module counter (clock0, reset, result);

	input clock0;
	input reset;
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
			result = result + 1;
	end
endmodule		
