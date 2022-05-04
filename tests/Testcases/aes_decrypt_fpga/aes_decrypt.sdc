# SDC file example
create_clock -period 2.5 -name clk [get_ports clk]
set_pin_loc -pin inpad00 [get_ports clk]
