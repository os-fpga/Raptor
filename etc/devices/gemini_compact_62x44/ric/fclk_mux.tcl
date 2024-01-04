#!/usr/bin/env tclsh
####################################
# created by Alex H , April 2, 2023
# FCLK_MUX definition 
####################################
define_block -name FCLK_MUX

####################################
# define configuration attributes (parameters) 
####################################
define_attr -block FCLK_MUX -name cfg_hp_rxclk_phase_sel_B_0 -addr 0  -width 1 -enumname FCLK_MUX_ATT_0  -enum {rxclk_phase_sel_B_0_A0 0} {rxclk_phase_sel_B_0_A1 1}
define_attr -block FCLK_MUX -name cfg_hp_rxclk_phase_sel_B_1 -addr 1  -width 1 -enumname FCLK_MUX_ATT_1  -enum {rxclk_phase_sel_B_1_A0 0} {rxclk_phase_sel_B_1_A1 1}
define_attr -block FCLK_MUX -name cfg_hp_rxclk_phase_sel_A_0 -addr 2  -width 1 -enumname FCLK_MUX_ATT_2  -enum {rxclk_phase_sel_A_0_A0 0} {rxclk_phase_sel_A_0_A1 1}
define_attr -block FCLK_MUX -name cfg_hp_rxclk_phase_sel_A_1 -addr 3  -width 1 -enumname FCLK_MUX_ATT_3  -enum {rxclk_phase_sel_A_1_A0 0} {rxclk_phase_sel_A_1_A1 1}
##
define_attr -block FCLK_MUX -name cfg_hp_rx_fclkio_sel_B_0   -addr 4  -width 1 -enumname FCLK_MUX_ATT_4  -enum {rx_fclkio_sel_B_0_A0 0} {rx_fclkio_sel_B_0_A1 1}
define_attr -block FCLK_MUX -name cfg_hp_rx_fclkio_sel_B_1   -addr 5  -width 1 -enumname FCLK_MUX_ATT_5  -enum {rx_fclkio_sel_B_1_A0 0} {rx_fclkio_sel_B_1_A1 1}
define_attr -block FCLK_MUX -name cfg_hp_rx_fclkio_sel_A_0   -addr 6  -width 1 -enumname FCLK_MUX_ATT_6  -enum {rx_fclkio_sel_A_0_A0 0} {rx_fclkio_sel_A_0_A1 1}
define_attr -block FCLK_MUX -name cfg_hp_rx_fclkio_sel_A_1   -addr 7  -width 1 -enumname FCLK_MUX_ATT_7  -enum {rx_fclkio_sel_A_1_A0 0} {rx_fclkio_sel_A_1_A1 1}
##
define_attr -block FCLK_MUX -name cfg_hp_vco_clk_sel_B_0     -addr 8  -width 1 -enumname FCLK_MUX_ATT_8  -enum {vco_clk_sel_B_0_A0 0} {vco_clk_sel_B_0_A1 1}
define_attr -block FCLK_MUX -name cfg_hp_vco_clk_sel_B_1     -addr 9  -width 1 -enumname FCLK_MUX_ATT_9  -enum {vco_clk_sel_B_1_A0 0} {vco_clk_sel_B_1_A1 1}
define_attr -block FCLK_MUX -name cfg_hp_vco_clk_sel_A_0     -addr 10 -width 1 -enumname FCLK_MUX_ATT_10 -enum {vco_clk_sel_A_0_A0 0} {vco_clk_sel_A_0_A1 1}
define_attr -block FCLK_MUX -name cfg_hp_vco_clk_sel_A_1     -addr 11 -width 1 -enumname FCLK_MUX_ATT_11 -enum {vco_clk_sel_A_1_A0 0} {vco_clk_sel_A_1_A1 1}
##
####################################
# Constraints within block attributes 
####################################
#################################################################
# Ports
#################################################################

# Note:  These are not correct
define_ports -block FCLK_MUX -in vco_clk_0 
define_ports -block FCLK_MUX -in vco_clk_1 
define_ports -block FCLK_MUX -in rx_io_clk_0_0 
define_ports -block FCLK_MUX -in rx_io_clk_0_1 
define_ports -block FCLK_MUX -in rx_io_clk_1_0 
define_ports -block FCLK_MUX -in rx_io_clk_1_1 
define_ports -block FCLK_MUX -in rx_io_clk_2_0 
define_ports -block FCLK_MUX -in rx_io_clk_2_1 
######################################
define_ports -block FCLK_MUX -out fast_phase_clk_0 
define_ports -block FCLK_MUX -out fast_phase_clk_1 
define_ports -block FCLK_MUX -out fast_phase_clk_2 
define_ports -block FCLK_MUX -out fast_phase_clk_3 
######################################
