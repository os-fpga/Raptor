####################################
# Gearbox block definition 
####################################
define_block -name GBOX_HV_40X3_VL 

####################################
# all IO chain description
####################################
#### create_instance -block IO_CFG -name u_io_cfg
####################################
# bank description
####################################
set PAR_IO_NUM 40
set NUM_BANKS 3

####################################
# num of bits in each cfg block
####################################
set IO_CHAIN_GBOX_SIZE 42
set IO_CHAIN_GBOX_BANK_SIZE [expr $IO_CHAIN_GBOX_SIZE*$PAR_IO_NUM]
set IO_CHAIN_GBOX_HV_PGEN_SIZE 3
set IO_CHAIN_GBOX_VCO_FAST_SIZE 18      ;# FCLK_MUX
set IO_CHAIN_GBOX_ROOT_BANK_MUX_SIZE 20

set IO_CHAIN_GBOX_HV_START 0
set IO_CHAIN_GBOX_HV_PGEN_START  [expr $IO_CHAIN_GBOX_HV_START + $NUM_BANKS * $IO_CHAIN_GBOX_BANK_SIZE]
set IO_CHAIN_GBOX_VCO_FAST_START [expr $IO_CHAIN_GBOX_HV_PGEN_START + $IO_CHAIN_GBOX_HV_PGEN_SIZE]
set IO_CHAIN_GBOX_ROOT_BANK_MUX_START [expr $IO_CHAIN_GBOX_VCO_FAST_START + $IO_CHAIN_GBOX_VCO_FAST_SIZE]

set IO_CHAIN_GBOX_HV_40X3_VL_SIZE [expr $IO_CHAIN_GBOX_ROOT_BANK_MUX_START + $NUM_BANKS * $IO_CHAIN_GBOX_ROOT_BANK_MUX_SIZE]

# puts "IO_CHAIN_GBOX_HV_40X3_VL_SIZE = $IO_CHAIN_GBOX_HV_40X3_VL_SIZE"

#
# Start at the MSB
#
###############################################################################
# ROOT_BANK_CLKMUX
###############################################################################
create_instance -block ROOT_BANK_CLKMUX -name u_gbox_root_bank_clkmux_hv_2 -logic_address [expr ${IO_CHAIN_GBOX_ROOT_BANK_MUX_START} + 20*2] -parent GBOX_HV_40X3_VL
create_instance -block ROOT_BANK_CLKMUX -name u_gbox_root_bank_clkmux_hv_1 -logic_address [expr ${IO_CHAIN_GBOX_ROOT_BANK_MUX_START} + 20] -parent GBOX_HV_40X3_VL
create_instance -block ROOT_BANK_CLKMUX -name u_gbox_root_bank_clkmux_hv_0 -logic_address [expr ${IO_CHAIN_GBOX_ROOT_BANK_MUX_START}  ] -parent GBOX_HV_40X3_VL
###############################################################################
# VCO_FAST (FCLK_MUX)
###############################################################################
create_instance -block FCLK_MUX -name u_gbox_fclk_mux_hv_all -logic_address $IO_CHAIN_GBOX_VCO_FAST_START -parent GBOX_HV_40X3_VL
###############################################################################
# HV_PGEN
###############################################################################
create_instance -block HV_PGEN -name u_HV_PGEN_dummy -logic_address $IO_CHAIN_GBOX_HV_PGEN_START -parent GBOX_HV_40X3_VL

#
# Note:  RTL code indicates A is at the higher address
#
###############################################################################
# BANK 2 (top)
###############################################################################
set tile_indices {47 48 49 50 51 52 53 54 55 56 58 59 60 61 62 63 64 65 66 67}
for {set IO_INDEX 0} {$IO_INDEX < [expr { $PAR_IO_NUM / 2}]} {incr IO_INDEX} {
    set tile_y [lindex $tile_indices $IO_INDEX]
    create_instance -block GBOX_TOP -name u_HV_GBOX_BK2_A_${IO_INDEX} \
	-logic_location [list 0 $tile_y A] \
	-logic_address [expr $IO_CHAIN_GBOX_HV_START + $PAR_IO_NUM * 42 * 2 + $IO_INDEX * 42 * 2 + 42] \
	-parent GBOX_HV_40X3_VL

    create_instance -block GBOX_TOP -name u_HV_GBOX_BK2_B_${IO_INDEX} \
	-logic_location [list 0 $tile_y B] \
	-logic_address [expr $IO_CHAIN_GBOX_HV_START + $PAR_IO_NUM * 42 * 2 + $IO_INDEX * 42 * 2 + 0] \
	-parent GBOX_HV_40X3_VL

}

###############################################################################
# BANK 1 (middle)
###############################################################################
set tile_indices {25 26 27 28 29 30 31 32 33 34 36 37 38 39 40 41 42 43 44 45}
for {set IO_INDEX 0} {$IO_INDEX < [expr { $PAR_IO_NUM / 2}]} {incr IO_INDEX} { 
    set tile_y [lindex $tile_indices $IO_INDEX]
    create_instance -block GBOX_TOP -name u_HV_GBOX_BK1_A_${IO_INDEX}  \
	-logic_location [list 0 $tile_y 0] \
	-logic_address [expr $IO_CHAIN_GBOX_HV_START + $PAR_IO_NUM * 42 + $IO_INDEX * 42 * 2 + 42] \
	-parent GBOX_HV_40X3_VL

    create_instance -block GBOX_TOP -name u_HV_GBOX_BK1_B_${IO_INDEX} \
	-logic_location [list 0 $tile_y 1] \
	-logic_address [expr $IO_CHAIN_GBOX_HV_START + $PAR_IO_NUM * 42 + $IO_INDEX * 42 * 2 + 0] \
	-parent GBOX_HV_40X3_VL

}

###############################################################################
# BANK 0 (bottom)
###############################################################################
set tile_indices {3 4 5 6 7 8 9 10 11 12 14 15 16 17 18 19 20 21 22 23}
for {set IO_INDEX 0} {$IO_INDEX < [expr { $PAR_IO_NUM / 2}]} {incr IO_INDEX} { 
    set tile_y [lindex $tile_indices $IO_INDEX]
    create_instance -block GBOX_TOP -name u_HV_GBOX_BK0_A_${IO_INDEX} \
	-logic_location [list 0 $tile_y 0] \
	-logic_address [expr $IO_CHAIN_GBOX_HV_START + $IO_INDEX * 42 * 2 +  42] \
	-parent GBOX_HV_40X3_VL
    create_instance -block GBOX_TOP -name u_HV_GBOX_BK0_B_${IO_INDEX}  \
	-logic_location [list 0 $tile_y 1] \
	-logic_address [expr $IO_CHAIN_GBOX_HV_START + $IO_INDEX * 42 * 2 + 0] \
	-parent GBOX_HV_40X3_VL

}

###############################################################################
define_ports -block GBOX_HV_40X3_VL -in system_reset_n 
define_ports -block GBOX_HV_40X3_VL -in cfg_done 
##
define_ports -block GBOX_HV_40X3_VL -out hv_0_root_core_clk_bit1 hv_0_root_core_clk_bit0 
define_ports -block GBOX_HV_40X3_VL -out hv_1_root_core_clk_bit1 hv_1_root_core_clk_bit0 
define_ports -block GBOX_HV_40X3_VL -out hv_2_root_core_clk_bit1 hv_2_root_core_clk_bit0 
define_ports -block GBOX_HV_40X3_VL -out hv_0_root_cdr_clk_bit1 hv_0_root_cdr_clk_bit0 
define_ports -block GBOX_HV_40X3_VL -out hv_1_root_cdr_clk_bit1 hv_1_root_cdr_clk_bit0 
define_ports -block GBOX_HV_40X3_VL -out hv_2_root_cdr_clk_bit1 hv_2_root_cdr_clk_bit0 
#####
define_ports -block GBOX_HV_40X3_VL -out hv_rx_io_clk0_bit1 hv_rx_io_clk0_bit0 
define_ports -block GBOX_HV_40X3_VL -out hv_rx_io_clk1_bit1 hv_rx_io_clk1_bit0 
define_ports -block GBOX_HV_40X3_VL -out hv_rx_io_clk2_bit1 hv_rx_io_clk2_bit0 
#####
for {set tt 2} {$tt <= 67} {incr tt} { 
  for {set run_idx 0} {$run_idx <= 48} {incr run_idx} { 
    define_ports -block GBOX_HV_40X3_VL -in left_${tt}_f2a_bit${run_idx}
 } 
} 
#####
for {set tt 2} {$tt <= 67} {incr tt} { 
  for {set run_idx 0} {$run_idx <= 23} {incr run_idx} { 
    define_ports -block GBOX_HV_40X3_VL -out left_${tt}_a2f_bit${run_idx}
 } 
} 
#####
###############################################################################
