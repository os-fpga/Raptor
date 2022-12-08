# Design
create_design and2_gemini_latest
add_design_file -V_2001 ./rtl/and2.v
set_top_module and2
add_constraint_file constraints.sdc

# Device target
target_device GEMINI_LATEST

# Compilation
analyze
synthesize delay
pnr_netlist_lang blif
packing
place
route
sta opensta
bitstream 
