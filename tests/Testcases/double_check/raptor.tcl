# Design
create_design param_up_counter
add_include_path ./rtl
add_library_path ./rtl
add_design_file -SV_2009 ./rtl/param_up_counter.v
set_top_module param_up_counter
add_constraint_file constraints.sdc

# Device target
target_device GEMINI_COMPACT_10x8

# Synthesis
synthesize delay
synthesize delay

# Pack/Place/Route
packing
packing
place
place
route
route

# Static Timing Analysis
sta opensta
sta opensta

# Bitstream Generation
bitstream
bitstream
