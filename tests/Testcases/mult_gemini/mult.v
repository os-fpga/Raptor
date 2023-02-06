module dsp_mult #(parameter A_WIDTH = 2, B_WIDTH = 2) (clk, reset, A, B, P);
	input clk, reset;
	input  [A_WIDTH-1:0] A;
	input  [B_WIDTH-1:0] B;
	output   reg [A_WIDTH + B_WIDTH-1:0] P;
	reg signed [A_WIDTH-1:0] i1;
	reg signed [B_WIDTH-1:0] i2;
	reg  [A_WIDTH + B_WIDTH-1:0] mul;
	always @(posedge clk) begin
			i1 <= A;
			i2 <= B;
		end

always @(posedge clk) begin
		P <= mul;
end

always @ (*)  begin
	mul  = i1 * i2;
end

endmodule
