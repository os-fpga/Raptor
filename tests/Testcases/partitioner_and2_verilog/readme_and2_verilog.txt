and2_verilog
============

Description
-----------
This is a simple design consisting of a registered, 2-input AND-gate constrained to a 200 MHz clock.  This example includes a Verilog testbench and executes an RTL simulation using the Icarus Verilog simulator prior to compiling design in the Raptor software.


Files
-----
Src/and2.v - Top-level design file containing and2 module
Src/testbench_and2.v - Simulation testbench for and2
constraints.sdc - Timing constraints file for and2
run_raptor.tcl - TCL file to create and execute project
gtkwave.tcl - TCL script to view simulation results in gtwwave viewer


Running the Design
------------------
The design can be run in one of three ways:

Intereactive GUI:
raptor
Then create the project using the following steps:
  New Project - Project Name: and2
  Type of Project: RTL Project
  Add Design Files: Add Src/and2.v
  Add Simulation Files: Add Src/testbench_and2.v
  Add Design Constraints: constraints.sdc
  Select target Design: Any device you like
Start Compilation of the Design
Once the run has completed, to view the simulation waveform:
gtkwave -S gtkwave.tcl


Automated GUI:
raptor --script run_raptor.tcl
Once the run has completed, to view the simulation waveform:
gtkwave -S gtkwave.tcl


Automated Command-line:
raptor --script run_raptor.tcl --batch

