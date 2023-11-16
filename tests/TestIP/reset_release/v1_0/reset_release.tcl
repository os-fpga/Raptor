create_design reset_release_prj

target_device 1GE100-ES1

#Configuring reset_release_v1_0 IP 
configure_ip reset_release_v1_0 -mod_name reset_release_wrapper -Pext_reset_width=7 -out_file ./reset_release_wrapper

#Generate IP
ipgenerate

#Add generate IP as a design and synthesize it
add_design_file ./rapidsilicon/ip/reset_release/v1_0/reset_release_wrapper/src/reset_release_wrapper.v
add_library_path rapidsilicon/ip/reset_release/v1_0/reset_release_wrapper/src/
set_top_module reset_release_wrapper
parser_type surelog
synth delay

