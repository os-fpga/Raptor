# Raptor
----
Raptor is the Rapid Silicon complete FPGA EDA design environment.  The primary user experience combines a Tcl-based scripting environment with a Qt-based GUI cockpit.  The back-end toolchain consists of a suite of open source tools with proprietary components for specific features.

| Open-Source tool | Description          | GitHub repository                       |
|------------------|----------------------|-----------------------------------------|
| FOEDAG           | GUI Toolkit          | https://github.com/os-fpga/FOEDAG       |
| LiteX            | IP Integration       | https://github.com/enjoy-digital/litex  |
| Yosys            | Synthesis            | https://github.com/YosysHQ/yosys        |
| ICARUS Verilog   | Synthesis (Verilog)  | https://github.com/steveicarus/iverilog |
| GHDL             | Synthesis (VHDL)     | https://github.com/ghdl/ghdl            |
| ABC              | Sequential Synthesis | https://github.com/berkeley-abc/abc     |
| VPR              | Place & Route        | https://github.com/verilog-to-routing   |
| OpenFPGA         | Bitstream Generation | https://github.com/lnis-uofu/OpenFPGA   |
| Verilator        | Simulation           | https://github.com/verilator/verilator  |

----
## SYSTEM REQUIREMENTS
In order to get the most out of Raptor, please ensure that your server or workstation meets the following hardware requirements:

| Hardware	            | Minimum Requirement	| Recommended	|
|-----------------------|-----------------------|---------------|
| CPU Processor			| 64-bit Intel or AMD @ 2.0GHz or higher| 4 cores+				|
| Memory				| 8GB						| 16GB+				|
| Disk Space			| 1GB						| 1.5GB+, SSD				|
| Display				| 1024x768 @ 24-bit color						| 1920x1080+ @24-bit color				|

## OPERATING SYSTEM SUPPRORT
The Raptor EDA environment requires a Linux-based server or workstation environment.

 * Current OS Support: CentOS 7.x, Ubuntu 20.04 LTS, Ubuntu 22.04.3 LTS 
 * Future OS Support: MacOS, Windows 10+

---- 
## RAPTOR DEVICE SUPPORT
This EA version of Raptor does not support device bitstream generation.

----
## KNOWN ISSUES
For a complete list of the known issues in this release, please see the accompanying [release-specific documentation](https://github.com/os-fpga/Raptor/releases).
