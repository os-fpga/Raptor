# Block
create_block(name="gbox_top")

# Ports
add_port(name="fast_clk", dir=DIR_IN)
add_port(name="rx_in",    dir=DIR_IN)
add_port(name="core_clk", dir=DIR_OUT)
add_port(name="cdr_clk",  dir=DIR_OUT)
add_port(name="tx_clk",   dir=DIR_OUT)

# Connections
add_connection(source="fast_clk", destinations=["cdr_clk"])

# Config
add_config_mux(out="core_clk", 
                selection={
                  0 : "fast_clk",
                  1 : "rx_in"
                },
                bits=[{"RX_CLOCK_IO" : 1}])
add_config_mux(out="tx_clk", 
                selection={
                  1 : "fast_clk"
                },
                bits=[{"TX_CLOCK_IO" : 1}])
