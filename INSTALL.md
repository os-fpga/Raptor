# Raptor
----
Raptor is the Rapid Silicon complete FPGA EDA design environment.  The primary user experience combines a Tcl-based scripting environment with a Qt-based GUI cockpit.  The back-end toolchain consists of a suite of open source tools with proprietary components for specific features.

* FOEDAG - https://github.com/os-fpga/FOEDAG
* LiteX - https://github.com/enjoy-digital/litex
* Yosys - https://github.com/YosysHQ/yosys
* ABC - https://github.com/berkeley-abc/abc
* VPR - https://github.com/verilog-to-routing
* OpenSTA - https://github.com/The-OpenROAD-Project/OpenSTA
* OpenFPGA - https://github.com/lnis-uofu/OpenFPGA

----
## SYSTEM REQUIREMENTS

### HARDWARE EQUIPMENT REQUIREMENTS
In order to get the most out of Raptor, please ensure that your server or workstation meets the following hardware requirements:

| Hardware Component	| Minimum Requirement	| Recommended	|
|-----------------------|-----------------------|---------------|
| CPU Processor			| 64-bit Intel or AMD @ 2.0GHz or higher (SMP supported)	| 4 cores or more recommended				|
| Memory				| 8GB						| 16GB+				|
| Disk Space			| 1GB						| 1.5GB+, SSD recommended				|
| Display				| 1024x768 @ 24-bit color						| 1920x1080+ @24-bit color				|

### OPERATING SYSTEM SUPPRORT
The Raptor EDA environment requires a Linux-based server or workstation environment.

 * Current OS Support: CentOS 7.x, Ubuntu 20.04 LTS, Ubuntu 21.04
 * Future OS Support: MacOS, Windows 10+

### OPERATING SYSTEM DEPENDENCIES
Raptor requires a select number of host system dependencies, outlined below

| Dependency	 | Ubuntu Package		                       | CentOS Package	                 |
|--------------|-----------------------------------------|---------------------------------|
| bzip2			   | apt install -y bzip2	                   | yum install -y bzip2	           |
| libtcmalloc  | apt install -y libgoogle-perftools-dev  | yum install -y gperftools-devel |
| QT5          | apt install -y qtbase5-dev qtchooser qt5-qmake qtbase5-dev-tools qtdeclarative5-dev  | yum install -y libxcb libxcb-devel xcb-util xcb-util-devel libxkbcommon-devel libxkbcommon-x11-devel xcb-util-image-devel xcb-util-keysyms-devel xcb-util-renderutil-devel xcb-util-wm-devel |
| Python3      | apt install -y python3  | yum install -y python3 |

---- 
## DOWNLOAD AND INSTALLATION

### DOWNLOADING RAPTOR
Raptor is provided via digital download either from the Rapid Silicon FTP server or from a private lounge web site.  Please reach out to your Rapid Silicon point-of-contact to determine which is appropriate for your use.

Raptor is provided as a TAR archive (.tar.gz) file.

### VERIFYING THE DOWNLOAD
Once the download is complete, please verify the integrity of the download by comparing the checksum of the downloaded file against the provided checksum file.

`$ md5sum -c <path_to_checsum_file>.md5 <path_to_downloaded_tar_file>`

### INSTALLING RAPTOR
#### Installing with default options

With default options, you are allowing the Raptor executable to install all the OS pre requisite and install Raptor in default location i.e. /opt
Providing any other options except `-v or --verbose` will have no effect in this case.
Go to the directory where you have extracted the Raptor executable after downloading and execute it with SUDO/admin account like below

```
./Raptor_x.x.run
```

Replace `Raptor_x.x.run` with the version of Raptor executable file you have received.

#### Installing with personal choices

Following command line options available:

* -b | --batch-mode   -> running in interactive mode. Mandatory if you want to use -r or -i option
* -r | --raptor-home  -> you are giving the absolute path of directory where Raptor will be installed
* -i | --install-dep  -> Turn on the OS dependecies installation
* -v | --verbose      -> increase the installer verbosity.

Go to the directory where you have extracted the Raptor exe after downloading and execute it with any or with all above mentioned options like below

```
./Raptor_x.x.run -- -i -v -r <absolute path where Raptor will be installed>
``` 

Replace `Raptor_x.x.run` with the version of Raptor exe file you have received.

## LICENSING

### OBTAINING A LICENSE
Some components of Raptor are currently based on 3rd-party proprietary tooling.  These components are license-controlled and require a FlexLM-based license in order to operate.

To obtain a FlexLM license for the Raptor installion, please reach out to your Rapid Silicon point-of-contact for additional guidance.  Licenses are available in two types:
* Node-Locked: A node-locked license restricts the license-controlled software to operating on a single computing device such as an individual workstation or server
* Floating: A floating license is hosted on a network-accessible server using FlexLM license hosting software.  Individual installations of Raptor can temporarily check-out a license during operation and return it to the pool when finished.  This technique allows sharing among multiple users.

Please let your Rapid Silicon point-of-contact know which type of license you would like to receive.

### INSTALLING AND CONFIGURING A LICENSE
On the client workstation, set the `LM_LICENSE_FILE` environment variable.  For node-locked workstations, specify the full path to the license file.  For floating licenses, a FlexLM server daemon is required.  This daemon is provided by Rapid Silicon and is used to host the floating license on a specific port.

Example license lookup variables:
* Node-Locked: `LM_LICENSE_FILE = path/on/disk/LICENSE.DAT`
* Floating: `LM_LICENSE_FILE = <por>@<hostname>`

### CHECKING LICENSE STATUS
The FlexLM licensing environment provides a suite of tools that can parse a given system (or server in the case of floating licenses) and report back the available licenses and the number of seats occupied.  The primary tool is `lmutil`.  This tool provides a suite of sub-commands such as `lmutil lmstat` that can be used to investigate the status of the floating license server.

----
## RAPTOR DEVICE SUPPORT
This EA version of Raptor does not support device bitstream generation.

----
## KNOWN ISSUES
For a complete list of the known issues in this release, please see the accompanying release-specific documentation.
