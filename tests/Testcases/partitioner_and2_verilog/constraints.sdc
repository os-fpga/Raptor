# SDC file example

# Setting a clock frequency of 200 MHz (5nS period)
create_clock -period 5 clk
set_input_delay 0.8 -clock clk [get_ports {a}] 
set_input_delay 0.8 -clock clk [get_ports {b}]

