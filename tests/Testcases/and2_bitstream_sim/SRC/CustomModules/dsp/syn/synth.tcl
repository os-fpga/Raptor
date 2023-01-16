source tech.tcl
set WORKLIB  "WORK"
exec mkdir -p ${WORKLIB}
define_design_lib WORK -path $WORKLIB

analyze -format sverilog { ../rtl/bw_multiplier.sv }
analyze -format sverilog { ../rtl/dsp_type1_bw.sv }
analyze -format sverilog { ../rtl/dsp.sv }

elaborate dsp

set FREQ 3.00
create_clock -name clk -period ${FREQ} clock_i
set_max_delay ${FREQ} -from a_i -to z_o
set_max_delay ${FREQ} -from b_i -to z_o
set_max_transition 1.0 dsp

# static configuration bits
set_false_path -from coef_0_i
set_false_path -from coef_1_i
set_false_path -from coef_2_i
set_false_path -from coef_3_i
set_false_path -from output_select_i
set_false_path -from register_inputs_i

check_design
compile -exact_map
check_design

report_design
report_area -hierarchy
report_timing
report_hierarchy -nosplit

write_file -hierarchy -output dsp.ddc

# insert scan
set_dft_signal -view existing_dft -type ScanClock  -port clock_i   -timing [list 45 55] 
set_dft_signal -view existing_dft -type Reset      -port greset_i -active_state 1
set_dft_signal -view spec         -type TestMode   -port scan_mode_i  -active_state 1
set_dft_signal -view existing_dft -type Constant   -port scan_mode_i  -active_state 1
set_dft_signal -view existing_dft -type ScanEnable -port scan_en_i    -active_state 1
set_dft_signal -view spec -type ScanEnable  -port scan_en_i -active_state 1

set chain_count 20
for {set i 0} {$i < $chain_count} {incr i} {
    set_dft_signal -view spec -type ScanDataIn  -port scan_i[$i]
    set_dft_signal -view spec -type ScanDataOut -port scan_o[$i]
}

create_test_protocol
dft_drc -pre_dft -verbose

set_scan_configuration -style multiplexed_flip_flop -clock_mixing no_mix -chain_count $chain_count

for {set i 0} {$i < $chain_count} {incr i} {
    set_scan_path ScanChain_$i -view spec -scan_data_in scan_i[$i] -scan_enable scan_en_i -scan_data_out scan_o[$i]
}

set_dft_insertion_configuration -synthesis_optimization none -preserve_design_name true
preview_dft -show all                  
insert_dft
dft_drc -coverage_estimate
write_test_protocol -out ../dft/dsp_dftc.spf  
write_scan_def -output ../dft/dsp_dftc.scandef
write -format verilog -out ../../dsp_dftc.v -hier dsp 

exit
