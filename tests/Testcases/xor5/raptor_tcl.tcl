create_design xor5
target_device GEMINI_COMPACT_10x8
add_design_file rtl/xor5.v
file copy -force rtl/private_key.pem xor5/private_key.pem
set_top_module xor5
#add_constraint_file <file>: Sets SDC + location constraints
#Constraints: set_pin_loc, set_region_loc, all SDC commands
#batch { cmd1 ... cmdn } : Run compilation script using commands below
#ipgenerate
#ipgenerate
pnr_netlist_lang verilog
synthesize delay
#pin_loc_assign_method free
packing
global_placement
place
route
sta
power
bitstream force
