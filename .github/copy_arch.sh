
#cp openfpga-pd-castor-rs/k6n8_TSMC16nm_7.5T/FPGA82x68_gemini_compact_pnr/fabric_task/flow_inputs/k6n8_vpr_annotated.xml etc/devices/gemini_compact_82x68/gemini_vpr.xml
#cp openfpga-pd-castor-rs/k6n8_TSMC16nm_7.5T/FPGA82x68_gemini_compact_pnr/fabric_task/flow_inputs/bitstream_setting.xml  etc/devices/gemini_compact_82x68/bitstream_setting.xml
#cp openfpga-pd-castor-rs/k6n8_TSMC16nm_7.5T/FPGA82x68_gemini_compact_pnr/fabric_task/flow_inputs/fpga_repack_constraints.xml  etc/devices/gemini_compact_82x68/fpga_repack_constraints.xml
#cp openfpga-pd-castor-rs/k6n8_TSMC16nm_7.5T/FPGA82x68_gemini_compact_pnr/fabric_task/flow_inputs/openfpga_arch_annotated.xml etc/devices/gemini_compact_82x68/gemini_openfpga.xml

cp gemini_plus_10x8/OpenFPGA/k6n8_vpr_annotated.xml etc/devices/gemini_compact_10x8/gemini_vpr.xml
cp gemini_plus_10x8/OpenFPGA/bitstream_setting.xml  etc/devices/gemini_compact_10x8/bitstream_setting.xml
cp gemini_plus_10x8/OpenFPGA/fpga_repack_constraints.xml  etc/devices/gemini_compact_10x8/fpga_repack_constraints.xml
cp gemini_plus_10x8/OpenFPGA/openfpga_arch_annotated.xml etc/devices/gemini_compact_10x8/gemini_openfpga.xml

#cp openfpga-pd-castor-rs/k6n8_TSMC16nm_7.5T/FPGA62x44_gemini_compact_pnr/fabric_task/flow_inputs/k6n8_vpr_annotated.xml etc/devices/gemini_compact_62x44/gemini_vpr.xml
#cp openfpga-pd-castor-rs/k6n8_TSMC16nm_7.5T/FPGA62x44_gemini_compact_pnr/fabric_task/flow_inputs/bitstream_setting.xml  etc/devices/gemini_compact_62x44/bitstream_setting.xml
#cp openfpga-pd-castor-rs/k6n8_TSMC16nm_7.5T/FPGA62x44_gemini_compact_pnr/fabric_task/flow_inputs/fpga_repack_constraints.xml  etc/devices/gemini_compact_62x44/fpga_repack_constraints.xml
#cp openfpga-pd-castor-rs/k6n8_TSMC16nm_7.5T/FPGA62x44_gemini_compact_pnr/fabric_task/flow_inputs/openfpga_arch_annotated.xml etc/devices/gemini_compact_62x44/gemini_openfpga.xml

cp gemini_plus_104x68/OpenFPGA/k6n8_vpr_annotated.xml etc/devices/gemini_compact_104x68/gemini_vpr.xml
cp gemini_plus_104x68/OpenFPGA/bitstream_setting.xml  etc/devices/gemini_compact_104x68/bitstream_setting.xml
cp gemini_plus_104x68/OpenFPGA/fpga_repack_constraints.xml  etc/devices/gemini_compact_104x68/fpga_repack_constraints.xml
cp gemini_plus_104x68/OpenFPGA/openfpga_arch_annotated.xml etc/devices/gemini_compact_104x68/gemini_openfpga.xml

#cp openfpga-pd-castor-rs/k6n8_TSMC16nm_7.5T/FPGA104x68_gemini_compact_pnr/fabric_task/flow_inputs/k6n8_vpr_annotated.xml etc/devices/gemini_compact_104x86/gemini_vpr.xml
#cp openfpga-pd-castor-rs/k6n8_TSMC16nm_7.5T/FPGA104x68_gemini_compact_pnr/fabric_task/flow_inputs/bitstream_setting.xml  etc/devices/gemini_compact_104x86/bitstream_setting.xml
#cp openfpga-pd-castor-rs/k6n8_TSMC16nm_7.5T/FPGA104x68_gemini_compact_pnr/fabric_task/flow_inputs/fpga_repack_constraints.xml  etc/devices/gemini_compact_104x86/fpga_repack_constraints.xml
#cp openfpga-pd-castor-rs/k6n8_TSMC16nm_7.5T/FPGA104x68_gemini_compact_pnr/fabric_task/flow_inputs/openfpga_arch_annotated.xml etc/devices/gemini_compact_104x86/gemini_openfpga.xml

echo "Done Copying XML"

cp gemini_plus_10x8/RIC/* etc/devices/gemini_compact_10x8/ric
cp gemini_plus_104x68/RIC/* etc/devices/gemini_compact_104x68/ric

echo "Done Copying RIC model files"
