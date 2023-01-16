// shim logic for BRAM preload read data muxes

module plmux_top (
// SOC intf
input [35:0] pl_data_i,
input [31:0] pl_addr_i,
input pl_ena_i,
input pl_clk_i,
input pl_ren_i,
input pl_init_i,
input [1:0] pl_wen_i,
output [35:0] pl_data_o,
// FABRIC intf
output [73:0] PL_IN_19,
output [73:0] PL_IN_27,
output [73:0] PL_IN_35,
output [73:0] PL_IN_43,
output [73:0] PL_IN_51,
output [73:0] PL_IN_59,
output [73:0] PL_IN_67,
input [35:0] PL_DATA_OUT_19,
input [35:0] PL_DATA_OUT_27,
input [35:0] PL_DATA_OUT_35,
input [35:0] PL_DATA_OUT_43,
input [35:0] PL_DATA_OUT_51,
input [35:0] PL_DATA_OUT_59,
input [35:0] PL_DATA_OUT_67
);

assign PL_IN_19 = {pl_init_i,pl_ena_i,pl_ren_i,pl_clk_i,pl_wen_i,pl_addr_i,pl_data_i};
assign PL_IN_27 = {pl_init_i,pl_ena_i,pl_ren_i,pl_clk_i,pl_wen_i,pl_addr_i,pl_data_i};
assign PL_IN_35 = {pl_init_i,pl_ena_i,pl_ren_i,pl_clk_i,pl_wen_i,pl_addr_i,pl_data_i};
assign PL_IN_43 = {pl_init_i,pl_ena_i,pl_ren_i,pl_clk_i,pl_wen_i,pl_addr_i,pl_data_i};
assign PL_IN_51 = {pl_init_i,pl_ena_i,pl_ren_i,pl_clk_i,pl_wen_i,pl_addr_i,pl_data_i};
assign PL_IN_59 = {pl_init_i,pl_ena_i,pl_ren_i,pl_clk_i,pl_wen_i,pl_addr_i,pl_data_i};
assign PL_IN_67 = {pl_init_i,pl_ena_i,pl_ren_i,pl_clk_i,pl_wen_i,pl_addr_i,pl_data_i};

wire [35:0] data_out_1,data_out_2,data_out_3,data_out_4,data_out_5,data_out_6;
PLMUX u1(.LAST_COL_i(1'b0),.COL_ID_i(10'd19),.PL_COL_i(pl_addr_i[31:22]),.MY_COL_DATA_i(PL_DATA_OUT_19),.PREV_COL_DATA_i(data_out_1),.TO_NEXT_COL_DATA_o(pl_data_o));
PLMUX u2(.LAST_COL_i(1'b0),.COL_ID_i(10'd27),.PL_COL_i(pl_addr_i[31:22]),.MY_COL_DATA_i(PL_DATA_OUT_27),.PREV_COL_DATA_i(data_out_2),.TO_NEXT_COL_DATA_o(data_out_1));
PLMUX u3(.LAST_COL_i(1'b0),.COL_ID_i(10'd35),.PL_COL_i(pl_addr_i[31:22]),.MY_COL_DATA_i(PL_DATA_OUT_35),.PREV_COL_DATA_i(data_out_3),.TO_NEXT_COL_DATA_o(data_out_2));
PLMUX u4(.LAST_COL_i(1'b0),.COL_ID_i(10'd43),.PL_COL_i(pl_addr_i[31:22]),.MY_COL_DATA_i(PL_DATA_OUT_43),.PREV_COL_DATA_i(data_out_4),.TO_NEXT_COL_DATA_o(data_out_3));
PLMUX u5(.LAST_COL_i(1'b0),.COL_ID_i(10'd51),.PL_COL_i(pl_addr_i[31:22]),.MY_COL_DATA_i(PL_DATA_OUT_51),.PREV_COL_DATA_i(data_out_5),.TO_NEXT_COL_DATA_o(data_out_4));
PLMUX u6(.LAST_COL_i(1'b0),.COL_ID_i(10'd59),.PL_COL_i(pl_addr_i[31:22]),.MY_COL_DATA_i(PL_DATA_OUT_59),.PREV_COL_DATA_i(data_out_6),.TO_NEXT_COL_DATA_o(data_out_5));
PLMUX u7(.LAST_COL_i(1'b1),.COL_ID_i(10'd67),.PL_COL_i(pl_addr_i[31:22]),.MY_COL_DATA_i(PL_DATA_OUT_67),.PREV_COL_DATA_i(36'b0),.TO_NEXT_COL_DATA_o(data_out_6));

endmodule
