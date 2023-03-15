# Design
create_design and2_compact_gate -type gate-level
read_netlist gate.v
set_top_module top

# Device target
target_device GEMINI_COMPACT_10x8
# Compilation
#analyze
#synthesize delay
packing
place
route
sta opensta
bitstream enable_simulation
