#####################################
##  Rapid Silicon Design Example     #
##  counter_verilog - RTL design     #
##  Raptor execution TCL file        #
##  raptor --script run_raptor.tcl   #
######################################


set project_name counter

message "Creating $project_name..."
create_design $project_name
add_design_file Src/counter.v
# optional to specify top-level as it can be automatically detected
set_top_module counter
add_constraint_file constraints.sdc
add_constraint_file pin_mapping.pin
# Device target
target_device 1VG28

# Compilation
message "Compiling $project_name..."
analyze
# Synthesize optimizing for delay
synthesize delay

# Setup simulation with auto-testbench "RTL vs gate" and "RTL vs pnr"
setup_lec_sim 

# Simulate RTL vs gate
simulation_options compilation icarus gate
simulate gate icarus

packing
place
route

# Simulate RTL vs post-pnr
simulation_options compilation icarus pnr
simulate pnr icarus

sta
power
bitstream 

message "Completed $project_name...\n"
