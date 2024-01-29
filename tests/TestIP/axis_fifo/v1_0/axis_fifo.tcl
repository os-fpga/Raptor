create_design axis_fifo_prj

target_device 1GE100-ES1

set IP_PATH ./axis_fifo_prj/axis_fifo_prj.IPs


#Configuring axis_fifo_v1_0 IP 
configure_ip axis_fifo_v1_0 -mod_name axis_fifo_wrapper -Pdepth=4096 -Pdata_width=16 -out_file $IP_PATH/axis_fifo_wrapper

#Generate IP
ipgenerate

#Add generate IP as a design and synthesize it
add_design_file  $IP_PATH/rapidsilicon/ip/axis_fifo/v1_0/axis_fifo_wrapper/src/axis_fifo_wrapper_v1_0.v
add_library_path $IP_PATH/rapidsilicon/ip/axis_fifo/v1_0/axis_fifo_wrapper/src/
set_top_module axis_fifo_wrapper
synth delay

