# Design
create_design counter_8bit
add_design_file -V_2001 counter_8bit.v
set_top_module counter_8bit
#add_constraint_file constraints.sdc

# Device target
target_device MPW1


# Compilation
synthesize delay
custom_openfpga_script ./counter_8bit.openfpga
pnr_netlist_lang edif
set_device_size 4x4

packing
place
route
sta
bitstream force
  