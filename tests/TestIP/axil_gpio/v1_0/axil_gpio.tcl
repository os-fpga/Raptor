create_design axil_gpio_prj

target_device 1GE100-ES1

set IP_PATH ./axil_gpio_prj/run_1/IPs/


#Configuring axil_gpio_v1_0 IP 
configure_ip axil_gpio_v1_0 -mod_name axil_gpio_wrapper -Pdata_width=32 -Paddr_width=16 -out_file $IP_PATH/axil_gpio_wrapper

#Generate IP
ipgenerate

#Add generate IP as a design and synthesize it
add_design_file  $IP_PATH/rapidsilicon/ip/axil_gpio/v1_0/axil_gpio_wrapper/src/axil_gpio_wrapper_v1_0.sv
add_library_path $IP_PATH/rapidsilicon/ip/axil_gpio/v1_0/axil_gpio_wrapper/src/
add_include_path $IP_PATH/rapidsilicon/ip/axil_gpio/v1_0/axil_gpio_wrapper/src/
add_library_ext .v .sv

set_top_module axil_gpio_wrapper
parser_type surelog
synth delay

