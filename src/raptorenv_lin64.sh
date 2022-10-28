#!/bin/bash
# Raptor environment setup script - 64-bit Linux
# Copyright(c) 2022 Rapid Silicon
# All licenses available in $RAPTOR_PATH/share/raptor/licenses
# Version 1.0

RETURN_PATH=`pwd`
SCRIPT_PATH=`dirname $BASH_SOURCE`
RAPTOR_PATH=`( cd "$SCRIPT_PATH" && pwd )`

source $RAPTOR_PATH/.raptorenv_lin64.sh
