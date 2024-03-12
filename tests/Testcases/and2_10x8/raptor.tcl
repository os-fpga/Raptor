# Design
create_design and2_10x8
add_design_file -V_2001 ./rtl/and2.v
set_top_module and2
add_constraint_file pin_mapping.pin

# Simulation
add_simulation_file ./rtl/testbench_and2.v
set_top_testbench testbench_and2

# Device target
target_device GEMINI_COMPACT_10x8

# RTL Simulation
simulate rtl icarus

# Analysis
analyze

# Synthesis
synthesize delay

# Gate-level simulation
simulate gate icarus

# Compilation
packing
place
route

# Post PnR simulation
simulate pnr icarus

sta
power
bitstream write_xml
