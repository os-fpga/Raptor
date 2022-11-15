# Design
create_design oneff_gemini
add_design_file -SV_2009 dut.v
set_top_module dut
add_constraint_file pin_mapping.pin
add_constraint_file constraints.sdc

add_simulation_file -SV_2009 syn_tb.v
add_simulation_file sim_main.cpp

# Device target
target_device GEMINI_LATEST
# Compilation
analyze

simulate rtl

synthesize delay

# simulate gate

packing
place
route
sta
bitstream 
