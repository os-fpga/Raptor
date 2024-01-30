# Design
create_design and2_gemini_legacy
add_design_file -V_2001 ./rtl/and2.v
set_top_module and2
add_constraint_file pin_mapping.pin
add_constraint_file constraints.sdc

# Device target
target_device MPW1
# Compilation
analyze
synthesize delay
packing
place
route
sta opensta
bitstream 
