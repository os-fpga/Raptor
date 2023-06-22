#####################################
#  Rapid Silicon Design Example     #
#  and2_verilog - RTL design        #
#  Raptor execution TCL file        #
#  raptor --script run_raptor.tcl   #
#####################################

# Project name
set project_name and2_10x8

puts "Creating $project_name..."
create_design $project_name
add_design_file -V_2001 ./Src/and2.v
set_top_module and2

# Device target
target_device GEMINI_COMPACT_10x8

# Compilation
puts "Compiling $project_name..."
analyze
synthesize delay
simulate gate icarus
packing
place
route
sta 
bitstream write_cache

puts "Completed $project_name...\n"
