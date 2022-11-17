# Design
create_design oneff_gemini
add_design_file -SV_2009 dut.v
set_top_module dut
add_constraint_file pin_mapping.pin
add_constraint_file constraints.sdc

# Testbench
add_simulation_file -SV_2009 syn_tb.v
add_simulation_file sim_main.cpp

# Device target
target_device GEMINI_LATEST

# Compilation/Simulation
analyze

# RTL Simulation
simulate rtl
file rename -force oneff_gemini/syn_tb.vcd oneff_gemini/syn_tb_rtl.vcd

# Synthesis
synthesize delay

# Post-Synthesis gate-level Simulation
simulate gate
file rename -force oneff_gemini/syn_tb.vcd oneff_gemini/syn_tb_gate.vcd

# Pack/Place/Route
packing
place
route

# Post-Route Simulation
simulate pnr
file rename -force oneff_gemini/syn_tb.vcd oneff_gemini/syn_tb_pnr.vcd

# Static Timing Analysis
sta

# Bitstream Generation
bitstream 
