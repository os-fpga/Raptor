 
module shift_register (
  input logic [3:0] data_in,
  input logic reset,
  input logic clock,
  input logic pll_fast_clock,
  input logic load_data,
  output logic clock_out,
  output logic q,
  output logic [5:0] delay_tap_value,
  input logic channel_bond_sync_in,
  output logic channel_bond_sync_out
);

  logic [3:0] data_register;
  logic loaded;

  always @(posedge clock) begin
    if (reset) begin
      data_register <= 4'b0;
      loaded <= 1'b0;
    end else if (load_data) begin
      data_register <= data_in;
      loaded <= 1'b1;
    end else begin
      data_register <= data_register;
      loaded <= loaded;
    end
  end

  // Instantiate the oserdes module
  O_SERDES #(
    .DATA_RATE("DDR"),   // DDR, SDR
    .CLOCK_PHASE(8'd0),     // 0, 90, 180,270
    .WIDTH(4'd4),           // 3,4,6,7,8,9,10
    .DELAY(6'd0)
  ) u_oserdes (
    .D(data_register),
    .RST(reset),
    .LOAD_WORD(loaded),
    .DLY_LOAD(1'b0),
    .DLY_ADJ(1'b0),
    .DLY_INCDEC(1'b0),
    .CLK_EN(1'b1),
    .CLK_IN(clock),
    .PLL_LOCK(1'b1),
    .PLL_FAST_CLK(pll_fast_clock),
    .FAST_PHASE_CLK({pll_fast_clock,pll_fast_clock,pll_fast_clock,pll_fast_clock}),
    .OE(1'b1),
    .CLK_OUT(clock_out),
    .Q(q),
    .DLY_TAP_VALUE(delay_tap_value),
    .CHANNEL_BOND_SYNC_IN(channel_bond_sync_in),
    .CHANNEL_BOND_SYNC_OUT(channel_bond_sync_out)
  );

endmodule
