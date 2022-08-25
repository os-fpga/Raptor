# Gate level netlist input
create_design AES_DECRYPT_GATE
target_device GEMINI
set_device_size 78x66
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
