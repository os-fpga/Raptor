#####################################
#  Rapid Silicon Design Example     #
#  sasc_testcase - RTL design       #
#  Raptor execution TCL file        #
#  raptor --script run_raptor.tcl   #
#####################################

# Project name
set project_name sasc

message "Creating $project_name..."
# Create Project
create_design $project_name
# Device setup
target_device 1VG28
# Design setup
add_design_file ./Src/timescale.v ./Src/sasc_brg.v ./Src/sasc_fifo4.v ./Src/sasc.v
add_constraint_file constraints.sdc
set_top_module sasc

# Compilation
message "Compiling $project_name..."

analyze
synthesize

setup_lec_sim

# Simulate RTL vs gate
simulation_options compilation verilator gate
simulate gate verilator

packing
place
route

# Simulate RTL vs post-pnr
simulation_options compilation verilator pnr
simulate pnr verilator

sta
power
bitstream

message "Completed $project_name...\n"
