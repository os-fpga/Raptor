# Design
create_design keep_test
add_design_file -V_2001 ./rtl/top.v
set_top_module top
add_constraint_file constraints.sdc

# Device target
target_device GEMINI_COMPACT_10x8

# Compilation
synthesize 
packing
