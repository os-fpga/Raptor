#####################################
#  Rapid Silicon Design Example     #
#  gen_clk - RTL design        #
#  Raptor execution TCL file        #
#  raptor --script run_raptor.tcl   #
#####################################

# Project name
set project_name gen_clk

message "Creating $project_name..."
create_design $project_name
add_design_file -V_2001 dut.v
set_top_module dut
add_constraint_file constraints.sdc
#add_constraint_file pin_constraints.pin

target_device GEMINI_COMPACT_22x4

message "Compiling $project_name..."

analyze
synthesize delay

setup_lec_sim 10

# Simulate RTL vs gate
simulate gate icarus

packing
place
route

# Simulate RTL vs post-pnr
#simulate timed_pnr icarus
simulate pnr icarus

sta
power
bitstream 

message "Completed $project_name...\n"
