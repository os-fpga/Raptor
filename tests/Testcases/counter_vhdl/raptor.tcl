# Design
create_design counter_vhdl_gemini
add_design_file -VHDL_1993 UP_COUNTER.vhd
set_top_module UP_COUNTER
add_constraint_file pin_mapping.pin
add_constraint_file constraints.sdc

# Testbench
add_simulation_file -VHDL_1993 testbench.vhd
set_top_testbench tb_counters

# Device target
target_device 1GE100

# Compilation/Simulation
analyze

# RTL Simulation
simulation_options "ghdl" "simulation" "--stop-time=1000ns"
simulate "rtl" "ghdl" syn_tb_rtl.fst

# Synthesis
pnr_netlist_lang vhdl
synthesize delay

# Post-Synthesis gate-level Simulation
simulate "gate" "ghdl" syn_tb_gate.fst

# Pack/Place/Route
packing
place
route

# Post-Route Simulation
#simulate pnr ghdl syn_tb_pnr.fst

# Static Timing Analysis
sta 

# Bitstream Generation
bitstream 
