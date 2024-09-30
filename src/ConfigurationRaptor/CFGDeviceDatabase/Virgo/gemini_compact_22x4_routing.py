from routing_library import function_library
load_model("routing_library/gemini_compact_22x4_gbox_hp_40x1.py")

# Block
create_block(name="Virgo", top=True)

# Ports
for type_bank in [["P", 1]] :
  for i in range(40) :
    if i in [16, 17, 36, 37]:
      continue
    location = function_library.get_location(type_bank[0], type_bank[1], i)
    add_port(name=location, dir=DIR_IN)
add_port(name="fabric_clk", dir=DIR_OUT,  bit=16)
add_port(name="fclk_buf",   dir=DIR_IN,   bit=8)

# Instances
add_instance(name="hp_40x1", block="gbox_hp_40x1")

# Connections
# pin --> hp/hvl/hvr pin
for type_bank in [["P", 1, 0]] :
  instance = "hp_40x1"
  bank = type_bank[2]
  bank_pin_name = "bank%d_rx_in" % bank
  for i in range(40) :
    if i in [16, 17, 36, 37]:
      continue
    top_location = function_library.get_location(type_bank[0], type_bank[1], i)
    add_connection(source=top_location,                 destinations=["%s->%s[%d]" % (instance, bank_pin_name, i)])
# hp instance fabric clk --> fabric clk
for i in range(16) :
  add_connection(source="hp_40x1->fabric_clk[%d]" % i,  destinations=["fabric_clk[%d]" % i])
# fclk buf --> instance fclk buf
for i in range(8) :
  add_connection(source="fclk_buf[%d]" % i,             destinations=["hp_40x1->fclk_buf[%d]" % i])
  
# Mapping to TCL model
for type_bank in [["P", 1, 0]] :
  # Pin location
  instance = "hp_40x1"
  bank = type_bank[2]
  for i in range(40) :
    if i in [16, 17, 36, 37]:
      continue
    # Gearbox mapping - hp_40x1.bank0_hpio.gearbox_P[0] ==> u_GBOX_HP_40X2.u_HP_GBOX_BK0_A_18 or HP_1_0_0P
    python_name = "%s.bank%d_hpio.%s" % (instance, bank, function_library.get_gbox_top_name(i))
    tcl_name = function_library.get_location(type_bank[0], type_bank[1], i)
    add_tcl_map(python_name, tcl_name)
# Top Level Type
add_tcl_map("hp_40x1", "u_GBOX_HP_40X2")
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
# The graphviz module that we use to draw diagram have difficulty supporting ":"
# Suppose we have to display [39:0]
mapped_name = "hp_bank0_pin[39:0]"
for i in range(40) :
  if i in [16, 17, 36, 37]:
    continue
  location = function_library.get_location("P", 1, i)
  add_diagram_map(name=location, mapped_name=mapped_name)
