set project_name ODDR

puts "Creating $project_name..."
create_design $project_name -type gate-level
target_device GEMINI_COMPACT_10x8
pnr_netlist_lang verilog
read_netlist output_ddr_gate.v
add_constraint_file output_ddr.sdc

# Compilation
puts "Compiling $project_name..."

packing
place
route
sta
bitstream

puts "Completed run_raptor.tcl\n"
