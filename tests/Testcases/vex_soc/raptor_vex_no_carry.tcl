create_design vex_soc_no_carry
target_device GEMINI
add_include_path ./
add_library_path rtl/
add_library_ext .v .sv
add_design_file rtl/vex_soc.v
add_constraint_file constraints.sdc
set_top_module vex_soc
synth_options -effort high -carry none
synthesize delay
pnr_options --gen_post_synthesis_netlist on
pnr_netlist_lang blif
packing
global_placement
place
route
sta opensta
power
bitstream
