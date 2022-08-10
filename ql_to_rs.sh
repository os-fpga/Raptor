#!/bin/bash
sed -i "s/ql_/rs_/g" ./etc/devices/gemini/gemini_openfpga.xml
sed -i "s/ql_/rs_/g" ./etc/devices/gemini/gemini_vpr.xml
sed -i "s/ql_/rs_/g" ./etc/devices/gemini/bitstream_annotation.xml
sed -i "s/ql_/rs_/g" ./tests/Arch/bitstream_annotation.xml
sed -i "s/QL_/RS_/g" ./etc/devices/gemini/gemini_openfpga.xml
sed -i "s/QL_/RS_/g" ./etc/devices/gemini/gemini_vpr.xml
sed -i "s/QL_/RS_/g" ./etc/devices/gemini/bitstream_annotation.xml
sed -i "s/QL_/RS_/g" ./tests/Arch/bitstream_annotation.xml
