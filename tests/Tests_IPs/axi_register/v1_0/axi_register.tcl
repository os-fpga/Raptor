create_design register_test
target_device GEMINI
add_litex_ip_catalog ./
puts "Catalog:"
foreach ip [ip_catalog] {
    puts "IP: $ip"
    foreach param [ip_catalog $ip] {
        puts "  PARAM [lindex $param 0], default: [lindex $param 1]"
    }
}
configure_ip axi_register_v1_0 -Pdata_width=128 -Paddr_width=32 -Pid_width=16 -mod_name register_wrapper -out_file ./register_wrapper.v
ipgenerate
add_design_file ./rapidsilicon/ip/axi_register/v1_0/register_wrapper/src/register_wrapper.v
add_library_path rapidsilicon/ip/axi_register/v1_0/register_wrapper/src/
set_top_module register_wrapper
synth delay

