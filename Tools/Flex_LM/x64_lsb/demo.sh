#!/bin/bash

# Example script to set file descriptor ulimit for the license server daemon.
#Script ensure the Setlimit doesn't exceed  the hard limit.
#Script ensure the Setlimit doesn't reduce the current limit if its over 10240.

#ignores case for string comparison.
shopt -s nocasematch

#Update the PATH for lmgrd binary, license file, and debug log file path.
Toolkit_PATH="/opt/FNPlm/lmgrd_lic_log_Path"

Hardlimit=`ulimit -Hn`
Softlimit=`ulimit -Sn`
Setlimit=10240

if [ "$Hardlimit" != "unlimited" ] && [ "$Hardlimit" -lt "$Setlimit" ]; then
	Setlimit=$Hardlimit
fi

if [ "$Softlimit" != "unlimited" ] && [ "$Softlimit" -gt "$Setlimit" ]; then
	Setlimit=$Softlimit
fi

if [ "$Softlimit" != "unlimited" ]; then
	`ulimit -n $Setlimit`
fi

#checking for the Platform type.
unamestr=`uname`

if [[ "$unamestr" == 'Linux' ]]; then
	export LD_LIBRARY_PATH=$Toolkit_PATH
	`$Toolkit_PATH/lmgrd -c $Toolkit_PATH/counted.lic -l $Toolkit_PATH/debug.log`
elif [[ "$unamestr" == 'Darwin' ]]; then
	`$Toolkit_PATH/lmgrd -c $Toolkit_PATH/counted.lic -d -l $Toolkit_PATH/debug.log`
fi
