#CORE
set base_path /cadlib/gemini/TSMC16NMFFC
set std_cell_path ${base_path}/library/std_cells/dti/7p5t/rev_220704
set std_fe_rvt_path 220704_dti_tm16ffc_90c_7p5t_stdcells_rev1p0p1_rapid_fe_views_svt/220704_dti_tm16ffc_90c_7p5t_stdcells_rev1p0p1_rapid
set STD_RVT_ROOT ${std_cell_path}/${std_fe_rvt_path}
set std_libfile_name dti_tm16ffc_90c_7p5t_stdcells_ssgnp_0p72v_neg40c_rev1p0p0
#
set search_path [concat $search_path  \
  ${STD_RVT_ROOT}/db \
  ]
# SS corner
set link_library   [concat "*" \
  ${std_libfile_name}.db \
]

set target_library [concat [list]  \
  ${std_libfile_name}.db \
]

### DFT stuff
#read_verilog -netlist [list ./net/dsp.vg ]

set test_default_period      100
set test_default_delay         0
set test_default_bidir_dealy   0
set test_default_strobe       40

set_dft_signal -view existing_dft -type ScanClock  -port scan_clk_i   -timing [list 45 55] 
set_dft_signal -view existing_dft -type Reset      -port scan_reset_i -active_state 1
set_dft_signal -view spec         -type TestMode   -port scan_mode_i  -active_state 1
set_dft_signal -view existing_dft -type Constant   -port scan_mode_i  -active_state 1
set_dft_signal -view existing_dft -type ScanEnable -port scan_en_i    -active_state 1

set_dft_signal -view spec -type ScanEnable  -port scan_en_i -active_state 1
set_dft_signal -view spec -type ScanDataIn  -port scan_i[0]
set_dft_signal -view spec -type ScanDataIn  -port scan_i[1]
set_dft_signal -view spec -type ScanDataIn  -port scan_i[2]

set_dft_signal -view spec -type ScanDataOut -port scan_o[0]
set_dft_signal -view spec -type ScanDataOut -port scan_o[1]
set_dft_signal -view spec -type ScanDataOut -port scan_o[2]


create_test_protocol
dft_drc -pre_dft -verbose

set_scan_configuration -style multiplexed_flip_flop \
                       -clock_mixing no_mix \
                       -chain_count  3
                         
set_scan_path ScanChain_1 -view spec \
                          -scan_data_in   scan_i[0] \
                          -scan_enable    scan_en_i \
                          -scan_data_out  scan_o[0] 

set_scan_path ScanChain_2 -view spec \
                          -scan_data_in   scan_i[1] \
                          -scan_enable    scan_en_i \
                          -scan_data_out  scan_o[1] 

set_scan_path ScanChain_3 -view spec \
                          -scan_data_in   scan_i[2] \
                          -scan_enable    scan_en_i \
                          -scan_data_out  scan_o[2] 


set_dft_insertion_configuration \
                  -synthesis_optimization none \
                  -preserve_design_name   true                  

preview_dft -show all                  

insert_dft

dft_drc -coverage_estimate

write_test_protocol \
      -out ./spf/${TOP_MODULE}.spf  

write_scan_def -output ./def/${TOP_MODULE}.scandef



write -format verilog \
      -out    ./net/${TOP_MODULE}_dftc.v \
      -hier   ${TOP_MODULE} 
