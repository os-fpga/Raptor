# External PLL Ref Clock
create_clock -period 10 clk

# PLL Clock output 
create_generated_clock -source [get_clocks clk] -divide_by 4 [get_nets clk_design]

# Design clock divider
create_generated_clock -source [get_clocks clk_design] -divide_by 2 [get_nets half_clk]
