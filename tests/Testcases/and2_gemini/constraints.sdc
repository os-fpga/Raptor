# -name is used for creating virtual clock and for actual clock -name option will not be used
create_clock -period 2.5 -name clk
set_input_delay 1 -clock clk [get_ports {*}]
set_output_delay 1 -clock clk [get_ports {*}]

set_pin_loc a Bank_H_1_12
set_pin_loc b Bank_H_1_14
set_pin_loc c Bank_H_1_15

