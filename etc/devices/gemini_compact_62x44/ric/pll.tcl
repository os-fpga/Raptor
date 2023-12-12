#!/usr/bin/env tclsh
####################################
# created by Alex H , April 2, 2023
# Updated by George C, Oct 2023
#
# pll block definition 
####################################
define_block -name PLL

####################################
# define configuration attributes (parameters) 
####################################
# From IO_BANK_VirgoTC.docx
# (see pllrefmux.tcl for bits above pll_DACEN)
define_attr -block PLL -name pll_DACEN			-addr  68 -width 1 -enum {DACEN_0 0} {DACEN_1 1} -enumname PLL_ATT_12
define_attr -block PLL -name pll_DSMEN			-addr  67 -width 1 -enum {DSMEN_0 0} {DSMEN_1 1} -enumname PLL_ATT_11
define_param -block PLL -name pll_POSTDIV2		-addr  64 -width 3 -type integer
define_param -block PLL -name pll_POSTDIV1		-addr  61 -width 3 -type integer
define_attr -block PLL -name pll_PLLEN			-addr  60 -width 1 -enum {PLLEN_0 0} {PLLEN_1 1} -enumname PLL_ATT_8
define_param -block PLL -name pll_REFDIV		-addr  54 -width 6 -type integer -enumname PLL_ATT_7
define_param -block PLL -name pll_FBDIV			-addr  42 -width 12 -type integer -enumname PLL_ATT_6
define_param -block PLL -name pll_FRAC			-addr  18 -width 24 -type integer -enumname PLL_ATT_5
define_attr -block PLL -name pll_DSKEWCALEN		-addr  17 -width 1 -enum {DSKEWCALEN_0 0} {DSKEWCALEN_1 1} -enumname PLL_ATT_4
define_attr -block PLL -name pll_DSKEWFASTCAL	        -addr  16 -width 1 -enum {DSKEWFASTCAL_0 0} {DSKEWFASTCAL_1 1} -enumname PLL_ATT_3
define_param -block PLL -name pll_DSKEWCALCNT	        -addr  13 -width 3 -type integer
define_param -block PLL -name pll_DSKEWCALIN	        -addr   1 -width 12 -type integer
define_attr -block PLL -name pll_DSKEWCALBYP	        -addr   0 -width 1 -enum {DSKEWCALBYP_0 0} {DSKEWCALBYP_1 1} -enumname PLL_ATT_0

####################################
# Constraints within block attributes 
####################################
######################################
define_ports -block PLL -in pll_FREF 
######################################
define_ports -block PLL -out pll_FOUTVCO 
define_ports -block PLL -out pll_LOCK 
define_ports -block PLL -out pll_FOUT_0 
define_ports -block PLL -out pll_FOUT_1 
define_ports -block PLL -out pll_FOUT_2 
define_ports -block PLL -out pll_FOUT_3 
######################################
map_rtl_user_names -user_name DIVIDE_CLK_IN_BY_2  -rtl_name pllref_use_div
map_rtl_user_names -user_name N 			-rtl_name pll_FBDIV
map_rtl_user_names -user_name D 			-rtl_name pll_REFDIV
map_rtl_user_names -user_name CLK_OUT0_DIV -rtl_name pll_POSTDIV0
map_rtl_user_names -user_name PLL_EN 		-rtl_name pll_PLLEN
map_rtl_user_names -user_name CLK_OUT0_EN 	-rtl_name pll_FOUTEN_0
map_rtl_user_names -user_name CLK_OUT1_EN 	-rtl_name pll_FOUTEN_1
map_rtl_user_names -user_name CLK_OUT2_EN 	-rtl_name pll_FOUTEN_2
map_rtl_user_names -user_name CLK_OUT3_EN 	-rtl_name pll_FOUTEN_3
####
map_rtl_user_names -user_name CLK_OUT0 -rtl_name pll_FOUT_0
map_rtl_user_names -user_name CLK_OUT1 -rtl_name pll_FOUT_1
map_rtl_user_names -user_name CLK_OUT2 -rtl_name pll_FOUT_2
map_rtl_user_names -user_name CLK_OUT3 -rtl_name pll_FOUT_3
map_rtl_user_names -user_name LOCK 	-rtl_name pll_LOCK
map_rtl_user_names -user_name CLK_IN 	-rtl_name pll_FREF
######################################
