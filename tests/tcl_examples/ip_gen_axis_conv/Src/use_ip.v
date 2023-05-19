`timescale 1 ps/ 1 ps 
///////////////////////////////////////////////////////
//  Rapid Silicon Example Desgin - ip_gen_axis_conv  //
//  use_ip - top-level file instantiating conv32_16  //
//               LiteX generated IP                  //
///////////////////////////////////////////////////////

module use_ip ( 
  input axis_clk,
  input axis_rst,
  input s_axis_tvalid,
  input s_axis_tlast,
  output s_axis_tready,
  input [31:0] s_axis_tdata,
  input [3:0] s_axis_tkeep,
  input s_axis_tid,
  input s_axis_tdest,
  input s_axis_tuser,
  output m_axis_tvalid,
  output m_axis_tlast,
  input m_axis_tready,
  output [15:0] m_axis_tdata,
  output [1:0] m_axis_tkeep,
  output m_axis_tid,
  output m_axis_tdest,
  output m_axis_tuser
); 

  conv32_16 U1 (
   .axis_clk(axis_clk),
   .axis_rst(axis_rst),
   .s_axis_tvalid(s_axis_tvalid),
   .s_axis_tlast(s_axis_tlast),
   .s_axis_tready(s_axis_tready),
   .s_axis_tdata(s_axis_tdata),
   .s_axis_tkeep(s_axis_tkeep),
   .s_axis_tid(s_axis_tid),
   .s_axis_tdest(s_axis_tdest),
   .s_axis_tuser(s_axis_tuser),
   .m_axis_tvalid(m_axis_tvalid),
   .m_axis_tlast(m_axis_tlast),
   .m_axis_tready(m_axis_tready),
   .m_axis_tdata(m_axis_tdata),
   .m_axis_tkeep(m_axis_tkeep),
   .m_axis_tid(m_axis_tid),
   .m_axis_tdest(m_axis_tdest),
   .m_axis_tuser(m_axis_tuser)
  );

endmodule 
