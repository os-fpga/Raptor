# Design
create_design counter
add_design_file -V_2001 counter.v
set_top_module counter
add_constraint_file constraints.sdc

# Device target
target_device GEMINI_10x8

# Compilation
synthesize delay
packing
place
route
sta
bitstream 
