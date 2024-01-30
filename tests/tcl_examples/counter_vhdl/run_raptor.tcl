#####################################
#  Rapid Silicon Design Example     #
#  counter_vhdl - RTL design        #
#  Raptor execution TCL file        #
#  raptor --script run_raptor.tcl   #
#####################################

# Project name
set project_name counter

puts "Creating $project_name..."
create_design $project_name
add_design_file -VHDL_1993 Src/UP_COUNTER.vhd
set_top_module UP_COUNTER
add_constraint_file constraints.sdc
# Testbench
add_simulation_file -VHDL_1993 Src/testbench.vhd
set_top_testbench tb_counters
# Device target
target_device 1GE75

# Compilation/Simulation
puts "Compiling $project_name..."
analyze

# RTL Simulation
simulation_options "ghdl" "simulation" "--stop-time=1000ns"
simulate "rtl" "ghdl" syn_tb_rtl.fst

# Synthesis
parser_type ghdl
pnr_netlist_lang vhdl
synthesize delay

# Post-Synthesis gate-level Simulation
simulate "gate" "ghdl" syn_tb_gate.fst

# Pack/Place/Route
packing
place
route

# Post-Route Simulation
# simulate pnr ghdl syn_tb_pnr.fst

# Static Timing Analysis
sta 

# Bitstream Generation
# bitstream 

puts "Completed $project_name...\n"
