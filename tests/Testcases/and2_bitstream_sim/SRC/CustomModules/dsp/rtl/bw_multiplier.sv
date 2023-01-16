//---- Default nettype ----
`default_nettype wire
module bw_multiplier #(
    parameter unsigned NBITS_A = 4,
    parameter unsigned NBITS_B = 4
) (
    input  logic [NBITS_A-1:0]              a_i,
    input  logic                            a_is_signed_i,
    input  logic [NBITS_B-1:0]              b_i,
    input  logic                            b_is_signed_i,
    output logic [NBITS_A + NBITS_B - 1:0]  z_o
);
  logic [NBITS_A-1:0] row_inputs [NBITS_B];
  // verilator lint_off UNOPTFLAT
  logic [NBITS_A-1:0] carry_inputs [NBITS_B+1];
  logic [NBITS_A-1:0] sum_inputs [NBITS_B+1];
  // verilator lint_on UNOPTFLAT
  logic [NBITS_B-1:0] partial_result;

  assign carry_inputs[0] = '0;
  assign sum_inputs[0] = '0;

  genvar row;
  generate
    for (row = 0; row < NBITS_B; row++) begin: g_gen_row
      assign row_inputs[row] = (row != (NBITS_B-1))?
                                  (b_i[row]? {a_is_signed_i ^ a_i[NBITS_A-1], a_i[NBITS_A-2:0]}
                                    : {a_is_signed_i,{(NBITS_A-1){1'b0}}})
                                  : (b_i[row]? {a_is_signed_i ^ b_is_signed_i ^ a_i[NBITS_A-1],
                                      b_is_signed_i?~a_i[NBITS_A-2:0]:a_i[NBITS_A-2:0]}
                                    : {a_is_signed_i ^ b_is_signed_i,
                                       b_is_signed_i? {(NBITS_A-1){1'b1}} : {(NBITS_A-1){1'b0}}});
      assign partial_result[row] = carry_inputs[row][0] ^ sum_inputs[row][0] ^ row_inputs[row][0];
      assign carry_inputs[row+1] = (sum_inputs[row] & row_inputs[row])
                                    | (carry_inputs[row] & (sum_inputs[row] ^ row_inputs[row]));
      assign sum_inputs[row+1] =  {
                                    (row == NBITS_B-1)
                                    ? a_is_signed_i | b_is_signed_i
                                    : 1'b0
                                  ,
                                    carry_inputs[row][NBITS_A-1:1]
                                    ^ sum_inputs[row][NBITS_A-1:1]
                                    ^ row_inputs[row][NBITS_A-1:1]
                                  };
    end
  endgenerate
  assign z_o[NBITS_A + NBITS_B - 1:0] = {{(NBITS_A){1'b0}},partial_result}
                                      + {sum_inputs[NBITS_B], {(NBITS_B){1'b0}}}
                                      + {carry_inputs[NBITS_B], {(NBITS_B){1'b0}}}
                                      + {{(NBITS_B){1'b0}}, a_is_signed_i, {(NBITS_A-1){1'b0}}}
                                      + {{(NBITS_A){1'b0}}, b_is_signed_i, {(NBITS_B-1){1'b0}}};
endmodule // bw_multiplier
`default_nettype none
