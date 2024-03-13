#!/bin/bash
# Raptor environment setup script - 64-bit Linux
# Copyright(c) 2022 Rapid Silicon
# All licenses available in $RAPTOR_PATH/share/raptor/licenses
# Version 1.0

RETURN_PATH=`pwd`
SCRIPT_PATH=`dirname $BASH_SOURCE`
RAPTOR_PATH=`( cd "$SCRIPT_PATH" && pwd )`

if [ -n "${LD_LIBRARY_PATH}" ]; then
	export LD_LIBRARY_PATH=$RAPTOR_PATH/lib64/raptor/lib:$RAPTOR_PATH/lib64/tbb:$RAPTOR_PATH/lib/tbb:$RAPTOR_PATH/lib64/openssl:$RAPTOR_PATH/lib/openssl:$RAPTOR_PATH/lib64:$RAPTOR_PATH/lib:$RAPTOR_PATH/lib/raptor/lib:$RAPTOR_PATH/bin/gtkwave/lib:$RAPTOR_PATH/external_libs/qt/lib:$RAPTOR_PATH/external_libs/lib:$LD_LIBRARY_PATH
else
	export LD_LIBRARY_PATH=$RAPTOR_PATH/lib64/raptor/lib:$RAPTOR_PATH/lib64/tbb:$RAPTOR_PATH/lib/tbb:$RAPTOR_PATH/lib64/openssl:$RAPTOR_PATH/lib/openssl:$RAPTOR_PATH/lib64:$RAPTOR_PATH/lib:$RAPTOR_PATH/lib/raptor/lib:$RAPTOR_PATH/bin/gtkwave/lib:$RAPTOR_PATH/external_libs/qt/lib:$RAPTOR_PATH/external_libs/lib
fi

if [ -n "${TCL_LIBRARY}" ]; then
	export TCL_LIBRARY=$RAPTOR_PATH/share/raptor/tcl8.6.12/library/:$TCL_LIBRARY
else
	export TCL_LIBRARY=$RAPTOR_PATH/share/raptor/tcl8.6.12/library/
fi

if [ -n "${PYTHONPATH}" ]; then
	export PYTHONPATH=$RAPTOR_PATH/share/litex_reference_designs/:$PYTHONPATH/$RAPTOR_PATH/share/raptor/IP_Catalog/:$PYTHONPATH
else
	export PYTHONPATH=$RAPTOR_PATH/share/litex_reference_designs/:$RAPTOR_PATH/share/raptor/IP_Catalog/
fi
# Contains bogus VERILATOR_ROOT
#[[ -f "$RAPTOR_PATH/bin/HDL_simulator/setup_sim" ]] && source $RAPTOR_PATH/bin/HDL_simulator/setup_sim

# uncomment to open the debug
#export QT_DEBUG_PLUGINS=1
#export QT_QPA_PLATFORM=xcb
#export QT_QPA_PLATFORM_PLUGIN_PATH=$RAPTOR_PATH/external_libs/qt/plugins
#export QT_PLUGIN_PATH=$RAPTOR_PATH/external_libs/qt/plugins

export PATH=$RAPTOR_PATH/share/envs/litex/bin:$PATH
export LIBPYTHON_LOC=$RAPTOR_PATH/share/envs/python3.8/lib/libpython3.8.so.1.0

TOP_DIR="$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )/bin/HDL_simulator"
ostype=`egrep '^(NAME)=' /etc/os-release  | grep -o -e Ubuntu -e CentOS`
if [ "$ostype" == "CentOS" ]
then
export PATH=$TOP_DIR/gcc-11.4.0/bin:$PATH
export LD_LIBRARY_PATH=$TOP_DIR/gcc-11.4.0/lib64:$LD_LIBRARY_PATH
fi
echo "$TOP_DIR/GHDL/lib"
export LD_LIBRARY_PATH=$TOP_DIR/GHDL/lib:$LD_LIBRARY_PATH
export SYSTEMC_INCLUDE=$TOP_DIR/systemc_2.3.3/include
export SYSTEMC_LIBDIR=$TOP_DIR/systemc_2.3.3/lib-linux64
export LD_LIBRARY_PATH=$TOP_DIR/systemc_2.3.3/lib-linux64:$LD_LIBRARY_PATH
export PATH=$TOP_DIR/verilator/bin:$PATH
export LIBRARY_PATH=/usr/lib/x86_64-linux-gnu:$LIBRARY_PATH
export PATH=$TOP_DIR/GHDL/bin:$PATH
export PATH=$TOP_DIR/iverilog/bin:$PATH
