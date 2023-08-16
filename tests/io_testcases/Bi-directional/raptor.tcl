set project_name BIDIRECTIONAL

puts "Creating $project_name..."
create_design $project_name -type gate-level
target_device GEMINI_COMPACT_10x8
read_netlist bidirectional_io_gate.v
#add_constraint_file bidirectional_io.sdc

# Compilation
puts "Compiling $project_name..."
pnr_netlist_lang verilog
packing
place
route
sta
bitstream

puts "Completed run_raptor.tcl\n"
