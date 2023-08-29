#!/usr/bin/env tclsh
####################################
# created by Alex H , June 26, 2023
# ROOT_MUX definition 
####################################
define_block -name ROOT_MUX

####################################
# define configuration attributes (parameters) 
####################################
#define_param -block ROOT_MUX -name ROOT_MUX_SEL	-addr  66 -width 6 -type integer
define_param -block ROOT_MUX -name ROOT_MUX_SEL	-addr  0 -width 6 -type integer

###define_attr -block ROOT_MUX -name cfg_hp_vco_clk_sel_A_2 -addr  17 -width 1 -enum {vco_clk_sel_A_2 0} {vco_clk_sel_A_2 1} -enumname ROOT_MUX_ATT_0
##
####################################
# Constraints within block attributes 
####################################
##### 34x1 mux
######################################
define_ports -block ROOT_MUX -out root_mux_clk_out
######################################
for {set CLK_INDEX 0} {$CLK_INDEX <= 35} {incr CLK_INDEX} { 
###	puts root_clk_in_$CLK_INDEX 
    define_ports -block ROOT_MUX -in root_clk_in_bit${CLK_INDEX}
} 
#####
######################################
