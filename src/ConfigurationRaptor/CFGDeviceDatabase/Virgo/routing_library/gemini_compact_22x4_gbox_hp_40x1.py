import function_library
load_model("gbox_osc.py")
load_model("gbox_pll_refmux.py")
load_model("gbox_pll.py")
load_model("gbox_fclk_mux.py")
load_model("gbox_hpio.py")
load_model("gbox_top.py")
load_model("gbox_root_bank_clkmux.py")

# Block
create_block(name="gbox_hp_40x1")

# Ports
add_port(name="bank0_rx_in",                    dir=DIR_IN,   bit=40)
add_port(name="fclk_buf",                       dir=DIR_IN,   bit=8)
add_port(name="fabric_clk",                     dir=DIR_OUT,  bit=16)

# Instances
add_instance(name="rc_osc_50mhz",               block="rc_osc_50mhz")
add_instance(name="pll_refmux[0]",              block="gbox_pll_refmux")
add_instance(name="pll_refmux[1]",              block="gbox_pll_refmux")
add_instance(name="pll[0]",                     block="PLL")
add_instance(name="pll[1]",                     block="PLL")
add_instance(name="bank0_fclk_mux_A",           block="gbox_fclk_mux")
add_instance(name="bank0_fclk_mux_B",           block="gbox_fclk_mux")
add_instance(name="bank0_hpio",                 block="gbox_hpio")
add_instance(name="bank0_root_bank_clkmux",     block="gbox_root_bank_clkmux")

# Connections
# osc + pin --> refmux
add_connection(source="rc_osc_50mhz->o_osc",                  destinations=["pll_refmux[0]->rosc_clk",              "pll_refmux[1]->rosc_clk"])
add_connection(source="bank0_hpio->rx_io_clk[0]",             destinations=["pll_refmux[0]->bank0_hp_rx_io_clk[0]", "pll_refmux[1]->bank0_hp_rx_io_clk[0]"])
add_connection(source="bank0_hpio->rx_io_clk[1]",             destinations=["pll_refmux[0]->bank0_hp_rx_io_clk[1]", "pll_refmux[1]->bank0_hp_rx_io_clk[1]"])
# refmux --> pll
add_connection(source="pll_refmux[0]->out",                   destinations=["pll[0]->fref"])
add_connection(source="pll_refmux[1]->out",                   destinations=["pll[1]->fref"])
# pll + pin --> fclk mux
add_connection(source="pll[0]->foutvco",                      destinations=["bank0_fclk_mux_A->vco_clk[0]"])
add_connection(source="pll[0]->fout[0]",                      destinations=["bank0_fclk_mux_A->vco_clk[1]"])
add_connection(source="bank0_hpio->rx_io_clk[0]",             destinations=["bank0_fclk_mux_A->rx_io_clk[0]"])
add_connection(source="bank0_hpio->rx_io_clk[1]",             destinations=["bank0_fclk_mux_A->rx_io_clk[1]"])
add_connection(source="pll[0]->foutvco",                      destinations=["bank0_fclk_mux_B->vco_clk[0]"])
add_connection(source="pll[0]->fout[0]",                      destinations=["bank0_fclk_mux_B->vco_clk[1]"])
add_connection(source="bank0_hpio->rx_io_clk[0]",             destinations=["bank0_fclk_mux_B->rx_io_clk[0]"])
add_connection(source="bank0_hpio->rx_io_clk[1]",             destinations=["bank0_fclk_mux_B->rx_io_clk[1]"])
# fclk mux --> gearbox fast clk
add_connection(source="bank0_fclk_mux_A->fast_clk",           destinations=["bank0_hpio->fast_clk_A"])
add_connection(source="bank0_fclk_mux_B->fast_clk",           destinations=["bank0_hpio->fast_clk_B"])
# pin --> gearbox pin
# gearbox core clk + cdr clk --> root bank core clk + cdr clk
gbox_hpio_name = "bank0_hpio"
gbox_root_bank_clkmux_name = "bank0_root_bank_clkmux"
for i in range(40) :
  if i in [16, 17, 36, 37]:
    continue
  gbox_pin = "bank0_rx_in[%d]" % (i)
  add_connection(source=gbox_pin,                                   destinations=["%s->rx_in[%d]" % (gbox_hpio_name, i)])
  add_connection(source="%s->core_clk[%d]" % (gbox_hpio_name, i),   destinations=["%s->core_clk_in[%d]" % (gbox_root_bank_clkmux_name, i)])
  add_connection(source="%s->cdr_clk[%d]" % (gbox_hpio_name, i),    destinations=["%s->cdr_clk_in[%d]" % (gbox_root_bank_clkmux_name, i)])

# Config
# Root selection for fabric_clk
root_mux_selection = {
  0   : "bank0_root_bank_clkmux->core_clk[0]",
  1   : "bank0_root_bank_clkmux->core_clk[1]",
  4   : "bank0_root_bank_clkmux->cdr_clk[0]",
  5   : "bank0_root_bank_clkmux->cdr_clk[1]",
  32  : "pll[0]->fout[0]",
  33  : "pll[0]->fout[1]",
  34  : "pll[0]->fout[2]",
  35  : "pll[0]->fout[3]",
  36  : "pll[1]->fout[0]",
  37  : "pll[1]->fout[1]",
  38  : "pll[1]->fout[2]",
  39  : "pll[1]->fout[3]",
  40  : "fclk_buf[4]",
  41  : "fclk_buf[5]",
  42  : "fclk_buf[6]",
  43  : "fclk_buf[7]",
  44  : "fclk_buf[0]",
  45  : "fclk_buf[1]",
  46  : "fclk_buf[2]",
  47  : "fclk_buf[3]",
  48  : "rc_osc_50mhz->o_osc"
}
for i in range(16) :
  add_config_mux(out="fabric_clk[%d]" % i,
                  selection=root_mux_selection,
                  bits=[{"root_mux_sel[%d]" % i : 6}])
                  