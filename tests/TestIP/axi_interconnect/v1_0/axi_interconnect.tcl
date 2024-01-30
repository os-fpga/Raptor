create_design axi_interconnect_prj

target_device 1GE100-ES1

#Configuring axi_interconnect_v1_0 IP 
configure_ip axi_interconnect_v1_0 -mod_name axi_interconnect_wrapper -Pdata_width=32 -Paddr_width=32 -out_file ./axi_interconnect_prj/run_1/IPs/axi_interconnect_wrapper

#Generate IP
ipgenerate

#Add generate IP as a design and synthesize it
add_design_file ./axi_interconnect_prj/run_1/IPs/rapidsilicon/ip/axi_interconnect/v1_0/axi_interconnect_wrapper/src/axi_interconnect_wrapper_v1_0.v
add_design_file ./axi_interconnect_prj/run_1/IPs/rapidsilicon/ip/axi_interconnect/v1_0/axi_interconnect_wrapper/src/axi_interconnect.v
add_design_file ./axi_interconnect_prj/run_1/IPs/rapidsilicon/ip/axi_interconnect/v1_0/axi_interconnect_wrapper/src/arbiter.v
add_design_file ./axi_interconnect_prj/run_1/IPs/rapidsilicon/ip/axi_interconnect/v1_0/axi_interconnect_wrapper/src/priority_encoder.v
set_top_module axi_interconnect_wrapper
parser_type surelog
synth_options -new_tdp36k
synth delay

