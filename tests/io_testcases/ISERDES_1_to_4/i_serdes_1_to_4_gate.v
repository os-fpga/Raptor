`timescale 1ns/1ps

//  This is an example Gate-level code to instantiate a DDR input or the post-syntehsis netlist result from iserdes_1_to_4_rtl 
//  This file can be read into synthesis to have direct implementation without inferrence or is considered an optimal output netlist from
//     iserdes_1_to_4_rtl.v showing primitve mapping and more optimal naming conventions for wires and instances.

module iserdes_1_to_4 (
  (* src = "iserdes_1_to_4_rtl.v:7" *)
  input D,
  (* src = "iserdes_1_to_4_rtl.v:8" *)
  input IO_RST,
  (* src = "iserdes_1_to_4_rtl.v:9" *)
  input CLK,
  (* src = "iserdes_1_to_4_rtl.v:10" *)
  output PLL_LOCK,
  (* src = "iserdes_1_to_4_rtl.v:11" *)
  output DATA_VALID,
  (* src = "iserdes_1_to_4_rtl.v:12" *)
  output [3:0] Q
);

  (* src = "iserdes_1_to_4_rtl.v:7" *)
  wire D_i_buf;
  (* src = "iserdes_1_to_4_rtl.v:8" *)
  wire IO_RST_i_buf;
  (* src = "iserdes_1_to_4_rtl.v:9" *)
  wire CLK_i_buf;
  (* src = "iserdes_1_to_4_rtl.v:12" *)
  wire [3:0] Q_reg;
  (* src = "iserdes_1_to_4_rtl.v:17" *)
  wire [3:0] iserdes_out;
  (* src = "iserdes_1_to_4_rtl.v:45" *) 
  wire PLL_LOCK_PLL_inst;
  (* src = "iserdes_1_to_4_rtl.v:16" *)
  wire pll_fast_clk;
  (* src = "iserdes_1_to_4_rtl.v:15" *)
  wire pll_fabric_clock;
  (* src = "iserdes_1_to_4_rtl.v:72" *)
  wire pll_fabric_clock_iserdes_4_inst;
  (* src = "iserdes_1_to_4_rtl.v:67" *)
  wire DATA_VALID_iserdes_4_inst;

  (* src = "iserdes_1_to_4_rtl.v:7" *)
  I_BUF #(
    .WEAK_KEEPER("NONE")
  ) D_i_buf_inst (
    .I(D),
    .EN(1'b1),
    .O(D_i_buf));

  (* src = "iserdes_1_to_4_rtl.v:8" *)
  I_BUF #(
    .WEAK_KEEPER("NONE")
  ) IO_RST_i_buf_inst (
    .I(IO_RST),
    .EN(1'b1),
    .O(IO_RST_i_buf));

  (* src = "iserdes_1_to_4_rtl.v:9" *)
  I_BUF #(
    .WEAK_KEEPER("NONE")
  ) CLK_i_buf_inst (
    .I(CLK),
    .EN(1'b1),
    .O(CLK_i_buf));
	
  (* src = "iserdes_1_to_4_rtl.v:9" *)
  CLK_BUF pll_fabric_clock_clk_buf_inst (
    .I(pll_fabric_clock_iserdes_4_inst),
	.O(pll_fabric_clock));

  (* src = "iserdes_1_to_4_rtl.v:20" *)
  dffre Q_0_inst (
    .D(iserdes_out[0]),
	.R(1'b0),
	.E(1'b1),
	.C(pll_fabric_clock),
	.Q(Q_reg[0]));

  (* src = "iserdes_1_to_4_rtl.v:20" *)
  dffre Q_1_inst (
    .D(iserdes_out[1]),
	.R(1'b0),
	.E(1'b1),
	.C(pll_fabric_clock),
	.Q(Q_reg[1]));

  (* src = "iserdes_1_to_4_rtl.v:20" *)
  dffre Q_2_inst (
    .D(iserdes_out[2]),
	.R(1'b0),
	.E(1'b1),
	.C(pll_fabric_clock),
	.Q(Q_reg[2]));

  (* src = "iserdes_1_to_4_rtl.v:20" *)
  dffre Q_3_inst (
    .D(iserdes_out[3]),
	.R(1'b0),
	.E(1'b1),
	.C(pll_fabric_clock),
	.Q(Q_reg[3]));

  (* src = "iserdes_1_to_4_rtl.v:25" *)
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

  (* src = "iserdes_1_to_4_rtl.v:49", DIFF_MODE="SINGLE_ENDED", CHANNEL_MASTER="MASTER" *)
  I_SERDES #(
    .DATA_RATE("DDR"), 
    .WIDTH(4),        
    .DPA_MODE("NONE"), 
    .DELAY(0)       
  ) iserdes_4_inst (
    .D(D_i_buf), 
	.RST(IO_RST_i_buf),
	.FIFO_RST(IO_RST_i_buf),
	.DLY_LOAD(1'b0),
	.DLY_ADJ(1'b0),
	.DLY_INCDEC(1'b0),
	.BITSLIP_ADJ(1'b0),
	.EN(1'b1),
	.CLK_IN(pll_fabric_clock),
	.CLK_OUT(pll_fabric_clock_iserdes_4_inst),
	.Q(iserdes_out), 
	.DATA_VALID(DATA_VALID_iserdes_4_inst),
	.DLY_TAP_VALUE(),
	.DPA_LOCK(),
	.DPA_ERROR(),
	.PLL_LOCK(PLL_LOCK_PLL_inst),
	.PLL_FAST_CLK(pll_fast_clk));  

  (* src = "iserdes_1_to_4_rtl.v:12" *)
  O_BUF #(
    .WEAK_KEEPER("NONE")
  ) Q_0_o_buf_inst (
    .I(Q_reg[0]),
    .O(Q[0]));

  (* src = "iserdes_1_to_4_rtl.v:12" *)
  O_BUF #(
    .WEAK_KEEPER("NONE")
  ) Q_1_o_buf_inst (
    .I(Q_reg[1]),
    .O(Q[1]));

  (* src = "iserdes_1_to_4_rtl.v:12" *)
  O_BUF #(
    .WEAK_KEEPER("NONE")
  ) Q_2_o_buf_inst (
    .I(Q_reg[2]),
    .O(Q[2]));

  (* src = "iserdes_1_to_4_rtl.v:12" *)
  O_BUF #(
    .WEAK_KEEPER("NONE")
  ) Q_3_o_buf_inst (
    .I(Q_reg[3]),
    .O(Q[3]));

  (* src = "iserdes_1_to_4_rtl.v:10" *)
  O_BUF #(
    .WEAK_KEEPER("NONE")
  ) PLL_LOCK_o_buf_inst (
    .I(PLL_LOCK_PLL_inst),
    .O(PLL_LOCK));

  (* src = "iserdes_1_to_4_rtl.v:11" *)
  O_BUF #(
    .WEAK_KEEPER("NONE")
  ) DATA_VALID_o_buf_inst (
    .I(DATA_VALID_iserdes_4_inst),
    .O(DATA_VALID));

endmodule
