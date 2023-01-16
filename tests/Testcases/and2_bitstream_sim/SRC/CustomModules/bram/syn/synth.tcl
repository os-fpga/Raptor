source tech.tcl
set WORKLIB  "WORK"
exec mkdir -p ${WORKLIB}
define_design_lib WORK -path $WORKLIB

analyze -format sverilog { ../rtl/bram.svh }
analyze -format sverilog { ../rtl/TDP18K_FIFO.sv }
analyze -format sverilog { ../rtl/TDP36K.sv }
analyze -format sverilog { ../rtl/TDP36K_top.sv }
analyze -format sverilog { ../rtl/ufifo_ctl.sv }
analyze -format verilog { ../rtl/ql_clkmux_net.v }
elaborate TDP36K_top

create_clock -name CLK_A1_i -period 4.0 CLK_A1_i
create_clock -name CLK_A2_i -period 4.0 CLK_A2_i
create_clock -name CLK_B1_i -period 4.0 CLK_B1_i
create_clock -name CLK_B2_i -period 4.0 CLK_B2_i
set_max_transition 1 TDP36K_top
set_dont_touch ql_clkmux

check_design
compile -exact_map
check_design

report_design
report_area -hierarchy
report_timing
report_hierarchy -nosplit

write_file -hierarchy -output bram.ddc

# insert scan
set_dft_signal -view existing_dft -type ScanClock  -port CLK_A1_i   -timing [list 45 55] 
set_dft_signal -view existing_dft -type ScanClock  -port CLK_B1_i   -timing [list 45 55] 
set_dft_signal -view existing_dft -type ScanClock  -port CLK_A2_i   -timing [list 45 55] 
set_dft_signal -view existing_dft -type ScanClock  -port CLK_B2_i   -timing [list 45 55] 
set_dft_signal -view existing_dft -type Reset      -port GRESET_i -active_state 1
set_dft_signal -view spec         -type TestMode   -port SCAN_MODE_i  -active_state 1
set_dft_signal -view existing_dft -type Constant   -port SCAN_MODE_i  -active_state 1
set_dft_signal -view existing_dft -type ScanEnable -port SCAN_EN_i    -active_state 1
set_dft_signal -view spec -type ScanEnable  -port SCAN_EN_i -active_state 1

set chain_count 44
for {set i 0} {$i < $chain_count} {incr i} {
    set_dft_signal -view spec -type ScanDataIn  -port SCAN_i[$i]
    set_dft_signal -view spec -type ScanDataOut -port SCAN_o[$i]
}

create_test_protocol
dft_drc -pre_dft -verbose

set_scan_configuration -style multiplexed_flip_flop -clock_mixing no_mix -chain_count $chain_count

for {set i 0} {$i < $chain_count} {incr i} {
    set_scan_path ScanChain_$i -view spec -scan_data_in SCAN_i[$i] -scan_enable SCAN_EN_i -scan_data_out SCAN_o[$i]
}

set_dft_insertion_configuration -synthesis_optimization none -preserve_design_name true
preview_dft -show all                  
insert_dft
dft_drc -coverage_estimate
write_test_protocol -out ../dft/TDP36K_top_dftc.spf  
write_scan_def -output ../dft/TDP36K_top_dftc.scandef
write -format verilog -out ../../TDP36K_top_dftc.v -hier TDP36K_top 

exit
