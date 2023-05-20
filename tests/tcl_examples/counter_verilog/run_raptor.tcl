#####################################
##  Rapid Silicon Design Example     #
##  counter_verilog - RTL design     #
##  Raptor execution TCL file        #
##  raptor --script run_raptor.tcl   #
######################################


set project_name counter

puts "Creating $project_name..."
create_design $project_name
add_design_file Src/counter.v
# optional to specify top-level as it can be automatically detected
set_top_module counter
add_constraint_file constraints.sdc
# Device target
target_device 1GE75

# Compilation
puts "Compiling $project_name..."
# Synthesize optimizing for delay
synthesize delay
packing
place
route
sta
# bitstream 

puts "Completed $project_name...\n"
