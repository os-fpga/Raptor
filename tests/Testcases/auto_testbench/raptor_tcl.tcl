create_design GJC1
target_device 1VG28
add_design_file GJC1.v
set_top_module GJC1
analyze
synthesize delay

#################### Get the current working directory ####################
set current_dir [pwd]
#################### run auto-testbench script ####################
#exec python3 tb_generator.py GJC1 $current_dir
auto_testbench

#################### add simulation files ####################
add_simulation_file ./sim/co_sim_tb/co_sim_GJC1.v GJC1.v
set_top_testbench co_sim_GJC1

#################### edit post-synth netlist to run co-simulation RTL vs post-synth netlist ####################

# Open the input file in read mode
set input_file [open "GJC1/run_1/synth_1_1/synthesis/GJC1\_post_synth.v" r]
# Read the file content
set file_content [read $input_file]
# Close the input file after reading
close $input_file
set modified_content [string map {"GJC1(" "GJC1_post_synth("} $file_content]
# Open the file again, this time in write mode to overwrite the old content
set output_file [open "GJC1/run_1/synth_1_1/synthesis/GJC1\_post_synth.v" w]
# Write the modified content back to the file
puts $output_file $modified_content
# Close the file
close $output_file
puts "Modification completed."

#################################################################################################################

simulation_options compilation icarus gate
simulate gate icarus
