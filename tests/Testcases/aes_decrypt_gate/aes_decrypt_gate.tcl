# Gate level netlist input
create_design AES_DECRYPT_GATE -type gate-level

target_device 1GE75

read_netlist aes_decrypt.blif
add_constraint_file aes_decrypt_gate.sdc
packing
place
route
sta
bitstream

