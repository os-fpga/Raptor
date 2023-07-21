`timescale 1ns/1ps
//  This is an example RTL code to instantiate a 1-to-4 input serdes
//  In theory, this rgeistercould be inferred but no noe does that and suggest we only support instantiation

module oserdes_4_to_1 (
  input [3:0] D,
  input  IO_RST,
  input  CLK,
  input  LOAD_WORD,
  output PLL_LOCK,
  output Q
);

  wire      pll_fabric_clock;
  wire      pll_fast_clock;
  reg [3:0] oserdes_in;
  
  always @(posedge pll_fabric_clock)
    oserdes_in <= D;

  //  The PLL must be instantiated by the user 
  //  This example uses a 100 MHz, M and D values create a 1.6 GHz VCO frequency

  PLL #(
    .DIVIDE_CLK_IN_BY_2("FALSE"), // Divide input clock by 2 prior to input to PLL
    .PLL_MULT(32), // Multiplier for VCO frequency, Values 16-1000
    .PLL_DIV(2),   // Divider for VCO frequency, Values 1-63
    .CLK_OUT0_DIV(12), // CLK_OUT0 output divider, Values 64,48,40,32,24,20,16,12,10,8,6,5,4,3,2
    .CLK_OUT1_DIV(),  // CLK_OUT1 output divider, Values 64,48,40,32,24,20,16,12,10,8,6,5,4,3,2
    .CLK_OUT2_DIV(),  // CLK_OUT2 output divider, Values 64,48,40,32,24,20,16,12,10,8,6,5,4,3,2
    .CLK_OUT3_DIV()   // CLK_OUT3 output divider, Values 64,48,40,32,24,20,16,12,10,8,6,5,4,3,2
  ) PLL_inst (
    .PLL_EN(1'b1),              // PLL Enable
    .CLK_IN(CLK),               // Input Clock
    .CLK_OUT0_EN(1'b1),         // CLK0 Enable
    .CLK_OUT1_EN(1'b0),         // CLK1 Enable
    .CLK_OUT2_EN(1'b0),         // CLK2 Enable
    .CLK_OUT3_EN(1'b0),         // CLK3 Enable
    .CLK_OUT0(), // CLK0 Output Clock
    .CLK_OUT1(), // CLK1 Output Clock
    .CLK_OUT2(), // CLK2 Output Clock
    .CLK_OUT3(), // CLK3 Output Clock
    .GEARBOX_FAST_CLK(pll_fast_clk),        // Fast Clock generated for the Gearbox
	.LOCK(PLL_LOCK));           // PLL Lock

  // The following Synthesis attributes are temproary until Raptor is able to sniff out things like it being connected to a differential 
  // buffer or another bonded channel
  (* DIFF_MODE="SINGLE_ENDED", CHANNEL_MASTER="MASTER" *)
  O_SERDES #(
    .DATA_RATE("DDR"),  // SDR,DDR
    .WIDTH(4),        // 3,4,6,7,8,9,10
    .CLOCK_PHASE(0),        // 0, 90, 180, 270
    .DELAY(0)        // 0-31 to WIDTH<=5, 0-63 for WIDTH>5
  ) oserdes_4_inst (
    .D(oserdes_in),               // 3-10-bit buss based i=on WIDTH
	.RST(IO_RST),
	.DLY_LOAD(1'b0),
	.DLY_ADJ(1'b0),
	.DLY_INCDEC(1'b0),
	.OE(1'b1),
	.CLK_EN(1'b1),
	.CLK_IN(pll_fabric_clock),
	.CLK_OUT(pll_fabric_clock),
	.Q(Q),                  
	.LOAD_WORD(LOAD_WORD),
	.DLY_TAP_VALUE(),  // 6-bit output
	.CHANNEL_BOND_SYNC_IN(1'b0),
	.CHANNEL_BOND_SYNC_OUT(),
	.PLL_LOCK(PLL_LOCK),
	.PLL_FAST_CLK(pll_fast_clk));  //  Q is a 2-bit output

endmodule
