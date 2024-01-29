create_design vex_soc_no_carry
target_device GEMINI_COMPACT_62x44
add_include_path ./
add_library_path rtl/
add_library_ext .v .sv
add_design_file rtl/vex_soc.v
add_constraint_file constraints.sdc
set_top_module vex_soc
analyze
synth_options -effort high -carry none
synthesize delay
packing
place
route
sta opensta
power
bitstream
