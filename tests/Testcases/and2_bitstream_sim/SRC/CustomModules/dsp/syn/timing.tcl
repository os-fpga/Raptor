source tech.tcl
define_design_lib WORK -path "WORK"
read_file dsp.ddc
#set_case_analysis 0 f_mode_i
#set_case_analysis 0 register_inputs_i
#set_case_analysis 0 feedback_i[0]
#set_case_analysis 0 feedback_i[1]
#set_case_analysis 0 feedback_i[2]
#set_case_analysis 0 dsp0/r_feedback_reg[0]/Q
#set_case_analysis 0 dsp0/r_feedback_reg[1]/Q
#set_case_analysis 0 dsp0/r_feedback_reg[2]/Q
#set_case_analysis 0 output_select_i[0]
#set_case_analysis 0 output_select_i[1]
#set_case_analysis 1 output_select_i[2]
#report_timing -to z_o

