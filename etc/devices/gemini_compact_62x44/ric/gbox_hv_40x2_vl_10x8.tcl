####################################
# Gearbox block definition 
####################################
define_block -name GBOX_HV_40X2_VL_10X8 

####################################
# all IO chain description
####################################
#### create_instance -block IO_CFG -name u_io_cfg
####################################
# bank description
####################################
set PAR_IO_NUM 2
set NUM_BANKS 2

####################################
# num of bits in each cfg block
####################################
set IO_CHAIN_GBOX_SIZE 42
set IO_CHAIN_GBOX_BANK_SIZE [expr $IO_CHAIN_GBOX_SIZE*$PAR_IO_NUM]
set IO_CHAIN_GBOX_HV_PGEN_SIZE 2
set IO_CHAIN_GBOX_VCO_FAST_SIZE 12      ;# FCLK_MUX
set IO_CHAIN_GBOX_ROOT_BANK_MUX_SIZE 20

set IO_CHAIN_GBOX_HV_START 0
set IO_CHAIN_GBOX_HV_PGEN_START  [expr $IO_CHAIN_GBOX_HV_START + $NUM_BANKS * $IO_CHAIN_GBOX_BANK_SIZE]
set IO_CHAIN_GBOX_VCO_FAST_START [expr $IO_CHAIN_GBOX_HV_PGEN_START + $IO_CHAIN_GBOX_HV_PGEN_SIZE]
set IO_CHAIN_GBOX_ROOT_BANK_MUX_START [expr $IO_CHAIN_GBOX_VCO_FAST_START + $IO_CHAIN_GBOX_VCO_FAST_SIZE]

set IO_CHAIN_GBOX_HV_40X2_VL_SIZE [expr $IO_CHAIN_GBOX_ROOT_BANK_MUX_START + $NUM_BANKS * $IO_CHAIN_GBOX_ROOT_BANK_MUX_SIZE]
# puts "IO_CHAIN_GBOX_HV_40X2_VL_SIZE = $IO_CHAIN_GBOX_HV_40X2_VL_SIZE"

#
# Start at the MSB
#
###############################################################################
# ROOT_BANK_CLKMUX
###############################################################################
create_instance -block ROOT_BANK_CLKMUX -name u_gbox_root_bank_clkmux_hv_1 -logic_address [expr ${IO_CHAIN_GBOX_ROOT_BANK_MUX_START} + 20] -parent GBOX_HV_40X2_VL_10X8
create_instance -block ROOT_BANK_CLKMUX -name u_gbox_root_bank_clkmux_hv_0 -logic_address [expr ${IO_CHAIN_GBOX_ROOT_BANK_MUX_START}  ] -parent GBOX_HV_40X2_VL_10X8
###############################################################################
# VCO_FAST (FCLK_MUX)
###############################################################################
create_instance -block FCLK_MUX -name u_gbox_fclk_mux_hv_all -logic_address [expr ${IO_CHAIN_GBOX_VCO_FAST_START}] -parent GBOX_HV_40X2_VL_10X8
###############################################################################
# HV_PGEN
###############################################################################
create_instance -block HV_PGEN -name u_HV_PGEN_dummy -logic_address [expr ${IO_CHAIN_GBOX_HV_PGEN_START}] -parent GBOX_HV_40X2_VL_10X8

#
#    Note:  RTL code indicates A is at the higher address
#

###############################################################################
# BANK 1 (top)
###############################################################################
set tile_indices {5 4}
for {set IO_INDEX 0} {$IO_INDEX < [expr { $PAR_IO_NUM / 2}]} {incr IO_INDEX} { 
    set tile_y [lindex $tile_indices $IO_INDEX]
    set instance_index [expr $PAR_IO_NUM / 2 - $IO_INDEX -1]
    create_instance -block GBOX_TOP -name u_HV_GBOX_BK1_A_${instance_index} \
	-logic_location [list 0 $tile_y 0] \
	-logic_address [expr $IO_CHAIN_GBOX_HV_START + $PAR_IO_NUM * 42 + $IO_INDEX * 42 * 2 + 42] \
	-parent GBOX_HV_40X2_VL_10X8

    create_instance -block GBOX_TOP -name u_HV_GBOX_BK1_B_${instance_index} \
	-logic_location [list 0 $tile_y 1] \
	-logic_address [expr $IO_CHAIN_GBOX_HV_START + $PAR_IO_NUM * 42 + $IO_INDEX * 42 * 2 + 0] \
	-parent GBOX_HV_40X2_VL_10X8

    # puts "create_instance -block GBOX_TOP -name u_HV_GBOX_BK1_A_${instance_index} \
    # 	-logic_location [list 0 $tile_y 0] \
    # 	-logic_address [expr $IO_CHAIN_GBOX_HV_START + $PAR_IO_NUM * 42 + $IO_INDEX * 42 * 2 + 42] \
    # 	-parent GBOX_HV_40X2_VL_10X8"

    # puts "create_instance -block GBOX_TOP -name u_HV_GBOX_BK1_B_${instance_index} \
    # 	-logic_location [list 0 $tile_y 1] \
    # 	-logic_address [expr $IO_CHAIN_GBOX_HV_START + $PAR_IO_NUM * 42 + $IO_INDEX * 42 * 2 + 0] \
    # 	-parent GBOX_HV_40X2_VL_10X8"

}

###############################################################################
# BANK 0 (bottom)
###############################################################################
set tile_indices {3 2}
for {set IO_INDEX 0} {$IO_INDEX < [expr { $PAR_IO_NUM / 2}]} {incr IO_INDEX} { 
    set tile_y [lindex $tile_indices $IO_INDEX]
    set instance_index [expr $PAR_IO_NUM / 2 - $IO_INDEX - 1]
    create_instance -block GBOX_TOP -name u_HV_GBOX_BK0_A_${instance_index} \
	-logic_location [list 0 $tile_y 0] \
	-logic_address [expr $IO_CHAIN_GBOX_HV_START + $IO_INDEX * 42 * 2 +  42] \
	-parent GBOX_HV_40X2_VL_10X8
    create_instance -block GBOX_TOP -name u_HV_GBOX_BK0_B_${instance_index} \
	-logic_location [list 0 $tile_y 1] \
	-logic_address [expr $IO_CHAIN_GBOX_HV_START + $IO_INDEX * 42 * 2 + 0] \
	-parent GBOX_HV_40X2_VL_10X8
    # puts "create_instance -block GBOX_TOP -name u_HV_GBOX_BK0_A_${instance_index} \
    # 	-logic_location [list 0 $tile_y 0] \
    # 	-logic_address [expr $IO_CHAIN_GBOX_HV_START + $IO_INDEX * 42 * 2 +  42] \
    # 	-parent GBOX_HV_40X2_VL_10X8"
    # puts "create_instance -block GBOX_TOP -name u_HV_GBOX_BK0_B_${instance_index} \
    # 	-logic_location [list 0 $tile_y 1] \
    # 	-logic_address [expr $IO_CHAIN_GBOX_HV_START + $IO_INDEX * 42 * 2 + 0] \
    # 	-parent GBOX_HV_40X2_VL_10X8"

}


###############################################################################
# Ports
###############################################################################
# Note:  Ports are not correct
define_ports -block GBOX_HV_40X2_VL_10X8 -in system_reset_n 
define_ports -block GBOX_HV_40X2_VL_10X8 -in cfg_done 
##
define_ports -block GBOX_HV_40X2_VL_10X8 -out hv_0_root_core_clk_bit1 hv_0_root_core_clk_bit0 
define_ports -block GBOX_HV_40X2_VL_10X8 -out hv_1_root_core_clk_bit1 hv_1_root_core_clk_bit0 
define_ports -block GBOX_HV_40X2_VL_10X8 -out hv_0_root_cdr_clk_bit1 hv_0_root_cdr_clk_bit0 
define_ports -block GBOX_HV_40X2_VL_10X8 -out hv_1_root_cdr_clk_bit1 hv_1_root_cdr_clk_bit0 
#####
define_ports -block GBOX_HV_40X2_VL_10X8 -out hv_rx_io_clk0_bit1 hv_rx_io_clk0_bit0 
define_ports -block GBOX_HV_40X2_VL_10X8 -out hv_rx_io_clk1_bit1 hv_rx_io_clk1_bit0 
#####
for {set tt 2} {$tt <= 8} {incr tt} { 
  for {set run_idx 0} {$run_idx <= 48} {incr run_idx} { 
    define_ports -block GBOX_HV_40X2_VL_10X8 -in left_${tt}_f2a_bit${run_idx}
 } 
} 
#####
for {set tt 2} {$tt <= 8} {incr tt} { 
  for {set run_idx 0} {$run_idx <= 23} {incr run_idx} { 
    define_ports -block GBOX_HV_40X2_VL_10X8 -out left_${tt}_a2f_bit${run_idx}
 } 
} 
#####
###############################################################################
