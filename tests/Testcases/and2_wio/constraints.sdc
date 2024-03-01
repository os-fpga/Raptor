# SDC file example

# Setting a clock frequency of 200 MHz (5nS period)
#create_clock -period 5 clk
#set_input_delay -max 0 -clock clk [get_ports {a}] 
#set_input_delay -max 0 -clock clk [get_ports {b}]
#set_input_delay -max 0 -clock clk [get_ports {reset}]
#set_output_delay -max 0 -clock clk [get_ports {c}]

