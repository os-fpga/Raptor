create_clock -period 2.5 clki
create_clock -period 2.5 clko
set_input_delay 1 -clock clki [get_ports {reg_*}]
set_output_delay 1 -clock clko [get_ports {Q}]
