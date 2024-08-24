# ------------------------------------------
#
# Raptor Design Suite simulation helpers
#
# ------------------------------------------

# Edit the post-synth or pnr-wrapper netlist to allow running LEC simulation "RTL vs post_synth netlist" or "RTL vs post_pnr netlist"
# Args: stage {"post_synth" or "post_pnr"}
proc rename_module_in_netlist { {stage "post_synth"} } {
    if {$stage == "post_synth"} {
        set fileToEdit "[get_design_name]/run_1/synth_1_1/synthesis/[get_design_name]_post_synth.v"
    } elseif {$stage == "post_pnr"} {
        set fileToEdit "[get_design_name]/run_1/synth_1_1/synthesis/post_pnr_wrapper_[get_design_name]_post_synth.v"
    } else {
        error "ERROR in rename_module_in_netlist, unknown stage: $stage"
    }
    puts "Modifying $fileToEdit"
    # Open the input file in read mode
    set input_file [open $fileToEdit r]
    # Read the file content
        set file_content [read $input_file]
    # Close the input file after reading
    close $input_file
    set modified_content [string map [list "[get_top_module](" "[get_top_module]_post_synth("] $file_content]
    # Open the file again, this time in write mode to overwrite the old content
    set output_file [open $fileToEdit w]
    # Write the modified content back to the file
    puts $output_file $modified_content
    # Close the file
    close $output_file
    puts "Modification completed."
}

# LEC Simulation setup
# Setup simulation with auto-testbench "RTL vs gate" and "RTL vs pnr"
# To be invoked after Synthesis
proc setup_lec_sim { } {
    # auto-testbench generation
    auto_testbench

    # Add simulation files
    #  1) Generated testbench:
    add_simulation_file ./sim/co_sim_tb/co_sim_[get_top_module].v
    #  2) RTL design:
    foreach file [get_design_files] {
        add_simulation_file $file
    }

    # Set top testbench name to auto-generated name
    set_top_testbench co_sim_[get_top_module]

    # Edit post-synth netlist to run co-simulation RTL vs post-synth netlist 
    rename_module_in_netlist post_synth

    # Edit PnR wrapper netlist to run co-simulation RTL vs post-pnr netlist 
    rename_module_in_netlist post_pnr    
}

