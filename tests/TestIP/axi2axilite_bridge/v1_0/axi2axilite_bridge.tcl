create_design axi2axilite_bridge_prj

target_device 1GE100-ES1

#set path here

set IP_PATH ./axi2axilite_bridge_prj/run_1/IPs/

#Configuring axi2axilite_bridge_v1_0 IP 
configure_ip axi2axilite_bridge_v1_0 -mod_name axi2axilite_bridge_wrapper -Pdata_width=64 -Paddr_width=8 -out_file $IP_PATH/axi2axilite_bridge_wrapper

#Generate IP
ipgenerate

#Add generate IP as a design and synthesize it
add_design_file  $IP_PATH/rapidsilicon/ip/axi2axilite_bridge/v1_0/axi2axilite_bridge_wrapper/src/axi2axilite_bridge_wrapper_v1_0.v
add_library_path $IP_PATH/rapidsilicon/ip/axi2axilite_bridge/v1_0/axi2axilite_bridge_wrapper/src/
set_top_module axi2axilite_bridge_wrapper
parser_type surelog
synth_options -new_tdp36k
synth delay

