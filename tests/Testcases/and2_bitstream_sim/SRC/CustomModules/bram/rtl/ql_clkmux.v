module ql_clkmux (
input SCAN_MODE_i,
input CLK_A1_i,
input CLK_B1_i,
input CLK_A2_i,
input CLK_B2_i,
input PL_CLK_i,
input FMODE1_i,
input FMODE2_i,
input SYNC_FIFO1_i,
input SYNC_FIFO2_i,
input SPLIT_i,
input preload1,
input preload2,
output sclk_a1,
output sclk_b1,
output sclk_a2,
output sclk_b2,
output PL_CLK,
output PL_CLKn
);
wire smux_clk_a1;
wire smux_clk_b1;
wire smux_clk_a2;
wire smux_clk_b2;

// QL code
assign PL_CLK = SCAN_MODE_i ? CLK_A1_i : PL_CLK_i;
assign PL_CLKn = SCAN_MODE_i ? CLK_A1_i : ~PL_CLK_i;
assign smux_clk_a1 =  preload1 ? PL_CLK_i : CLK_A1_i;
assign smux_clk_b1 =  FMODE1_i ? (SYNC_FIFO1_i ? CLK_A1_i : CLK_B1_i) : CLK_B1_i;
assign smux_clk_a2 =  preload2 ? PL_CLK_i : SPLIT_i ? CLK_A2_i : CLK_A1_i;
assign smux_clk_b2 =  SPLIT_i ? (FMODE2_i ? (SYNC_FIFO2_i ? CLK_A2_i: CLK_B2_i) : CLK_B2_i ) : FMODE1_i ? (SYNC_FIFO1_i ? CLK_A1_i : CLK_B1_i) : CLK_B1_i;
assign sclk_a1 = SCAN_MODE_i ? CLK_A1_i : smux_clk_a1;
assign sclk_b1 = SCAN_MODE_i ? CLK_B1_i : smux_clk_b1;
assign sclk_a2 = SCAN_MODE_i ? CLK_A2_i : smux_clk_a2;
assign sclk_b2 = SCAN_MODE_i ? CLK_B2_i : smux_clk_b2;

endmodule
