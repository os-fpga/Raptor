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
target_device 1VG28

# Compilation/Simulation
analyze

# clean default flags for wrapper
simulation_options verilator compilation rtl ""
# RTL Simulation
simulate rtl verilator syn_tb_rtl.fst

# Synthesis
synth_options -inferred_io
synthesize delay

# clean default flags for wrapper
simulation_options verilator compilation gate ""
# Post-Synthesis gate-level Simulation
simulate gate verilator syn_tb_gate.fst

# Pack/Place/Route
packing
place
route

# clean default flags for wrapper
simulation_options verilator compilation pnr ""
# Post-Route Simulation
simulate pnr verilator syn_tb_pnr.fst

# Static Timing Analysis
sta 

# Bitstream Generation
#bitstream 
