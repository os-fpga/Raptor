`timescale 1 ps/ 1 ps 

module use_ip( 
    input  wire axis_clk,
    input  wire axis_rst,
    input  wire axis_in_tvalid,
    input  wire axis_in_tlast,
    output wire axis_in_tready,
    input  wire [31:0] axis_in_tdata,
    output wire axis_out_tvalid,
    output wire axis_out_tlast,
    input  wire axis_out_tready,
    output wire [15:0] axis_out_tdata
    ); 

    conv32_16 U1 (
        axis_clk,
        axis_rst,
        axis_in_tvalid,
       axis_in_tlast,
        axis_in_tready,
      axis_in_tdata,
       axis_out_tvalid,
       axis_out_tlast,
      axis_out_tready,
       axis_out_tdata);

endmodule 
