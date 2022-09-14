create_design AES_DECRYPT
target_device GEMINI
set_device_size 78x66
set_top_module wrapper
add_design_file aes_decrypt128.sv aes_decrypt256.sv  gfmul.sv InvMixCol_slice.sv InvSbox.sv InvSubBytes.sv KeyExpand192.sv  KschBuffer.sv  Sbox.sv aes_decrypt192.sv  decrypt.sv InvAddRoundKey.sv InvMixColumns.sv  InvShiftRows.sv KeyExpand128.sv KeyExpand256.sv RotWord.sv  SubWord.sv  -SV_2012
add_design_file generic_muxfx.v wrapper.v

add_constraint_file aes_decrypt.sdc
synth_options -effort high
analyze
synthesize area
pnr_netlist_lang verilog
packing
place
route
sta
power
bitstream
puts "done!"
