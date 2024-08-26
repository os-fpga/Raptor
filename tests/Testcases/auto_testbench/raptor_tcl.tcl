create_design GJC1_design

target_device GEMINI_COMPACT_22x4

add_design_file GJC1.v
set_top_module GJC1

analyze

synthesize delay

# auto-testbench generation
auto_testbench

# Add simulation files
#  1) Generated testbench:
add_simulation_file ./sim/co_sim_tb/co_sim_[get_top_module].v
#  2) RTL design:
add_simulation_file GJC1.v 

# Set top testbench name to auto-generated name
set_top_testbench co_sim_[get_top_module]

# Edit post-synth netlist to run co-simulation RTL vs post-synth netlist 
rename_module_in_netlist post_synth

# Simulate RTL vs gate
simulation_options compilation icarus gate
simulate gate icarus

packing
place
route

# Edit PnR wrapper netlist to run co-simulation RTL vs post-pnr netlist 
rename_module_in_netlist post_pnr

# Simulate RTL vs post-pnr
simulation_options compilation icarus pnr
simulate pnr icarus

sta
power
bitstream
