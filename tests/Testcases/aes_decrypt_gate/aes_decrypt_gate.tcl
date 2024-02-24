# Gate level netlist input example

# Specifc project type "gate-level"
create_design AES_DECRYPT_GATE -type gate-level

target_device 1VG28

# Specific command to load a netlist file
read_netlist aes_decrypt.v

add_constraint_file aes_decrypt_gate.sdc

# Analyze the Verilog netlist (Hierarchy)
analyze 

# MUST: Synthesis only converts Verilog to Eblif for VPR (No optimization)
synthesize

packing
place
route
sta
power
bitstream

