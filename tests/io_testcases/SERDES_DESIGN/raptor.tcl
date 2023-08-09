set project_name SERDES

puts "Creating $project_name..."
create_design $project_name -type gate-level
target_device GEMINI_COMPACT_104x68
pnr_netlist_lang verilog
read_netlist serdes_post_synth.v

# Compilation
puts "Compiling $project_name..."

packing
place
route
sta
bitstream

puts "Completed run_raptor.tcl\n"
