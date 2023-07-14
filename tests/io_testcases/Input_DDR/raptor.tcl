#########################################
#  Rapid Silicon Design Example         #
#  ibuf_obuf - Gate-level Design #
#  Raptor compilation TCL file          #
#  raptor --script run_raptor.tcl       #
#########################################

# Project name
set project_name inDDR

puts "Creating $project_name..."
create_design $project_name -type gate-level
target_device GEMINI_COMPACT_10x8
read_netlist input_ddr_gate.v
#add_constraint_file constraints.sdc

# Compilation
puts "Compiling $project_name..."
pnr_netlist_lang verilog
packing
place
route
sta
bitstream

puts "Completed run_raptor.tcl\n"
