create_design axi_cdma_prj

target_device 1GE100-ES1

#Configuring axi_cdma_v1_0 IP 
configure_ip axi_cdma_v1_0 -mod_name axi_cdma_wrapper -Pdata_width=64 -Paddr_width=8 -out_file ./axi_cdma_wrapper

#Generate IP
ipgenerate

#Add generate IP as a design and synthesize it
add_design_file ./rapidsilicon/ip/axi_cdma/v1_0/axi_cdma_wrapper/src/axi_cdma_wrapper.v
add_library_path rapidsilicon/ip/axi_cdma/v1_0/axi_cdma_wrapper/src/
set_top_module axi_cdma_wrapper
parser_type surelog
synth delay

