//--- Default net type ----
`default_nettype wire
/*****************************
 dsp_type1_bw multiplier
 July 1, 2022 - fixed saturation register value for output register select - updated July 6
 July 7, 2022 - fixed additional bug in unsigned saturation for negative numbers
 
******************************/
  
module dsp_type1_bw #(
      parameter NBITS_ACC = 64,
      parameter NBITS_A = 20,
      parameter NBITS_B = 18,
      parameter NBITS_Z = 38
    )(

      input logic                          s_reset,
      // scan control
      input logic                          scan_mode_i, 

      // Inputs from Fabric - 45 total
      input logic                          clock_i,
      input logic [2:0]                    feedback_i,
      input logic                          unsigned_a_i,
      input logic                          unsigned_b_i,
      input logic                          load_acc_i,
      input logic [NBITS_A-1:0]            a_i,
      input logic [NBITS_B-1:0]            b_i,
      input logic [5:0] acc_fir_i,

      // outputs to Fabric - 56 total
      output logic [NBITS_Z-1:0]           z_o,
      output logic [NBITS_B-1:0]           dly_b_o,

      // inputs from Configuration Bits -  12 total
      input logic                          saturate_enable_i,
      input logic [2:0]                    output_select_i,
      input logic                          round_i,
      input logic [5:0]                    shift_right_i,
      input logic                          subtract_i,
      input logic                          register_inputs_i,
      input logic [NBITS_A-1:0]         coef_0_i,
      input logic [NBITS_A-1:0]         coef_1_i,
      input logic [NBITS_A-1:0]         coef_2_i,
      input logic [NBITS_A-1:0]         coef_3_i

    ) ;

   logic                         s_clk;
   
   logic [NBITS_A-1:0]           r_a, a;
   logic [NBITS_B-1:0]           r_b, b;
   logic [5:0]           r_acc_fir, acc_fir;
           
   logic                         r_unsigned_a, unsigned_a, signed_a;
   logic                         r_unsigned_b, unsigned_b, signed_b;
   logic                         r_load_acc, load_acc;
   
   logic [2:0]                   r_feedback, feedback;
   logic [5:0]                   r_shift_d1, shift_d1;
   logic [5:0]                   r_shift_d2, shift_d2;
   
   logic                         r_subtract, subtract;

   logic                   r_sat_d1, r_sat_d2, sat_d1, sat_d2;
   logic                   r_rnd_d1, r_rnd_d2, rnd_d1, rnd_d2;



   
   logic [NBITS_A-1:0]           mult_a;
   logic [NBITS_B-1:0]           mult_b;
   logic [NBITS_A+NBITS_B-1:0]   mult;
   logic [NBITS_ACC-1:0]         z0, z1, z3; 
   logic [NBITS_ACC-1:0]         mult_xtnd;
   logic [NBITS_ACC-1:0]         add_a;
   logic [NBITS_ACC-1:0]         add_b;
   logic [NBITS_ACC-1:0]         mult_acc;
   logic [NBITS_ACC-1:0]         acc_fir_int;
   
   logic signed [NBITS_ACC-1:0]  acc;
   logic signed [NBITS_ACC-1:0]  acc_out;
   logic signed [NBITS_ACC-1:0]  round;
   logic signed [NBITS_ACC-1:0]  shift;
   logic signed [NBITS_ACC-1:0]  saturate;
   logic signed [NBITS_ACC-1:0]  unsigned_saturate;
   logic signed [NBITS_ACC-1:0]  signed_saturate;
   
   logic                         unsigned_mode;
   
   assign s_clk = clock_i;
   assign unsigned_a = register_inputs_i ? r_unsigned_a : unsigned_a_i;
   assign signed_a = ~unsigned_a;
   assign signed_b = ~unsigned_b;
   assign unsigned_b = register_inputs_i ? r_unsigned_b : unsigned_b_i;
   assign feedback = register_inputs_i ? r_feedback : feedback_i;
   assign shift_d1 = register_inputs_i ? r_shift_d1 : shift_right_i;
   assign shift_d2 = output_select_i[1] ? shift_d1 : r_shift_d2;
   assign subtract = register_inputs_i ? r_subtract : subtract_i;
   assign load_acc = register_inputs_i ? r_load_acc : load_acc_i;   
   assign acc_fir = register_inputs_i ? r_acc_fir : acc_fir_i;
   assign sat_d1 = register_inputs_i ?  r_sat_d1 : saturate_enable_i;
   assign sat_d2 = output_select_i[1] ?  sat_d1 : r_sat_d2;
   assign rnd_d1 = register_inputs_i ?  r_rnd_d1 : round_i;
   assign rnd_d2 = output_select_i[1] ?  rnd_d1 : r_rnd_d2;
   
   
   always @ (posedge s_clk or posedge s_reset) begin
      if (s_reset == 1'b1) begin
        r_a <= 'h0;
        r_b <= 'h0;
         r_unsigned_a <= '0;
         r_unsigned_b <= '0;
         r_feedback <= '0;
         r_shift_d1 <= '0;
         r_shift_d2 <= '0;
         r_subtract <= '0;
         r_load_acc <= '0;
         r_sat_d1 <= '0;
         r_sat_d2 <= '0;
         r_rnd_d1 <=  '0;
         r_rnd_d2 <=  '0;
         r_acc_fir <= '0;
      end else begin // if (reset_i == 1'b0)
        r_a <= a_i;
        r_b <= b_i;
         r_unsigned_a <= unsigned_a_i;
         r_unsigned_b <= unsigned_b_i;
         r_feedback <= feedback_i;
         r_shift_d1 <= shift_right_i;
         r_shift_d2 <= shift_d1;
         r_subtract <= subtract_i;
         r_load_acc <= load_acc_i;
         r_sat_d1 <= saturate_enable_i;
         r_sat_d2 <= sat_d1;
         r_rnd_d1 <=  round_i;
         r_rnd_d2 <=  rnd_d1;
         r_acc_fir <= acc_fir_i;
      end // else: if(reset_i == 1'b0)
   end // always @ (posedge s_clk or posedge reset_i)


   
 
   always_comb begin
      a = register_inputs_i ? r_a : a_i;
      b = register_inputs_i ? r_b : b_i;
      case (feedback)
        0,1,2: mult_a = a;
        3: mult_a = acc[NBITS_A-1:0];
        4: mult_a = coef_0_i;
        5: mult_a = coef_1_i;
        6: mult_a = coef_2_i;
        7: mult_a = coef_3_i;
      endcase
      mult_b= (feedback == 3'h2) ? {NBITS_B{1'b0}} : b;
   end
   bw_multiplier #(
    .NBITS_A(NBITS_A),
    .NBITS_B(NBITS_B)
    ) i_bw_multiplier (
      .a_i(mult_a),
      .b_i(mult_b),
      .a_is_signed_i(signed_a),
      .b_is_signed_i(signed_b),
      .z_o (mult)
    );

   assign unsigned_mode = unsigned_a & unsigned_b;
   
   
   always_comb begin
      mult_xtnd = unsigned_mode  ? {{(NBITS_ACC-NBITS_A-NBITS_B){1'b0}}, mult[NBITS_A+NBITS_B-1:0]}
                  : {{(NBITS_ACC-NBITS_A-NBITS_B){mult[NBITS_A+NBITS_B-1]}}, mult[NBITS_A+NBITS_B-1:0]};
      
      z0 = unsigned_mode ? mult_xtnd >> shift_d2 : mult_xtnd >>> shift_d2;
      
      add_a = subtract ? (~mult_xtnd + 1) : mult_xtnd;
      acc_fir_int = unsigned_a ? {{(NBITS_ACC-NBITS_A){1'b0}},a} : {{(NBITS_ACC-NBITS_A){a[NBITS_A-1]}},a} ;
 

      case (feedback)
        0: add_b = acc;
        1: add_b = 'h0;
        default: begin
           if (acc_fir < 6'd44)
             add_b = acc_fir_int << acc_fir;
           else
             add_b = acc_fir_int << 6'd44;
        end
      endcase // case (feedback_i)
      mult_acc = add_a + add_b;    // mult + acc
      acc_out = output_select_i[1] ? mult_acc : acc;

      round = (rnd_d2 && (shift_d2 != 0)) ? (acc_out + ({{(NBITS_ACC-1){1'b0}}, 1'b1} << (shift_d2 - 1))) : acc_out;

      shift = (round >>> shift_d2);

      if (shift[NBITS_ACC-1] == 1'b1) begin // Negative number
         unsigned_saturate = {NBITS_ACC{1'b0}};
      end else if (|shift[NBITS_ACC-1:NBITS_Z] == 1'b0) begin
        unsigned_saturate =  {{(NBITS_ACC-NBITS_Z){1'b0}},{shift[NBITS_Z-1:0]}};
      end else begin
	 unsigned_saturate = {{(NBITS_ACC-NBITS_Z){1'b0}},{NBITS_Z{1'b1}}};
      end

      if ((|shift[NBITS_ACC-1:NBITS_Z-1] == 1'b0) ||
	  (&shift[NBITS_ACC-1:NBITS_Z-1] == 1'b1) ) begin
	 signed_saturate = {{(NBITS_ACC-NBITS_Z){1'b0}},{shift[NBITS_Z-1:0]}};
      end
      else begin
	 signed_saturate = {{(NBITS_ACC-NBITS_Z){1'b0}},{shift[NBITS_ACC-1],{NBITS_Z-1{~shift[NBITS_ACC-1]}}}};
      end

      saturate = sat_d2  ? (unsigned_mode ? unsigned_saturate : signed_saturate)
                                    : shift;
      z3 = saturate;
   end

   always@(posedge s_clk or posedge s_reset) begin
      if (s_reset == 1'b1) begin
         acc <= 'h0;
         z1 <= '0;
         dly_b_o <= '0;
      end
      else begin
         acc <= load_acc  ? mult_acc : acc;
         z1 <= (output_select_i == 3'b100) ? mult_xtnd : z3;
         dly_b_o <= b_i;
      end
   end
   
  always_comb begin
     case (output_select_i)
       0: z_o = mult_xtnd[NBITS_Z-1:0];
       1,2,3: z_o = z3[NBITS_Z-1:0];
       4,5,6,7: z_o = z1[NBITS_Z-1:0];
     endcase // case (output_select_i)
  end

endmodule // dsp_type1
`default_nettype none
