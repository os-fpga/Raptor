# Design
create_design and2_bitstream_sim
add_design_file -V_2001 ./rtl/and2.v
set_top_module and2
add_constraint_file constraints.sdc

# Testbench
add_simulation_file testbench.sv
set_top_testbench tb_and2

# Device target
target_device GEMINI_COMPACT_10x8

# Compilation/Simulation
analyze

# RTL Simulation
simulate "rtl" "icarus" dump.fst

# Compilation
synthesize delay

# Gate Simulation
simulate "gate" "icarus" dump.fst

# Pack/Place/Route
packing
place
route

# Timing 
sta

# Bitstream generation with bitstream testbench generation
bitstream enable_simulation

# Bitstream Simulation
clear_simulation_files
add_simulation_file SRC/CustomModules/rs_mmff.v SRC/cells.v 
add_library_path ../../Castor_V2/IPs/openfpga-pd-castor-rs/k6n8_TSMC16nm_7.5T/FPGA10x8_dp_castor_pnr/release/FPGA10x8_dp_castor_verilog/SRC/submodules/
simulate "bitstream_bd" "icarus" 
