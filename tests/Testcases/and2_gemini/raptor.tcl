# Design
create_design and2_gemini
add_design_file -V_2001 ./rtl/and2.v
set_top_module and2
add_constraint_file constraints.sdc

# Device target
target_device GEMINI_TEST_DSP
# Compilation
analyze
synthesize delay
packing
place
route
sta
bitstream 
