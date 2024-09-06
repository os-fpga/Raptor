create_design test
architecture ../../Arch/k6_frac_N10_tileable_40nm.xml ../../Arch/k6_N10_40nm_openfpga.xml
set_macro P1=10  P2=20
add_include_path  inc/
add_design_file test.v bottom.v -SV_2012
set_top_module top
synth
packing
place
route
sta
power
#bitstream
message "done!"


