create_design add__a_to_output
target_device GEMINI_COMPACT_10x8
add_include_path ./rtl
add_library_path ./rtl
add_library_ext .v .sv
add_design_file add__a_to_output.v
set_top_module add__a_to_output
synthesize delay


add_simulation_file -SV_2012 tb.v
set_top_testbench tb

simulate gate icarus syn_tb_pnr.fst

packing
global_placement
place
route
power
simulate pnr icarus syn_tb_pnr.fst

bitstream
