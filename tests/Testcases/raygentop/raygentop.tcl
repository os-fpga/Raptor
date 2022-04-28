create_design RAYGENTOP
set_top_module paj_raygentop_hierarchy_no_mem
add_design_file raygentop.v
add_constraint_file raygentop.sdc
synth
packing
place
route
# VPR errors:
#sta
#power
#bitstream
puts "done!"
