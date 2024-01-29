# Design
create_design param_up_counter
add_include_path ./rtl
add_library_path ./rtl
add_design_file -SV_2009 ./rtl/param_up_counter.v
set_top_module param_up_counter
add_constraint_file constraints.sdc

# Testbench
add_simulation_file sim_main.cpp
set_top_testbench param_up_counter

# Device target
target_device GEMINI_COMPACT_10x8

# clean default flags for wrapper
simulation_options verilator compilation rtl ""
# RTL Simulation
simulate rtl verilator counter_rtl.fst

# Synthesis
synthesize delay

# clean default flags for wrapper
simulation_options verilator compilation gate ""
# Post-Synthesis gate-level Simulation
simulate gate verilator counter_gate.fst

# Pack/Place/Route
packing
place
route

# clean default flags for wrapper
simulation_options verilator compilation pnr ""
# Post-Route Simulation
simulate pnr verilator counter_pnr.fst

# Static Timing Analysis
sta opensta

# Bitstream Generation
bitstream
