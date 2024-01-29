# Design
create_design and2_gemini_latest
add_design_file -V_2001 mult.v
set_top_module dsp_mult
add_constraint_file constraints.sdc

# Device target
target_device GEMINI_COMPACT_10x8

# Compilation
analyze
synthesize delay
packing
place
route
sta opensta
bitstream 
