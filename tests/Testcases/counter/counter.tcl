# Device target
target_device MPW1
set_device_size 4x4

# Design
create_design counter
add_design_file -V_2001 counter.v
set_top_module counter
add_constraint_file constraints.sdc

# Compilation
synthesize delay
packing
place
route
sta
bitstream force
