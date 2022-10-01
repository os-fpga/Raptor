create_design ram_test
target_device GEMINI
add_litex_ip_catalog ./
puts "Catalog:"
foreach ip [ip_catalog] {
    puts "IP: $ip"
    foreach param [ip_catalog $ip] {
        puts "  PARAM [lindex $param 0], default: [lindex $param 1]"
    }
}
configure_ip axi_ram_v1_0 -mod_name ram_wrapper -Pdata_width=64 -Paddr_width=8 -Pid_width=8 -Ppip_out=1 -out_file ./ram_wrapper.v
ipgenerate
add_design_file ./rapidsilicon/ip/axi_ram/v1_0/ram_wrapper/src/ram_wrapper.v
add_library_path rapidsilicon/ip/axi_ram/v1_0/ram_wrapper/src/
set_top_module ram_wrapper
synth delay

