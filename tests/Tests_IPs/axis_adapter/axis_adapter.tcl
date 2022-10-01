create_design adapter_test
target_device GEMINI
add_litex_ip_catalog ./
puts "Catalog:"
foreach ip [ip_catalog] {
    puts "IP: $ip"
    foreach param [ip_catalog $ip] {
        puts "  PARAM [lindex $param 0], default: [lindex $param 1]"
    }
}
configure_ip axis_adapter_v1_0 -mod_name adapter_wrapper -Ps_data_width=1024 -Pm_data_width=512 -Pid_en=1 -Pid_width=16 -out_file ./adapter_wrapper.v
ipgenerate
add_design_file ./rapidsilicon/ip/axis_adapter/v1_0/adapter_wrapper/src/adapter_wrapper.v
add_library_path rapidsilicon/ip/axis_adapter/v1_0/adapter_wrapper/src/
set_top_module adapter_wrapper
synth delay

