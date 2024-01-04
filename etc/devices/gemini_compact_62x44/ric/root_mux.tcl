#!/usr/bin/env tclsh
####################################
# created by Alex H , June 26, 2023
# updated by George C, Oct 2023
# ROOT_MUX definition 
####################################
define_block -name ROOT_MUX

####################################
# define configuration attributes (parameters) 
####################################
# default is to ground the output (select 0 as input)
define_attr -block ROOT_MUX -name ROOT_MUX_SEL -addr 0 -width 6 -default 63

####################################
# Constraints within block attributes 
####################################

# 52x1 mux
######################################
define_ports -block ROOT_MUX -out root_mux_clk_out
for {set CLK_INDEX 0} {$CLK_INDEX <= 51} {incr CLK_INDEX} { 
  define_ports -block ROOT_MUX -in root_clk_in_bit${CLK_INDEX}
} 
######################################
