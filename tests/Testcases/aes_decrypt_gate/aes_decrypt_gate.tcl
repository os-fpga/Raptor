# Gate level netlist input
create_design AES_DECRYPT_GATE -type gate-level
target_device 1GE75
read_netlist aes_decrypt.blif
pnr_netlist_lang blif
add_constraint_file aes_decrypt_gate.sdc
packing
place
pnr_options --router_lookahead classic
route
sta
pnr_options
power
bitstream

