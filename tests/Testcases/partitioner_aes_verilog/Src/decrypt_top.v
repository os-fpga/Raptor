`timescale 1ns/1ps
//////////////////////////////////////////////////
// Rapid Silicon Raptor Example Design          //
// aes_decrypt_verilog                          //
// decrypt_top - Top-level file that serialized //
// input and output data to reduce needed I/O   //
//////////////////////////////////////////////////

module decrypt_top (
  input   ct,
  output  ct_rdy,
        
  input   ct_rt_en,

  input   rkey,
  output  next_rkey,
        
  output  pt,
  output  reg pt_vld = 1'b0,
        
  input   clk,
  input   rst
);

  reg   [0:127] ct_reg   = {128{1'b0}};
  reg   [0:127] rkey_reg = {128{1'b0}};
  reg   [6:0]   ct_rkey_count = {7{1'b0}};
  reg           ct_rkey_vld   = 1'b0;

  reg  [0:127] pt_reg = {128{1'b0}};
  wire [0:127] pt_wire;
  reg   [6:0]  pt_count = {7{1'b0}};
  wire         pt_vld_decrypt;

  reg  ct_vld_decrypt = 1'b0;

  always @(posedge clk)
    if (rst) begin
      ct_reg   <= {128{1'b0}};
      rkey_reg <= {128{1'b0}};
      ct_rkey_vld<= 1'b0;
	  ct_rkey_count <= {7{1'b0}};
    end else if (ct_rt_en) begin
      ct_reg   <= {ct, ct_reg[0:126]};
      rkey_reg <= {rkey, rkey_reg[0:126]};
	  ct_rkey_count <= ct_rkey_count + 1'b1;
      ct_rkey_vld<= &ct_rkey_count;
    end
  
  always @(posedge clk)
    if (rst)
      pt_reg <= {128{1'b0}};
    else if (pt_vld_decrypt)
      pt_reg <= pt_wire;
    else
      pt_reg <= {1'b0, pt_reg[0:126]};

  always @(posedge clk)
    if (rst || pt_vld_decrypt)
      pt_count <= {7{1'b0}};
    else if (pt_vld)
      pt_count <= pt_count + 1'b1;

  always @(posedge clk)
    if (rst || &pt_count)
      pt_vld   <= 1'b0;
    else if (pt_vld_decrypt)
      pt_vld   <= 1'b1;

  assign pt = pt_reg[127];

   decrypt U1 (
    .ct(ct_reg),
    .ct_vld(ct_rkey_vld),
    .ct_rdy(ct_ready),
        
    .rkey(rkey_reg),
    .rkey_vld(ct_rkey_vld),
    .next_rkey(next_rkey),
        
    .pt(pt_wire),
    .pt_vld(pt_vld_decrypt),
        
    .klen_sel(2'b00),
        
    .clk(clk),
    .rst(rst)
  );

endmodule
