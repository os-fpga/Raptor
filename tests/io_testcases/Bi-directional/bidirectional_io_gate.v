`timescale 1ns/1ps
//  This is an example Gate-level code to instantiate a bi-directionbal I/O or the post-syntehsis netlist result from bidirectional_io_rtl 
//  This file can be read into synthesis to have direct implementation without inferrence or is considered an optimal output netlist from
//     bidirectional_io_rtl.v showing primitve mapping and more optimal naming conventions for wires and instances.

module bidirectional_io (
  (* src = "bidirectional_io_rtl.v:5" *)
  input BIDIR_IN,
  (* src = "bidirectional_io_rtl.v:6" *)
  input CLK,
  (* src = "bidirectional_io_rtl.v:7" *)
  output OE,
  (* src = "bidirectional_io_rtl.v:8" *)
  output BIDIR_OUT,
  (* src = "bidirectional_io_rtl.v:9" *)
  inout BIDIR
);

  (* src = "bidirectional_io_rtl.v:5" *)
  wire BIDIR_IN_i_buf;
  (* src = "bidirectional_io_rtl.v:6" *)
  wire CLK_i_buf;
  (* src = "bidirectional_io_rtl.v:6" *)
  wire CLK_clk_buf;
  (* src = "bidirectional_io_rtl.v:7" *)
  wire OE_i_buf;
  (* src = "bidirectional_io_rtl.v:12" *)
  wire q_reg;
  (* src = "bidirectional_io_rtl.v:9" *)
  wire BIDIR_i_buf;
  wire BIDIR_OUT_reg;

  (* src = "bidirectional_io_rtl.v:5" *)
  I_BUF #(
    .WEAK_KEEPER("NONE")
  ) BIDIR_IN_i_buf_inst (
    .I(BIDIR_IN),
    .EN(1'b1),
    .O(BIDIR_IN_i_buf));

  (* src = "bidirectional_io_rtl.v:6" *)
  I_BUF #(
    .WEAK_KEEPER("NONE")
  ) CLK_i_buf_inst (
    .I(CLK),
    .EN(1'b1),
    .O(CLK_i_buf));
	
  (* src = "bidirectional_io_rtl.v:6" *)
  CLK_BUF CLK_clk_buf_inst (
    .I(CLK_i_buf),
	.O(CLK_clk_buf));

  (* src = "bidirectional_io_rtl.v:7" *)
  I_BUF #(
    .WEAK_KEEPER("NONE")
  ) OE_i_buf_inst (
    .I(OE),
    .EN(1'b1),
    .O(OE_i_buf));
	
  (* src = "bidirectional_io_rtl.v:15" *)
  dffre q_reg_inst (
    .D(BIDIR_IN_i_buf),
	.R(1'b0),
	.E(1'b1),
	.C(CLK_clk_buf),
	.Q(q_reg));

  (* src = "bidirectional_io_rtl.v:16" *)
  dffre BIDIR_OUT_reg_inst (
    .D(BIDIR_i_buf),
	.R(1'b0),
	.E(1'b1),
	.C(CLK_clk_buf),
	.Q(BIDIR_OUT_reg));

  (* src = "bidirectional_io_rtl.v:16" *)
  O_BUF #(
    .WEAK_KEEPER("NONE")
  ) BIDIR_OUT_o_buf_inst (
    .I(BIDIR_OUT_reg),
    .O(BIDIR_OUT));

  (* src = "bidirectional_io_rtl.v:8" *)
  I_BUF #(
    .WEAK_KEEPER("NONE")
  ) BIDIR_i_buf_inst (
    .I(BIDIR),
    .EN(1'b1),
    .O(BIDIR_i_buf));
	
  (* src = "bidirectional_io_rtl.v:9" *)
  O_BUFT #(
    .WEAK_KEEPER("NONE")
  ) BIDIR_o_buf_inst (
    .I(q_reg),
    .OE(OE_i_buf),
    .O(BIDIR));

endmodule
