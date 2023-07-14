# Design
create_design and2_reg_gemini
add_design_file ./rtl/and2.sv
set_top_module reg_and2
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
