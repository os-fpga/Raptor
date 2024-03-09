#####################################
#  Rapid Silicon Design Example     #
#  sasc_testcase - RTL design       #
#  Raptor execution TCL file        #
#  raptor --script run_raptor.tcl   #
#####################################

# Project name
set project_name sasc

puts "Creating $project_name..."
# Create Project
create_design $project_name
# Device setup
target_device 1VG28
# Design setup
add_design_file ./Src/timescale.v ./Src/sasc_brg.v ./Src/sasc_fifo4.v ./Src/sasc.v
add_constraint_file constraints.sdc
set_top_module sasc

# Compilation
puts "Compiling $project_name..."

synthesize
packing
place
route
sta
# bitstream

puts "Completed $project_name...\n"
