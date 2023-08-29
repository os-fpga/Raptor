#!/usr/bin/env tclsh
####################################
# created by Alex H , June 27, 2023
# RC_OSC_50MHZ block definition 
####################################
define_block -name RC_OSC_50MHZ

####################################
# define configuration attributes (parameters) 
####################################
define_param -block RC_OSC_50MHZ -name cfg_bank_osc_cal		-addr 9 -width 6 -type integer
define_param -block RC_OSC_50MHZ -name cfg_bank_osc_ib_cop	-addr 7 -width 2 -type integer
define_attr  -block RC_OSC_50MHZ -name cfg_bank_osc_pd		-addr 6 -width 1 -enum {cfg_bank_osc_pd_0 0} {cfg_bank_osc_pd_1 1} -enumname RC_OSC_50MHZ_ATT_2
define_param -block RC_OSC_50MHZ -name cfg_bank_osc_bgr		-addr 3 -width 3 -type integer
define_param -block RC_OSC_50MHZ -name cfg_bank_osc_rsv		-addr 0 -width 3 -type integer
####################################
#### define_constraint -block RC_OSC_50MHZ -constraint {(DFEN == SingleEnded) 			-> {RATE inside {[Three:Ten]}} }
######################################
define_ports -block RC_OSC_50MHZ -out bank_osc_clk
######################################
