`default_nettype wire
module   sram1024x18(
		     input logic clk_a,
		     input logic cen_a, 
		     input logic wen_a,
		     input logic [9:0] addr_a,
		     input logic [17:0] wmsk_a,
		     input logic [17:0] wdata_a,
		     output logic [17:0] rdata_a,

		     input logic clk_b,
		     input logic cen_b, 
		     input logic wen_b,
		     input logic [9:0] addr_b,
		     input logic [17:0] wmsk_b,
		     input logic [17:0] wdata_b,
		     output logic [17:0] rdata_b );
   
/* verilator lint_off UNOPT */
   logic [17:0] 	ram [1023:0];
/* verilator lint_on UNOPT */
   logic [9:0] 	laddr_a;
   logic [9:0] 	laddr_b;
/* verilator lint_off UNOPT */
   logic 		lcen_a;
   logic 		lwen_a;
/* verilator lint_on UNOPT */
   logic [17:0]		lwdata_a;
/* verilator lint_off UNOPT */
   logic 		lcen_b;
   logic 		lwen_b;
/* verilator lint_on UNOPT */
   logic [17:0]		lwdata_b;
   logic [17:0] 	lwmsk_a, lwmsk_b;
   

   
   
   always@(posedge clk_a) begin
      laddr_a <= addr_a;
      lwdata_a <= wdata_a;
      lwmsk_a <= wmsk_a;
      lcen_a <= cen_a;
      lwen_a <= wen_a;
   end

   always@(posedge clk_b) begin
      laddr_b <= addr_b;
      lwdata_b <= wdata_b;
      lwmsk_b <= wmsk_b;
      lcen_b <= cen_b;
      lwen_b <= wen_b;
   end

   always@(*) begin
      if ((lwen_b == 0) && (lcen_b == 0)) begin
	 ram[laddr_b][0] = lwmsk_b[0] ? ram[laddr_b][0] : lwdata_b[0];
	 ram[laddr_b][1] = lwmsk_b[1] ? ram[laddr_b][1] : lwdata_b[1];
	 ram[laddr_b][2] = lwmsk_b[2] ? ram[laddr_b][2] : lwdata_b[2];
	 ram[laddr_b][3] = lwmsk_b[3] ? ram[laddr_b][3] : lwdata_b[3];
	 ram[laddr_b][4] = lwmsk_b[4] ? ram[laddr_b][4] : lwdata_b[4];
	 ram[laddr_b][5] = lwmsk_b[5] ? ram[laddr_b][5] : lwdata_b[5];
	 ram[laddr_b][6] = lwmsk_b[6] ? ram[laddr_b][6] : lwdata_b[6];
	 ram[laddr_b][7] = lwmsk_b[7] ? ram[laddr_b][7] : lwdata_b[7];
	 ram[laddr_b][8] = lwmsk_b[8] ? ram[laddr_b][8] : lwdata_b[8];
	 ram[laddr_b][9] = lwmsk_b[9] ? ram[laddr_b][9] : lwdata_b[9];
	 ram[laddr_b][10] = lwmsk_b[10] ? ram[laddr_b][10] : lwdata_b[10];
	 ram[laddr_b][11] = lwmsk_b[11] ? ram[laddr_b][11] : lwdata_b[11];
	 ram[laddr_b][12] = lwmsk_b[12] ? ram[laddr_b][12] : lwdata_b[12];
	 ram[laddr_b][13] = lwmsk_b[13] ? ram[laddr_b][13] : lwdata_b[13];
	 ram[laddr_b][14] = lwmsk_b[14] ? ram[laddr_b][14] : lwdata_b[14];
	 ram[laddr_b][15] = lwmsk_b[15] ? ram[laddr_b][15] : lwdata_b[15];
	 ram[laddr_b][16] = lwmsk_b[16] ? ram[laddr_b][16] : lwdata_b[16];
	 ram[laddr_b][17] = lwmsk_b[17] ? ram[laddr_b][17] : lwdata_b[17];
	 lwen_b = 1;
      end // if ((lwen_b == 0) && (lcen_b == 0))
      if (lcen_b == 0) begin
	 rdata_b = ram[laddr_b];
	 lcen_b = 1;
      end else
	rdata_b = rdata_b;
   end // always@ (*)
   
   always@(*) begin   
      if ((lwen_a == 0) && (lcen_a == 0)) begin
	 ram[laddr_a][0] = lwmsk_a[0] ? ram[laddr_a][0] : lwdata_a[0];
	 ram[laddr_a][1] = lwmsk_a[1] ? ram[laddr_a][1] : lwdata_a[1];
	 ram[laddr_a][2] = lwmsk_a[2] ? ram[laddr_a][2] : lwdata_a[2];
	 ram[laddr_a][3] = lwmsk_a[3] ? ram[laddr_a][3] : lwdata_a[3];
	 ram[laddr_a][4] = lwmsk_a[4] ? ram[laddr_a][4] : lwdata_a[4];
	 ram[laddr_a][5] = lwmsk_a[5] ? ram[laddr_a][5] : lwdata_a[5];
	 ram[laddr_a][6] = lwmsk_a[6] ? ram[laddr_a][6] : lwdata_a[6];
	 ram[laddr_a][7] = lwmsk_a[7] ? ram[laddr_a][7] : lwdata_a[7];
	 ram[laddr_a][8] = lwmsk_a[8] ? ram[laddr_a][8] : lwdata_a[8];
	 ram[laddr_a][9] = lwmsk_a[9] ? ram[laddr_a][9] : lwdata_a[9];
	 ram[laddr_a][10] = lwmsk_a[10] ? ram[laddr_a][10] : lwdata_a[10];
	 ram[laddr_a][11] = lwmsk_a[11] ? ram[laddr_a][11] : lwdata_a[11];
	 ram[laddr_a][12] = lwmsk_a[12] ? ram[laddr_a][12] : lwdata_a[12];
	 ram[laddr_a][13] = lwmsk_a[13] ? ram[laddr_a][13] : lwdata_a[13];
	 ram[laddr_a][14] = lwmsk_a[14] ? ram[laddr_a][14] : lwdata_a[14];
	 ram[laddr_a][15] = lwmsk_a[15] ? ram[laddr_a][15] : lwdata_a[15];
	 ram[laddr_a][16] = lwmsk_a[16] ? ram[laddr_a][16] : lwdata_a[16];
	 ram[laddr_a][17] = lwmsk_a[17] ? ram[laddr_a][17] : lwdata_a[17];
	 lwen_a = 1;
      end // if ((lwen_a == 0) && (lcen_a == 0))
      if (lcen_a == 0) begin
	 rdata_a = ram[laddr_a];
	 lcen_a = 1;
      end else
	rdata_a = rdata_a;
   end // always@ (*)

endmodule
`default_nettype none
