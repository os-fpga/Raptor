source tech.tcl
define_design_lib WORK -path "WORK"

analyze -format verilog { ../rtl/ql_clkmux.v }
elaborate ql_clkmux
check_design

create_clock -name CLK_A1_i -period 4.0 CLK_A1_i
create_clock -name CLK_A2_i -period 4.0 CLK_A2_i
create_clock -name CLK_B1_i -period 4.0 CLK_B1_i
create_clock -name CLK_B2_i -period 4.0 CLK_B2_i

set_dont_use tcbn16ffcllbwp7d5t16p96cpdssgnp0p72vm40c_ccs/*
remove_attribute tcbn16ffcllbwp7d5t16p96cpdssgnp0p72vm40c_ccs/CKMUX2D4BWP7D5T16P96CPD dont_use
remove_attribute tcbn16ffcllbwp7d5t16p96cpdssgnp0p72vm40c_ccs/CKND4BWP7D5T16P96CPD dont_use
remove_attribute tcbn16ffcllbwp7d5t16p96cpdssgnp0p72vm40c_ccs/CKNR2D4BWP7D5T16P96CPD dont_use
remove_attribute tcbn16ffcllbwp7d5t16p96cpdssgnp0p72vm40c_ccs/CKND2D4BWP7D5T16P96CPD dont_use
remove_attribute tcbn16ffcllbwp7d5t16p96cpdssgnp0p72vm40c_ccs/CKOR2D4BWP7D5T16P96CPD dont_use
remove_attribute tcbn16ffcllbwp7d5t16p96cpdssgnp0p72vm40c_ccs/CKXOR2D4BWP7D5T16P96CPD dont_use
remove_attribute tcbn16ffcllbwp7d5t16p96cpdssgnp0p72vm40c_ccs/CKAN2D4BWP7D5T16P96CPD dont_use

set_max_delay 0.000 -from [all_inputs] -to [all_outputs]
set_max_area 0
compile -exact_map

report_cell
report_design
report_area -hierarchy
report_timing

write_file -hierarchy -output ql_clkmux.ddc
write -format verilog -out ../rtl/ql_clkmux_net.v -hier ql_clkmux

exit
