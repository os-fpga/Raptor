#!/bin/bash
# Raptor environment setup script - 64-bit Linux
# Copyright(c) 2022 Rapid Silicon
# Version 1.0 - Tony McDowell (tony.mcdowell@rapidsilicon.com)

RETURN_PATH=`pwd`
SCRIPT_PATH=`dirname $BASH_SOURCE`
RAPTOR_PATH=`( cd "$SCRIPT_PATH" && pwd )`

export $PATH=$RAPTOR_PATH/bin:$PATH

export $LD_LIBRARY_PATH=$RAPTOR_PATH/external_libs:$LD_LIBRARY_PATH=$RAPTOR_PATH/lib64:$LD_LIBRARY_PATH=$RAPTOR_PATH/lib:$LD_LIBRARY_PATH
