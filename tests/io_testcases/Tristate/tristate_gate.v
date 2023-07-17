`timescale 1ns/1ps
//  This is an example Gate-level code to instantiate a 3-state output or the post-syntehsis netlist result from tristate_rtl 
//  This file can be read into synthesis to have direct implementation without inferrence or is considered an optimal output netlist from
//     tristate_rtl.v showing primitve mapping and more optimal naming conventions for wires and instances.

module tristate (
  (* src = "tristate_rtl.v:5" *)
  input D,
  (* src = "tristate_rtl.v:6" *)
  input CLK,
  (* src = "tristate_rtl.v:7" *)
  output OE,
  (* src = "tristate_rtl.v:8" *)
  output T_OUT
);

  (* src = "tristate_rtl.v:5" *)
  wire D_i_buf;
  (* src = "tristate_rtl.v:6" *)
  wire CLK_i_buf;
  (* src = "tristate_rtl.v:6" *)
  wire CLK_clk_buf;
  (* src = "tristate_rtl.v:7" *)
  wire OE_i_buf;
  (* src = "tristate_rtl.v:11" *)
  wire q_reg;

  (* src = "tristate_rtl.v:5" *)
  I_BUF #(
    .WEAK_KEEPER("NONE")
  ) D_i_buf_inst (
    .I(D),
    .EN(1'b1),
    .O(D_i_buf));

  (* src = "tristate_rtl.v:6" *)
  I_BUF #(
    .WEAK_KEEPER("NONE")
  ) CLK_i_buf_inst (
    .I(CLK),
    .EN(1'b1),
    .O(CLK_i_buf));
	
  (* src = "tristate_rtl.v:6" *)
  CLK_BUF CLK_clk_buf_inst (
    .I(CLK_i_buf),
	.O(CLK_clk_buf));

  (* src = "tristate_rtl.v:7" *)
  I_BUF #(
    .WEAK_KEEPER("NONE")
  ) OE_i_buf_inst (
    .I(OE),
    .EN(1'b1),
    .O(OE_i_buf));
	
  (* src = "tristate_rtl.v:14" *)
  dffre q_reg_inst (
    .D(D_i_buf),
	.R(1'b0),
	.E(1'b1),
	.C(CLK_clk_buf),
	.Q(q_reg));

  (* src = "tristate_rtl.v:16" *)
  O_BUFT #(
    .WEAK_KEEPER("NONE")
  ) T_OUT_o_buf_inst (
    .I(q_reg),
    .OE(OE_i_buf),
    .O(T_OUT));

endmodule
