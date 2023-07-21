module i_bufds (
  input logic clk,
  input logic I_N,
  input logic I_P,
  input logic oe,
  output logic buffered_output
);
  
  logic buf1_result, result;
  logic [1:0] cnt;

  I_BUF_DS #(
    .SLEW_RATE("SLOW"),
    .DELAY(0)
  ) myBuffer0 (
    .OE(oe),
    .C(clk),
    .I_N(I_N),
    .I_P(I_P),
    .O(buf1_result)
  );
  
  
  always@(posedge clk) begin
  if(oe)
   cnt <= cnt + 1;
  end
  
  // Complex combinational circuit
  always_comb begin
    case (cnt)
      2'b00: result = buf1_result;  
      2'b01: result = buf1_result & oe;  
      2'b10: result = buf1_result | oe;  
      2'b11: result = buf1_result ^ oe;  
      default: result = buf1_result;  
    endcase
  end

  I_BUF_DS #(
    .SLEW_RATE("SLOW"),
    .DELAY(0)
  ) myBuffer1 (
    .OE(oe),
    .C(clk),
    .I_N(1'b0),
    .I_P(result),
    .O(buffered_output)
  );
  
endmodule
