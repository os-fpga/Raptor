# -name is used for creating virtual clock and for actual clock -name option will not be used
create_clock -period 2.5 -name clk
set_input_delay 1 -clock clk [get_ports {A[0]}]
set_input_delay 1 -clock clk [get_ports {A[1]}]
set_input_delay 1 -clock clk [get_ports {B[0]}]
set_input_delay 1 -clock clk [get_ports {B[1]}]
set_output_delay 0.5 -clock clk [get_ports {P[0]}]
set_output_delay 0.5 -clock clk [get_ports {P[1]}]
set_output_delay 0.5 -clock clk [get_ports {P[2]}]
set_output_delay 0.5 -clock clk [get_ports {P[3]}]
