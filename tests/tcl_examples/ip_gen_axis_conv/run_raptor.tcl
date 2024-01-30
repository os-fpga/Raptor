#####################################
#  Rapid Silicon Design Example     #
#  ip_gen_axis_conv - IP design     #
#  Raptor execution TCL file        #
#  raptor --script run_raptor.tcl   #
#####################################

# Project name
set project_name ip_test

puts "Creating $project_name..."
create_design $project_name
target_device 1GE75
add_design_file Src/use_ip.v
add_constraint_file constraints.sdc
set_top_module use_ip

# Generate IP
configure_ip axis_width_converter_v1_0 -mod_name conv32_16 -version 1.0 -Pcore_in_width=32 -Pcore_out_width=16 -Pcore_reverse=0 -out_file rs_ips/conv32_16.v
ipgenerate
add_design_file ./rs_ips/rapidsilicon/ip/axis_width_converter/v1_0/conv32_16/src/conv32_16.v

# Compile
puts "Compiling $project_name..."

# Synthesize optimizing for delay
synth delay
packing
place
route
sta

puts "Completed $project_name...\n"
