import function_library
load_model("gbox_hpio.py")

# Block
create_block(name="gbox_hv_40x2")

# Ports
add_port(name="bank0_rx_in",          dir=DIR_IN,   bit=40)
add_port(name="bank1_rx_in",          dir=DIR_IN,   bit=40)
add_port(name="pll_fout",             dir=DIR_IN,   bit=1)
add_port(name="pll_foutvco",          dir=DIR_IN,   bit=1)
add_port(name="bank0_rx_io_clk",      dir=DIR_OUT,  bit=2)
add_port(name="bank1_rx_io_clk",      dir=DIR_OUT,  bit=2)
add_port(name="bank0_root_core_clk",  dir=DIR_OUT,  bit=2)
add_port(name="bank1_root_core_clk",  dir=DIR_OUT,  bit=2)
add_port(name="bank0_root_cdr_clk",   dir=DIR_OUT,  bit=2)
add_port(name="bank1_root_cdr_clk",   dir=DIR_OUT,  bit=2)

# Instances
add_instance(name="bank0_fclk_mux_A",       block="gbox_fclk_mux")
add_instance(name="bank1_fclk_mux_A",       block="gbox_fclk_mux")
add_instance(name="bank0_fclk_mux_B",       block="gbox_fclk_mux")
add_instance(name="bank1_fclk_mux_B",       block="gbox_fclk_mux")
add_instance(name="bank0_hpio",             block="gbox_hpio")
add_instance(name="bank1_hpio",             block="gbox_hpio")
add_instance(name="bank0_root_bank_clkmux", block="gbox_root_bank_clkmux")
add_instance(name="bank1_root_bank_clkmux", block="gbox_root_bank_clkmux")

# Connections
# pll + pin --> fclk mux
add_connection(source="pll_foutvco",                destinations=["bank0_fclk_mux_A->vco_clk[0]"])
add_connection(source="pll_fout",                   destinations=["bank0_fclk_mux_A->vco_clk[1]"])
add_connection(source="bank0_hpio->rx_io_clk[0]",   destinations=["bank0_fclk_mux_A->rx_io_clk[0]"])
add_connection(source="bank0_hpio->rx_io_clk[1]",   destinations=["bank0_fclk_mux_A->rx_io_clk[1]"])
add_connection(source="pll_foutvco",                destinations=["bank1_fclk_mux_A->vco_clk[0]"])
add_connection(source="pll_fout",                   destinations=["bank1_fclk_mux_A->vco_clk[1]"])
add_connection(source="bank1_hpio->rx_io_clk[0]",   destinations=["bank1_fclk_mux_A->rx_io_clk[0]"])
add_connection(source="bank1_hpio->rx_io_clk[1]",   destinations=["bank1_fclk_mux_A->rx_io_clk[1]"])
add_connection(source="pll_foutvco",                destinations=["bank0_fclk_mux_B->vco_clk[0]"])
add_connection(source="pll_fout",                   destinations=["bank0_fclk_mux_B->vco_clk[1]"])
add_connection(source="bank0_hpio->rx_io_clk[0]",   destinations=["bank0_fclk_mux_B->rx_io_clk[0]"])
add_connection(source="bank0_hpio->rx_io_clk[1]",   destinations=["bank0_fclk_mux_B->rx_io_clk[1]"])
add_connection(source="pll_foutvco",                destinations=["bank1_fclk_mux_B->vco_clk[0]"])
add_connection(source="pll_fout",                   destinations=["bank1_fclk_mux_B->vco_clk[1]"])
add_connection(source="bank1_hpio->rx_io_clk[0]",   destinations=["bank1_fclk_mux_B->rx_io_clk[0]"])
add_connection(source="bank1_hpio->rx_io_clk[1]",   destinations=["bank1_fclk_mux_B->rx_io_clk[1]"])
# fclk mux --> gearbox fast clk
add_connection(source="bank0_fclk_mux_A->fast_clk", destinations=["bank0_hpio->fast_clk_A"])
add_connection(source="bank0_fclk_mux_B->fast_clk", destinations=["bank0_hpio->fast_clk_B"])
add_connection(source="bank1_fclk_mux_A->fast_clk", destinations=["bank1_hpio->fast_clk_A"])
add_connection(source="bank1_fclk_mux_B->fast_clk", destinations=["bank1_hpio->fast_clk_B"])
# pin --> gearbox pin
# gearbox core clk + cdr clk --> root bank core clk + cdr clk
for bank in range(2) :
  gbox_hpio_name = "bank%d_hpio" % bank
  gbox_root_bank_clkmux_name = "bank%d_root_bank_clkmux" % bank
  for i in range(40) :
    source = "bank%d_rx_in[%d]" % (bank, i)
    add_connection(source=source,                                   destinations=["%s->rx_in[%d]" % (gbox_hpio_name, i)])
    source = "%s->core_clk[%d]" % (gbox_hpio_name, i)
    add_connection(source=source,                                   destinations=["%s->core_clk_in[%d]" % (gbox_root_bank_clkmux_name, i)])
    source = "%s->cdr_clk[%d]" % (gbox_hpio_name, i)
    add_connection(source=source,                                   destinations=["%s->cdr_clk_in[%d]" % (gbox_root_bank_clkmux_name, i)])
  for i in range(2) :
    add_connection(source="bank%s_hpio->rx_io_clk[%d]" % (bank, i), destinations=["bank%d_rx_io_clk[%d]" % (bank, i)])
# root bank core clk + cdr clk --> pin core clk + cdr clk
add_connection(source="bank0_root_bank_clkmux->core_clk[0]",        destinations=["bank0_root_core_clk[0]"])
add_connection(source="bank0_root_bank_clkmux->core_clk[1]",        destinations=["bank0_root_core_clk[1]"])
add_connection(source="bank1_root_bank_clkmux->core_clk[0]",        destinations=["bank1_root_core_clk[0]"])
add_connection(source="bank1_root_bank_clkmux->core_clk[1]",        destinations=["bank1_root_core_clk[1]"])
add_connection(source="bank0_root_bank_clkmux->cdr_clk[0]",         destinations=["bank0_root_cdr_clk[0]"])
add_connection(source="bank0_root_bank_clkmux->cdr_clk[1]",         destinations=["bank0_root_cdr_clk[1]"])
add_connection(source="bank1_root_bank_clkmux->cdr_clk[0]",         destinations=["bank1_root_cdr_clk[0]"])
add_connection(source="bank1_root_bank_clkmux->cdr_clk[1]",         destinations=["bank1_root_cdr_clk[1]"])
