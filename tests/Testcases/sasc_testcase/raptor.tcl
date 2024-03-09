create_design sasc
# Device setup
target_device 1VG28
# Design setup
add_design_file -V_2001 ./rtl/timescale.v ./rtl/sasc_brg.v ./rtl/sasc_fifo4.v ./rtl/sasc.v
set_top_module sasc
add_constraint_file constraints.sdc
# Compilation
synthesize
packing
place
route
sta
bitstream
