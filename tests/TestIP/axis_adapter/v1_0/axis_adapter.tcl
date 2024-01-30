create_design axis_adapter_prj

target_device 1GE100-ES1


set IP_PATH ./axis_adapter_prj/run_1/IPs/


#Configuring axis_adapter_v1_0 IP 
configure_ip axis_adapter_v1_0 -mod_name axis_adapter_wrapper -Ps_data_width=32 -out_file $IP_PATH/axis_adapter_wrapper
ipgenerate
add_design_file  $IP_PATH/rapidsilicon/ip/axis_adapter/v1_0/axis_adapter_wrapper/src/axis_adapter_wrapper_v1_0.v
add_library_path $IP_PATH/rapidsilicon/ip/axis_adapter/v1_0/axis_adapter_wrapper/src/
set_top_module axis_adapter_wrapper
parser_type surelog
synth delay


