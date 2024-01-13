device_name VIRGOTC_BANK_10X8

#
# This file modified from the original virgotc_bank.tcl.
#
# The comments are still relative to that device
#

####################################
# Gearbox block definition 
####################################

set script_directory [file dirname [info script]]

source [file join $script_directory "gbox_top.tcl"]
source [file join $script_directory "fclk_mux.tcl"]
source [file join $script_directory "hp_pgen.tcl"]
source [file join $script_directory "hv_pgen.tcl"]
source [file join $script_directory "pll_refmux.tcl"]
source [file join $script_directory "pll.tcl"]
source [file join $script_directory "rc_osc_50mhz.tcl"]
source [file join $script_directory "root_bank_clkmux.tcl"]
source [file join $script_directory "root_mux.tcl"]
source [file join $script_directory "gbox_hp_40x2_10x8.tcl"]
source [file join $script_directory "gbox_hv_40x2_vl_10x8.tcl"]
source [file join $script_directory "gbox_hv_40x2_vr_10x8.tcl"]

####################################
define_block -name VIRGOTC_BANK_10X8
####################################
set IO_CHAIN_GBOX_VR_START 0
set IO_CHAIN_GBOX_HP_START [expr $IO_CHAIN_GBOX_VR_START + $IO_CHAIN_GBOX_HV_40X2_VR_SIZE]
set IO_CHAIN_GBOX_VL_START [expr $IO_CHAIN_GBOX_HP_START + $IO_CHAIN_GBOX_HP_40X2_SIZE]

set IO_CHAIN_VIRGOTC_BANK_SIZE [expr $IO_CHAIN_GBOX_VL_START + $IO_CHAIN_GBOX_HV_40X2_VL_SIZE]

puts "IO_CHAIN_VIRGOTC_BANK_SIZE = $IO_CHAIN_VIRGOTC_BANK_SIZE"

####################################
# If we used the gbox_ioc_cfg.tcl, we would need to instantiate that RIC module.
# create_instance -block GBOX_IOC_CFG -name u_io_cfg -logic_address 0 -parent VIRGOTC_BANK
####################################
####################################
# bank description
####################################
create_instance -block GBOX_HV_40X2_VL_10X8 -name u_GBOX_HV_40X2_VL_10X8  -logic_address [expr ${IO_CHAIN_GBOX_VL_START}] -parent VIRGOTC_BANK_10X8
create_instance -block GBOX_HP_40X2_10X8  -name u_GBOX_HP_40X2_10X8   -logic_address [expr ${IO_CHAIN_GBOX_HP_START}] -parent VIRGOTC_BANK_10X8
create_instance -block GBOX_HV_40X2_VR_10X8 -name u_GBOX_HV_40X2_VR_10X8  -logic_address [expr ${IO_CHAIN_GBOX_VR_START}] -parent VIRGOTC_BANK_10X8
###############################################################################

# Mapping between RIC instance and PinTable
# Added by George Chen
# 8/2023
#
# PAR_IO_NUM = 40 for Gemini Plus
#
# Here is the email from Abdul Rehman about the mapping, 8/11/2023
#
# u_HV_GBOX_BK0_A_0 to Bank_VL_1_1
# u_HV_GBOX_BK0_B_0 to Bank_VL_1_2
# u_HV_GBOX_BK0_A_1 to Bank_VL_1_3
# u_HV_GBOX_BK0_B_1 to Bank_VL_1_4
# ...
# u_HV_GBOX_BK0_A_19 to Bank_VL_1_39
# u_HV_GBOX_BK0_B_19 to Bank_VL_1_40


# u_HV_GBOX_BK1_A_0 to Bank_VL_2_1
# u_HV_GBOX_BK1_B_0 to Bank_VL_2_2
# u_HV_GBOX_BK1_A_1 to Bank_VL_2_3
# u_HV_GBOX_BK1_B_1 to Bank_VL_2_4
# ...
# u_HV_GBOX_BK1_A_19 to Bank_VL_2_39
# u_HV_GBOX_BK1_B_19 to Bank_VL_2_40

# For Right Banks:
# u_HV_GBOX_BK0_A_0 to Bank_VR_1_1
# u_HV_GBOX_BK0_B_0 to Bank_VR_1_2
# u_HV_GBOX_BK0_A_1 to Bank_VR_1_3
# u_HV_GBOX_BK0_B_1 to Bank_VR_1_4
# ...
# u_HV_GBOX_BK0_A_19 to Bank_VR_1_39
# u_HV_GBOX_BK0_B_19 to Bank_VR_1_40

# u_HV_GBOX_BK1_A_0 to Bank_VR_2_1
# u_HV_GBOX_BK1_B_0 to Bank_VR_2_2
# u_HV_GBOX_BK1_A_1 to Bank_VR_2_3
# u_HV_GBOX_BK1_B_1 to Bank_VR_2_4
# ...
# u_HV_GBOX_BK1_A_19 to Bank_VR_2_39
# u_HV_GBOX_BK1_B_19 to Bank_VR_2_40

# For Bottom Banks:
# u_HP_GBOX_BK0_A_0 to Bank_H_1_1
# u_HP_GBOX_BK0_B_0 to Bank_H_1_2
# u_HP_GBOX_BK0_A_1 to Bank_H_1_3
# u_HP_GBOX_BK0_B_1 to Bank_H_1_4
# ...
# u_HP_GBOX_BK0_A_19 to Bank_H_1_39
# u_HP_GBOX_BK0_B_19 to Bank_H_1_40


# u_HP_GBOX_BK1_A_0 to Bank_H_2_1
# u_HP_GBOX_BK1_B_0 to Bank_H_2_2
# u_HP_GBOX_BK1_A_1 to Bank_H_2_3
# u_HP_GBOX_BK1_B_1 to Bank_H_2_4
# ...
# u_HP_GBOX_BK1_A_19 to Bank_H_2_39
# u_HP_GBOX_BK1_B_19 to Bank_H_2_40

################################################################
proc get_ball_name {bump_name} {

  # bump_names follow the following fields
  #   BANK_[inst]_[side]_[i]
  # where 
  #   inst_side = VR, H, or VL
  #   bank = 1, 2
  #   i = 1 .. 40

  # Ball names are broken up into the following fields
  # [type_bank]_[group]_[j][PN]

  #  type_bank = 
  #    HP_1 if inst_side == H_1
  #    HP_2 if inst_side == H_2
  #    HR_1 if inst_side == VL_1
  #    HR_2 if inst_side == VL_2
  #    HR_4 if inst_side == VR_1
  #    HR_5 if inst_side == VR_2
  #
  #  group = i - 1
  #
  #  j = int[(i + 1)/2] -1
  #  PN = P if i is odd, N if i is even
  #
  # Clock Pin Exceptions:
  #
  #   Bank_H_1_30 => HP_1_CC_29_14N
  #   Bank_H_1_29 => HP_1_CC_28_14P
  #   Bank_H_1_12 => HP_1_CC_11_5N
  #   Bank_H_1_11 => HP_1_CC_10_5P
  #
  # (We insert _CC after group if i = {11, 10, 30, 29})


  set tokens [split $bump_name "_"]
  set inst_side [lindex $tokens 1]_[lindex $tokens 2]
  set i [lindex $tokens 3]

  switch $inst_side {
    "H_1" { set type_bank HP_1 }
    "H_2" { set type_bank HP_2 }
    "VL_1" { set type_bank HR_1 }
    "VL_2" { set type_bank HR_2 }
    "VR_1" { set type_bank HR_4 }
    "VR_2" { set type_bank HR_5 }
    default { error "ERROR:  Cannot compute customer name with bump name $bump_name" 1}
  }

  # add clock capable pin, if index is 11, 12, 29, 30
  if {($i == "11") || ($i == "12") || ($i == "29") || ($i == "30")} {
    set type_bank "${type_bank}_CC"
  }

  set group [expr $i - 1]

  set j [expr int(($i + 1)/2) - 1]

  if {[expr $i % 2]} {
    set suffix P
  } else {
    set suffix N
  }
  
  # put it all together
  set retval "${type_bank}_${group}_$j${suffix}"

  return $retval
}
################################################################
#
# BANK_VL_1_*
for {set i 0} {$i < [expr $PAR_IO_NUM /2]} {incr i} {
  set BANK_INDEX [expr 2*$i +1]
  set model_name u_GBOX_HV_40X2_VL.u_HV_GBOX_BK0_A_$i
  set bank_name BANK_VL_1_${BANK_INDEX}
  set cust_name [get_ball_name $bank_name]
  #puts "$model_name, $bank_name, $cust_name"
  map_model_user_names -model_name $model_name -user_name $cust_name
  incr BANK_INDEX
  set model_name u_GBOX_HV_40X2_VL.u_HV_GBOX_BK0_B_$i
  set bank_name BANK_VL_1_${BANK_INDEX}
  set cust_name [get_ball_name $bank_name]
  #puts "$model_name, $bank_name, $cust_name"
  map_model_user_names -model_name $model_name -user_name $cust_name
}
# BANK_VL_2_*
for {set i 0} {$i < [expr $PAR_IO_NUM /2]} {incr i} {
  set BANK_INDEX [expr 2*$i +1]
  set model_name u_GBOX_HV_40X2_VL.u_HV_GBOX_BK1_A_$i
  set bank_name BANK_VL_2_${BANK_INDEX}
  set cust_name [get_ball_name $bank_name]
  #puts "$model_name, $bank_name, $cust_name"
  map_model_user_names -model_name $model_name -user_name $cust_name
  incr BANK_INDEX
  set model_name u_GBOX_HV_40X2_VL.u_HV_GBOX_BK1_B_$i
  set bank_name BANK_VL_2_${BANK_INDEX}
  set cust_name [get_ball_name $bank_name]
  #puts "$model_name, $bank_name, $cust_name"
  map_model_user_names -model_name $model_name -user_name $cust_name
}

#
# BANK_VR_1_*
for {set i 0} {$i < [expr $PAR_IO_NUM /2]} {incr i} {
  set BANK_INDEX [expr 2*$i +1]
  set model_name u_GBOX_HV_40X2_VR.u_HV_GBOX_BK0_A_$i
  set bank_name BANK_VR_1_${BANK_INDEX}
  set cust_name [get_ball_name $bank_name]
  #puts "$model_name, $bank_name, $cust_name"
  map_model_user_names -model_name $model_name -user_name $cust_name
  incr BANK_INDEX
  set model_name u_GBOX_HV_40X2_VR.u_HV_GBOX_BK0_B_$i
  set bank_name BANK_VR_1_${BANK_INDEX}
  set cust_name [get_ball_name $bank_name]
  #puts "$model_name, $bank_name, $cust_name"
  map_model_user_names -model_name $model_name -user_name $cust_name
}
# BANK_VR_2_*
for {set i 0} {$i < [expr $PAR_IO_NUM /2]} {incr i} {
  set BANK_INDEX [expr 2*$i +1]
  set model_name u_GBOX_HV_40X2_VR.u_HV_GBOX_BK1_A_$i
  set bank_name BANK_VR_2_${BANK_INDEX}
  set cust_name [get_ball_name $bank_name]
  #puts "$model_name, $bank_name, $cust_name"
  map_model_user_names -model_name $model_name -user_name $cust_name
  incr BANK_INDEX
  set model_name u_GBOX_HV_40X2_VR.u_HV_GBOX_BK1_B_$i
  set bank_name BANK_VR_2_${BANK_INDEX}
  set cust_name [get_ball_name $bank_name]
  #puts "$model_name, $bank_name, $cust_name"
  map_model_user_names -model_name $model_name -user_name $cust_name
}

# BANK_H_1_*
for {set i 0} {$i < [expr $PAR_IO_NUM /2]} {incr i} {
  set BANK_INDEX [expr 2*$i +1]
  set model_name u_GBOX_HP_40X2.u_HP_GBOX_BK0_A_$i
  set bank_name BANK_H_1_${BANK_INDEX}
  set cust_name [get_ball_name $bank_name]
  #puts "$model_name, $bank_name, $cust_name"
  map_model_user_names -model_name $model_name -user_name $cust_name
  incr BANK_INDEX
  set model_name u_GBOX_HP_40X2.u_HP_GBOX_BK0_B_$i
  set bank_name BANK_H_1_${BANK_INDEX}
  set cust_name [get_ball_name $bank_name]
  #puts "$model_name, $bank_name, $cust_name"
  map_model_user_names -model_name $model_name -user_name $cust_name
}

# BANK_H_2_*
for {set i 0} {$i < [expr $PAR_IO_NUM /2]} {incr i} {
  set BANK_INDEX [expr 2*$i +1]
  set model_name u_GBOX_HP_40X2.u_HP_GBOX_BK1_A_$i
  set bank_name BANK_H_2_${BANK_INDEX}
  set cust_name [get_ball_name $bank_name]
  #puts "$model_name, $bank_name, $cust_name"
  map_model_user_names -model_name $model_name -user_name $cust_name
  incr BANK_INDEX
  set model_name u_GBOX_HP_40X2.u_HP_GBOX_BK1_B_$i
  set bank_name BANK_H_2_${BANK_INDEX}
  set cust_name [get_ball_name $bank_name]
  #puts "$model_name, $bank_name, $cust_name"
  map_model_user_names -model_name $model_name -user_name $cust_name
}
