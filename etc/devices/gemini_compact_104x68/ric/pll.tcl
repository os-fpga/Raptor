#!/usr/bin/env tclsh
####################################
# created by Alex H , April 2, 2023
# pll block definition 
####################################
define_block -name PLL

####################################
# define configuration attributes (parameters) 
####################################


#  not in this block
#define_attr -block PLL -name pllref_use_div		-addr  95 -width 1 -enum {pllref_use_div_0 0} {Fpllref_use_div_1 1} -enumname PLL_ATT_0

define_attr -block PLL -name pll_FOUTDIFFEN		-addr  94 -width 1 -enum {FOUTDIFFEN_0 0} {FOUTDIFFEN_1 1} -enumname PLL_ATT_0
define_attr -block PLL -name pll_FOUTCMLEN		-addr  93 -width 1 -enum {FOUTCMLEN_0 0} {FOUTCMLEN_1 1} -enumname PLL_ATT_1
define_attr -block PLL -name pll_FOUTEN_3		-addr  92 -width 1 -enum {FOUTEN_bit3_0 0} {FOUTEN_bit3_1 1} -enumname PLL_ATT_2
define_attr -block PLL -name pll_FOUTEN_2		-addr  91 -width 1 -enum {FOUTEN_bit2_0 0} {FOUTEN_bit2_1 1} -enumname PLL_ATT_3
define_attr -block PLL -name pll_FOUTEN_1		-addr  90 -width 1 -enum {FOUTEN_bit1_0 0} {FOUTEN_bit1_1 1} -enumname PLL_ATT_4
define_attr -block PLL -name pll_FOUTEN_0		-addr  89 -width 1 -enum {FOUTEN_bit0_0 0} {FOUTEN_bit0_1 1} -enumname PLL_ATT_5
define_attr -block PLL -name pll_FOUTVCOEN		-addr  88 -width 1 -enum {FOUTVCOEN_0 0} {FOUTVCOEN_1 1} -enumname PLL_ATT_6
define_attr -block PLL -name pll_FREFCMLEN		-addr  87 -width 1 -enum {FREFCMLEN_0 0} {FREFCMLEN_1 1} -enumname PLL_ATT_7
define_attr -block PLL -name pll_DACEN			-addr  86 -width 1 -enum {DACEN_0 0} {DACEN_1 1} -enumname PLL_ATT_8
define_attr -block PLL -name pll_DSMEN			-addr  85 -width 1 -enum {DSMEN_0 0} {DSMEN_1 1} -enumname PLL_ATT_9
define_param -block PLL -name pll_FOUTVCOBYP 	-addr  80 -width 5 -type integer
define_param -block PLL -name pll_POSTDIV4		-addr  78 -width 2 -type integer
define_param -block PLL -name pll_POSTDIV3		-addr  74 -width 4 -type integer
define_param -block PLL -name pll_POSTDIV2		-addr  70 -width 4 -type integer
define_param -block PLL -name pll_POSTDIV1		-addr  66 -width 4 -type integer
define_param -block PLL -name pll_POSTDIV0		-addr  62 -width 4 -type integer
### define_attr -block PLL -name pll_FOUTVCOBYP		-addr  80 -width 5 -enum {FOUTVCOBYP_0 0} {FOUTVCOBYP_1 1} -enumname PLL_ATT_10
### define_attr -block PLL -name pll_POSTDIV4		-addr  78 -width 2 -enum {POSTDIV4_0 0} {POSTDIV4_1 1} -enumname PLL_ATT_POSTDIV4
### define_attr -block PLL -name pll_POSTDIV3		-addr  74 -width 4 -enum {POSTDIV3_0 0} {POSTDIV3_1 1} -enumname PLL_ATT_POSTDIV3
### define_attr -block PLL -name pll_POSTDIV2		-addr  70 -width 4 -enum {POSTDIV2_0 0} {POSTDIV2_1 1} -enumname PLL_ATT_POSTDIV2
### define_attr -block PLL -name pll_POSTDIV1		-addr  66 -width 4 -enum {POSTDIV1_0 0} {POSTDIV1_1 1} -enumname PLL_ATT_POSTDIV1
### define_attr -block PLL -name pll_POSTDIV0		-addr  62 -width 4 -enum {POSTDIV0_0 0} {POSTDIV0_1 1} -enumname PLL_ATT_POSTDIV0
define_attr -block PLL -name pll_FREFCMLN		-addr  61 -width 1 -enum {FREFCMLN_0 0} {FREFCMLN_1 1} -enumname PLL_ATT_11
define_attr -block PLL -name pll_PLLEN			-addr  60 -width 1 -enum {PLLEN_0 0} {PLLEN_1 1} -enumname PLL_ATT_12
define_param -block PLL -name pll_REFDIV		-addr  54 -width 6 -type integer -enumname PLL_ATT_13
define_param -block PLL -name pll_FBDIV			-addr  42 -width 12 -type integer -enumname PLL_ATT_14
define_param -block PLL -name pll_FRAC			-addr  18 -width 24 -type integer -enumname PLL_ATT_15
### define_attr -block PLL -name pll_REFDIV			-addr  54 -width 6 -enum {REFDIV_0 0} {REFDIV_1 1} -enumname PLL_ATT_REFDIV
### define_attr -block PLL -name pll_FBDIV			-addr  42 -width 12 -enum {FBDIV_0 0} {FBDIV_1 1} -enumname PLL_ATT_FBDIV
### define_attr -block PLL -name pll_FRAC			-addr  18 -width 24 -enum {FRAC_0 0} {FRAC_1 1} -enumname PLL_ATT_FRAC
define_attr -block PLL -name pll_DSKEWCALEN		-addr  17 -width 1 -enum {DSKEWCALEN_0 0} {DSKEWCALEN_1 1} -enumname PLL_ATT_21
define_attr -block PLL -name pll_DSKEWFASTCAL	-addr  16 -width 1 -enum {DSKEWFASTCAL_0 0} {DSKEWFASTCAL_1 1} -enumname PLL_ATT_22
define_param -block PLL -name pll_DSKEWCALCNT	-addr  13 -width 3 -type integer
define_param -block PLL -name pll_DSKEWCALIN	-addr   1 -width 12 -type integer
### define_attr -block PLL -name pll_DSKEWCALCNT	-addr  13 -width 3 -enum {DSKEWCALCNT_0 0} {DSKEWCALCNT_1 1} -enumname PLL_ATT_23
### define_attr -block PLL -name pll_DSKEWCALIN		-addr   1 -width 12 -enum {DSKEWCALIN_0 0} {DSKEWCALIN_1 1} -enumname PLL_ATT_24
define_attr -block PLL -name pll_DSKEWCALBYP	-addr   0 -width 1 -enum {DSKEWCALBYP_0 0} {DSKEWCALBYP_1 1} -enumname PLL_ATT_25

####################################
# Constraints within block attributes 
####################################
#### define_constraint -block PLL -constraint {(DFEN == SingleEnded) 			-> {RATE inside {[Three:Ten]}} }
######################################
define_ports -block PLL -in pll_FREF 
######################################
define_ports -block PLL -in pll_FREF 
######################################
define_ports -block PLL -out pll_FOUTVCO 
define_ports -block PLL -out pll_LOCK 
define_ports -block PLL -out pll_FOUT_0 
define_ports -block PLL -out pll_FOUT_1 
define_ports -block PLL -out pll_FOUT_2 
define_ports -block PLL -out pll_FOUT_3 
###################################### * ref/D * N 
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
