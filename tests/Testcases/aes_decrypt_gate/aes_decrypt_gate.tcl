# Gate level netlist input
target_device GEMINI
set_device_size 78x66
create_design AES_DECRYPT_GATE
read_netlist aes_decrypt.blif
add_constraint_file aes_decrypt_gate.sdc
packing
place
pnr_options --router_lookahead classic
route
##pnr_options --disp on
sta
pnr_options
power
bitstream
puts "done!"
