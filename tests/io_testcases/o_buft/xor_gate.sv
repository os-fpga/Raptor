module xor_gate (
    input logic a,
    input logic oe,
    output logic y
);

    // Combinational logic
    logic x;
  always_comb begin
    if (oe)
      x = a ^ 1'b1;
    else
      x = 0;
  end
  O_BUFT #(
    .SLEW_RATE("SLOW"),
    .DELAY(0)
  ) myBuffer (
    .I(x),
    .OE(oe),
    .O(y)
  );


endmodule
