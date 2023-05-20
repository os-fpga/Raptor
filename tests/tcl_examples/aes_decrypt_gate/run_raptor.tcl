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
target_device 1GE75
read_netlist decrypt_top.blif
add_constraint_file constraints.sdc

# Compilation
puts "Compiling $project_name..."
pnr_netlist_lang blif
packing
place
route
sta
# bitstream

puts "Completed run_raptor.tcl\n"
