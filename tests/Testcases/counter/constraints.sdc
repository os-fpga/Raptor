create_clock -period 7.935999999999999 -name clk[0]
set_input_delay 1 -clock clk[0] [get_ports {*}]
set_output_delay 1 -clock clk[0] [get_ports {*}]

set_pin_loc clk[0] pad_fpga_io[15]
set_pin_loc clk[1] pad_fpga_io[16]
set_pin_loc clk[2] pad_fpga_io[17]
set_pin_loc clk[3] pad_fpga_io[18]
set_pin_loc reset pad_fpga_io[19]
set_pin_loc result[0] pad_fpga_io[20]
set_pin_loc result[1] pad_fpga_io[21]
set_pin_loc result[2] pad_fpga_io[22]
set_pin_loc result[3] pad_fpga_io[23]
set_pin_loc result[4] pad_fpga_io[24]
set_pin_loc result[5] pad_fpga_io[25]
set_pin_loc result[6] pad_fpga_io[26]
set_pin_loc result[7] pad_fpga_io[27]


