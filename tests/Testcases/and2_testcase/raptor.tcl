# Device target
target_device MPW1
set_device_size 4x4

# Design
create_design and2
add_design_file -V_2001 ./rtl/and2.v
set_top_module and2
add_constraint_file constraints.sdc

# Compilation
synthesize

packing

pnr_options --fix_pins ./pinmap.place
place

route
sta
bitstream force
