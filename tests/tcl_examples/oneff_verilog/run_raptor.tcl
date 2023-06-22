# Design
create_design oneff_gemini
add_design_file -SV_2009 dut.v
set_top_module dut
add_constraint_file pin_mapping.pin
add_constraint_file constraints.sdc

# Testbench
add_simulation_file -SV_2009 syn_tb.v
add_simulation_file sim_main.cpp
set_top_testbench syn_tb

# Device target
target_device 1GE100-ES1

# Compilation/Simulation
analyze

# RTL Simulation
simulate rtl verilator syn_tb_rtl.fst

# Synthesis
synthesize delay

# Post-Synthesis gate-level Simulation
simulate gate verilator syn_tb_gate.fst

# Pack/Place/Route
packing
place
route

# Post-Route Simulation
simulate pnr verilator syn_tb_pnr.fst

# Static Timing Analysis
sta 

# Bitstream Generation
#bitstream 
