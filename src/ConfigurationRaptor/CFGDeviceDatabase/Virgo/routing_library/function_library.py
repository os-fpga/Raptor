import os
import sys
current_dir = os.path.dirname(os.path.abspath(__file__))
if current_dir not in sys.path:
  sys.path.insert(0, current_dir)

def get_gbox_top_name(index) :
  
  pn = "P" if (index % 2) == 0 else "N"
  return "gearbox_%s[%d]" % (pn, index//2)
  
def get_location(type, bank, index) :

  pn = "P" if (index % 2) == 0 else "N"
  if index in [18, 19, 38, 39] :
    return "H%s_%d_CC_%d_%d%s" % (type, bank, index, index//2, pn)
  else :
    return "H%s_%d_%d_%d%s" % (type, bank, index, index//2, pn)
