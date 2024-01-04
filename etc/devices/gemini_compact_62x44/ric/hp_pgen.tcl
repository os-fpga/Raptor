#!/usr/bin/env tclsh
####################################
# created by Alex H , June 27, 2023
# HP_PGEN block definition 
####################################
define_block -name HP_PGEN

####################################
# define configuration attributes (parameters) 
####################################
define_attr  -block HP_PGEN -name hp_cfg_PGEN_1      -addr 5 -width 1 -enumname HP_PGEN_5 -enum {hp_cfg_PGEN_1_0 0} {hp_cfg_PGEN_1_1 1}
define_attr  -block HP_PGEN -name hp_cfg_PGEN_0      -addr 4 -width 1 -enumname HP_PGEN_4 -enum {hp_cfg_PGEN_0_0 0} {hp_cfg_PGEN_0_1 1}
define_attr  -block HP_PGEN -name hp_cfg_EN_1        -addr 3 -width 1 -enumname HP_PGEN_3 -enum {hp_cfg_EN_1_0 0} {hp_cfg_EN_1_1 1}
define_attr  -block HP_PGEN -name hp_cfg_EN_0        -addr 2 -width 1 -enumname HP_PGEN_2 -enum {hp_cfg_EN_0_0 0} {hp_cfg_EN_1_0 1}
define_attr  -block HP_PGEN -name hp_cfg_RCAL_MSTR_1 -addr 1 -width 1 -enumname HP_PGEN_1 -enum {hp_cfg_RCAL_MSTR_1_0 0} {hp_cfg_RCAL_MSTR_1_1 1}
define_attr  -block HP_PGEN -name hp_cfg_RCAL_MSTR_0 -addr 0 -width 1 -enumname HP_PGEN_0 -enum {hp_cfg_RCAL_MSTR_0_0 0} {hp_cfg_RCAL_MSTR_0_1 1}
####################################
#### define_constraint -block RC_OSC_50MHZ -constraint {(DFEN == SingleEnded) -> {RATE inside {[Three:Ten]}}}
######################################
