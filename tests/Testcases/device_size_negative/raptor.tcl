# Design
create_design and2_gemini
# Device target
target_device GEMINI_COMPACT_10x8
set_device_size 42x43
add_design_file -V_2001 ./rtl/and2.v
set_top_module and2
add_constraint_file constraints.sdc
# Compilation
synthesize delay
packing
place
route
sta
bitstream 
