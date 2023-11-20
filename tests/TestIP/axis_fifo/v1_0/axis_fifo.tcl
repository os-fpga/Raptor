create_design axis_fifo_prj

target_device 1GE100-ES1

#Configuring axis_fifo_v1_0 IP 
configure_ip axis_fifo_v1_0 -mod_name axis_fifo_wrapper -Pdepth=4096 -Pdata_width=16 -out_file ./axis_fifo_wrapper

#Generate IP
ipgenerate

#Add generate IP as a design and synthesize it
add_design_file ./rapidsilicon/ip/axis_fifo/v1_0/axis_fifo_wrapper/src/axis_fifo_wrapper.v
add_library_path rapidsilicon/ip/axis_fifo/v1_0/axis_fifo_wrapper/src/
set_top_module axis_fifo_wrapper
synth delay

