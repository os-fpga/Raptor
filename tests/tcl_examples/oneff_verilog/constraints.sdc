# -name is used for creating virtual clock and for actual clock -name option will not be used
create_clock -period 2.5 -name clock0
set_input_delay 1 -clock clock0 [get_ports {d}]
set_input_delay 1 -clock clock0 [get_ports {rstn}]
set_output_delay 1 -clock clock0 [get_ports {q}]
