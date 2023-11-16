create_design axil_gpio_prj

target_device 1GE100-ES1

#Configuring axil_gpio_v1_0 IP 
configure_ip axil_gpio_v1_0 -mod_name axil_gpio_wrapper -Pdata_width=32 -Paddr_width=16 -out_file ./axil_gpio_wrapper

#Generate IP
ipgenerate

#Add generate IP as a design and synthesize it
add_design_file ./rapidsilicon/ip/axil_gpio/v1_0/axil_gpio_wrapper/src/axil_gpio_wrapper.sv
add_library_path rapidsilicon/ip/axil_gpio/v1_0/axil_gpio_wrapper/src/
add_include_path rapidsilicon/ip/axil_gpio/v1_0/axil_gpio_wrapper/src/
add_library_ext .v .sv

set_top_module axil_gpio_wrapper
parser_type surelog
synth delay

