create_design ip_test
target_device 1VG28
configure_ip axis_width_converter_v1_0 -mod_name conv32_16 -version 1.0 -Pcore_in_width=32 -Pcore_out_width=16 -Pcore_reverse=0 -out_file rs_ips/conv32_16.v
ipgenerate
set_top_module use_ip
add_design_file rs_ips/conv32_16.v use_ip.v
synth delay
packing
place
route
sta

