# Design
create_design rom
add_design_file top.v
set_top_module SBox

# Device target
target_device 1VG28
# Compilation
analyze
synthesize delay
packing
place
route
sta 
