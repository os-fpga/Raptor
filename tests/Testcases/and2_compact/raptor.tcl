# Design
create_design and2_compact
add_design_file -V_2001 ./rtl/and2.v
set_top_module and2
add_constraint_file pin_mapping.pin
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
