create_design GJC1_design

target_device GEMINI_COMPACT_22x4

add_design_file GJC1.v 
set_top_module GJC1

analyze

synthesize delay

# Setup simulation with auto-testbench "RTL vs gate" and "RTL vs pnr"
setup_lec_sim 

# Simulate RTL vs gate
simulation_options compilation icarus gate
simulate gate icarus

packing
place
route

# Simulate RTL vs post-pnr
simulation_options compilation icarus pnr
simulate pnr icarus

sta
power
bitstream
