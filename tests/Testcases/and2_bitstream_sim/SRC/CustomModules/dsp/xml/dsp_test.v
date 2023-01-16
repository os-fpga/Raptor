// test VPR packer
// the unsigned_a and unsigned_b inputs are driven by const0 cells which
// should be absorbed into the DSP. after routing they should be no connect.
// in this test, the feedback[2:0] control input is driven by a CLB
//

`define MULT
//`define MULT_REGIN

module DUAL_TDP18K_1Kx18(WDATA_A1,RDATA_A1,ADDR_A1,CLK_A1,REN_A1,WEN_A1,BE_A1,WDATA_B1,RDATA_B1,ADDR_B1,CLK_B1,REN_B1,WEN_B1,BE_B1,FLUSH1,RESET1,WDATA_A2,RDATA_A2,ADDR_A2,CLK_A2,REN_A2,WEN_A2,BE_A2,WDATA_B2,RDATA_B2,ADDR_B2,CLK_B2,REN_B2,WEN_B2,BE_B2,FLUSH2,RESET2);
    input [17:0] WDATA_A1;
    output [17:0] RDATA_A1;
    input [9:0] ADDR_A1;
    input CLK_A1;
    input REN_A1;
    input WEN_A1;
    input [1:0] BE_A1;
    input [17:0] WDATA_B1;
    output [17:0] RDATA_B1;
    input [9:0] ADDR_B1;
    input CLK_B1;
    input REN_B1;
    input WEN_B1;
    input [1:0] BE_B1;
    input FLUSH1;
    input RESET1;
    input [17:0] WDATA_A2;
    output [17:0] RDATA_A2;
    input [9:0] ADDR_A2;
    input CLK_A2;
    input REN_A2;
    input WEN_A2;
    input [1:0] BE_A2;
    input [17:0] WDATA_B2;
    output [17:0] RDATA_B2;
    input [9:0] ADDR_B2;
    input CLK_B2;
    input REN_B2;
    input WEN_B2;
    input [1:0] BE_B2;
    input FLUSH2;
    input RESET2;
endmodule

module RS_DSP_MULT(a,b,feedback,unsigned_a,unsigned_b,z);
input [19:0] a;
input [17:0] b;
input [2:0] feedback;
input unsigned_a;
input unsigned_b;
output [37:0] z;
endmodule

module RS_DSP_MULT_REGIN(clk,reset,a,b,feedback,unsigned_a,unsigned_b,z);
input clk;
input reset;
input [19:0] a;
input [17:0] b;
input [2:0] feedback;
input unsigned_a;
input unsigned_b;
output [37:0] z;
endmodule

module const0(const0);
output const0;
endmodule

module const1(const1);
output const1;
endmodule

module dsp_test (
input clock0,
input reset,
output parity
);

reg [17:0] a;
reg [17:0] b;
wire [17:0] c;
wire [17:0] d;
wire [37:0] z;
wire const0_0, const0_1;
const0 foo0 (.const0(const0_0));
const0 foo1 (.const0(const0_1));

always @(posedge clock0) begin
    if (reset) begin
        a <= 'd0;
        b <= 'd0;
    end
    else begin
        a <= a+'d1;
        b <= b+'d3;
    end
end

DUAL_TDP18K_1Kx18 u1 (
    .WDATA_A1(a),
    .RDATA_A1(),
    .ADDR_A1(a[9:0]),
    .CLK_A1(clock0),
    .REN_A1(1'b1),
    .WEN_A1(1'b1),
    .BE_A1(2'b11),
    .WDATA_B1(),
    .RDATA_B1(c),
    .ADDR_B1(b[9:0]),
    .CLK_B1(clock0),
    .REN_B1(1'b1),
    .WEN_B1(1'b1),
    .BE_B1(2'b11),
    .FLUSH1(1'b0),
    .RESET1(1'b0),
    .WDATA_A2(b),
    .RDATA_A2(),
    .ADDR_A2(a[9:0]),
    .CLK_A2(clock0),
    .REN_A2(1'b1),
    .WEN_A2(1'b1),
    .BE_A2(2'b11),
    .WDATA_B2(),
    .RDATA_B2(d),
    .ADDR_B2(b[9:0]),
    .CLK_B2(clock0),
    .REN_B2(1'b1),
    .WEN_B2(1'b1),
    .BE_B2(2'b11),
    .FLUSH2(1'b0),
    .RESET2(1'b0));

`ifdef MULT_REGIN
RS_DSP_MULT_REGIN u0 (
    .clk(clock0),
    .reset(reset),
    .a(c),
    .b(d),
    .z(z),
    .feedback(3'b0),
    .unsigned_a(const0_0),
    .unsigned_b(const0_1));
`endif

`ifdef MULT
RS_DSP_MULT u0 (
    .a(c),
    .b(d),
    .z(z),
    .feedback(3'b0),
    .unsigned_a(const0_0),
    .unsigned_b(const0_1));
`endif

assign parity = ^z;
endmodule		
