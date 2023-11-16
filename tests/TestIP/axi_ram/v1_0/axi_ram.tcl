create_design axi_ram_prj

target_device 1GE100-ES1

#Configuring axi_ram_v1_0 IP 
configure_ip axi_ram_v1_0 -mod_name axi_ram_wrapper -Pdata_width=64 -Paddr_width=8 -Pid_width=8 -Ppip_out=1 -out_file ./axi_ram_wrapper

#Generate IP
ipgenerate

#Add generate IP as a design and synthesize it
add_design_file ./rapidsilicon/ip/axi_ram/v1_0/axi_ram_wrapper/src/axi_ram_wrapper.v
add_library_path rapidsilicon/ip/axi_ram/v1_0/axi_ram_wrapper/src/
set_top_module axi_ram_wrapper
parser_type surelog
synth delay

