#  aes_decrypt_verilog - RTL design.
#  Behnam's example related to EDA-2202, EDA-2227
#################################################

# Create or open vpr_constraints.xml file for writing
set filename "vpr_constraints.xml"
set fileId [open $filename "w"]

# XML content to write
set xml_content {<vpr_constraints tool_name="vpr">
</vpr_constraints>}

# Write the XML content to the file
puts $fileId $xml_content

# Close the file
close $fileId

# Project name
set project_name AES_DECRYPT_partitioner

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
# message_severity VERI-1209 IGNORE
# Synthesize optimizing for delay
# pnr_options --use_partitioning_in_pack on --number_of_molecules_in_partition 16

pnr_options --use_partitioning_in_pack on --number_of_molecules_in_partition 16 --read_vpr_constraints vpr_constraints.xml

synthesize delay
packing
place
route
sta
# bitstream

puts "Completed $project_name...\n"

