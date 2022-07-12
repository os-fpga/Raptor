target_device GEMINI
add_litex_ip_catalog ./
create_design ip_test
configure_ip axis_converter -mod_name conv32_16 -version 1.0 -Pcore_in_width=32 -Pcore_out_width=16 -out_file rs_ips/conv32_16.v
ipgenerate
set_top_module use_ip
add_design_file rs_ips/conv_32_16.v use_ip.v
synth
