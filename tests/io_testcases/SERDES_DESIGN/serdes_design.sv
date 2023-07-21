  module serdes_design #(
    parameter DATA_RATE = "SDR",
	parameter DATA_SIZE = 10,

    parameter WIDTH = 4
  )
  (
      input clk,
      input fast_clk,
      input cdr_core_clk,
      input reset,
      input dpa_rst,
      input D,
      input pll_lock,
      output clk_out,
      output Q,
      output [5:0]delay_tap_value,
      output dpa_lock,
      output dpa_error
      
  );
    
    
    logic [WIDTH-1:0] data_register,des_data,processedData;
 
    logic loaded, ser_data, channel_bond_sync_out;
    
    assign Q = ser_data;
    assign clk_out = clk_out1 & clk_out2;
    assign delay_tap_value = delay_tap_value_1 & delay_tap_value_2;

  always @(posedge clk) begin
    if (reset) begin
      data_register <= 4'b0;
      loaded <= 1'b0;
    end else if (load_data) begin
      data_register <= des_data;
      loaded <= 1'b1;
    end else begin
      data_register <= data_register;
      loaded <= loaded;
    end
  end
  
   always @* begin
    // Processing logic goes here
    processedData = data_register + 1; // Add '1 to each data element
    processedData = processedData << DATA_SIZE; // Left shift by DATA_SIZE
    // Add more operations as needed
  end
  
  
  // Instantiate the I_SERDES module
  I_SERDES #(
    .DATA_RATE(DATA_RATE),
    .WIDTH(WIDTH),
    .DPA_MODE("NONE"),
    .DELAY(6'd0)
  ) u_iserdes (
    .D(D),
    .RST(reset),
    .DPA_RST(dpa_rst),
    .FIFO_RST(reset),
    .DLY_LOAD(1'b1),
    .DLY_ADJ(1'b0),
    .DLY_INCDEC(1'b0),
    .BITSLIP_ADJ(1'b0),
    .EN(1'b1),
    .CLK_IN(clk),
    .PLL_FAST_CLK(fast_clk),
    .FAST_PHASE_CLK({fast_clk,fast_clk,fast_clk,fast_clk}),
    .PLL_LOCK(pll_lock),
    .CLK_OUT(clk_out1),
    .CDR_CORE_CLK(cdr_core_clk),
    .Q(des_data),
    .DATA_VALID(load_data),
    .DLY_TAP_VALUE(delay_tap_value_1),
    .DPA_LOCK(dpa_lock),
    .DPA_ERROR(dpa_error)
  );
  
    // Instantiate the oserdes module
  O_SERDES #(
    .DATA_RATE(DATA_RATE),   // DDR, SDR
    .CLOCK_PHASE(8'd0),     // 0, 90, 180,270
    .WIDTH(WIDTH),           // 3,4,6,7,8,9,10
    .DELAY(6'd0)
  ) u_oserdes (
    .D(processedData),
    .RST(reset),
    .LOAD_WORD(loaded),
    .DLY_LOAD(1'b0),
    .DLY_ADJ(1'b0),
    .DLY_INCDEC(1'b0),
    .CLK_EN(1'b1),
    .CLK_IN(clk),
    .PLL_LOCK(pll_lock),
    .PLL_FAST_CLK(fast_clk),
    .FAST_PHASE_CLK({fast_clk,fast_clk,fast_clk,fast_clk}),
    .OE(1'b1),
    .CLK_OUT(clk_out2),
    .Q(ser_data),
    .DLY_TAP_VALUE(delay_tap_value_2),
    .CHANNEL_BOND_SYNC_IN(1'b0),
    .CHANNEL_BOND_SYNC_OUT(channel_bond_sync_out)
  );
  endmodule