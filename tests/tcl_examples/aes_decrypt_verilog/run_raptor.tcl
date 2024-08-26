#####################################
#  Rapid Silicon Design Example     #
#  aes_decrypt_verilog - RTL design #
#  Raptor compilation TCL file      #
#  raptor --script run_raptor.tcl   #
#####################################

# Project name
set project_name AES_DECRYPT_VERILOG

puts "Creating $project_name..."
create_design $project_name
target_device 1VG28
add_design_file Src/aes_decrypt128.sv Src/aes_decrypt256.sv  Src/gfmul.sv Src/InvMixCol_slice.sv Src/InvSbox.sv Src/InvSubBytes.sv Src/KeyExpand192.sv  Src/KschBuffer.sv  Src/Sbox.sv Src/aes_decrypt192.sv  Src/decrypt.sv Src/InvAddRoundKey.sv Src/InvMixColumns.sv  Src/InvShiftRows.sv Src/KeyExpand128.sv Src/KeyExpand256.sv Src/RotWord.sv  Src/SubWord.sv  -SV_2012
add_design_file Src/MUXF7.v Src/MUXF8.v Src/decrypt_top.v
add_constraint_file constraints.sdc
# Top-module can automatically be indentified or implicitly specified here
set_top_module decrypt_top

#Compile
puts "Compiling $project_name..."

# Synthesize with high effort 
synth_options -effort high

analyze
# Synthesize optimizing for delay
synthesize delay

setup_lec_sim

# Simulate RTL vs gate
simulation_options compilation verilator gate
simulate gate verilator

packing
place
route

# Simulate RTL vs post-pnr
simulation_options compilation verilator pnr
simulate pnr verilator

sta 
power
bitstream

puts "Completed $project_name...\n"
