#! /bin/sh

# Client installer script
# =======================

trap 'echo ; exit 13' TERM INT

# Function to set directory ownership and permissions
setPerm()
{
	dir=$1
	recurse=$2
	owner=$3
	if [ -z "$TSNAME" ]
	then
		perm=777
		if [ -z "$owner" ]
		then
			owner="root"
		fi
	else
		group=`echo "$TSNAME" | cut -s -f2 -d:`
		if [ -z "$group" ]
		then
			perm=700
		else
			perm=770
		fi
		owner="$TSNAME"
	fi
		
	chmod $recurse $perm "$dir"
	rc=$?
	if [ $rc -ne 0 ]
	then
   		echo "ERROR: Unable to set permissions on $dir, chmod returned error $rc."
		exit $rc
	fi
	if [ ! -z "$recurse" ]
	then
		find "$dir" -type f -exec chmod a-x {} \;
	fi

	chown $recurse $owner "$dir"
	rc=$?
	if [ $rc -ne 0 ]
	then
   		echo "ERROR: Unable to set owner on $dir, chown returned error $rc."
		exit $rc
	fi
}

# Make sure run by sudo
USERID=`id | cut -d \( -f 1 | cut -d \= -f 2`
CHFLAGS=`which chflags 2> /dev/null`
CHATTR=`which chattr 2> /dev/null`
if [ "$SUDO_USER" ]
then
        OWNER=$SUDO_USER
else
        OWNER=$USER
fi

if [ "$USERID" -ne 0 ]
then
echo "ERROR: Please run this script using 'sudo' or as root (su)."
    exit 1
fi

# Process Command-Line Args
# ============================
while [ $# -ne 0 ]
do
	if [ "$1" = "--help" ]
	then
		echo "install_fnp.sh [--help] [--user <name>] [--cert] [--nolsb] [--nodaemon] [ <path to FNLS> ]"
		exit
	elif [ "$1" = "--cert" ]
	then
		CERT_ONLY="true"
	elif [ "$1" = "--nolsb" ]
	then
		FAKE_LSB="true"
	elif [ "$1" = "--nodaemon" ]
	then
		NO_DAEMON="true"
	elif [ "$1" = "--user" ]
	then
		if [ $# -eq 1 ]
		then
			echo "ERROR: Missing username for --user"
			exit 1
		fi
		shift
		TSNAME=$1
		TSUSER=`echo "$TSNAME": | cut -s -f1 -d:`
		echo "$TSUSER" | grep '[A-Za-z_]' >/dev/null 2>&1
		res=$?
		if [ $res -eq 0 ]
		then
			id "$TSUSER" >/dev/null 2>&1
			res=$?
		fi
		if [ $res -eq 0 ]
		then
			echo Single-User Mode: $TSNAME
		else
			echo "ERROR: Bad username for --user"
			exit 1
		fi
	else
		service=$1
	fi
	shift
done


# If no $service provided try ./bin/FNPLicensingService/FNPLicensingService and 
# ./FNPLicensingService/FNPLicensingService

if [ -z $service ]
then
   if [ -f ./bin/FNPLicensingService/FNPLicensingService ]
   then
       service=./bin/FNPLicensingService/FNPLicensingService
   else
       service=./FNPLicensingService/FNPLicensingService
   fi
fi

# Check that service is available
if [ ! -f "$service" ]
then
    echo "ERROR: Unable to locate anchor service to install, please specify correctly on command line"
    exit 1
fi

case `uname` in
    "Darwin")
        SERVICE_DEST="/Library/Application Support/FLEXnet Publisher/Service/11.19.1"
        ;;
    "Linux")
PID_DIR="/var/run/FNP"
mkdir -p "$PID_DIR"
setPerm "$PID_DIR" -R
if file ${service} | grep "ELF 64" > /dev/null
then
SERVICE_DEST="/usr/local/share/FNP/service64/11.19.1"
PID_DEST="$PID_DIR/FNPLicensingService64.pid"
else
SERVICE_DEST="/usr/local/share/FNP/service/11.19.1"
PID_DEST="$PID_DIR/FNPLicensingService.pid"
fi
#
# Terminate any running service
#
echo
echo Checking for active FNP Licensing Service Daemon...
if [ -x "${SERVICE_DEST}/FNPLicensingService" ]
        then
INSTALLED_VERSION=`${SERVICE_DEST}/FNPLicensingService -s | cut -f1 -d:`
if [ -n "$INSTALLED_VERSION" ]
then
ins_maj=`echo -n ${INSTALLED_VERSION} | cut -f1 -d"."`
ins_min=`echo -n ${INSTALLED_VERSION} | cut -f2 -d"."`
ins_mai=`echo -n ${INSTALLED_VERSION} | cut -f3 -d"."`
ins_hot=`echo -n ${INSTALLED_VERSION} | cut -f4 -d"."`
ins_val=`expr $ins_maj \* 1000000 + $ins_min \* 10000 + $ins_mai \* 100 + $ins_hot`
else
ins_val=0
fi

cur_maj=`echo -n 11.19.1.0 | cut -f1 -d"."`
cur_min=`echo -n 11.19.1.0 | cut -f2 -d"."`
cur_mai=`echo -n 11.19.1.0 | cut -f3 -d"."`
cur_hot=`echo -n 11.19.1.0 | cut -f4 -d"."`
cur_val=`expr $cur_maj \* 1000000 + $cur_min \* 10000 + $cur_mai \* 100 + $cur_hot`

if [ $cur_val -lt $ins_val ]
then
echo "No need to restart FNP Licensing Service Daemon"
else
echo "Need to restart FNP Licensing Service Daemon"
${SERVICE_DEST}/FNPLicensingService -k
fi

fi
echo FNP Licensing Service Daemon checks complete

# Check for SELINUX
#
echo
echo Checking for SELinux
SELINUXENABLED=`which selinuxenabled 2> /dev/null`
if [ -x "$SELINUXENABLED" ]
then
selinuxenabled
if [ $? -eq 0 ]
then
echo "*** WARNING: Running with SELINUX enabled can affect the operation of the FlexNet Licensing Service"
echo "***          Refer to the FlexNet Publisher Documentation for further details"
fi
fi
echo SELinux checks complete

# Check for LSB compatibility
#
echo
echo "Checking LSB compatibility..."
if [ -e /lib/ld-linux.so.2 -a ! -e /lib/ld-lsb.so.3 ]
then
echo "*** WARNING: 32-bit LSB packages not installed"
if [ -n "$FAKE_LSB" ]
then
echo "             Fix attempted by creating symlink for /lib/ld-lsb.so.3"
ln -s ld-linux.so.2 /lib/ld-lsb.so.3
fi
fi

if [ -e /lib64/ld-linux-x86-64.so.2 -a ! -e /lib64/ld-lsb-x86-64.so.3 ]
then
echo "*** WARNING: 64-bit LSB packages not installed"
if [ -n "$FAKE_LSB" ]
then
echo "             Fix attempted by creating symlink for /lib64/ld-lsb-x86-64.so.3"
ln -s ld-linux-x86-64.so.2 /lib64/ld-lsb-x86-64.so.3
fi
fi
echo "LSB compatibility checks complete"

;;
esac

# Install the Licensing Service executable
#
echo
echo "Installing licensing service from $service to $SERVICE_DEST"
mkdir -p "${SERVICE_DEST}"
chmod -R 755 "${SERVICE_DEST}" 
rm -f "${SERVICE_DEST}/FNPLicensingService"
cp "${service}" "${SERVICE_DEST}"
chown root "${SERVICE_DEST}/FNPLicensingService"
chmod 4755 "${SERVICE_DEST}/FNPLicensingService"


if [ -z "$CERT_ONLY" ]
then

	echo 
	echo "Checking system for trusted storage area..."

	if [ -d '/Library/Preferences' ]
	then
    	platform="Mac OS X"
    	rootpath="/Library/Preferences/FLEXnet Publisher"
	else
    	platform=`uname`
    	rootpath="/usr/local/share/macrovision/storage"
	fi

	echo "Configuring for $platform, Trusted Storage path $rootpath..."

	set +e

	#
	# Check for existance of directory, if not present try to create
	#

	if [ -d "$rootpath" ]
	then
    	if [ -x "$CHFLAGS" ]
    	then
			find "$rootpath" -flags uchg -exec chflags nouchg "$rootpath/FLEXnet" {} \;
    	elif [ -x "$CHATTR" ]
    	then
        	chattr -i "$rootpath/FLEXnet"
    	fi
    	echo "$rootpath already exists..."
	else
    	echo "Creating $rootpath..."
	fi
   	mkdir -p "$rootpath/FLEXnet"

	#
	# Set correct permissions on directory
	#

	echo "Setting permissions on $rootpath..."
	setPerm "$rootpath" -R $OWNER
	echo "Permissions set..."

	echo 
fi


if [ -z "$CERT_ONLY" ]
then

	echo 
	echo "Checking system for Replicated Anchor area..."

	if [ -d '/Library/Preferences' ]
	then
    	platform="Mac OS X"
    	temprootpath="/Library/Preferences/.com.flexnetlicensing"
	else
    	platform=`uname`
    	temprootpath="/usr/local/share/applications/.com.flexnetlicensing"
	fi

	echo "Configuring Replicated Anchor area..."

	set +e

	#
	# Check for existance of directory, if not present try to create
	#

	if [ -d "$temprootpath" ]
	then
    	if [ -x "$CHFLAGS" ]
    	then
			find "$temprootpath" -flags uchg -exec chflags nouchg {} \;
    	elif [ -x "$CHATTR" ]
    	then
        	chattr -i "$temprootpath"
    	fi
    	echo "Replicated Anchor area already exists..."
	else
    	echo "Creating Replicated Anchor area..."
    	mkdir -p "$temprootpath"
	fi

	#
	# Set correct permissions on directory
	#

	echo "Setting permissions on Replicated Anchor area..."
	setPerm "$temprootpath" -R $OWNER
	echo "Replicated Anchor area permissions set..."

fi

if [ -z "$CERT_ONLY" ]
then

	echo "Configuring Temporary area..."

	temppath="/tmp/FLEXnet"
	set +e

	#
	# Check for existance of directory, if not present try to create
	#

	if [ -d "$temppath" ]
	then
    	if [ -x "$CHFLAGS" ]
    	then
			find "$temppath" -flags uchg -exec chflags nouchg {} \;
    	elif [ -x "$CHATTR" ]
    	then
        	chattr -i "$temppath"
    	fi
    	echo "Temporary area already exists..."
	else
    	echo "Creating Temporary area..."
    	mkdir -p "$temppath"
	fi

	#
	# Set correct permissions on directory
	#

	echo "Setting permissions on Temporary area..."
	setPerm "$temppath" -R $OWNER
	echo "Temporary area permissions set..."

fi


	#
	# Ensure existence of /usr/tmp directory
	#
	if [ ! -e /usr/tmp ]
	then
		echo "Creating /usr/tmp link to /var/tmp"
		ln -s /var/tmp /usr/tmp
	fi

	#
	# Validate FUSE config file
	#
	fusermountfile=`which fusermount 2>/dev/null`
	if [ "$fusermountfile" ]
	then

    	echo "Setting FUSE configuration"

		fusefile=/etc/fuse.conf
		if [ ! -f $fusefile ]
		then
			touch $fusefile
			chmod 644 $fusefile
		fi

    	set found=0
    	while read myLine
    	do
        	myLine="${myLine#"${myLine%%[![:space:]]*}"}"   # remove leading whitespace characters
        	myLine="${myLine%"${myLine##*[![:space:]]}"}"   # remove trailing whitespace characters
        	if [ "$myLine" = "user_allow_other" ]
        	then
            	found=1
        	fi
    	done < $fusefile
    	if [ ! $found ]
    	then
        	echo "user_allow_other" >> $fusefile
    	fi

		if [ -f $fusermountfile ]
		then
			chmod +x $fusermountfile
		fi

		# Decide whether FlexNetFs needs starting, assume yes
        #
        ffs_start=1
        #

        # Check whether /dev/shm exists and has full access rights
        #
		if [ -d /dev/shm ]
		then
			stat -c "%A" /dev/shm | grep drwxrwxrw > /dev/null 2>&1
			bad_perm=$?
			if [ "$bad_perm" -eq "1" ]
			then
				echo "ERROR: FlexNetFs mount-point inaccessible because of /dev/shm file permissions"
			fi
		else
			echo "ERROR: FlexNetFs mount-point inaccessible because /dev/shm does not exist or is not a directory"
		fi

        # Check whether there is unique 1 FlexNetFs active
        #
        ffs_count=`ls -d /dev/shm/FlexNetFs.*/SFA 2>/dev/null | wc -l`
        if [ "$ffs_count" -eq "1" ]
        then
            # Check whether there is exactly one FlexNetFs process
            #
            ffs_pcount=`ps -eo args 2>/dev/null | grep -v grep | grep FlexNetFs | wc -l`
			if [ "$ffs_pcount" -eq "1" ]
			then
				# No need to restart FlexNetFs
				ffs_start=0
			fi
		fi

		if [ "$ffs_start" -ne "0" ]
		then
			# Be as forceful as we can to remove old FlexNetFs mounts
			#
    		umount /dev/shm/FlexNetFs* 2>/dev/null
    		umount /dev/shm/FlexNetFs* 2>/dev/null
			rm -rf /dev/shm/FlexNetFs*

			# We'll also need to restart the Flexnet Licensing Service
			#
			${SERVICE_DEST}/FNPLicensingService -k
		fi
else
	echo "ERROR: The FUSE sub-system is either not installed or not running"
	exit 1	
fi

if [ -z "$NO_DAEMON" ]
then
	# Start the FNP Licensing Daemon
	#
	echo
	if [ -z "$TSNAME" ]
	then
		FNLS_USER="$OWNER"
	else
		FNLS_USER=`echo "$TSNAME" | cut -f1 -d:`
	fi
	echo "Starting FNPLicensingService daemon as user $FNLS_USER"

	sudo -u $FNLS_USER "${SERVICE_DEST}/FNPLicensingService" -r
	echo "Checking FNPLicensingService is running"
	sleep 2
	svc_pid=`cat ${PID_DEST} 2>/dev/null`
	if [ -z $svc_pid ]
	then
		echo "No PID file for FNPLicensingService"
		exit 1
	fi
	svc_count=`(ps -elf | grep -v grep | grep ${SERVICE_DEST}/FNPLicensingService | grep $svc_pid | wc -l) 2>/dev/null`
	if [ "$svc_count" -ne "1" ]
	then
		echo "FNPLicensingService not running"
		exit 1
	fi
else
	${SERVICE_DEST}/FNPLicensingService -f
fi


# All done.

echo "Configuration completed successfully."
echo 
