# -name is used for creating virtual clock and for actual clock -name option will not be used
# create_clock -period 7.935999999999999 clk (actual clock)
create_clock -period 7.935999999999999 -name clk
set_input_delay 1 -clock clk [get_ports {*}]
set_output_delay 1 -clock clk [get_ports {*}]
