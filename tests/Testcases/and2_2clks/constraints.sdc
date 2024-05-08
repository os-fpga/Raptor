# SDC file example

# Setting a clock frequency of 200 MHz (5nS period)
create_clock -period 5 [get_ports {clk1}] -name clk1
create_generated_clock -source [get_clocks clk1] -divide_by 2 [get_nets clk2]

set_clock_groups -group [get_clocks {clk1}] -group [get_clocks {clk2}] -physically_exclusive
create_clock -name clk3 -period 6
set_false_path -from [get_clocks {clk1}] -to [get_clocks {clk2}]
