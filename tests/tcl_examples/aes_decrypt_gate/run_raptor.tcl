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
target_device 1VG28
read_netlist decrypt_top.v
add_constraint_file constraints.sdc

# Compilation
puts "Compiling $project_name..."

analyze

# Converts Verilog to Eblif
synthesize

packing
place
route
sta
power
# bitstream

puts "Completed run_raptor.tcl\n"
