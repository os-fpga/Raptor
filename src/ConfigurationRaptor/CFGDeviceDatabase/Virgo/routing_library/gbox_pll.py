# PLL
create_block(name="PLL")

# Ports
add_port(name="fref",         dir=DIR_IN)
add_port(name="fout",         dir=DIR_OUT, bit=4)
add_port(name="foutvco",      dir=DIR_OUT, bit=1)

# Connections
add_connection(source="fref", destinations=["fout[0]", "fout[1]", "fout[2]", "fout[3]", "foutvco"])

# Add parameters
parameter_script = """
if 'PLL_DIV' in self.parameters and 'PLL_MULT' in self.parameters and 'PLL_POST_DIV' in self.parameters and '__pll_enable__' in self.parameters:
  post_div = int(self.parameters['PLL_POST_DIV'], 0)
  self.defined_parameters['pll_DSKEWCALBYP'] = 'DSKEWCALBYP_0'
  self.defined_parameters['pll_DSKEWCALIN'] = 0
  self.defined_parameters['pll_DSKEWCALCNT'] = 2
  self.defined_parameters['pll_DSKEWFASTCAL'] = 'DSKEWFASTCAL_0'
  self.defined_parameters['pll_DSKEWCALEN'] = 'DSKEWCALEN_0'
  self.defined_parameters['pll_FRAC'] = 0
  self.defined_parameters['pll_FBDIV'] = int(self.parameters['PLL_MULT'], 0)
  self.defined_parameters['pll_REFDIV'] = int(self.parameters['PLL_DIV'], 0)
  self.defined_parameters['pll_PLLEN'] = int(self.parameters['__pll_enable__'], 0)
  self.defined_parameters['pll_POSTDIV1'] = (post_div >> 4) & 7
  self.defined_parameters['pll_POSTDIV2'] = post_div & 7
  self.defined_parameters['pll_DSMEN'] = 'DSMEN_0'
  self.defined_parameters['pll_DACEN'] = 'DACEN_0' 
"""
add_parameter("PLL", parameter_script)
