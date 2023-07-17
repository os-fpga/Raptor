`timescale 1ns/1ps
//  This is an example Gate-level code to instantiate a DDR output or the post-syntehsis netlist result from output_ddr_rtl 
//  This file can be read into synthesis to have direct implementation without inferrence or is considered an optimal output netlist from
//     output_ddr_rtl.v showing primitve mapping and more optimal naming conventions for wires and instances.

module output_ddr (
  (* src = "output_ddr_rtl.v:6" *)
  input [1:0] D,
  (* src = "output_ddr_rtl.v:7" *)
  input CLK,
  (* src = "output_ddr_rtl.v:8" *)
  output DDR_OUT
);

  (* src = "output_ddr_rtl.v:6" *)
  wire D_0_i_buf;
  (* src = "output_ddr_rtl.v:6" *)
  wire D_1_i_buf;
  (* src = "output_ddr_rtl.v:7" *)
  wire CLK_i_buf;
  (* src = "output_ddr_rtl.v:7" *)
  wire CLK_clk_buf;
  (* src = "output_ddr_rtl.v:11" *)
  wire [1:0] q_reg;
  (* src = "output_ddr_rtl.v:21" *)
  wire DDR_OUT_oddr_inst;

  (* src = "output_ddr_rtl.v:6" *)
  I_BUF #(
    .WEAK_KEEPER("NONE")
  ) D_0_i_buf_inst (
    .I(D[0]),
    .EN(1'b1),
    .O(D_0_i_buf));

  (* src = "output_ddr_rtl.v:6" *)
  I_BUF #(
    .WEAK_KEEPER("NONE")
  ) D_1_i_buf_inst (
    .I(D[1]),
    .EN(1'b1),
    .O(D_1_i_buf));

  (* src = "output_ddr_rtl.v:7" *)
  I_BUF #(
    .WEAK_KEEPER("NONE")
  ) CLK_i_buf_inst (
    .I(CLK),
    .EN(1'b1),
    .O(CLK_i_buf));
	
  (* src = "output_ddr_rtl.v:7" *)
  CLK_BUF CLK_clk_buf_inst (
    .I(CLK_i_buf),
	.O(CLK_clk_buf));

  (* src = "output_ddr_rtl.v:14" *)
  DFFRE q_reg_0_inst (
    .D(D_0_i_buf),
	.R(1'b0),
	.E(1'b1),
	.C(CLK_clk_buf),
	.Q(q_reg[0]));

  (* src = "output_ddr_rtl.v:14" *)
  DFFRE q_reg_1_inst (
    .D(D_1_i_buf),
	.R(1'b0),
	.E(1'b1),
	.C(CLK_clk_buf),
	.Q(q_reg[1]));

  (* src = "output_ddr_rtl.v:16" *)
  O_DDR oddr_inst (
    .D(q_reg),
	.R(1'b0),
	.E(1'b1),
	.C(CLK_clk_buf),
	.Q(DDR_OUT_oddr_inst));

  (* src = "output_ddr_rtl.v:8" *)
  O_BUF #(
    .WEAK_KEEPER("NONE")
  ) DDR_OUT_o_buf_inst (
    .I(DDR_OUT_oddr_inst),
    .O(DDR_OUT));

endmodule
