Raptor Design Suite Examples
============================

This directory contains several example designs introducing some of the capabilities of the Rapid Silicon Raptor Design Suite.
Within each directory contains design source files, example constraints file(s) and TCL execution script(s).
These designs can be run in one of 3 ways:

1) In an interactive batch mode by executing the Raptor GUI and manually adding the design, simulation and constraints files and then running the design.
The Raptor GUI can be launched by typing: raptor

2) In the GUI with automated project creation and executions.  To run the tool in this way: raptor --script run_raptor.tcl

3) In a command-line batch mode by executing: raptor --script run_raptor.tcl --batch

Within each directory is a readme file with more details on that particular example.

Different designs address different topics. Below attempts to guide to possible designs of interest.

Introduction Examples - The following are simple designs that introduce project creation, simulation and implementation
  and2_verilog - A simple registered 2-input AND-gate 
  counter_verilog - A simple paramatizable Verilog counter design
  counter_vhdl - A simple VHDL counter design (code from fpga4student.com)
  oneff_verilog - A simple FF example with a Verilator C testbench

Verilog RTL Designs:
  aes_decrypt_verilog - 128-bit AES decrption code from opencores.org
  and2_verilog - A simple registered 2-input AND-gate
  counter_verilog - A simple paramatizable Verilog counter design
  axi2axilite_bridge  - A simple design using the axi2axilite_bridge generated IP within Raptor
  sasc_testcase - Simple Asynchronous Serial Comm. Device from opencores.org

VHDL RTL Designs:
  counter_vhdl - A simple VHDL counter design (code from fpga4student.com)

Gate-level Designs:
  aes_decrypt_gate - Design using gate-level BLIF file produced from aes_decrypt_verilog using Raptor synthesis

Designs containing IP:
  axi2axilite_bridge - A simple design using the axi2axilite_bridge generated IP within Raptor


