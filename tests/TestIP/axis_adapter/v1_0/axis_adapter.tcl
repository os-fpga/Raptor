create_design axis_adapter_prj

target_device 1GE100-ES1

#Configuring axis_adapter_v1_0 IP 
configure_ip axis_adapter_v1_0 -mod_name axis_adapter_wrapper -Ps_data_width=32 -out_file ./axis_adapter_wrapper
ipgenerate
add_design_file ./rapidsilicon/ip/axis_adapter/v1_0/axis_adapter_wrapper/src/axis_adapter_wrapper.v
add_library_path rapidsilicon/ip/axis_adapter/v1_0/axis_adapter_wrapper/src/
set_top_module axis_adapter_wrapper
parser_type surelog
synth delay


