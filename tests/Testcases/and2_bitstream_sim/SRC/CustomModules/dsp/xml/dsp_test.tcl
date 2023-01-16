# Design
create_design dsp_test
add_design_file -V_2001 dsp_test.v
set_top_module dsp_test

add_constraint_file constraints.sdc

target_device GEMINI
architecture ../../../arch/mako/vpr.xml ../../../arch/mako/openfpga.xml
#set_device_size 12x12
set_device_size 78x66

pnr_options --echo_file on --connection_driven_clustering on --timing_driven_clustering off --allow_unrelated_clustering on --pack_verbosity 3
synthesize none
packing
place
route
sta
bitstream force
