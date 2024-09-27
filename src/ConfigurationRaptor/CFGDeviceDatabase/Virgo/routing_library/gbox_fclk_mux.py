# Block
create_block(name="gbox_fclk_mux")

# Ports
add_port(name="vco_clk",    dir=DIR_IN, bit=2)
add_port(name="rx_io_clk",  dir=DIR_IN, bit=2)
add_port(name="fast_clk",   dir=DIR_OUT, bit=1)

# Config
add_config_mux(out="fast_clk",
                selection={0b000 : "vco_clk[0]",
                           0b001 : "vco_clk[1]",
                           0b100 : "rx_io_clk[0]",
                           0b110 : "rx_io_clk[1]"},
                bits=[{"vco_clk_sel" : 1},
                      {"rx_fclkio_sel" : 1},
                      {"rxclk_phase_sel":1}])
