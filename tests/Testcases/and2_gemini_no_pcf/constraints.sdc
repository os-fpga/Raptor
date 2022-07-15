# -name is used for creating virtual clock and for actual clock -name option will not be used
create_clock -period 2.5 -name clk
set_input_delay 1 -clock clk [get_ports {*}]
set_output_delay 1 -clock clk [get_ports {*}]

# intentionally provide no pin constraint here.
# in such case, raptor should generate a default constraint file to make sure vpr only use legal pins in device to connect user's design ports.
# legal pins are pins inside fpga which are actually wired to top level bump pin of device package.
