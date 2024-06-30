create_design vex_soc_no_carry
target_device 1VG28
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
