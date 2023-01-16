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

set_dft_signal -view spec -type ScanDataIn  -port SCAN_i[0]
set_dft_signal -view spec -type ScanDataIn  -port SCAN_i[1]
set_dft_signal -view spec -type ScanDataIn  -port SCAN_i[2]
set_dft_signal -view spec -type ScanDataIn  -port SCAN_i[3]
set_dft_signal -view spec -type ScanDataIn  -port SCAN_i[4]
set_dft_signal -view spec -type ScanDataIn  -port SCAN_i[5]

set_dft_signal -view spec -type ScanDataOut -port SCAN_o[0]
set_dft_signal -view spec -type ScanDataOut -port SCAN_o[1]
set_dft_signal -view spec -type ScanDataOut -port SCAN_o[2]
set_dft_signal -view spec -type ScanDataOut -port SCAN_o[3]
set_dft_signal -view spec -type ScanDataOut -port SCAN_o[4]
set_dft_signal -view spec -type ScanDataOut -port SCAN_o[5]

create_test_protocol
dft_drc -pre_dft -verbose

set_scan_configuration -style multiplexed_flip_flop -clock_mixing no_mix -chain_count 6
set_scan_path ScanChain_1 -view spec -scan_data_in SCAN_i[0] -scan_enable SCAN_EN_i -scan_data_out SCAN_o[0] 
set_scan_path ScanChain_2 -view spec -scan_data_in SCAN_i[1] -scan_enable SCAN_EN_i -scan_data_out SCAN_o[1] 
set_scan_path ScanChain_3 -view spec -scan_data_in SCAN_i[2] -scan_enable SCAN_EN_i -scan_data_out SCAN_o[2] 
set_scan_path ScanChain_4 -view spec -scan_data_in SCAN_i[3] -scan_enable SCAN_EN_i -scan_data_out SCAN_o[3] 
set_scan_path ScanChain_5 -view spec -scan_data_in SCAN_i[4] -scan_enable SCAN_EN_i -scan_data_out SCAN_o[4] 
set_scan_path ScanChain_6 -view spec -scan_data_in SCAN_i[5] -scan_enable SCAN_EN_i -scan_data_out SCAN_o[5] 

set_dft_insertion_configuration -synthesis_optimization none -preserve_design_name true
preview_dft -show all                  
insert_dft
dft_drc -coverage_estimate
write_test_protocol -out ./spf/TDP36K_top.spf  
write_scan_def -output ./def/TDP36K_top.scandef
write -format verilog -out ./net/TDP36K_top_dftc.v -hier TDP36K_top 
