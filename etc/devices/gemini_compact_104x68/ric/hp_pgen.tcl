#!/usr/bin/env tclsh
####################################
# created by Alex H , June 27, 2023
# HP_PGEN block definition 
####################################
define_block -name HP_PGEN

####################################
# define configuration attributes (parameters) 
####################################
define_attr  -block HP_PGEN -name hp_cfg_PGEN 		-addr 2 -width 1 -enum {hp_cfg_PGEN_0 0} {hp_cfg_PGEN_1 1} -enumname HP_PGEN_1
define_attr  -block HP_PGEN -name hp_cfg_EN 		-addr 1 -width 1 -enum {hp_cfg_EN_0 0} {hp_cfg_EN_1 1} -enumname HP_PGEN_2
define_attr  -block HP_PGEN -name hp_cfg_RCAL_MSTR 	-addr 0 -width 1 -enum {hp_cfg_RCAL_MSTR_0 0} {hp_cfg_RCAL_MSTR_1 1} -enumname HP_PGEN_3
####################################
#### define_constraint -block RC_OSC_50MHZ -constraint {(DFEN == SingleEnded) 			-> {RATE inside {[Three:Ten]}} }
######################################
