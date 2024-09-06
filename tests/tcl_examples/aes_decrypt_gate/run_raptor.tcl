#########################################
#  Rapid Silicon Design Example         #
#  aes_decrypt_gate - Gate-level Design #
#  Raptor compilation TCL file          #
#  raptor --script run_raptor.tcl       #
#########################################

# Project name
set project_name AES_DECRYPT_GATE

message "Creating $project_name of type gate-level..."
create_design $project_name -type gate-level
target_device 1VG28
read_netlist decrypt_top.v
add_constraint_file constraints.sdc

# Compilation
message "Compiling $project_name..."

analyze

# Converts Verilog gate-level netlist to Eblif
synthesize

packing
place
route
sta
power
bitstream

message "Completed run_raptor.tcl\n"
