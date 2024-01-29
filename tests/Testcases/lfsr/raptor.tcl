# Design
create_design lfsr
add_design_file top.sv
set_top_module lfsr
add_constraint_file constraints.sdc

# Device target
target_device GEMINI_COMPACT_10x8
# Compilation
analyze
synthesize delay
packing
place
route
sta
bitstream 
