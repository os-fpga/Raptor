`timescale 1ns/1ps
//  This is an example Gate-level code to instantiate a DDR input or the post-syntehsis netlist result from input_ddr_rtl 
//  This file can be read into synthesis to have direct implementation without inferrence or is considered an optimal output netlist from
//     input_ddr_rtl.v showing primitve mapping and more optimal naming conventions for wires and instances.

module input_ddr (
  (* src = "input_ddr_rtl.v:6" *)
  input D,
  (* src = "input_ddr_rtl.v:7" *)
  input CLK,
  (* src = "input_ddr_rtl.v:8" *)
  output [1:0] Q
);

  (* src = "input_ddr_rtl.v:6" *)
  wire D_i_buf;
  (* src = "input_ddr_rtl.v:7" *)
  wire CLK_i_buf;
  (* src = "input_ddr_rtl.v:7" *)
  wire CLK_clk_buf;
  (* src = "input_ddr_rtl.v:12" *)
  wire [1:0] Q_reg;
  (* src = "input_ddr_rtl.v:12" *)
  wire [1:0] ddr_out;

  (* src = "input_ddr_rtl.v:6" *)
  I_BUF #(
    .WEAK_KEEPER("NONE")
  ) D_i_buf_inst (
    .I(D),
    .EN(1'b1),
    .O(D_i_buf));

  (* src = "input_ddr_rtl.v:7" *)
  I_BUF #(
    .WEAK_KEEPER("NONE")
  ) CLK_i_buf_inst (
    .I(CLK),
    .EN(1'b1),
    .O(CLK_i_buf));
	
  (* src = "input_ddr_rtl.v:7" *)
  CLK_BUF CLK_clk_buf_inst (
    .I(CLK_i_buf),
	.O(CLK_clk_buf));

  (* src = "input_ddr_rtl.v:14" *)
  dffre Q_0_inst (
    .D(ddr_out[0]),
	.R(1'b0),
	.E(1'b1),
	.C(CLK_clk_buf),
	.Q(Q_reg[0]));

  (* src = "input_ddr_rtl.v:14" *)
  dffre Q_1_inst (
    .D(ddr_out[1]),
	.R(1'b0),
	.E(1'b1),
	.C(CLK_clk_buf),
	.Q(Q_reg[1]));

  (* src = "input_ddr_rtl.v:16" *)
  I_DDR iddr_inst (
    .D(D_i_buf),
	.R(1'b0),
	.E(1'b1),
	.C(CLK_clk_buf),
	.Q(ddr_out));  //  Q is a 2-bit output

  (* src = "input_ddr_rtl.v:8" *)
  O_BUF #(
    .WEAK_KEEPER("NONE")
  ) Q_0_o_buf_inst (
    .I(Q_reg[0]),
    .O(Q[0]));

  (* src = "input_ddr_rtl.v:8" *)
  O_BUF #(
    .WEAK_KEEPER("NONE")
  ) Q_1_o_buf_inst (
    .I(Q_reg[1]),
    .O(Q[1]));

endmodule
