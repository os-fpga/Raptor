import sys
import os
import json
import xml.etree.ElementTree as XML

class DEVICE :
  def __init__(self, name, family, series) :
    self.name = name
    self.family = family
    self.series = series
    self.protocol = None
    self.blwl = None

def get_device_config_protocol(file) :
  tree = XML.parse(file)
  root = tree.getroot()
  assert root.tag == "openfpga_architecture"
  cindex = 0
  protocol = None
  blwl = None
  for config in root.iter("configuration_protocol") :
    assert cindex == 0
    oindex = 0
    for organize in config :
      assert oindex == 0
      assert organize.tag == "organization"
      assert "type" in organize.attrib
      protocol = organize.attrib["type"]
      if protocol == "ql_memory_bank" :
        bl = None
        wl = None
        for child in organize :
          assert "protocol" in child.attrib
          if child.tag == "bl" :
            assert bl == None
            bl = child.attrib["protocol"]
          else :
            assert child.tag == "wl"
            assert wl == None
            wl = child.attrib["protocol"]
        assert bl in ["flatten", "shift_register"], "File %s BL %s" % (file, bl)
        assert wl in ["flatten", "shift_register"], "File %s WL %s" % (file, wl)
        assert bl == wl
        blwl = bl
      else :
        assert protocol == "scan_chain"
      oindex += 1
    assert oindex == 1
    cindex += 1
  assert cindex == 1
  return [protocol, blwl]

def main() :
  assert len(sys.argv) >= 3, "At least two inputs: devices directory and output JSON need to be specified"
  print("Device Directory: %s" % sys.argv[1])
  print("Output JSON: %s" % sys.argv[2])
  xml_input = "%s/device.xml" % sys.argv[1]
  print("Read %s ..." % xml_input)
  tree = XML.parse(xml_input)
  root = tree.getroot()
  assert root.tag == "device_list"
  devices = {}
  for device in root :
    assert device.tag == "device"
    assert "name" in device.attrib
    assert "family" in device.attrib
    assert "series" in device.attrib
    dev = DEVICE(device.attrib["name"], device.attrib["family"], device.attrib["series"])
    assert dev.name not in devices
    '''
    assert dev.family in ["Gemini", "TestChip"], "Family %s is not supported" % dev.family
    if dev.family == "TestChip" :
      assert dev.series in ["Gemini"], "Series %s is not supported by family %s" % (dev.series, dev.family)
    elif dev.family == "Gemini" :
      assert dev.series in ["Gemini", "Virgo"], "Series %s is not supported by family %s" % (dev.series, dev.family)
    else :
      assert False, "Family %s is not supported" % dev.family
    '''
    for child in device :
      if child.tag == "internal" :
        assert "type" in child.attrib
        if child.attrib["type"] == "openfpga_arch" :
          assert "file" in child.attrib
          file = "%s/%s" % (sys.argv[1], child.attrib["file"])
          assert os.path.exists(file), "File %s does not exist" % file
          (dev.protocol, dev.blwl) = get_device_config_protocol(file)
          break
    assert dev.protocol != None
    devices[dev.name] = dev
  database = {}
  for dev in devices :
    database[dev] = {"family" : devices[dev].family, \
                      "series"  : devices[dev].series, \
                      "protocol" : devices[dev].protocol, \
                      "blwl" : devices[dev].blwl}
  file = open(sys.argv[2], "w")
  file.write(json.dumps(database, indent=2))
  file.close()
if __name__ == "__main__":
  main()
