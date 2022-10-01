create_design fifo_test
target_device GEMINI
add_litex_ip_catalog ./
puts "Catalog:"
foreach ip [ip_catalog] {
    puts "IP: $ip"
    foreach param [ip_catalog $ip] {
        puts "  PARAM [lindex $param 0], default: [lindex $param 1]"
    }
}
configure_ip axis_fifo_v1_0 -mod_name fifo_wrapper -Pdepth=1024 -Pdata_width=512 -Pid_width=16 -Pdest_width=16 -Puser_width=4096 -Ppip_out=1 -out_file ./fifo_wrapper.v
ipgenerate
add_design_file ./rapidsilicon/ip/axis_fifo/v1_0/fifo_wrapper/src/fifo_wrapper.v
add_library_path rapidsilicon/ip/axis_fifo/v1_0/fifo_wrapper/src/
set_top_module fifo_wrapper
synth delay

