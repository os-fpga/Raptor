#!/bin/bash
# Raptor environment setup script - 64-bit Linux
# Copyright(c) 2022 Rapid Silicon
# All licenses available in $RAPTOR_PATH/share/raptor/licenses
# Version 1.0

RETURN_PATH=`pwd`
SCRIPT_PATH=`dirname $BASH_SOURCE`
RAPTOR_PATH=`( cd "$SCRIPT_PATH" && pwd )`

if [ -n "${PATH}" ]; then
	export PATH=$RAPTOR_PATH/bin:$PATH
else
	export PATH=$RAPTOR_PATH/bin
fi
