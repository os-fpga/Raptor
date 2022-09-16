create_design ip_test
target_device GEMINI
add_litex_ip_catalog ip_generators/
puts "Catalog:"
foreach ip [ip_catalog] {
    puts "IP: $ip"
    foreach param [ip_catalog $ip] {
        puts "  PARAM [lindex $param 0], default: [lindex $param 1]"
    }
}

configure_ip axis_converter_ip_generators -mod_name conv32_16 -version 1.0 -Pcore_in_width=32 -Pcore_out_width=16 -Pcore_reverse=0 -out_file rs_ips/conv32_16.v
ipgenerate
set_top_module use_ip
add_design_file rs_ips/conv32_16.v use_ip.v
synth delay
packing
place
route
sta

