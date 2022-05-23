# -name is used for creating virtual clock and for actual clock -name option will not be used
# create_clock -period 7.935999999999999 clk (actual clock)
create_clock -period 7.935999999999999 -name clk
set_input_delay 1 -clock clk [get_ports {*}]
set_output_delay 1 -clock clk [get_ports {*}]

# set_io a pad_fpga_io[18]
# set_io b pad_fpga_io[19]
# set_io c pad_fpga_io[20]