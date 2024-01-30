#####################################
#  Rapid Silicon Design Example     #
#  and2_verilog - RTL design        #
#  Raptor execution TCL file        #
#  raptor --script run_raptor.tcl   #
#####################################

# Project name
set project_name and2

puts "Creating $project_name..."
create_design $project_name
add_design_file -V_2001 ./Src/and2.v
# Top-module can automatically be indentified or implicitly specified here
set_top_module and2
add_constraint_file constraints.sdc
# Simulation
add_simulation_file ./Src/testbench_and2.v
set_top_testbench testbench_and2
# Device target
target_device 1GE75

# RTL Simulation
simulate rtl icarus

#wave_open and2/and2.vcd
#wave_show reset
#wave_show clk
#wave_show a
#wave_show b
#wave_show a_delay_by_2
#wave_show b_delay_by_2
#wave_show c
#wave_cmd gtkwave::/Time/Zoom/Zoom_Full

# Compilation
puts "Compiling $project_name..."
analyze
synthesize delay

# Gate Simulation
simulate gate icarus

packing
place
route

# Post PnR Simulation
simulate pnr icarus

sta 
# bitstream 

puts "Completed $project_name...\n"
