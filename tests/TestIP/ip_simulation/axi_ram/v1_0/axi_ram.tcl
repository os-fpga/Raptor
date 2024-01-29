create_design axi_ram_prj
target_device 1GE100-ES1

set IP_PATH ./axi_ram_prj/axi_ram_prj.IPs

configure_ip axi_ram_v1_0 -mod_name axi_ram_wrapper -Pdata_width=64 -Paddr_width=8 -Pid_width=8 -Ppip_out=1 -out_file $IP_PATH/axi_ram_wrapper
ipgenerate

add_design_file $IP_PATH/rapidsilicon/ip/axi_ram/v1_0/axi_ram_wrapper/src/axi_ram_wrapper_v1_0.v
add_library_path $IP_PATH/rapidsilicon/ip/axi_ram/v1_0/axi_ram_wrapper/src/
set_top_module axi_ram_wrapper

simulate_ip axi_ram_wrapper

set SUCCESS [file exists $IP_PATH/simulation/rapidsilicon/ip/axi_ram/v1_0/axi_ram_wrapper/axi_ram_wrapper.fst]
if {$SUCCESS == 0} {
	error "File axi_ram_wrapper.fst not found"
}
