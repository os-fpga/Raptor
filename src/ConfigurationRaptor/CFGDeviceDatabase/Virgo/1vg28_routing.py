from routing_library import function_library
load_model("routing_library/gbox_hp_40x2.py")
load_model("routing_library/gbox_hv_40x2.py")

# Block
create_block(name="Virgo", top=True)

# Ports
for type_bank in [["P", 1], ["P", 2], ["R", 1], ["R", 2], ["R", 3], ["R", 5]] :
  for i in range(40) :
    location = function_library.get_location(type_bank[0], type_bank[1], i)
    add_port(name=location, dir=DIR_IN)
add_port(name="fabric_clk", dir=DIR_OUT,  bit=16)
add_port(name="fclk_buf",   dir=DIR_IN,   bit=8)

# Instances
add_instance(name="hp_40x2", block="gbox_hp_40x2")
add_instance(name="hvl_40x2", block="gbox_hv_40x2")
add_instance(name="hvr_40x2", block="gbox_hv_40x2")

# Connections
# pin --> hp/hvl/hvr pin
for type_bank in [["P", 1, 0], ["P", 2, 1], ["R", 1, 0], ["R", 2, 1], ["R", 3, 0], ["R", 5, 1]] :
  instance = "hp" if type_bank[0] == "P" else ("hvl" if type_bank[1] in [1, 2] else "hvr")
  instance = "%s_40x2" % instance
  bank = type_bank[2]
  bank_pin_name = "bank%d_rx_in" % bank
  for i in range(40) :
    top_location = function_library.get_location(type_bank[0], type_bank[1], i)
    add_connection(source=top_location,                     destinations=["%s->%s[%d]" % (instance, bank_pin_name, i)])
# hvl/hvr clk pin --> hp clk pin
for bank in range(2) :
  for pin in range(2) :
    source_pin = "bank%d_rx_io_clk[%d]" % (bank, pin)
    add_connection(source="hvl_40x2->%s" % (source_pin),    destinations=["hp_40x2->hvl_bank%d_rx_io_clk[%d]" % (bank, pin)])
    add_connection(source="hvr_40x2->%s" % (source_pin),    destinations=["hp_40x2->hvr_bank%d_rx_io_clk[%d]" % (bank, pin)])
# hp pll --> hvl pll
add_connection(source="hp_40x2->pll_foutvco[0]",            destinations=["hvl_40x2->pll_foutvco"])
add_connection(source="hp_40x2->pll_fout[0]",               destinations=["hvl_40x2->pll_fout"])
# hp pll --> hvr pll
add_connection(source="hp_40x2->pll_foutvco[1]",            destinations=["hvr_40x2->pll_foutvco"])
add_connection(source="hp_40x2->pll_fout[1]",               destinations=["hvr_40x2->pll_fout"])
# hvl core clk + cdr clk --> HP core clk + cdr clk
add_connection(source="hvl_40x2->bank0_root_core_clk[0]",   destinations=["hp_40x2->hvl_bank0_root_core_clk[0]"])
add_connection(source="hvl_40x2->bank0_root_core_clk[1]",   destinations=["hp_40x2->hvl_bank0_root_core_clk[1]"])
add_connection(source="hvl_40x2->bank0_root_cdr_clk[0]",    destinations=["hp_40x2->hvl_bank0_root_cdr_clk[0]"])
add_connection(source="hvl_40x2->bank0_root_cdr_clk[1]",    destinations=["hp_40x2->hvl_bank0_root_cdr_clk[1]"])
add_connection(source="hvl_40x2->bank1_root_core_clk[0]",   destinations=["hp_40x2->hvl_bank1_root_core_clk[0]"])
add_connection(source="hvl_40x2->bank1_root_core_clk[1]",   destinations=["hp_40x2->hvl_bank1_root_core_clk[1]"])
add_connection(source="hvl_40x2->bank1_root_cdr_clk[0]",    destinations=["hp_40x2->hvl_bank1_root_cdr_clk[0]"])
add_connection(source="hvl_40x2->bank1_root_cdr_clk[1]",    destinations=["hp_40x2->hvl_bank1_root_cdr_clk[1]"])
# hvr core clk + cdr clk --> HP core clk + cdr clk
add_connection(source="hvr_40x2->bank0_root_core_clk[0]",   destinations=["hp_40x2->hvr_bank0_root_core_clk[0]"])
add_connection(source="hvr_40x2->bank0_root_core_clk[1]",   destinations=["hp_40x2->hvr_bank0_root_core_clk[1]"])
add_connection(source="hvr_40x2->bank0_root_cdr_clk[0]",    destinations=["hp_40x2->hvr_bank0_root_cdr_clk[0]"])
add_connection(source="hvr_40x2->bank0_root_cdr_clk[1]",    destinations=["hp_40x2->hvr_bank0_root_cdr_clk[1]"])
add_connection(source="hvr_40x2->bank1_root_core_clk[0]",   destinations=["hp_40x2->hvr_bank1_root_core_clk[0]"])
add_connection(source="hvr_40x2->bank1_root_core_clk[1]",   destinations=["hp_40x2->hvr_bank1_root_core_clk[1]"])
add_connection(source="hvr_40x2->bank1_root_cdr_clk[0]",    destinations=["hp_40x2->hvr_bank1_root_cdr_clk[0]"])
add_connection(source="hvr_40x2->bank1_root_cdr_clk[1]",    destinations=["hp_40x2->hvr_bank1_root_cdr_clk[1]"])
# hp instance fabric clk --> fabric clk
for i in range(16) :
  add_connection(source="hp_40x2->fabric_clk[%d]" % i,      destinations=["fabric_clk[%d]" % i])
# fclk buf --> instance fclk buf
for i in range(8) :
  add_connection(source="fclk_buf[%d]" % i,                 destinations=["hp_40x2->fclk_buf[%d]" % i])
  
# Mapping to TCL model
for type_bank in [["P", 1, 0], ["P", 2, 1], ["R", 1, 0], ["R", 2, 1], ["R", 3, 0], ["R", 5, 1]] :
  # Pin location
  instance = "hp" if type_bank[0] == "P" else ("hvl" if type_bank[1] in [1, 2] else "hvr")
  instance = "%s_40x2" % instance
  bank = type_bank[2]
  for i in range(40) :
    # Gearbox mapping - hp_40x2.bank0_hpio.gearbox_P[0] ==> u_GBOX_HP_40X2.u_HP_GBOX_BK0_A_18 or HP_1_0_0P
    python_name = "%s.bank%d_hpio.%s" % (instance, bank, function_library.get_gbox_top_name(i))
    tcl_name = function_library.get_location(type_bank[0], type_bank[1], i)
    add_tcl_map(python_name, tcl_name)
for type_bank in [["p", "P", ""], ["vl", "V", "_VL"], ["vr", "V", "_VR"]] :
  # Top Level Type
  add_tcl_map("h%s_40x2" % type_bank[0], "u_GBOX_H%s_40X2%s" % (type_bank[1], type_bank[2]))
for type_bank in [[0, "A"], [0, "B"], [1, "A"], [1, "B"]] :
  # FCLK
  for bit in ["vco_clk_sel", "rx_fclkio_sel", "rxclk_phase_sel"]:
    python_name = "bank%d_fclk_mux_%s->%s" % (type_bank[0], type_bank[1], bit)
    tcl_name = "u_gbox_fclk_mux_all->cfg_%s_%s_%d" % (bit, type_bank[1], type_bank[0])
    add_tcl_map(python_name, tcl_name)
for bank in [0, 1] :
  # Root Bank
  add_tcl_map("bank%d_root_bank_clkmux" % bank, "u_gbox_root_bank_clkmux_%d" % bank)
# PLL REFMUX
add_tcl_map("pll_refmux[0]", "u_gbox_pll_refmux_0")
add_tcl_map("pll_refmux[1]", "u_gbox_pll_refmux_1")
# PLL
add_tcl_map("pll[0]", "u_gbox_PLLTS16FFCFRACF_0")
add_tcl_map("pll[1]", "u_gbox_PLLTS16FFCFRACF_1")
for slot in range(16):
  python_name = "->root_mux_sel[%d]" % slot
  tcl_name = ".u_gbox_clkmux_52x1_left_%d->ROOT_MUX_SEL" % slot
  add_tcl_map(python_name, tcl_name)

# Diagram mapping
for type_bank in [["P", 1, 0], ["P", 2, 1], ["R", 1, 0], ["R", 2, 1], ["R", 3, 0], ["R", 5, 1]] :
  instance = "hp" if type_bank[0] == "P" else ("hvl" if type_bank[1] in [1, 2] else "hvr")
  # The graphviz module that we use to draw diagram have difficulty supporting ":"
  # Suppose we have to display [39:0]
  mapped_name = "%s_bank%s_pin[39:0]" % (instance, type_bank[2])
  for i in range(40) :
    location = function_library.get_location(type_bank[0], type_bank[1], i)
    add_diagram_map(name=location, mapped_name=mapped_name)