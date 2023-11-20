create_design vex_100K
target_device 1GE100-ES1
add_include_path ./
add_library_path rtl/
add_library_ext .v .sv
add_design_file rtl/vex_soc.v
set_top_module vex_soc

analyze 
synth_options -effort high -carry all 
synthesize delay
packing
global_placement
place
route
sta 
#bitstream
