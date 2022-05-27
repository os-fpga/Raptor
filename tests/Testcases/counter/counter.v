module counter (clk, reset, result);

	input [3:0] clk;
	input reset;
	output [7:0] result;

	reg [7:0] result;

    initial begin
      result <= 0;
    end

	always @(posedge clk[0] or posedge reset)
	begin
		if (reset) 
			result = 0;		
		else 
			result = result + 1;
	end
endmodule		
