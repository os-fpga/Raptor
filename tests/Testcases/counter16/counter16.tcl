# module load fpga_tools/raptor/latest
# raptor --batch --script build.tcl
create_design counter16
add_design_file -V_2001 counter16.v
set_top_module counter16

#target_device GEMINI_10x8
target_device GEMINI_COMPACT_10x8

analyze
synth_options -no_dsp
synthesize delay
packing
place
route
sta
#bitstream_config_files -key ../fabric_task/flow_inputs/output_fabric_key.xml -repack repack.xml
bitstream enable_simulation
