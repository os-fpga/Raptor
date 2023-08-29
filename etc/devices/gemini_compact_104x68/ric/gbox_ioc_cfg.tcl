#!/usr/bin/env tclsh
#
# This file is not used as we create instances with addresses that would have been generated through the chains below.
#
####################################
# created by Alex H , July 20 , 2023
####################################
define_block -name GBOX_IOC_CFG
####################################
set IO_CHAIN_GBOX_VR_END   [expr { 78 + 3 + 40 * 42 *3 + 416 + 15 + 158 + 9 + 40 * 42 *3 + 78 + 3 + 40 * 42 *3 } ]
set HV_X3_VL_VCO_FAST_START 0
set HV_X3_VL_PGEN_START [expr $HV_X3_VL_VCO_FAST_START + 78] 
set HV_X3_VL_GBOX_START_0 [expr $HV_X3_VL_PGEN_START +3 ]
set HV_X3_VL_GBOX_START_1 [expr $HV_X3_VL_GBOX_START_0 + 42 ]
set HP_X3_PLL_START [expr $HV_X3_VL_GBOX_START_0 + 40 * 42 *3]
set HP_X3_OSC_START [expr $HP_X3_PLL_START + 416]
set HP_X3_VCO_FAST_START [expr $HP_X3_OSC_START + 15]
set HP_X3_PGEN_START [expr $HP_X3_VCO_FAST_START + 158]
set HP_X3_GBOX_START_0 [expr $HP_X3_PGEN_START + 9]
set HP_X3_GBOX_START_1 [expr $HP_X3_GBOX_START_0 + 42]
set HV_X3_VR_VCO_FAST_START [expr $HP_X3_GBOX_START_0 + 40 * 42 *3]
set HV_X3_VR_PGEN_START [expr $HV_X3_VR_VCO_FAST_START + 78] 
set HV_X3_VR_GBOX_START_0 [expr $HV_X3_VR_PGEN_START +3 ]
set HV_X3_VR_GBOX_START_1 [expr $HV_X3_VR_GBOX_START_0 + 42 ]
set PAR_IO_NUM 40
####################################
create_chain_instance -type ICB_CHAIN -name GBOX_IOC_CFG -start_address 0 -end_address [expr $IO_CHAIN_GBOX_VR_END - 1]
####################################
define_block -name HV_X3_VL_VCO_FAST_W78 
define_param -block HV_X3_VL_VCO_FAST_W78 	-name HV_X3_VL_VCO_FAST_CFG -addr 0 -width 78 -type integer
create_instance -block HV_X3_VL_VCO_FAST_W78 -name u_HV_X3_VL_VCO_FAST_W78 -parent GBOX_IOC_CFG
link_chain -inst u_HV_X3_VL_VCO_FAST_W78 -chain GBOX_IOC_CFG
###
define_block -name HV_X3_VL_PGEN_W3
define_param -block HV_X3_VL_PGEN_W3 		-name HV_X3_VL_PGEN_CFG 	-addr 0 -width 3 -type integer
create_instance -block HV_X3_VL_PGEN_W3 -name u_HV_X3_VL_PGEN_W3
link_chain -inst u_HV_X3_VL_PGEN_W3 -chain GBOX_IOC_CFG
######
for {set BANK_INDEX 2} {$BANK_INDEX >= 0 } {incr BANK_INDEX -1} { 
 for {set IO_INDEX 0} {$IO_INDEX < [expr $PAR_IO_NUM ]} {incr IO_INDEX} { 
define_block -name GBOX_HV_VL_BANK${BANK_INDEX}_${IO_INDEX}_W42
define_param -block GBOX_HV_VL_BANK${BANK_INDEX}_${IO_INDEX}_W42   -name GBOX_CFG -addr 0 -width 42 -type integer
create_instance -block GBOX_HV_VL_BANK${BANK_INDEX}_${IO_INDEX}_W42 -name GBOX_HV_VL_BANK${BANK_INDEX}_${IO_INDEX}_W42
link_chain -inst GBOX_HV_VL_BANK${BANK_INDEX}_${IO_INDEX}_W42 -chain GBOX_IOC_CFG
 }
}
###
define_block -name HP_X3_PLL_CFG_W416
define_param -block HP_X3_PLL_CFG_W416 		-name HP_X3_PLL_CFG__CFG 	-addr 0 -width 416 -type integer
create_instance -block HP_X3_PLL_CFG_W416 -name u_HP_X3_PLL_CFG_W416
link_chain -inst u_HP_X3_PLL_CFG_W416 -chain GBOX_IOC_CFG
###
define_block -name HP_X3_OSC_CFG_W15
define_param -block HP_X3_OSC_CFG_W15 		-name HP_X3_OSC_CFG__CFG 	-addr 0 -width 15 -type integer
create_instance -block HP_X3_OSC_CFG_W15 -name u_HP_X3_OSC_CFG_W15
link_chain -inst u_HP_X3_OSC_CFG_W15 -chain GBOX_IOC_CFG
###
define_block -name HP_X3_VCO_FAST_W158
define_param -block HP_X3_VCO_FAST_W158 	-name HHP_X3_VCO_FAST_CFG 	-addr 0 -width 158 -type integer
create_instance -block HP_X3_VCO_FAST_W158 -name u_HP_X3_VCO_FAST_W158
link_chain -inst u_HP_X3_VCO_FAST_W158 -chain GBOX_IOC_CFG
###
define_block -name HP_X3_PGEN_W9
define_param -block HP_X3_PGEN_W9 			-name HP_X3_PGEN_CFG 		-addr 0 -width 9 -type integer
######
for {set BANK_INDEX 0} {$BANK_INDEX < 3 } {incr BANK_INDEX} { 
 for {set IO_INDEX 0} {$IO_INDEX < [expr $PAR_IO_NUM ]} {incr IO_INDEX} { 
define_block -name GBOX_HP_BANK${BANK_INDEX}_${IO_INDEX}_W42
define_param -block GBOX_HP_BANK${BANK_INDEX}_${IO_INDEX}_W42   -name GBOX_CFG -addr 0 -width 42 -type integer
create_instance -block GBOX_HP_BANK${BANK_INDEX}_${IO_INDEX}_W42 -name GBOX_HP_BANK${BANK_INDEX}_${IO_INDEX}_W42
link_chain -inst GBOX_HP_BANK${BANK_INDEX}_${IO_INDEX}_W42 -chain GBOX_IOC_CFG
 }
}
###
define_block -name HV_X3_VR_VCO_FAST_W78
define_param -block HV_X3_VR_VCO_FAST_W78 	-name HV_X3_VR_VCO_FAST_CFG -addr 0 -width 78 -type integer
create_instance -block HV_X3_VR_VCO_FAST_W78 -name u_HV_X3_VR_VCO_FAST_W78
link_chain -inst u_HV_X3_VR_VCO_FAST_W78 -chain GBOX_IOC_CFG
###
define_block -name HV_X3_VR_PGEN_W3
define_param -block HV_X3_VR_PGEN_W3 		-name HV_X3_VR_PGEN_CFG 	-addr 0 -width 3 -type integer
create_instance -block HV_X3_VR_PGEN_W3 -name u_HV_X3_VR_PGEN_W3
link_chain -inst u_HV_X3_VR_PGEN_W3 -chain GBOX_IOC_CFG
######
for {set BANK_INDEX 2} {$BANK_INDEX >= 0 } {incr BANK_INDEX -1} { 
 for {set IO_INDEX 0} {$IO_INDEX < [expr $PAR_IO_NUM ]} {incr IO_INDEX} { 
define_block -name GBOX_HV_VR_BANK${BANK_INDEX}_${IO_INDEX}_W42
define_param -block GBOX_HV_VR_BANK${BANK_INDEX}_${IO_INDEX}_W42   -name GBOX_CFG -addr 0 -width 42 -type integer
create_instance -block GBOX_HV_VR_BANK${BANK_INDEX}_${IO_INDEX}_W42 -name GBOX_HV_VR_BANK${BANK_INDEX}_${IO_INDEX}_W42
link_chain -inst GBOX_HV_VR_BANK${BANK_INDEX}_${IO_INDEX}_W42 -chain GBOX_IOC_CFG
 }
}
####################################
