####################################
# Gearbox block definition 
####################################
define_block -name GBOX_HP_40X2_10X8 

####################################
# all IO chain description
####################################
####create_instance -block IO_CFG -name u_io_cfg
####################################
# bank description
####################################
set PAR_IO_NUM 2
set NUM_BANKS 2
set NUM_ROOT_MUXES 16
####################################
#### set IO_CHAIN_GBOX_START_VL [expr { 78 + 3 + 40 * 42 *3 } ]

#
# sizes
#
set IO_CHAIN_GBOX_SIZE 42
set IO_CHAIN_GBOX_BANK_SIZE [expr $IO_CHAIN_GBOX_SIZE * $PAR_IO_NUM]
set IO_CHAIN_GBOX_HP_PGEN_SIZE 6
set IO_CHAIN_GBOX_ROOT_BANK_MUX_SIZE 20
set IO_CHAIN_GBOX_VCO_FAST_SIZE 12 ;# FCLK_MUX
set IO_CHAIN_GBOX_OSC_SIZE 15
set IO_CHAIN_GBOX_PLLREF_MUX_SIZE 9
set IO_CHAIN_GBOX_PLL_SIZE 69
set IO_CHAIN_GBOX_ROOT_MUX_SIZE 6

# Start locations
set IO_CHAIN_GBOX_HP_START 0
set IO_CHAIN_GBOX_HP_PGEN_START [expr $IO_CHAIN_GBOX_HP_START + $NUM_BANKS * $IO_CHAIN_GBOX_BANK_SIZE]
set IO_CHAIN_GBOX_VCO_FAST_START [expr $IO_CHAIN_GBOX_HP_PGEN_START + $IO_CHAIN_GBOX_HP_PGEN_SIZE]
set IO_CHAIN_GBOX_ROOT_BANK_MUX_START [expr $IO_CHAIN_GBOX_VCO_FAST_START + $IO_CHAIN_GBOX_VCO_FAST_SIZE]
set IO_CHAIN_GBOX_ROOT_MUX_START [expr $IO_CHAIN_GBOX_ROOT_BANK_MUX_START + $NUM_BANKS * $IO_CHAIN_GBOX_ROOT_BANK_MUX_SIZE]
set IO_CHAIN_GBOX_OSC_START  [expr $IO_CHAIN_GBOX_ROOT_MUX_START + $NUM_ROOT_MUXES * $IO_CHAIN_GBOX_ROOT_MUX_SIZE]
set IO_CHAIN_GBOX_PLL0_START [expr $IO_CHAIN_GBOX_OSC_START + $IO_CHAIN_GBOX_OSC_SIZE]
set IO_CHAIN_GBOX_PLLREF_MUX0_START [expr $IO_CHAIN_GBOX_PLL0_START + $IO_CHAIN_GBOX_PLL_SIZE]
set IO_CHAIN_GBOX_PLL1_START [expr $IO_CHAIN_GBOX_PLLREF_MUX0_START + $IO_CHAIN_GBOX_PLLREF_MUX_SIZE]
set IO_CHAIN_GBOX_PLLREF_MUX1_START [expr $IO_CHAIN_GBOX_PLL1_START + $IO_CHAIN_GBOX_PLL_SIZE]
set IO_CHAIN_GBOX_HP_40X2_SIZE [expr $IO_CHAIN_GBOX_PLLREF_MUX1_START + $IO_CHAIN_GBOX_PLLREF_MUX_SIZE]

puts "IO_CHAIN_GBOX_HP_40X2_SIZE = $IO_CHAIN_GBOX_HP_40X2_SIZE"

#set IO_CHAIN_GBOX_PLL_START 0
#set IO_CHAIN_GBOX_OSC_START 416
#set IO_CHAIN_GBOX_VCO_FAST_START 		[expr $IO_CHAIN_GBOX_OSC_START + 15 ]
#set IO_CHAIN_GBOX_ROOT_BANK_MUX_START    	[expr $IO_CHAIN_GBOX_VCO_FAST_START + 18 ]
#set IO_CHAIN_GBOX_ROOT_MUX_START 		[expr $IO_CHAIN_GBOX_ROOT_BANK_MUX_START + 60 ]
#set IO_CHAIN_GBOX_HP_PGEN_START  		[expr $IO_CHAIN_GBOX_VCO_FAST_START + 158 ]
#set IO_CHAIN_GBOX_HP_START 		 	[expr $IO_CHAIN_GBOX_HP_PGEN_START + 9 ]

###set IO_CHAIN_GBOX_HP_END   [expr { 78 + 3 + 40 * 42 *3 + 416 + 15 + 158 + 9 + 40 * 42 *3 } ]
###set IO_CHAIN_GBOX_VL_START [expr { 78 + 3 } ]
###set IO_CHAIN_GBOX_VL_END   [expr { 78 + 3 + 40 * 42 *3 } ]
###set IO_CHAIN_GBOX_VR_START [expr { 78 + 3 + 40 * 42 *3 + 416 + 15 + 158 + 9 + 40 * 42 *3 + 78 + 3 } ]
###set IO_CHAIN_GBOX_VR_END   [expr { 78 + 3 + 40 * 42 *3 + 416 + 15 + 158 + 9 + 40 * 42 *3 + 78 + 3 + 40 * 42 *3 } ]
#### create_instance -block <type> -name <name> -id <id> -logic_location {x y} -logic_address <logical_address> -io_bank <io_bank_name_optional> 
###############################################################################
# 4 sets of PLLREF_MUX/PLL 
###############################################################################
create_instance -block PLLREF_MUX -name u_gbox_pll_refmux_1      -logic_address $IO_CHAIN_GBOX_PLLREF_MUX1_START -parent GBOX_HP_40X2_10X8
create_instance -block PLL        -name u_gbox_PLLTS16FFCFRACF_1 -logic_address $IO_CHAIN_GBOX_PLL1_START        -parent GBOX_HP_40X2_10X8 -location [list 62 0 2]
create_instance -block PLLREF_MUX -name u_gbox_pll_refmux_0      -logic_address $IO_CHAIN_GBOX_PLLREF_MUX0_START -parent GBOX_HP_40X2_10X8
create_instance -block PLL        -name u_gbox_PLLTS16FFCFRACF_0 -logic_address $IO_CHAIN_GBOX_PLL0_START        -parent GBOX_HP_40X2_10X8 -location [list 1 0 2]
###############################################################################
# RC_OSC_50MHZ
###############################################################################
create_instance -block RC_OSC_50MHZ -name u_bank_osc -logic_address $IO_CHAIN_GBOX_OSC_START -parent GBOX_HP_40X2_10X8

###############################################################################
# ROOT MUXES (12 bits total)
################################################################
for {set run_idx 0} {$run_idx <= 15} {incr run_idx} { 
    create_instance -block ROOT_MUX -name u_gbox_clkmux_52x1_left_${run_idx} -logic_address [expr ${IO_CHAIN_GBOX_ROOT_MUX_START} + ${IO_CHAIN_GBOX_ROOT_MUX_SIZE}*${run_idx}] -parent GBOX_HP_40X2_10X8
}

###############################################################################
# ROOT BANK CLKMUXes
###############################################################################
create_instance -block ROOT_BANK_CLKMUX -name u_gbox_root_bank_clkmux_hp_1 -logic_address [expr ${IO_CHAIN_GBOX_ROOT_BANK_MUX_START} + 20] -parent GBOX_HP_40X2_10X8
create_instance -block ROOT_BANK_CLKMUX -name u_gbox_root_bank_clkmux_hp_0 -logic_address [expr ${IO_CHAIN_GBOX_ROOT_BANK_MUX_START}  ] -parent GBOX_HP_40X2_10X8
###############################################################################
# VCO_FAST FCLK
###############################################################################
create_instance -block FCLK_MUX -name u_gbox_fclk_mux_hp_all -logic_address $IO_CHAIN_GBOX_VCO_FAST_START -parent GBOX_HP_40X2_10X8
###############################################################################
# HP_PGEN
###############################################################################
create_instance -block HP_PGEN -name u_HP_PGEN_dummy -logic_address $IO_CHAIN_GBOX_HP_PGEN_START -parent GBOX_HP_40X2_10X8

###############################################################################
# 3 BANKS
###############################################################################
# bottom left
set tile_indices {3 2}
for {set IO_INDEX 0} {$IO_INDEX < [expr { $PAR_IO_NUM / 2}]} {incr IO_INDEX} { 
    set tile_x [lindex $tile_indices $IO_INDEX]
    set instance_index [expr $PAR_IO_NUM / 2 - $IO_INDEX - 1]
    create_instance -block GBOX_TOP -name u_HP_GBOX_BK0_A_${instance_index} \
	-logic_location [list $tile_x 0 0] \
	-logic_address [expr $IO_CHAIN_GBOX_HP_START + $PAR_IO_NUM * 42 + $IO_INDEX * 42 * 2 + 42] \
	-parent GBOX_HP_40X2_10X8
    create_instance -block GBOX_TOP -name u_HP_GBOX_BK0_B_${instance_index} \
	-logic_location [list $tile_x 0 1] \
	-logic_address [expr $IO_CHAIN_GBOX_HP_START + $PAR_IO_NUM * 42 + $IO_INDEX * 42 * 2 +  0] \
	-parent GBOX_HP_40X2_10X8

    # puts "create_instance -block GBOX_TOP -name u_HP_GBOX_BK0_A_${instance_index} \
    # 	-logic_location [list $tile_x 0 0] \
    # 	-logic_address [expr $IO_CHAIN_GBOX_HP_START + $PAR_IO_NUM * 42 + $IO_INDEX * 42 * 2 + 42] \
    # 	-parent GBOX_HP_40X2_10X8"
    # puts "create_instance -block GBOX_TOP -name u_HP_GBOX_BK0_B_${instance_index} \
    # 	-logic_location [list $tile_x 0 1] \
    # 	-logic_address [expr $IO_CHAIN_GBOX_HP_START + $PAR_IO_NUM * 42 + $IO_INDEX * 42 * 2 +  0] \
    # 	-parent GBOX_HP_40X2_10X8"

}

# bottom right
set tile_indices {5 6}
for {set IO_INDEX 0} {$IO_INDEX < [expr { $PAR_IO_NUM / 2}]} {incr IO_INDEX} { 
    set tile_x [lindex $tile_indices $IO_INDEX]
    set instance_index [expr $PAR_IO_NUM / 2 - $IO_INDEX - 1]
    create_instance -block GBOX_TOP -name u_HP_GBOX_BK1_A_${instance_index} \
	-logic_location [list $tile_x 0 0] \
	-logic_address [expr $IO_CHAIN_GBOX_HP_START + $IO_INDEX * 42 * 2 + 42] \
	-parent GBOX_HP_40X2_10X8
    create_instance -block GBOX_TOP -name u_HP_GBOX_BK1_B_${instance_index} \
	-logic_location [list $tile_x 0 1] \
	-logic_address [expr $IO_CHAIN_GBOX_HP_START + $IO_INDEX * 42 * 2 +  0] \
	-parent GBOX_HP_40X2_10X8
    # puts "create_instance -block GBOX_TOP -name u_HP_GBOX_BK1_A_${instance_index} \
    # 	-logic_location [list $tile_x 0 0] \
    # 	-logic_address [expr $IO_CHAIN_GBOX_HP_START + $IO_INDEX * 42 * 2 + 42] \
    # 	-parent GBOX_HP_40X2_10X8"
    # puts "create_instance -block GBOX_TOP -name u_HP_GBOX_BK1_B_${instance_index} \
    # 	-logic_location [list $tile_x 0 1] \
    # 	-logic_address [expr $IO_CHAIN_GBOX_HP_START + $IO_INDEX * 42 * 2 +  0] \
    # 	-parent GBOX_HP_40X2_10X8"
}

###############################################################################
# Ports
###############################################################################
# Note: Ports are not correct
define_ports -block GBOX_HP_40X2_10X8 -in system_reset_n 
define_ports -block GBOX_HP_40X2_10X8 -in cfg_done 
define_ports -block GBOX_HP_40X2_10X8 -in hv_vl_0_rx_io_clk_bit1 hv_vl_0_rx_io_clk_bit0 
define_ports -block GBOX_HP_40X2_10X8 -in hv_vl_1_rx_io_clk_bit1 hv_vl_1_rx_io_clk_bit0 
##
define_ports -block GBOX_HP_40X2_10X8 -in hv_vl_0_root_core_clk_bit1 hv_vl_0_root_core_clk_bit0 
define_ports -block GBOX_HP_40X2_10X8 -in hv_vl_1_root_core_clk_bit1 hv_vl_1_root_core_clk_bit0 
define_ports -block GBOX_HP_40X2_10X8 -in hv_vr_0_root_core_clk_bit1 hv_vr_0_root_core_clk_bit0 
define_ports -block GBOX_HP_40X2_10X8 -in hv_vr_1_root_core_clk_bit1 hv_vr_1_root_core_clk_bit0 
define_ports -block GBOX_HP_40X2_10X8 -in hv_vl_0_root_cdr_clk_bit1 hv_vl_0_root_cdr_clk_bit0 
define_ports -block GBOX_HP_40X2_10X8 -in hv_vl_1_root_cdr_clk_bit1 hv_vl_1_root_cdr_clk_bit0 
define_ports -block GBOX_HP_40X2_10X8 -in hv_vr_0_root_cdr_clk_bit1 hv_vr_0_root_cdr_clk_bit0 
define_ports -block GBOX_HP_40X2_10X8 -in hv_vr_1_root_cdr_clk_bit1 hv_vr_1_root_cdr_clk_bit0 
#####
set tile {2 3 5 6}
foreach tt $tile {
  for {set run_idx 0} {$run_idx <= 48} {incr run_idx} { 
    define_ports -block GBOX_HP_40X2_10X8 -in bottom_${tt}_f2a_bit${run_idx} 
    ### puts "bottom_${tt}_f2a_bit${run_idx} ABCD"
  } 
} 
#####
foreach tt $tile {
  for {set run_idx 0} {$run_idx <= 23} {incr run_idx} { 
    define_ports -block GBOX_HP_40X2_10X8 -out bottom_${tt}_a2f_bit${run_idx} 
  } 
} 

#####
set tile {4 11 12 18 19 24 26 31 36 43 54 69 72 74 81 88 93 97 102 103 7 21 42 50 55 63 84 98 14 28 38 46 59 67 77 91}
foreach tt $tile {
  for {set run_idx 0} {$run_idx <= 23} {incr run_idx} { 
      define_ports -block GBOX_HP_40X2_10X8 -out bottom_real_${tt}_a2f_bit${run_idx} 
  } 
}

#####
define_ports -block GBOX_HP_40X2_10X8 -out pll_LOCK_bit3 pll_LOCK_bit2 pll_LOCK_bit1 pll_LOCK_bit0 
define_ports -block GBOX_HP_40X2_10X8 -out pll_FOUT_0_bit3 pll_FOUT_0_bit2 pll_FOUT_0_bit1 pll_FOUT_0_bit0 
define_ports -block GBOX_HP_40X2_10X8 -out pll_FOUT_1_bit3 pll_FOUT_1_bit2 pll_FOUT_1_bit1 pll_FOUT_1_bit0 
define_ports -block GBOX_HP_40X2_10X8 -out pll_FOUT_2_bit3 pll_FOUT_2_bit2 pll_FOUT_2_bit1 pll_FOUT_2_bit0 
define_ports -block GBOX_HP_40X2_10X8 -out pll_FOUT_3_bit3 pll_FOUT_3_bit2 pll_FOUT_3_bit1 pll_FOUT_3_bit0 
define_ports -block GBOX_HP_40X2_10X8 -out pll_FOUTVCO_bit3 pll_FOUTVCO_bit2 pll_FOUTVCO_bit1 pll_FOUTVCO_bit0 
define_ports -block GBOX_HP_40X2_10X8 -out root_mux_left_clk_out_bit7 root_mux_left_clk_out_bit6 root_mux_left_clk_out_bit5 root_mux_left_clk_out_bit4 root_mux_left_clk_out_bit3 root_mux_left_clk_out_bit2 root_mux_left_clk_out_bit1 root_mux_left_clk_out_bit0 
define_ports -block GBOX_HP_40X2_10X8 -out root_mux_right_clk_out_bit7 root_mux_right_clk_out_bit6 root_mux_right_clk_out_bit5 root_mux_right_clk_out_bit4 root_mux_right_clk_out_bit3 root_mux_right_clk_out_bit2 root_mux_right_clk_out_bit1 root_mux_right_clk_out_bit0 
#####
