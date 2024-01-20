# Raptor Preview Release

Rapid Silicon Raptor Design Suite

Raptor is a classic RTL (User design + IPs) 2 Bitstream FPGA compiler.
It can be ran in batch mode or GUI mode with complete Tcl scripting capability.

## INSTALLATION

See the Raptor Quick Start Guide.

## LICENSING

Please refer the LICENSE for details.

## RAPTOR OPTIONS

```
raptor --help
=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
=-= Rapid Silicon Raptor Design Suite Help  =-=
=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Command-line Options:
   --help           : Help
   --version        : Tool version
   --batch          : Tcl only, no GUI
   --script <script>: Execute a Tcl script
   --project <project file>: Open a project
   --mute           : Mutes stdout in batch mode

Tcl commands (Available in GUI or Batch console or Batch script):
---------------
--- General ---
---------------
   help                       : Help
   chatgpt <command> "<message>" ?-c <path>?: Send message to ChatGPT
       send                   : Command to send message
       reset                  : Command to reset history
       -c <path>              : Specify ini file path with API key. The key needs to be set only once for a session
                                [OpenAI]
                                API_KEY: <api key>

---------------
--- Project ---
---------------
   create_design <name> ?clean? ?-type <project type>? : Creates a design with <name> name
     <project type>           : rtl (Default), gate-level
     clean                    : If project folder already exists, remove recursively folder content
   open_project <file>        : Opens a project
   run_project <file>         : Opens and immediately runs the project
   target_device <name>       : Targets a device with <name>
   device_file <file>         : Set file <file> with supported devices which replaces default file (device.xml)
   add_design_file <file list> ?type?   ?-work <libName>?   ?-L <libName>? 
       <type>                 : -VHDL_1987, -VHDL_1993, -VHDL_2000, -VHDL_2008, -VHDL_2019, -V_1995, -V_2001, -SV_2005, -SV_2009, -SV_2012, -SV_2017, default auto-detect 
       -work <libName>        : Compiles the compilation unit into library <libName>, default is "work"
       -L <libName>           : Import the library <libName> needed to compile the compilation unit, default is "work"
   set_top_module <top> ?-work <libName>? : Sets the top-level design module/entity for synthesis
   add_include_path <paths>   : Specify paths for Verilog includes (Not applicable to VHDL)
   add_library_path <paths>   : Specify paths for libraries (Not applicable to VHDL)
   add_library_ext <ext>      : Spcify library extensions (Not applicable to VHDL)
   set_macro <name>=<value>   : As in -D<macro>=<value>
   read_netlist <file>        : Read a netlist (.blif/.eblif) instead of an RTL design (Skip Synthesis)
   add_constraint_file <file> : Sets constraints file (SDC) filename and location
   add_simulation_file <file list> ?type?   ?-work <libName>?   ?-L <libName>? 
       <type>                 : -VHDL_1987, -VHDL_1993, -VHDL_2000, -VHDL_2008, -VHDL_2019, -V_1995, -V_2001, -SV_2005, -SV_2009, -SV_2012, -SV_2017, -C, -CPP 
       -work <libName>        : Compiles the compilation unit into library <libName>, default is "work"
       -L <libName>           : Import the library <libName> needed to compile the compilation unit, default is "work"
   set_top_testbench <module> : Sets the top-level testbench module/entity for simulation
   clear_simulation_files     : Remove all simulation files
   script_path                : Returns the path of the Tcl script passed with --script
   max_threads <-1/[2:64]>    : Maximum number of threads to be used (-1 is for automatic selection)

--------------------
--- Design query ---
--------------------
   all_inputs                 : Return all input ports for the current design.
   all_outputs                : Return all output ports for the current design.
   get_ports <pattern>        : Return all ports for the current design that match a specified pattern. If pattern is empty, get_ports returns empty string.
       <pattern>              : Examples of supported patterns: {*}, {cl*}, {d[0]}, {clock}, {clock1 clock2}

-------------------
--- Constraints ---
-------------------
   keep <signal list> OR all_signals : Keeps the list of signals or all signals through Synthesis unchanged (unoptimized in certain cases)
   message_severity <message_id> <ERROR/WARNING/INFO/IGNORE> : Upgrade/downgrade message severity

----------
--- IP ---
----------
   add_litex_ip_catalog <directory> : Browses directory for LiteX IP generators, adds the IP(s) to the IP Catalog
   ip_catalog ?<ip_name>?     : Lists all available IPs, and their parameters if <ip_name> is given 
   configure_ip <IP_NAME> -mod_name <name> -out_file <filename> -version <ver_name> -P<param>="<value>"...
                              : Configures an IP <IP_NAME> and generates the corresponding file with module name
   ipgenerate ?clean?         : Generates all IP instances set by ip_configure
     clean                    : Deletes files generated from this task
   simulate_ip  <module name> : Simulate IP with module name <module name>

------------------
--- Simulation ---
------------------
   simulation_options <simulator> <phase> ?<level>? <options> : Sets the simulator specific options for the specified phase
                      <phase> : compilation, elaboration, simulation, extra_options
   simulate <level> ?<simulator>? ?<waveform_file>?: Simulates the design and testbench
                      <level> : rtl, gate, pnr. rtl: RTL simulation, gate: post-synthesis simulation, pnr: post-pnr simulation
                  <simulator> : verilator, ghdl, icarus
   wave_*                     : All wave commands will launch a GTKWave process if one hasn't been launched already. Subsequent commands will be sent to the launched process.
   wave_cmd ...               : Sends given tcl commands to GTKWave process. See GTKWave docs for gtkwave:: commands.
   wave_open <filename>       : Load given file in current GTKWave process.
   wave_refresh               : Reloads the current active wave file
   wave_show <signal>         : Add the given signal to the GTKWave window and highlight it.
   wave_time <time>           : Set the primary marker to <time>. Time units can be specified, without a space. Ex: wave_time 100ps.

-----------------
--- Synthesis ---
-----------------
   synth_options <option list>:
     -effort <level>          : Optimization effort level (high, medium, low)
       high                   : Most compute, generally impacting runtime (default)
       medium                 : Balanced compute
       low                    : least compute, least runtime
     -fsm_encoding <encoding> : FSM encoding (binary, onehot)
       binary                 : Compact encoding - using minimum of registers to cover the N states
       onehot                 : One hot encoding - using N registers for N states (default)
     -carry <mode>            : Carry logic inference mode (all, auto, none)
       all                    : Infer as much as possible
       auto                   : Infer carries based on internal heuristics (default)
       none                   : Do not infer carries
     -clke_strategy <strategy>: Clock enable extraction strategy for FFs (early, late)
       early                  : Perform early extraction (default)
       late                   : Perform late extraction
     -fast                    : Perform the fastest synthesis. QoR can be impacted.
     -no_flatten              : Do not flatten design.
     -no_simplify             : Do not run special simplification algorithms in synthesis. 
   set_limits <type> <int>    : Sets a user limit on object of type (dsp, bram), specify 0 to disable block inferrence
       dsp                    : Maximum number of usable DSPs
       bram                   : Maximum number of usable BRAMs
   analyze ?clean?            : Analyzes the RTL design, generates top-level, pin and hierarchy information
     clean                    : Deletes files generated from this task
   synthesize <optimization>  ?clean? : RTL Synthesis, optional opt. (area, delay, mixed)
     <optimization>           : area, delay, mixed
       area                   : Optimize for reduce resource area 
       delay                  : Optimize for performance
       mixed                  : Optimize for area and performance (default)
     clean                    : Deletes files generated from this task

---------------
--- Packing ---
---------------
   pnr_netlist_lang <format>  : Chooses post-synthesis netlist format, (blif, edif, verilog, vhdl)
   packing_options <option list>: Packing options
     -clb_packing <directive> : Performance optimization flags (auto, dense, timing_driven)
       auto                   : CLB packing automatically determined (default)
       dense                  : Pack logic more densely into CLBs resulting in fewer utilized CLBs however may negatively impact timing
       timing_driven          : Pack logic to optimize timing
   packing ?clean?            : Packing
     clean                    : Deletes files generated from this task

-------------
--- Place ---
-------------
   pin_loc_assign_method <method>: Algortihm for automatic pin assignment (in_define_order, random, pin_constraint_disabled)
       in_define_order        : Port order pin assignment (default)
       random                 : Random pin assignment
       pin_constraint_disabled: No automatic pin assignment
   place ?clean?              : Placer
     clean                    : Deletes files generated from this task
   pnr_options <option list>  : PnR Options

-------------
--- Route ---
-------------
   route ?clean?              : Router
     clean                    : Deletes files generated from this task

------------------------------
--- Static Timing Analysis ---
------------------------------
   sta ?clean? ?opensta?      : Statistical Timing Analysis
     clean                    : Deletes files generated from this task
     opensta                  : Use OpenSTA tool for timing analysis

------------------------------
--- Power Analysis -----------
------------------------------
   power ?clean?              : Statistical Power Analysis
     clean                    : Deletes files generated from this task

-----------------
--- Bitstream ---
-----------------
   bitstream ?clean?          : Bitstream generation
     clean                    : Deletes files generated from this task

---------------------------------
--- Implementation Strategy   ---
---------------------------------
timing_flow <"default" | "analytic"> : Timing driven compilation flow. This proc performs a series of steps in the design flow to achieve timing-driven placement and routing.
  "analytic"                   : Use timing-driven analytic placement as initial placement. The result is then passed through the simulated annealing (SA) placer.
  "default"                    : Uses VPR's default initial placer (default)

routability_flow <number_of_molecules_in_partition, hmetisPath> : Routability driven compilation flow. This proc accepts two optional arguments:
  number_of_molecules_in_partition <int>: Average number of molecules in each cluster (default is 200).
  hmetisPath <file>            : The path to the hmetis executable (default is "~/bin/hmetis")
  
congestion_flow <congestion_type> : Congestion driven compilation flow
  "uniform"                    : For uniformely congested high utilization designs
  "hotspot"                    : For hotspots of congestion in moderately utilized designs

---------------
--- Program ---
---------------
  program_device <-b> "<bitstream_file>" <-c> "<config_file>" <-n> "<index>" : Program the device via JTAG
    -b <bitstream>            : Target bitstream file for loading
    -c <config_file>          : Configuration file
    -n <index>                : Index of device in JTAG chain
    
-----------------------------------------------
```

## RAPTOR EXAMPLE DESIGNS

```
Example designs are installed with Raptor under:

<install_path>/share/raptor/examples (ie: /usr/local/share/raptor/examples) for Project examples
and
<install_path>/share/raptor/tcl_examples (ie: /usr/local/share/raptor/tcl_examples) for Tcl examples

To run one of the examples, cd into any local user directory and run:

raptor --script $RAPTOR_PATH/share/raptor/tcl_examples/and2_verilog/raptor.tcl (GUI mode)
or
raptor --batch --script $RAPTOR_PATH/share/raptor/tcl_examples/and2_verilog/raptor.tcl (Batch mode)

A project directory gets created with all the artefacts generated by Raptor.

A log file gets created too: raptor.log with all the compilation steps logs agregated.

Example designs, please refer to:
<install_path>/share/raptor/tcl_examples/readme_raptor_examples.txt for a complete list of examples.

```

## User design


### Synthesis considerations

```
Each call to the "add_design_file" creates a separate Compilation Unit (In the Verilog sense),
the files in that call share the same macro definition space.

Verilog and VHDL files cannot be mixed in the same "add_design_file" call.

VHDL Libraries are not supported at this time, only individual .vhdl file compilation is.

```

### SDC considerations

```

A .sdc file (Tcl file with special handling of square brackets[]) can be provided using the "add_constraint_file" command.
The file and contain classic SDC commands, Tcl scripting and set_pin_loc commands.
SDC are honored by the Placement and STA stages.
Synthesis will preserve with best effort the names of the signals referenced in the contraints in order for the Placement and STA to find them in the netlist.

```
