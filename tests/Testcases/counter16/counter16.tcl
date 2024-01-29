# module load fpga_tools/raptor/latest
# raptor --batch --script build.tcl
create_design counter16
add_design_file -V_2001 counter16.v
set_top_module counter16
add_constraint_file pin_mapping.pin

target_device GEMINI_COMPACT_10x8

analyze
set_limits dsp 0
synthesize delay
packing
place
route
sta
#bitstream enable_simulation
bitstream
