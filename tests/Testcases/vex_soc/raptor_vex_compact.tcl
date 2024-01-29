create_design vex_soc
target_device GEMINI_COMPACT_62x44
add_include_path ./
add_library_path rtl/
add_library_ext .v .sv
add_design_file rtl/vex_soc.v
add_constraint_file pin_constraints.pin
set_top_module vex_soc
synth_options -effort high -carry all 
synthesize delay
packing
global_placement
place
route
sta 
power
bitstream
