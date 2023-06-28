# -name is used for creating virtual clock and for actual clock -name option will not be used
create_clock -period 0.1 clock0
set_input_delay 0.1 -clock clock0 [get_ports rst_counter]
set_output_delay 0.1 -clock clock0 [get_ports q_counter[0]]
