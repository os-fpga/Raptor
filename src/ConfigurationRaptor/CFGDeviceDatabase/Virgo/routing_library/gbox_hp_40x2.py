import function_library
load_model("gbox_osc.py")
load_model("gbox_pll_refmux.py")
load_model("gbox_pll.py")
load_model("gbox_fclk_mux.py")
load_model("gbox_hpio.py")
load_model("gbox_top.py")
load_model("gbox_root_bank_clkmux.py")

# Block
create_block(name="gbox_hp_40x2")

# Ports
add_port(name="bank0_rx_in",              dir=DIR_IN,   bit=40)
add_port(name="bank1_rx_in",              dir=DIR_IN,   bit=40)
add_port(name="hvl_bank0_rx_io_clk",      dir=DIR_IN,   bit=2)
add_port(name="hvl_bank1_rx_io_clk",      dir=DIR_IN,   bit=2)
add_port(name="hvr_bank0_rx_io_clk",      dir=DIR_IN,   bit=2)
add_port(name="hvr_bank1_rx_io_clk",      dir=DIR_IN,   bit=2)
add_port(name="hvl_bank0_root_core_clk",  dir=DIR_IN,   bit=2)
add_port(name="hvl_bank0_root_cdr_clk",   dir=DIR_IN,   bit=2)
add_port(name="hvl_bank1_root_core_clk",  dir=DIR_IN,   bit=2)
add_port(name="hvl_bank1_root_cdr_clk",   dir=DIR_IN,   bit=2)
add_port(name="hvr_bank0_root_core_clk",  dir=DIR_IN,   bit=2)
add_port(name="hvr_bank0_root_cdr_clk",   dir=DIR_IN,   bit=2)
add_port(name="hvr_bank1_root_core_clk",  dir=DIR_IN,   bit=2)
add_port(name="hvr_bank1_root_cdr_clk",   dir=DIR_IN,   bit=2)
add_port(name="fclk_buf",                 dir=DIR_IN,   bit=8)
add_port(name="pll_foutvco",              dir=DIR_OUT,  bit=2)
add_port(name="pll_fout",                 dir=DIR_OUT,  bit=2)
add_port(name="fabric_clk",               dir=DIR_OUT,  bit=16)

# Instances
add_instance(name="rc_osc_50mhz",           block="rc_osc_50mhz")
add_instance(name="pll_refmux[0]",          block="gbox_pll_refmux")
add_instance(name="pll_refmux[1]",          block="gbox_pll_refmux")
add_instance(name="pll[0]",                 block="PLL")
add_instance(name="pll[1]",                 block="PLL")
add_instance(name="bank0_fclk_mux_A",       block="gbox_fclk_mux")
add_instance(name="bank1_fclk_mux_A",       block="gbox_fclk_mux")
add_instance(name="bank0_fclk_mux_B",       block="gbox_fclk_mux")
add_instance(name="bank1_fclk_mux_B",       block="gbox_fclk_mux")
add_instance(name="bank0_hpio",             block="gbox_hpio")
add_instance(name="bank1_hpio",             block="gbox_hpio")
add_instance(name="bank0_root_bank_clkmux", block="gbox_root_bank_clkmux")
add_instance(name="bank1_root_bank_clkmux", block="gbox_root_bank_clkmux")

# Connections
# osc + pin --> refmux
add_connection(source="rc_osc_50mhz->o_osc",        destinations=["pll_refmux[0]->rosc_clk",              "pll_refmux[1]->rosc_clk"])
add_connection(source="bank0_hpio->rx_io_clk[0]",   destinations=["pll_refmux[0]->bank0_hp_rx_io_clk[0]", "pll_refmux[1]->bank0_hp_rx_io_clk[0]"])
add_connection(source="bank0_hpio->rx_io_clk[1]",   destinations=["pll_refmux[0]->bank0_hp_rx_io_clk[1]", "pll_refmux[1]->bank0_hp_rx_io_clk[1]"])
add_connection(source="bank1_hpio->rx_io_clk[0]",   destinations=["pll_refmux[0]->bank1_hp_rx_io_clk[0]", "pll_refmux[1]->bank1_hp_rx_io_clk[0]"])
add_connection(source="bank1_hpio->rx_io_clk[1]",   destinations=["pll_refmux[0]->bank1_hp_rx_io_clk[1]", "pll_refmux[1]->bank1_hp_rx_io_clk[1]"])
add_connection(source="hvl_bank0_rx_io_clk[0]",     destinations=["pll_refmux[0]->bank0_hv_rx_io_clk[0]"])
add_connection(source="hvl_bank0_rx_io_clk[1]",     destinations=["pll_refmux[0]->bank0_hv_rx_io_clk[1]"])
add_connection(source="hvl_bank1_rx_io_clk[0]",     destinations=["pll_refmux[0]->bank1_hv_rx_io_clk[0]"])
add_connection(source="hvl_bank1_rx_io_clk[1]",     destinations=["pll_refmux[0]->bank1_hv_rx_io_clk[1]"])
add_connection(source="hvr_bank0_rx_io_clk[0]",     destinations=["pll_refmux[1]->bank0_hv_rx_io_clk[0]"])
add_connection(source="hvr_bank0_rx_io_clk[1]",     destinations=["pll_refmux[1]->bank0_hv_rx_io_clk[1]"])
add_connection(source="hvr_bank1_rx_io_clk[0]",     destinations=["pll_refmux[1]->bank1_hv_rx_io_clk[0]"])
add_connection(source="hvr_bank1_rx_io_clk[1]",     destinations=["pll_refmux[1]->bank1_hv_rx_io_clk[1]"])
# refmux --> pll
add_connection(source="pll_refmux[0]->out",         destinations=["pll[0]->fref"])
add_connection(source="pll_refmux[1]->out",         destinations=["pll[1]->fref"])
# pll + pin --> fclk mux
add_connection(source="pll[0]->foutvco",            destinations=["bank0_fclk_mux_A->vco_clk[0]", "pll_foutvco[0]"])
add_connection(source="pll[0]->fout[0]",            destinations=["bank0_fclk_mux_A->vco_clk[1]", "pll_fout[0]"])
add_connection(source="bank0_hpio->rx_io_clk[0]",   destinations=["bank0_fclk_mux_A->rx_io_clk[0]"])
add_connection(source="bank0_hpio->rx_io_clk[1]",   destinations=["bank0_fclk_mux_A->rx_io_clk[1]"])
add_connection(source="pll[1]->foutvco",            destinations=["bank1_fclk_mux_A->vco_clk[0]", "pll_foutvco[1]"])
add_connection(source="pll[1]->fout[0]",            destinations=["bank1_fclk_mux_A->vco_clk[1]", "pll_fout[1]"])
add_connection(source="bank1_hpio->rx_io_clk[0]",   destinations=["bank1_fclk_mux_A->rx_io_clk[0]"])
add_connection(source="bank1_hpio->rx_io_clk[1]",   destinations=["bank1_fclk_mux_A->rx_io_clk[1]"])
add_connection(source="pll[0]->foutvco",            destinations=["bank0_fclk_mux_B->vco_clk[0]"])
add_connection(source="pll[0]->fout[0]",            destinations=["bank0_fclk_mux_B->vco_clk[1]"])
add_connection(source="bank0_hpio->rx_io_clk[0]",   destinations=["bank0_fclk_mux_B->rx_io_clk[0]"])
add_connection(source="bank0_hpio->rx_io_clk[1]",   destinations=["bank0_fclk_mux_B->rx_io_clk[1]"])
add_connection(source="pll[1]->foutvco",            destinations=["bank1_fclk_mux_B->vco_clk[0]"])
add_connection(source="pll[1]->fout[0]",            destinations=["bank1_fclk_mux_B->vco_clk[1]"])
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
    gbox_pin = "bank%d_rx_in[%d]" % (bank, i)
    add_connection(source=gbox_pin,                                 destinations=["%s->rx_in[%d]" % (gbox_hpio_name, i)])
    add_connection(source="%s->core_clk[%d]" % (gbox_hpio_name, i), destinations=["%s->core_clk_in[%d]" % (gbox_root_bank_clkmux_name, i)])
    add_connection(source="%s->cdr_clk[%d]" % (gbox_hpio_name, i),  destinations=["%s->cdr_clk_in[%d]" % (gbox_root_bank_clkmux_name, i)])

# Config
# Root selection for fabric_clk
root_mux_selection = {
  0   : "bank0_root_bank_clkmux->core_clk[0]",
  1   : "bank0_root_bank_clkmux->core_clk[1]",
  2   : "bank1_root_bank_clkmux->core_clk[0]",
  3   : "bank1_root_bank_clkmux->core_clk[1]",
  4   : "bank0_root_bank_clkmux->cdr_clk[0]",
  5   : "bank0_root_bank_clkmux->cdr_clk[1]",
  6   : "bank1_root_bank_clkmux->cdr_clk[0]",
  7   : "bank1_root_bank_clkmux->cdr_clk[1]",
  8   : "hvl_bank0_root_core_clk[0]",
  9   : "hvl_bank0_root_core_clk[1]",
  10  : "hvl_bank1_root_core_clk[0]",
  11  : "hvl_bank1_root_core_clk[1]",
  12  : "hvl_bank0_root_cdr_clk[0]",
  13  : "hvl_bank0_root_cdr_clk[1]",
  14  : "hvl_bank1_root_cdr_clk[0]",
  15  : "hvl_bank1_root_cdr_clk[1]",
  16  : "hvr_bank0_root_core_clk[0]",
  17  : "hvr_bank0_root_core_clk[1]",
  18  : "hvr_bank1_root_core_clk[0]",
  19  : "hvr_bank1_root_core_clk[1]",
  20  : "hvr_bank0_root_cdr_clk[0]",
  21  : "hvr_bank0_root_cdr_clk[1]",
  22  : "hvr_bank1_root_cdr_clk[0]",
  24  : "hvr_bank1_root_cdr_clk[1]",
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
                  