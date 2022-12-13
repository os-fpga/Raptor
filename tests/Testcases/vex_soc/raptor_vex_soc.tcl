create_design vex_soc
target_device GEMINI_LATEST
add_include_path ./
add_library_path rtl/
add_library_ext .v .sv
add_design_file rtl/vex_soc.v
set_top_module vex_soc
synthesize 
pnr_options --gen_post_synthesis_netlist on
packing
global_placement
place
route
sta opensta
power
bitstream
