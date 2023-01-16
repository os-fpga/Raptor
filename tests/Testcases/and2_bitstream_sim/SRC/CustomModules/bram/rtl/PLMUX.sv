module PLMUX  
  (
   input logic LAST_COL_i,
   input logic [9:0]   COL_ID_i,
   input logic [9:0]   PL_COL_i,
   input logic [35:0]  MY_COL_DATA_i,
   input logic [35:0]  PREV_COL_DATA_i,
   output logic [35:0] TO_NEXT_COL_DATA_o
   );

   logic              my_col;
   assign my_col = ((COL_ID_i == PL_COL_i) | LAST_COL_i);
   assign TO_NEXT_COL_DATA_o = my_col ? MY_COL_DATA_i : 
                               (PL_COL_i == 10'h00 ? MY_COL_DATA_i | PREV_COL_DATA_i : PREV_COL_DATA_i);
   
endmodule // PLMUX
