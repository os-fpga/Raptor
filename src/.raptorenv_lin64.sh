#!/bin/bash
# Raptor environment setup script - 64-bit Linux
# Copyright(c) 2022 Rapid Silicon
# Version 1.0 - Tony McDowell (tony.mcdowell@rapidsilicon.com)

RETURN_PATH=`pwd`
SCRIPT_PATH=`dirname $BASH_SOURCE`
RAPTOR_PATH=`( cd "$SCRIPT_PATH" && pwd )`

if [ -n "${PATH}" ]; then
	export PATH=$RAPTOR_PATH/bin:$PATH
else
	export PATH=$RAPTOR_PATH/bin
fi

if [ -n "${LD_LIBRARY_PATH}" ]; then
	export LD_LIBRARY_PATH=$RAPTOR_PATH/lib64/raptor/lib:$RAPTOR_PATH/lib:$RAPTOR_PATH/lib/raptor/lib:$RAPTOR_PATH/external_libs/qt_5_15_lib/lib:$RAPTOR_PATH/external_libs/gcc_9_lib:$LD_LIBRARY_PATH
else
	export LD_LIBRARY_PATH=$RAPTOR_PATH/lib64/raptor/lib:$RAPTOR_PATH/lib:$RAPTOR_PATH/lib/raptor/lib:$RAPTOR_PATH/external_libs/qt_5_15_lib/lib:$RAPTOR_PATH/external_libs/gcc_9_lib
fi

if [ -n "${TCL_LIBRARY}" ]; then
	export TCL_LIBRARY=$RAPTOR_PATH/share/raptor/tcl8.6.12/library/:$TCL_LIBRARY
else
	export TCL_LIBRARY=$RAPTOR_PATH/share/raptor/tcl8.6.12/library/
fi
