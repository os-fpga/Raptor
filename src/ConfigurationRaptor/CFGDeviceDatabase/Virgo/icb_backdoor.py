import os
import sys
import re

COMBINATION_MAPPING = {
  "u_gbox_pll_cfg" : ["u_gbox_PLLTS16FFCFRACF_0",
                       "u_gbox_pll_refmux_0",
                       "u_gbox_PLLTS16FFCFRACF_1",
                       "u_gbox_pll_refmux_1"],
  "u_gbox_vco_fask_cfg" : ["u_gbox_fclk_mux_all",
                           "u_gbox_root_bank_clkmux_0",
                           "u_gbox_root_bank_clkmux_1"] +
                          ["u_gbox_clkmux_52x1_left_%d" % i for i in range(16)]
}

RTL_MAPPING = {
  # Instantiated by TOP
  "u_GBOX_HV_40X2_VL" : "u_gbox_hv_VL_EW_BANK_x2",
  "u_GBOX_HP_40X2" : "u_gbox_hp_NS_BANK_x2",
  "u_GBOX_HV_40X2_VR" : "u_gbox_hv_VR_EW_BANK_x2",
  # Instantiated by HP_40x2
  "u_bank_osc" : "u_gbox_osc_cfg",
  "u_HP_PGEN_dummy" : "u_gbox_pgen_cfg0",
  # Instantiated by HV_40x2
  "u_HV_PGEN_dummy" : "u_gbox_pgen_cfg"
}

# Add gbox top mapping
for i in range(20) :
  RTL_MAPPING["u_HP_GBOX_BK0_A_%d" % i] = "u_gbox_hp_40_NS_0.u_gbox_io_cfg_A[%d]" % i
  RTL_MAPPING["u_HP_GBOX_BK0_B_%d" % i] = "u_gbox_hp_40_NS_0.u_gbox_io_cfg_B[%d]" % i
  RTL_MAPPING["u_HP_GBOX_BK1_A_%d" % i] = "u_gbox_hp_40_NS_1.u_gbox_io_cfg_A[%d]" % i
  RTL_MAPPING["u_HP_GBOX_BK1_B_%d" % i] = "u_gbox_hp_40_NS_1.u_gbox_io_cfg_B[%d]" % i
  RTL_MAPPING["u_HV_GBOX_BK0_A_%d" % i] = "u_gbox_hv_40_EW_0.u_gbox_io_cfg_A[%d]" % i
  RTL_MAPPING["u_HV_GBOX_BK0_B_%d" % i] = "u_gbox_hv_40_EW_0.u_gbox_io_cfg_B[%d]" % i
  RTL_MAPPING["u_HV_GBOX_BK1_A_%d" % i] = "u_gbox_hv_40_EW_1.u_gbox_io_cfg_A[%d]" % i
  RTL_MAPPING["u_HV_GBOX_BK1_B_%d" % i] = "u_gbox_hv_40_EW_1.u_gbox_io_cfg_B[%d]" % i

class ATTRIBUTE :
  def __init__(self, line) :
    # Example syntax RATE - Addr: 0x00000000, Size:  4, Value: (0x00000000) 0
    # Example syntax RATE - Addr: 0x00000000, Size:  4, Value: (0x00000000) 0 {xxx}
    m = re.search(r"(\w+) - Addr: 0x([0-9A-Z]+), Size: (\d+), Value: \(0x([0-9A-Z]+)\) (\d+)(|\W?)", line)
    assert m != None
    assert len(m.groups()) == 6
    self.name = m.group(1)
    self.addr = int(m.group(2), 16)
    self.size = int(m.group(3))
    assert self.size > 0
    self.value = int(m.group(4), 16)
    assert self.value == int(m.group(5))
    assert self.value < (1 << (self.size))
    self.reason = m.group(6)

class BLOCK :
  def __init__(self, line) :
    self.parse(line)
    self.attributes = []
    self.addr = 0
    self.size = 0
    self.value = 0
    self.bits = []
    self.bitstring = "" # from left to right
    self.rtl_name = ""
    self.indexing = ""

  def parse(self, line) :
    assert line.find("Block ") == 0
    line = line[6:]
    assert len(line)
    index = line.find(" [")
    assert index != -1 and line[-1] == ']'
    self.fullname = line[:index]
    self.fullname = self.fullname.strip()
    assert len(self.fullname)
    self.customer_name = line[index+1:]
    self.customer_name = self.customer_name.strip()
    assert len(self.customer_name) >= 2
    assert self.customer_name[0] == '[', self.customer_name
    assert self.customer_name[-1] == ']'
    self.customer_name = self.customer_name[1:-1]
    self.modules = self.fullname.split(".")
    self.mapped_modules = list(self.modules)

  def gen_rtl_name(self) :
    for module in self.mapped_modules :
      if module in RTL_MAPPING :
        module = RTL_MAPPING[module]
      if len(self.rtl_name) :
        self.rtl_name = "%s.%s" % (self.rtl_name, module)
      else :
        self.rtl_name = module

  def add_attribute(self, line) :
    # Remove double space
    index = line.find("  ")
    if index != -1 :
      line = line.replace("  ", " ")
    attribute = ATTRIBUTE(line)
    if len(self.attributes) :
      assert (self.addr + self.size) == attribute.addr
    else :
      self.addr = attribute.addr
    for i in range(attribute.size) :
      if attribute.value & (1 << i) :
        self.bits.append(1)
        self.bitstring += "1"
      else :
        self.bits.append(0)
        self.bitstring += "0"
    self.value |= (attribute.value << self.size)
    self.size += attribute.size
    self.attributes.append(attribute)

class PARSER :
  
  def __init__(self, filepath) :
    self.filepath = filepath
    self.blocks = []
    self.parse()
    
  def parse(self) :
    file = open(self.filepath)
    size = 0
    for line in file :
      line = line.strip()
      if len(line) == 0 :
        # Empty line
        pass
      elif line.find("//") == 0 :
        # This is comment
        pass
      elif line.find("Block ") == 0 :
        if len(self.blocks) :
          assert size == self.blocks[-1].addr
          size += self.blocks[-1].size
        self.blocks.append(BLOCK(line))
      elif line.find("Attributes:") == 0 :
        # This information is not important
        pass
      else :
        assert len(self.blocks)
        self.blocks[-1].add_attribute(line)
    file.close()
    
  def combine_block(self) :
    for rtl_name in COMBINATION_MAPPING :
      tcl_names = COMBINATION_MAPPING[rtl_name]
      assert len(tcl_names) > 1
      addr = 0
      size = 0
      for block in self.blocks :
        assert len(block.mapped_modules)
        # Not so good code
        index = -1 
        for i, module in enumerate(block.mapped_modules) :
          if module in tcl_names :
            assert index == -1
            index = i
        if index == -1 :
          size = 0
        else :
          if size == 0 :
            addr = block.addr
          else :
            # If you wanna combine blocks, they must be continuous
            assert (addr + size) == block.addr
          block.indexing = "[%d:%d]" % (size + block.size - 1, size)
          size += block.size
          assert size
          block.mapped_modules[index] = rtl_name
    for block in self.blocks :
      block.gen_rtl_name()
  def write(self, filepath) :
    file = open(filepath, "w")
    for block in self.blocks :
      file.write("// %s [Customer Name: %s]\n" % (block.fullname, block.customer_name))
      file.write("force u_dut.%s.control%s = %d'b%s;\n\n" % (block.rtl_name, block.indexing, block.size, block.bitstring[::-1]))
    file.close()
    
def main(infile, outfile, print_debug=False) :
  if print_debug :
    print("Input file: %s" % infile)
    print("Output file: %s" % outfile)
  parser = PARSER(infile)
  parser.combine_block()
  parser.write(outfile)
  return [True]
  
if __name__ == "__main__" :
  assert len(sys.argv) >= 3, "Missing input file and/or output file"
  main(sys.argv[1], sys.argv[2], True)