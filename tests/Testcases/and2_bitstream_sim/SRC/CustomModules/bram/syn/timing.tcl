source tech.tcl
define_design_lib WORK -path "WORK"
read_file bram.ddc

create_clock -name CLK_A1_i -period 4.0 CLK_A1_i
create_clock -name CLK_A2_i -period 4.0 CLK_A2_i
create_clock -name CLK_B1_i -period 4.0 CLK_B1_i
create_clock -name CLK_B2_i -period 4.0 CLK_B2_i

set_case_analysis 0 FMODE1_i
set_case_analysis 0 FMODE2_i
set_case_analysis 0 SPLIT_i
set_case_analysis 0 WMODE_A1_i[0]
set_case_analysis 0 WMODE_A1_i[1]
set_case_analysis 0 WMODE_A1_i[2]
set_case_analysis 0 WMODE_A2_i[0]
set_case_analysis 0 WMODE_A2_i[1]
set_case_analysis 0 WMODE_A2_i[2]
set_case_analysis 0 WMODE_B1_i[0]
set_case_analysis 0 WMODE_B1_i[1]
set_case_analysis 0 WMODE_B1_i[2]
set_case_analysis 0 WMODE_B2_i[0]
set_case_analysis 0 WMODE_B2_i[1]
set_case_analysis 0 WMODE_B2_i[2]
set_case_analysis 0 RMODE_A1_i[0]
set_case_analysis 0 RMODE_A1_i[1]
set_case_analysis 0 RMODE_A1_i[2]
set_case_analysis 0 RMODE_A2_i[0]
set_case_analysis 0 RMODE_A2_i[1]
set_case_analysis 0 RMODE_A2_i[2]
set_case_analysis 0 RMODE_B1_i[0]
set_case_analysis 0 RMODE_B1_i[1]
set_case_analysis 0 RMODE_B1_i[2]
set_case_analysis 0 RMODE_B2_i[0]
set_case_analysis 0 RMODE_B2_i[1]
set_case_analysis 0 RMODE_B2_i[2]
report_timing -to RDATA_B1_o
