create_design sasc
architecture ./TSMC22nm_vpr.xml ./TSMC22nm_openfpga.xml
bitstream_config_files -bitstream ../../Arch/bitstream_annotation_empty.xml -sim ./fixed_sim_openfpga.xml -repack ./repack_design_constraint.xml
add_design_file -V_2001 ./rtl/timescale.v ./rtl/sasc_brg.v ./rtl/sasc_fifo4.v ./rtl/sasc.v
set_top_module sasc
set_device_size 16x16
add_constraint_file constraints.sdc
synthesize
packing
place
route
sta
bitstream
