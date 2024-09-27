import function_library
load_model("gbox_top.py")

# Block
create_block(name="gbox_hpio")

# Ports
add_port(name="fast_clk_A", dir=DIR_IN)
add_port(name="fast_clk_B", dir=DIR_IN)
add_port(name="rx_in",      dir=DIR_IN,   bit=40)
add_port(name="rx_io_clk",  dir=DIR_OUT,  bit=2)
add_port(name="core_clk",   dir=DIR_OUT,  bit=40)
add_port(name="cdr_clk",    dir=DIR_OUT,  bit=40)
add_port(name="tx_clk",     dir=DIR_OUT,  bit=40)

# Instances
for i in range(40) :
  instance_name = function_library.get_gbox_top_name(i)
  add_instance(name=instance_name, block="gbox_top")
  
# Connections
for i in range(40) :
  instance_name = function_library.get_gbox_top_name(i)
  if i < 20 :
    add_connection(source="fast_clk_A",                 destinations=["%s->fast_clk" % instance_name])
  else :
    add_connection(source="fast_clk_B",                 destinations=["%s->fast_clk" % instance_name])
  add_connection(source="rx_in[%d]" % i,                destinations=["%s->rx_in" % instance_name])
  add_connection(source="%s->core_clk" % instance_name, destinations=["core_clk[%d]" % i])
  add_connection(source="%s->cdr_clk" % instance_name,  destinations=["cdr_clk[%d]" % i])
  add_connection(source="%s->tx_clk" % instance_name,   destinations=["tx_clk[%d]" % i])
add_connection(source="rx_in[18]",                      destinations=["rx_io_clk[0]"])
add_connection(source="rx_in[38]",                      destinations=["rx_io_clk[1]"])
