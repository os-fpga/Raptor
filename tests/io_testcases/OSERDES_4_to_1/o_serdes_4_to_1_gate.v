`timescale 1ns/1ps

//  This is an example Gate-level code to instantiate a OSERDES 4-to-1 or the post-syntehsis netlist result from oserdes_4_to_1_rtl 
//  This file can be read into synthesis to have direct implementation without inferrence or is considered an optimal output netlist from
//     oserdes_4_to_1_rtl.v showing primitve mapping and more optimal naming conventions for wires and instances.

module oserdes_4_to_1 (
  (* src = "oserdes_4_to_1_rtl.v:6" *)
  input [3:0] D,
  (* src = "oserdes_4_to_1_rtl.v:7" *)
  input IO_RST,
  (* src = "oserdes_4_to_1_rtl.v:8" *)
  input CLK,
  (* src = "oserdes_4_to_1_rtl.v:9" *)
  input LOAD_WORD,
  (* src = "oserdes_4_to_1_rtl.v:10" *)
  output PLL_LOCK,
  (* src = "oserdes_4_to_1_rtl.v:11" *)
  output Q
);

  (* src = "oserdes_4_to_1_rtl.v:6" *)
  wire [3:0] D_i_buf;
  (* src = "oserdes_4_to_1_rtl.v:7" *)
  wire IO_RST_i_buf;
  (* src = "oserdes_4_to_1_rtl.v:8" *)
  wire CLK_i_buf;
  (* src = "oserdes_4_to_1_rtl.v:8" *)
  wire CLK_clk_buf;
  (* src = "oserdes_4_to_1_rtl.v:16" *)
  wire [3:0] oserdes_in;
  (* src = "oserdes_4_to_1_rtl.v:44" *) 
  wire PLL_LOCK_PLL_inst;
  (* src = "oserdes_4_to_1_rtl.v:15" *)
  wire pll_fast_clk;
  (* src = "oserdes_4_to_1_rtl.v:64" *)
  wire Q_oserdes_4_inst;
  (* src = "oserdes_4_to_1_rtl.v:9" *)
  wire LOAD_WORD_i_buf;
  wire pll_fabric_clock;

  (* src = "oserdes_4_to_1_rtl.v:6" *)
  I_BUF #(
    .WEAK_KEEPER("NONE")
  ) D_0_i_buf_inst (
    .I(D[0]),
    .EN(1'b1),
    .O(D_i_buf[0]));

  (* src = "oserdes_4_to_1_rtl.v:6" *)
  I_BUF #(
    .WEAK_KEEPER("NONE")
  ) D_1_i_buf_inst (
    .I(D[1]),
    .EN(1'b1),
    .O(D_i_buf[1]));

  (* src = "oserdes_4_to_1_rtl.v:6" *)
  I_BUF #(
    .WEAK_KEEPER("NONE")
  ) D_2_i_buf_inst (
    .I(D[2]),
    .EN(1'b1),
    .O(D_i_buf[2]));

  (* src = "oserdes_4_to_1_rtl.v:6" *)
  I_BUF #(
    .WEAK_KEEPER("NONE")
  ) D_3_i_buf_inst (
    .I(D[3]),
    .EN(1'b1),
    .O(D_i_buf[3]));

  (* src = "oserdes_4_to_1_rtl.v:7" *)
  I_BUF #(
    .WEAK_KEEPER("NONE")
  ) IO_RST_i_buf_inst (
    .I(IO_RST),
    .EN(1'b1),
    .O(IO_RST_i_buf));

  (* src = "oserdes_4_to_1_rtl.v:9" *)
  I_BUF #(
    .WEAK_KEEPER("NONE")
  ) LOAD_WORD_i_buf_inst (
    .I(LOAD_WORD),
    .EN(1'b1),
    .O(LOAD_WORD_i_buf));

  (* src = "oserdes_4_to_1_rtl.v:8" *)
  I_BUF #(
    .WEAK_KEEPER("NONE")
  ) CLK_i_buf_inst (
    .I(CLK),
    .EN(1'b1),
    .O(CLK_i_buf));
	
  (* src = "oserdes_4_to_1_rtl.v:8" *)
  CLK_BUF CLK_clk_buf_inst (
    .I(pll_fabric_clock_oserdes_4_inst),
	.O(pll_fabric_clock));

  (* src = "oserdes_4_to_1_rtl.v:19" *)
  DFFRE Q_0_inst (
    .D(D_i_buf[0]),
	.R(1'b0),
	.E(1'b1),
	.C(pll_fabric_clock),
	.Q(oserdes_in[0]));

  (* src = "oserdes_4_to_1_rtl.v:19" *)
  DFFRE Q_1_inst (
    .D(D_i_buf[1]),
	.R(1'b0),
	.E(1'b1),
	.C(pll_fabric_clock),
	.Q(oserdes_in[1]));

  (* src = "oserdes_4_to_1_rtl.v:19" *)
  DFFRE Q_2_inst (
    .D(D_i_buf[2]),
	.R(1'b0),
	.E(1'b1),
	.C(pll_fabric_clock),
	.Q(oserdes_in[2]));

  (* src = "oserdes_4_to_1_rtl.v:19" *)
  DFFRE Q_3_inst (
    .D(D_i_buf[3]),
	.R(1'b0),
	.E(1'b1),
	.C(pll_fabric_clock),
	.Q(oserdes_in[3]));

  (* src = "oserdes_4_to_1_rtl.v:24" *)
  PLL #(
    .DIVIDE_CLK_IN_BY_2("FALSE"), 
    .PLL_MULT(32),
    .PLL_DIV(2), 
    .CLK_OUT0_DIV(12), 
    .CLK_OUT1_DIV(),  
    .CLK_OUT2_DIV(),  
    .CLK_OUT3_DIV()   
  ) PLL_inst (
    .PLL_EN(1'b1),            
    .CLK_IN(CLK_i_buf),        
    .CLK_OUT0_EN(1'b1),     
    .CLK_OUT1_EN(1'b0),       
    .CLK_OUT2_EN(1'b0),    
    .CLK_OUT3_EN(1'b0),    
    .CLK_OUT0(),
    .CLK_OUT1(),
    .CLK_OUT2(),
    .CLK_OUT3(),
    .GEARBOX_FAST_CLK(pll_fast_clk),
	.LOCK(PLL_LOCK_PLL_inst));

  (* src = "oserdes_4_to_1_rtl.v:49", DIFF_MODE = "SINGLE_ENDED", CHANNEL_MASTER = "MASTER" *)
  O_SERDES #(
    .DATA_RATE("DDR"),  // SDR,DDR
    .WIDTH(4),        // 3,4,6,7,8,9,10
    .CLOCK_PHASE(0),        // 0, 90, 180, 270
    .DELAY(0)        // 0-31 to WIDTH<=5, 0-63 for WIDTH>5
  ) oserdes_4_inst (
    .D(oserdes_in),               // 3-10-bit buss based i=on WIDTH
	.RST(IO_RST_i_buf),
	.DLY_LOAD(1'b0),
	.DLY_ADJ(1'b0),
	.DLY_INCDEC(1'b0),
	.OE(1'b1),
	.CLK_EN(1'b1),
	.CLK_IN(pll_fabric_clock),
	.CLK_OUT(pll_fabric_clock_oserdes_4_inst),
	.Q(Q_oserdes_4_inst),                  
	.LOAD_WORD(LOAD_WORD_i_buf),
	.DLY_TAP_VALUE(),  // 6-bit output
        .CHANNEL_BOND_SYNC_IN(1'b0),
        .CHANNEL_BOND_SYNC_OUT(),
        .PLL_LOCK(PLL_LOCK_PLL_inst),
	.PLL_FAST_CLK(pll_fast_clk));  //  Q is a 2-bit output

  (* src = "oserdes_4_to_1_rtl.v:11" *)
  O_BUF #(
    .WEAK_KEEPER("NONE")
  ) Q_o_buf_inst (
    .I(Q_oserdes_4_inst),
    .O(Q));

  (* src = "oserdes_4_to_1_rtl.v:10" *)
  O_BUF #(
    .WEAK_KEEPER("NONE")
  ) PLL_LOCK_o_buf_inst (
    .I(PLL_LOCK_PLL_inst),
    .O(PLL_LOCK));

endmodule
