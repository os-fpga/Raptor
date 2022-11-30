# Gate level netlist input
create_design AES_DECRYPT_GATE -type gate-level
target_device GEMINI
read_netlist aes_decrypt.v
pnr_netlist_lang verilog
add_constraint_file aes_decrypt_gate.sdc
packing
place
pnr_options --router_lookahead classic
route
sta
pnr_options
power
bitstream

