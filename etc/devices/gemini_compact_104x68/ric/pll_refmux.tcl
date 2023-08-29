#!/usr/bin/env tclsh
####################################
# created by Alex H , April 4, 2023
# PLLREF_MUX definition 
####################################
define_block -name PLLREF_MUX

####################################
# define configuration attributes (parameters) 
####################################
define_attr -block PLLREF_MUX -name cfg_pllref_hv_rx_io_sel	       -addr  0 -width 1 -enum {pllref_hv_rx_io_sel_0 0} {pllref_hv_rx_io_sel_1 1} -enumname PLLREF_MUX_ATT_0
define_param -block PLLREF_MUX -name cfg_pllref_hv_bank_rx_io_sel      -addr  1 -width 2 -type integer
define_attr -block PLLREF_MUX -name cfg_pllref_hp_rx_io_sel	       -addr  3 -width 2 -enum {pllref_hp_rx_io_sel_0 0} {pllref_hp_rx_io_sel_1 1} -enumname PLLREF_MUX_ATT_2
define_attr -block PLLREF_MUX -name cfg_pllref_hp_bank_rx_io_sel	-addr  5 -width 1 -enum {pllref_hp_bank_rx_io_sel_0 0} {pllref_hp_bank_rx_io_sel_1 1} -enumname PLLREF_MUX_ATT_3
define_attr -block PLLREF_MUX -name cfg_pllref_use_hv				-addr  6 -width 1 -enum {pllref_use_hv_0 0} {pllref_use_hv_1 1} -enumname PLLREF_MUX_ATT_4
define_attr -block PLLREF_MUX -name cfg_pllref_use_rosc				-addr  7 -width 1 -enum {pllref_use_rosc_0 0} {pllref_use_rosc_1 1} -enumname PLLREF_MUX_ATT_5
define_attr -block PLLREF_MUX -name cfg_pllref_use_div				-addr  8 -width 1 -enum {pllref_use_div_0 0} {pllref_use_div_1 1} -enumname PLLREF_MUX_ATT_6
##
####################################
# Constraints within block attributes 
####################################
define_constraint -block PLLREF_MUX -constraint {(cfg_pllref_hv_bank_rx_io_sel > 2) -> FALSE }
######################################
define_ports -block PLLREF_MUX -in system_reset_n 
define_ports -block PLLREF_MUX -in cfg_done 
define_ports -block PLLREF_MUX -in rosc_clk 
define_ports -block PLLREF_MUX -in hp_rx_io_clk_0 
define_ports -block PLLREF_MUX -in hp_rx_io_clk_1 
define_ports -block PLLREF_MUX -in hv_rx_io_clk_0 
define_ports -block PLLREF_MUX -in hv_rx_io_clk_1 
define_ports -block PLLREF_MUX -in hv_rx_io_clk_2 
######################################
define_ports -block PLLREF_MUX -out pll_refmux_out 
######################################
