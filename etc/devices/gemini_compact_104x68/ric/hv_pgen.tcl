#!/usr/bin/env tclsh
####################################
# created by Alex H , June 27, 2023
# HV_PGEN block definition 
####################################
define_block -name HV_PGEN

####################################
# define configuration attributes (parameters) 
####################################
define_attr  -block HV_PGEN -name hv_cfg_EN_BK2 	-addr 2 -width 1 -enum {hhv_cfg_EN_BK2_OFF 0} {hv_cfg_EN_BK2_ON 1} -enumname HV_PGEN_2
define_attr  -block HV_PGEN -name hv_cfg_EN_BK1 	-addr 1 -width 1 -enum {hhv_cfg_EN_BK1_OFF 0} {hv_cfg_EN_BK1_ON 1} -enumname HV_PGEN_1
define_attr  -block HV_PGEN -name hv_cfg_EN_BK0 	-addr 0 -width 1 -enum {hhv_cfg_EN_BK0_OFF 0} {hv_cfg_EN_BK0_ON 1} -enumname HV_PGEN_0
####################################
#### define_constraint -block RC_OSC_50MHZ -constraint {(DFEN == SingleEnded) 			-> {RATE inside {[Three:Ten]}} }
######################################
