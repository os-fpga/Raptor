set project_name I_BUF_DS

puts "Creating $project_name..."
create_design $project_name -type gate-level
target_device GEMINI_COMPACT_10x8
read_netlist ibuf_ds_post_synth.v

# Compilation
puts "Compiling $project_name..."
pnr_netlist_lang verilog
packing
place
route
sta
bitstream

puts "Completed run_raptor.tcl\n"
