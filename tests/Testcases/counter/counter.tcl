# Design
create_design counter
add_design_file -V_2001 counter.v
set_top_module counter
add_constraint_file constraints.sdc

# Device target
target_device GEMINI_COMPACT_10x8

analyze
# Compilation
synth_options -effort high -carry all
synthesize delay
pin_loc_assign_method free
packing
place
route
sta
bitstream 
