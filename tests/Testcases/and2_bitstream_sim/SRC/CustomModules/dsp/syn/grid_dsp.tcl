source tech.tcl
set WORKLIB  "WORK"
exec mkdir -p ${WORKLIB}
define_design_lib WORK -path $WORKLIB

analyze -format verilog { ../../../../../FPGA10x8_dp_castor_pnr/release/FPGA10x8_dp_castor_verilog/SRCSynth/grid_dsp.v }
elaborate grid_dsp

# CCFF
create_clock -name prog_clock -period 10.0 prog_clock
set_input_delay 0 ccff_head -clock prog_clock
set_output_delay 0 ccff_tail -clock prog_clock

# FABRIC
create_clock -name clk -period 3.0 [get_ports *_clk_*]
set_input_delay 0 [get_ports *_pin_I*] -clock clk
set_output_delay 0 [get_ports *_pin_O*] -clock clk
set_max_delay 3.0 -from [get_ports *_pin_I*] -to [get_ports *_pin_O*]

# DFT
set_input_delay 0 [get_ports *_sc_in_*] -clock clk
set_output_delay 0 [get_ports *_sc_out_*] -clock clk

proc set_config_mode { } {
    puts "operating mode = CONFIG (HOLD FIX ONLY)"
    set_case_analysis 0 config_enable
    set_case_analysis 1 global_reset
    set_case_analysis 0 scan_en
    set_case_analysis 0 scan_mode
}

proc set_dft_mode { } {
    puts "operating mode = DFT (HOLD FIX ONLY)"
    set_case_analysis 1 config_enable
    set_case_analysis 0 global_reset
    set_case_analysis 1 scan_en
    set_case_analysis 1 scan_mode
    set_disable_timing [get_flat_pins config_block/*CCFF/Q]
}

proc set_user_mode { } {
    puts "operating mode = USER"
    set_case_analysis 1 config_enable
    set_case_analysis 0 global_reset
    set_case_analysis 0 scan_en
    set_case_analysis 0 scan_mode
    set_disable_timing [get_flat_pins config_block/*CCFF/Q]
}

set_config_mode
check_design
report_timing -max_paths 10 -nworst 3

set_dft_mode
check_design
report_timing -max_paths 10 -nworst 3

set_user_mode
check_design
report_timing -from [get_ports *_pin_I*] -to [get_ports *_pin_O*]
report_timing -from clk -to [get_ports *_pin_O*]
report_timing -from [get_ports *_pin_I*] -to clk
report_timing -max_paths 10 -nworst 3

exit
