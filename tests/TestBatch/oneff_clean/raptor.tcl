# Design
create_design oneff_gemini_clean
add_design_file -SV_2009 dut.v
set_top_module dut
add_constraint_file pin_mapping.pin
add_constraint_file constraints.sdc

# Testbench
add_simulation_file -SV_2009 syn_tb.v
add_simulation_file sim_main.cpp
set_top_testbench syn_tb

# Device target
target_device GEMINI_10x8

# Compilation/Simulation
analyze

# RTL Simulation
simulate rtl verilator syn_tb_rtl.fst
set projName oneff_gemini_clean
set simRtlPath [pwd]/$projName/$projName.runs/run_1/synth_1/simulate_rtl
set exists [file exists $simRtlPath/syn_tb_rtl.fst]
if {$exists != 1} { puts "syn_tb_rtl.fst does not exists"; exit 1 }


# Synthesis
synthesize delay

# Post-Synthesis gate-level Simulation
simulate gate verilator syn_tb_gate.fst
set simGatePath [pwd]/$projName/$projName.runs/run_1/synth_1/simulate_gate
set exists [file exists $simGatePath/syn_tb_gate.fst]
if {$exists != 1} { puts "syn_tb_gate.fst does not exists"; exit 1 }

# Pack/Place/Route
packing
place
route

# Post-Route Simulation
simulate pnr verilator syn_tb_pnr.fst
set simPnrPath [pwd]/$projName/$projName.runs/run_1/synth_1/simulate_pnr
set exists [file exists $simPnrPath/syn_tb_pnr.fst]
if {$exists != 1} { puts "syn_tb_pnr.fst does not exists"; exit 1 }

# Static Timing Analysis
sta opensta

# Bitstream Generation
bitstream

# bitstream simulation
#simulate bitstream_bd syn_tb_bitstream.fst

#clean all
simulate rtl  clean
simulate gate clean
simulate pnr clean
#simulate bitstream_bd clean

# check is files exist
set exists [file exists $simRtlPath/syn_tb_rtl.fst]
if {$exists != 0} { puts "syn_tb_rtl.fst exists"; exit 1 }
set exists [file exists $simGatePath/syn_tb_gate.fst]
if {$exists != 0} { puts "syn_tb_gate.fst exists"; exit 1 }
set exists [file exists $simPnrPath/syn_tb_pnr.fst]
if {$exists != 0} { puts "syn_tb_pnr.fst exists"; exit 1 }
#set exists [file exists $path/syn_tb_bitstream.fst]
#if {$exists != 0} { puts "syn_tb_bitstream.fst exists"; exit 1 }


