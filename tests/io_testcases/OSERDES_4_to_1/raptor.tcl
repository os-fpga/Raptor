#create_design OSERDES
#target_device GEMINI_COMPACT_104x68
#add_design_file o_serdes_4_to_1_rtl.v
#
##add_constraint_file o_serdes_4_to_1.sdc
#synth_options -inferred_io -effort high
#message_severity VERI-1209 IGNORE
#synthesize delay

set project_name OSERDES

puts "Creating $project_name..."
create_design $project_name -type gate-level
target_device GEMINI_COMPACT_10x8
pnr_netlist_lang verilog
read_netlist o_serdes_4_to_1_gate.v
#add_constraint_file o_serdes_4_to_1.sdc

# Compilation
puts "Compiling $project_name..."

packing
place
route
sta
bitstream

puts "Completed run_raptor.tcl\n"
