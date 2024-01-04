#!/usr/bin/env tclsh
####################################
# created by Alex H , June 26 2023
# ROOT_BANK_CLKMUX definition 
####################################
define_block -name ROOT_BANK_CLKMUX

####################################
# define configuration attributes (parameters) 
####################################
define_attr -block ROOT_BANK_CLKMUX -name CDR_CLK_ROOT_SEL_B	-addr 0  -width 5
define_attr -block ROOT_BANK_CLKMUX -name CDR_CLK_ROOT_SEL_A	-addr 5  -width 5
define_attr -block ROOT_BANK_CLKMUX -name CORE_CLK_ROOT_SEL_B	-addr 10 -width 5
define_attr -block ROOT_BANK_CLKMUX -name CORE_CLK_ROOT_SEL_A	-addr 15 -width 5

###define_attr -block ROOT_BANK_CLKMUX -name cfg_hp_vco_clk_sel_A_2 -addr 17 -width 1 -enumname ROOT_BANK_CLKMUX_ATT_0 -enum {vco_clk_sel_A_2 0} {vco_clk_sel_A_2 1}
##
####################################
# Constraints within block attributes 
####################################
# The value must not be greater than 19
define_constraint -block ROOT_BANK_CLKMUX -constraint {(CORE_CLK_ROOT_SEL_A >= 20) -> False} 
define_constraint -block ROOT_BANK_CLKMUX -constraint {(CORE_CLK_ROOT_SEL_B >= 20) -> False} 
define_constraint -block ROOT_BANK_CLKMUX -constraint {(CDR_CLK_ROOT_SEL_A  >= 20) -> False} 
define_constraint -block ROOT_BANK_CLKMUX -constraint {(CDR_CLK_ROOT_SEL_B  >= 20) -> False} 
######################################
#####
set PAR_IO_NUM 40
for {set IO_INDEX 0} {$IO_INDEX <= $PAR_IO_NUM} {incr IO_INDEX} { 
  define_ports -block ROOT_BANK_CLKMUX -in core_clk_in_bit${IO_INDEX} 
} 
for {set IO_INDEX 0} {$IO_INDEX <= $PAR_IO_NUM} {incr IO_INDEX} { 
  define_ports -block ROOT_BANK_CLKMUX -in cdr_clk_in_bit${IO_INDEX} 
} 
#####
######################################
define_ports -block ROOT_BANK_CLKMUX -out root_core_clk_0
define_ports -block ROOT_BANK_CLKMUX -out root_core_clk_1 
define_ports -block ROOT_BANK_CLKMUX -out root_cdr_clk_0 
define_ports -block ROOT_BANK_CLKMUX -out root_cdr_clk_1 
######################################
