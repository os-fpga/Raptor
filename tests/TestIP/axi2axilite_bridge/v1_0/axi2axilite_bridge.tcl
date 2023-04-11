create_design axi2axilite_bridge_prj

target_device 1GE100

#Configuring axi2axilite_bridge_v1_0 IP 
configure_ip axi2axilite_bridge_v1_0 -mod_name axi2axilite_bridge_wrapper -Pdata_width=64 -Paddr_width=8 -out_file ./axi2axilite_bridge_wrapper.v

#Generate IP
ipgenerate

#Add generate IP as a design and synthesize it
add_design_file ./rapidsilicon/ip/axi2axilite_bridge/v1_0/axi2axilite_bridge_wrapper/src/axi2axilite_bridge_wrapper.v
add_library_path rapidsilicon/ip/axi2axilite_bridge/v1_0/axi2axilite_bridge_wrapper/src/
set_top_module axi2axilite_bridge_wrapper
synth delay

