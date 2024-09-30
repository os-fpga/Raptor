# Block
create_block(name="rc_osc_50mhz")

# Ports
add_port(name="osc",    dir=DIR_IN)
add_port(name="o_osc",  dir=DIR_OUT)

# Connections
add_connection(source="osc", destinations=["o_osc"])
