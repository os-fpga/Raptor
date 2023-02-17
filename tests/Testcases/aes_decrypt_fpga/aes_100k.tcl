create_design AES_100K
target_device 1GE100
set_top_module wrapper
add_design_file aes_decrypt128.sv aes_decrypt256.sv  gfmul.sv InvMixCol_slice.sv InvSbox.sv InvSubBytes.sv KeyExpand192.sv  KschBuffer.sv  Sbox.sv aes_decrypt192.sv  decrypt.sv InvAddRoundKey.sv InvMixColumns.sv  InvShiftRows.sv KeyExpand128.sv KeyExpand256.sv RotWord.sv  SubWord.sv  -SV_2012
add_design_file generic_muxfx.v wrapper.v

add_constraint_file aes_decrypt.sdc
synth_options -effort high
analyze
message_severity VERI-1209 IGNORE
synthesize delay
packing
place
route
sta 
#bitstream
