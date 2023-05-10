[![main](https://github.com/RapidSilicon/Raptor/actions/workflows/main.yml/badge.svg)](https://github.com/RapidSilicon/Raptor/actions/workflows/main.yml)

# Raptor Preview Release

Rapid Silicon Raptor Design Suite

Raptor is a classic RTL (User design + IPs) 2 Bitstream FPGA compiler.
It can be ran in batch mode or GUI mode with complete Tcl scripting capability.

## INSTALLATION

See [`Install Raptor`](INSTALL.md)

## LICENSING

Raptor uses the FlexLM License manager, please setup the license file:
```setenv LM_LICENSE_FILE <path to license file or lmgrd IP address>```

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
   open_project <file>        : Opens a project
   run_project <file>         : Opens and immediately runs the project
   target_device <name>       : Targets a device with <name> (default is 1GE75)
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
   ip_configure <IP_NAME> -mod_name <name> -out_file <filename> -version <ver_name> -P<param>="<value>"...
                              : Configures an IP <IP_NAME> and generates the corresponding file with module name
   ipgenerate ?clean?         : Generates all IP instances set by ip_configure
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
     -no_simplify             : Do not run special simplification algorithms in synthesis. 
   set_limits <type> <int>    : Sets a user limit on object of type (dsp, bram), specify 0 to disable block inferrence
       dsp                    : Maximum number of usable DSPs
       bram                   : Maximum number of usable BRAMs
   analyze ?clean?            : Analyzes the RTL design, generates top-level, pin and hierarchy information, clean removes related files
   synthesize <optimization>  ?clean? : RTL Synthesis, optional opt. (area, delay, mixed), clean removes related files
       area                   : Optimize for reduce resource area 
       delay                  : Optimize for performance
       mixed                  : Optimize for area and performance (default)

---------------
--- Packing ---
---------------
   pnr_netlist_lang <format>  : Chooses post-synthesis netlist format, (blif, edif, verilog, vhdl)
   clb_packing   <directive>  : Performance optimization flags (auto, dense)
       auto                   : CLB packing automatically determined (default)
       dense                  : Pack logic more densely into CLBs resulting in fewer utilized CLBs however may negatively impact timing
       timing_driven          : Pack logic to topimize timing
   packing ?clean?            : Packing, clean removes related files


-------------
--- Place ---
-------------
   pin_loc_assign_method <method>: Algortihm for automatic pin assignment (in_define_order, random, free)
       in_define_order        : Port order pin assignment (default)
       random                 : Random pin assignment
       free                   : No automatic pin assignment
   place ?clean?              : Placer, clean removes related files

-------------
--- Route ---
-------------
   route ?clean?              : Router, clean removes related files

------------------------------
--- Static Timing Analysis ---
------------------------------
   sta ?clean?                : Statistical Timing Analysis, clean removes related files

-----------------
--- Bitstream ---
-----------------
   bitstream ?clean?          : Bitstream generation, clean removes related files
   program_device <-b> "<bitstream_file>" <-c> "<config_file>" <-n> "<index>": Perform device programming.
     -b <bitstream_file>      : Specify bitstream file path to program. Ex: -b /home/user/mybitstream.bit
     -c <config_file>         : Specify config file. Ex: -c gemini.cfg
     -n <index>               : Optional. Default value is 0. Specify index of the device. Ex. -n 0

-----------------------------------------------
```

## RAPTOR EXAMPLE DESIGNS

```
Example designs are installed with Raptor under:

<install_path>/share/raptor/examples (ie: /usr/local/share/raptor/examples)

To run one of the examples, cd into any local user directory and run:

raptor --script $RAPTOR_PATH/share/raptor/examples/and2_gemini/raptor.tcl (GUI mode)
or
raptor --batch --script $RAPTOR_PATH/share/raptor/examples/and2_gemini/raptor.tcl (Batch mode)

A project directory gets created with all the artefacts generated by Raptor.

A log file gets created too: raptor.log with all the compilation steps logs agregated.

Example designs:

and2_gemini       : an AND2 design targeting the 1GE75 device
                    raptor --script $RAPTOR_PATH/share/raptor/tcl_examples/and2_gemini/raptor.tcl

oneff             : an async-reset FF design targeting the 1GE75 device with Verilator simulation
                    raptor --script $RAPTOR_PATH/share/raptor/tcl_examples/oneff/raptor.tcl

aes_decrypt_fpga  : an AES decryption design targeting the 1GE75 device
                    raptor --script $RAPTOR_PATH/share/raptor/tcl_examples/aes_decrypt_fpga/aes_decrypt.tcl

ip_gen_axis_conv  : a LiteX IP generation example targeting the 1GE75 device (Requires LiteX installed separately)
                    raptor --script $RAPTOR_PATH/share/raptor/tcl_examples/ip_gen_axis_conv/raptor.tcl

sasc_testcase    : a FIFO design targeting the 1GE75 device
                    raptor --script $RAPTOR_PATH/share/raptor/tcl_examples/sasc_testcase/raptor.tcl
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
