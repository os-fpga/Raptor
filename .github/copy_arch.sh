
#cp openfpga-pd-castor-rs/k6n8_TSMC16nm_7.5T/FPGA82x68_gemini_compact_pnr/fabric_task/flow_inputs/k6n8_vpr_annotated.xml etc/devices/gemini_compact_82x68/gemini_vpr.xml
#cp openfpga-pd-castor-rs/k6n8_TSMC16nm_7.5T/FPGA82x68_gemini_compact_pnr/fabric_task/flow_inputs/bitstream_setting.xml  etc/devices/gemini_compact_82x68/bitstream_setting.xml
#cp openfpga-pd-castor-rs/k6n8_TSMC16nm_7.5T/FPGA82x68_gemini_compact_pnr/fabric_task/flow_inputs/fpga_repack_constraints.xml  etc/devices/gemini_compact_82x68/fpga_repack_constraints.xml
#cp openfpga-pd-castor-rs/k6n8_TSMC16nm_7.5T/FPGA82x68_gemini_compact_pnr/fabric_task/flow_inputs/openfpga_arch_annotated.xml etc/devices/gemini_compact_82x68/gemini_openfpga.xml

cp -v gemini_plus_10x8/OpenFPGA/k6n8_vpr_annotated.xml etc/devices/gemini_compact_10x8/gemini_vpr.xml
cp -v gemini_plus_10x8/OpenFPGA/bitstream_setting.xml  etc/devices/gemini_compact_10x8/bitstream_setting.xml
cp -v gemini_plus_10x8/OpenFPGA/fpga_repack_constraints.xml  etc/devices/gemini_compact_10x8/fpga_repack_constraints.xml
cp -v gemini_plus_10x8/OpenFPGA/openfpga_arch_annotated.xml etc/devices/gemini_compact_10x8/gemini_openfpga.xml

cp -v spica_62x44/OpenFPGA/k6n8_vpr_annotated.xml etc/devices/gemini_compact_62x44/gemini_vpr.xml
cp -v spica_62x44/OpenFPGA/bitstream_setting.xml  etc/devices/gemini_compact_62x44/bitstream_setting.xml
cp -v spica_62x44/OpenFPGA/fpga_repack_constraints.xml  etc/devices/gemini_compact_62x44/fpga_repack_constraints.xml
cp -v spica_62x44/OpenFPGA/openfpga_arch_annotated.xml etc/devices/gemini_compact_62x44/gemini_openfpga.xml

cp -v gemini_plus_104x68/OpenFPGA/k6n8_vpr_annotated.xml etc/devices/gemini_compact_104x68/gemini_vpr.xml
cp -v gemini_plus_104x68/OpenFPGA/bitstream_setting.xml  etc/devices/gemini_compact_104x68/bitstream_setting.xml
cp -v gemini_plus_104x68/OpenFPGA/fpga_repack_constraints.xml  etc/devices/gemini_compact_104x68/fpga_repack_constraints.xml
cp -v gemini_plus_104x68/OpenFPGA/openfpga_arch_annotated.xml etc/devices/gemini_compact_104x68/gemini_openfpga.xml

#cp openfpga-pd-castor-rs/k6n8_TSMC16nm_7.5T/FPGA104x68_gemini_compact_pnr/fabric_task/flow_inputs/k6n8_vpr_annotated.xml etc/devices/gemini_compact_104x86/gemini_vpr.xml
#cp openfpga-pd-castor-rs/k6n8_TSMC16nm_7.5T/FPGA104x68_gemini_compact_pnr/fabric_task/flow_inputs/bitstream_setting.xml  etc/devices/gemini_compact_104x86/bitstream_setting.xml
#cp openfpga-pd-castor-rs/k6n8_TSMC16nm_7.5T/FPGA104x68_gemini_compact_pnr/fabric_task/flow_inputs/fpga_repack_constraints.xml  etc/devices/gemini_compact_104x86/fpga_repack_constraints.xml
#cp openfpga-pd-castor-rs/k6n8_TSMC16nm_7.5T/FPGA104x68_gemini_compact_pnr/fabric_task/flow_inputs/openfpga_arch_annotated.xml etc/devices/gemini_compact_104x86/gemini_openfpga.xml

echo "Done Copying XML"

cp -v gemini_plus_10x8/RIC/* etc/devices/gemini_compact_10x8/ric
cp -v gemini_plus_104x68/RIC/* etc/devices/gemini_compact_104x68/ric
echo "Done Copying RIC model files"

cp -v gemini_plus_10x8/PinTable/GeminiPlus_Pin_Table_generated_10_8.csv etc/devices/gemini_compact_10x8/Gemini_Pin_Table.csv
cp -v gemini_plus_104x68/PinTablePinTable_104x68/GeminiPlus_Pin_Table_generated.csv etc/devices/gemini_compact_104x68/Gemini_Pin_Table.csv
cp -v spica_62x44/PinTablePinTable_104x68/Virgo_Pin_Table_generated.csv etc/devices/gemini_compact_62x44/Gemini_Pin_Table.csv
echo "Done copying pin table"

cp -v raptor/etc/device.xml etc/device.xml
echo "Done copying device.xml"

