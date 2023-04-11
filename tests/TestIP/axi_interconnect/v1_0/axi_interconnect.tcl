create_design axi_interconnect_prj

target_device 1GE100

#Configuring axi_interconnect_v1_0 IP 
configure_ip axi_interconnect_v1_0 -mod_name axi_interconnect_wrapper -Pdata_width=32 -Paddr_width=8 -out_file ./axi_interconnect_wrapper.v

#Generate IP
ipgenerate

#Add generate IP as a design and synthesize it
add_design_file ./rapidsilicon/ip/axi_interconnect/v1_0/axi_interconnect_wrapper/src/axi_interconnect_wrapper.v
add_library_path rapidsilicon/ip/axi_interconnect/v1_0/axi_interconnect_wrapper/src/
set_top_module axi_interconnect_wrapper
synth delay

