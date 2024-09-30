# Block
create_block(name="gbox_pll_refmux")

# Ports
add_port(name="rosc_clk",           dir=DIR_IN)
add_port(name="bank0_hp_rx_io_clk", dir=DIR_IN, bit=2)
add_port(name="bank1_hp_rx_io_clk", dir=DIR_IN, bit=2)
add_port(name="bank0_hv_rx_io_clk", dir=DIR_IN, bit=2)
add_port(name="bank1_hv_rx_io_clk", dir=DIR_IN, bit=2)
add_port(name="out",                dir=DIR_OUT)

# Config
'''
  selection              |  use_rosc  use_hv  hp_bank_rx_io_sel  hp_rx_io_sel  hv_bank_rx_io_sel  hv_rx_io_sel
  ---------              |  --------  ------  -----------------  ------------  -----------------  ------------  
  rosc_clk               |      1        x            x               xx               xx              x
  bank0_hp_rx_io_clk[0]  |      0        0            0               x0               xx              x
  bank0_hp_rx_io_clk[1]  |      0        0            0               x1               xx              x
  bank1_hp_rx_io_clk[0]  |      0        0            1               0x               xx              x
  bank1_hp_rx_io_clk[1]  |      0        0            1               1x               xx              x
  bank0_hv_rx_io_clk[0]  |      0        1            x               xx               00              0
  bank0_hv_rx_io_clk[1]  |      0        1            x               xx               00              1
  bank1_hv_rx_io_clk[0]  |      0        1            x               xx               01              0
  bank1_hv_rx_io_clk[1]  |      0        1            x               xx               01              1
'''
selection = {
  0b10000000 : "rosc_clk",
  0b00000000 : "bank0_hp_rx_io_clk[0]",
  0b00001000 : "bank0_hp_rx_io_clk[1]",
  0b00100000 : "bank1_hp_rx_io_clk[0]",
  0b00110000 : "bank1_hp_rx_io_clk[1]",
  0b01000000 : "bank0_hv_rx_io_clk[0]",
  0b01000001 : "bank0_hv_rx_io_clk[1]",
  0b01000010 : "bank1_hv_rx_io_clk[0]",
  0b01000011 : "bank1_hv_rx_io_clk[1]"
}
bits = [
  {"cfg_pllref_hv_rx_io_sel" : 1},
  {"cfg_pllref_hv_bank_rx_io_sel" : 2},
  {"cfg_pllref_hp_rx_io_sel" : 2},
  {"cfg_pllref_hp_bank_rx_io_sel" : 1},
  {"cfg_pllref_use_hv" : 1},
  {"cfg_pllref_use_rosc" : 1}
]
add_config_mux(out="out",
                selection=selection,
                bits=bits)

# Add parameters
parameter_script = """
if 'DIVIDE_CLK_IN_BY_2' in self.parameters and self.parameters['DIVIDE_CLK_IN_BY_2'] in ['TRUE', 'ON', '1']:
  self.defined_parameters['cfg_pllref_use_div'] = 1
else:
  self.defined_parameters['cfg_pllref_use_div'] = 0
"""
add_parameter("PLL", parameter_script)