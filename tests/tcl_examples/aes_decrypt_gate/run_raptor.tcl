#########################################
#  Rapid Silicon Design Example         #
#  aes_decrypt_gate - Gate-level Design #
#  Raptor compilation TCL file          #
#  raptor --script run_raptor.tcl       #
#########################################

# Project name
set project_name AES_DECRYPT_GATE

puts "Creating $project_name..."
create_design $project_name -type gate-level
target_device 1GE100-ES1
read_netlist decrypt_top.v
add_constraint_file constraints.sdc

# Compilation
puts "Compiling $project_name..."
packing
place
route
sta
# bitstream

puts "Completed run_raptor.tcl\n"
