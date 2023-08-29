####################################
# Gearbox block definition 
####################################
define_block -name GBOX_TOP

####################################
define_attr -block "GBOX_TOP" -name "RATE"	        -addr 0x000  -width 4 -enum {Three 3} {Four 4} {Five 5} {Six 6} {Seven 7} {Eight 8} {Night 9} {Ten 10} -enumname GBOX_ATT_0
define_attr -block "GBOX_TOP" -name "MASTER_SLAVE"	-addr 0x004  -width 1 -enum {Slave 0 } {Master 1} -enumname GBOX_ATT_1
define_attr -block "GBOX_TOP" -name "PEER_IS_ON"	-addr 0x005  -width 1 -enum {PEER_off 0 } {PEER_ON 1} -enumname GBOX_ATT_2
define_attr -block "GBOX_TOP" -name "TX_CLOCK_IO"	-addr 0x006  -width 1 -enum {TX_normal_IO 0} {TX_clock_IO 1}  -enumname GBOX_ATT_3
define_attr -block "GBOX_TOP" -name "TX_DDR_MODE"	-addr 0x007  -width 2 -enum {TX_direct 0} {TX_ddr 1} {TX_sdr 2} -enumname GBOX_ATT_4
define_attr -block "GBOX_TOP" -name "TX_BYPASS"  	-addr 0x009  -width 1 -enum {TX_gear_on 0} {TX_bypass 1}  -enumname GBOX_ATT_5
define_attr -block "GBOX_TOP" -name "TX_CLK_PHASE"	-addr 0x00A  -width 2 -enum {TX_phase_0 0} {TX_phase_90 1} {TX_phase_180 2} {TX_phase_270 3} -enumname GBOX_ATT_6
define_param -block GBOX_TOP -name TX_DLY -addr 0x00C -width 6 -type integer
### define_attr -block "GBOX_TOP" -name "TX_DLY"	-addr 0x00C  -width 6 -enum {TX_dly0 0} {TX_dly1 1}  -enumname GBOX_ATT_7
define_attr -block "GBOX_TOP" -name "RX_DDR_MODE"	-addr 0x012  -width 2 -enum {RX_direct 0} {RX_ddr 1} {RX_sdr 2} -enumname GBOX_ATT_8
define_attr -block "GBOX_TOP" -name "RX_BYPASS"  	-addr 0x014  -width 1 -enum {RX_gear_on 0} {RX_bypass 1}  -enumname GBOX_ATT_9
define_param -block GBOX_TOP -name RX_DLY -addr 0x015 -width 6 -type integer
### define_attr -block "GBOX_TOP" -name "RX_DLY"	-addr 0x015  -width 6 -enum {RX_dly0 0} {RX_dly1 1}  -enumname GBOX_ATT_10
define_attr -block "GBOX_TOP" -name "RX_DPA_MODE"	-addr 0x01B  -width 2 -enum {RX_dpa_off 0} {RX_resv 1} {RX_dpa 2} {RX_cdr 3} -enumname GBOX_ATT_11
define_attr -block "GBOX_TOP" -name "RX_MIPI_MODE"	-addr 0x01D  -width 1 -enum {RX_mipi_off 0} {RX_mipi_on 1}  -enumname GBOX_ATT_12
define_attr -block "GBOX_TOP" -name "TX_MODE"		-addr 0x01E  -width 1 -enum {TX_disable 0} {TX_enable 1}  -enumname GBOX_ATT_13
define_attr -block "GBOX_TOP" -name "RX_MODE"		-addr 0x01F  -width 1 -enum {RX_disable 0} {RX_enable 1}  -enumname GBOX_ATT_14
define_attr -block "GBOX_TOP" -name "RX_CLOCK_IO"	-addr 0x020  -width 1 -enum {RX_normal_IO 0} {RX_clock_IO 1}  -enumname GBOX_ATT_15
define_attr -block "GBOX_TOP" -name "DFEN"			-addr 0x021  -width 1 -enum {SingleEnded 0} {Differential 1 }  -enumname GBOX_ATT_16
define_attr -block "GBOX_TOP" -name "SR"			-addr 0x022  -width 1 -enum {SR_disable 0} {SR_enable 1 }  -enumname GBOX_ATT_17
define_attr -block "GBOX_TOP" -name "PE"			-addr 0x023  -width 1 -enum {PE_disable 0} {PE_enable 1 }  -enumname GBOX_ATT_18
define_attr -block "GBOX_TOP" -name "PUD"			-addr 0x024  -width 1 -enum {PUD_disable 0} {PUD_enable 1 }  -enumname GBOX_ATT_19
define_attr -block "GBOX_TOP" -name "DFODTEN"		-addr 0x025  -width 1 -enum {DF_odt_disable 0} {DF_odt_enable 1 }  -enumname GBOX_ATT_20
###define_attr -block "GBOX_TOP" -name "MC"			-addr 0x026  -width 4 -enum {IO_mode_0 0} {IO_mode_0 1 }  -enumname GBOX_ATT_21
define_param -block GBOX_TOP -name MC -addr 0x026 -width 4 -type integer
####################################
# Constraints within block attributes 
####################################

define_constraint -block GBOX_TOP -constraint {(DFEN == SingleEnded) 			-> {RATE inside {[Three:Ten]}} }
define_constraint -block GBOX_TOP -constraint {(DFEN == Differential) 			-> {RATE inside {[Three:Ten]}} }
define_constraint -block GBOX_TOP -constraint {(DFEN == Differential)			-> (PEER_IS_ON == PEER_off)	}
define_constraint -block GBOX_TOP -constraint {(PEER_IS_ON == PEER_ON) 			-> {RATE inside {[Three:Five]}} }
define_constraint -block GBOX_TOP -constraint {(RX_MIPI_MODE == RX_mipi_on) 	-> (DFEN == Differential)} 
define_constraint -block GBOX_TOP -constraint {(TX_DDR_MODE == TX_direct) 		-> (TX_BYPASS == TX_bypass)} 
define_constraint -block GBOX_TOP -constraint {(RX_DDR_MODE == RX_direct) 		-> (RX_BYPASS == RX_bypass)} 

######################################
### define_ports -block GBOX_TOP -in a1 b1 c1 -out aa1 bb1 cc1
define_ports -block GBOX_TOP -in system_reset_n 
define_ports -block GBOX_TOP -in pll_lock 
##### define_ports -block GBOX_TOP -in [3:0] cfg_rate_sel 
define_ports -block GBOX_TOP -in cfg_rate_sel_bit3 cfg_rate_sel_bit2 cfg_rate_sel_bit1 cfg_rate_sel_bit0 
define_ports -block GBOX_TOP -in cfg_done 
define_ports -block GBOX_TOP -in cfg_dif 
define_ports -block GBOX_TOP -in cfg_chan_master 
define_ports -block GBOX_TOP -in cfg_mipi_mode 
##### define_ports -block GBOX_TOP -in [1:0] cfg_tx_clk_phase 
define_ports -block GBOX_TOP -in cfg_tx_clk_phase_bit1 cfg_tx_clk_phase_bit0 
define_ports -block GBOX_TOP -in cfg_peer_is_on 
define_ports -block GBOX_TOP -in cfg_tx_bypass 
define_ports -block GBOX_TOP -in cfg_rx_bypass 
define_ports -block GBOX_TOP -in cfg_tx_mode 
define_ports -block GBOX_TOP -in cfg_rx_mode 
define_ports -block GBOX_TOP -in cfg_rx_use_rx_clk_io 
#####
set PAR_TWID 6
##### for {set cfg_tx_dly_bit 0} {$cfg_tx_dly_bit < $PAR_TWID} {incr cfg_tx_dly_bit} { 
#####     define_ports -block GBOX_TOP -in cfg_tx_dly_$cfg_tx_dly_bit
##### } 
define_ports -block GBOX_TOP -in cfg_tx_dly_bit5 cfg_tx_dly_bit4 cfg_tx_dly_bit3 \
                                         cfg_tx_dly_bit2 cfg_tx_dly_bit1 cfg_tx_dly_bit0
##### for {set cfg_rx_dly_bit 0} {$cfg_rx_dly_bit <= $PAR_TWID} {incr cfg_rx_dly_bit} { 
#####     define_ports -block GBOX_TOP -in cfg_rx_dly_$cfg_rx_dly_bit
##### } 
define_ports -block GBOX_TOP -in cfg_rx_dly_bit5 cfg_rx_dly_bit4 cfg_rx_dly_bit3 \
                                         cfg_rx_dly_bit2 cfg_rx_dly_bit1 cfg_rx_dly_bit0

#####
define_ports -block GBOX_TOP -in cfg_tx_ddr_mode_bit1 cfg_tx_ddr_mode_bit0
define_ports -block GBOX_TOP -in cfg_rx_ddr_mode_bit1 cfg_rx_ddr_mode_bit0 
define_ports -block GBOX_TOP -in fast_clk 
define_ports -block GBOX_TOP -in i2g_rx_in 
define_ports -block GBOX_TOP -in fast_phase_clk_0   
define_ports -block GBOX_TOP -in fast_phase_clk_1 
define_ports -block GBOX_TOP -in fast_phase_clk_2   
define_ports -block GBOX_TOP -in fast_phase_clk_3 
define_ports -block GBOX_TOP -in f2g_tx_reset_n 
define_ports -block GBOX_TOP -in f2g_trx_core_clk 
define_ports -block GBOX_TOP -in f2g_tx_dly_ld 
define_ports -block GBOX_TOP -in f2g_tx_dly_adj 
define_ports -block GBOX_TOP -in f2g_tx_dly_inc 
define_ports -block GBOX_TOP -in f2g_tx_oe 
#####
set PAR_TX_DWID 10
##### for {set f2g_tx_out_bit 0} {$f2g_tx_out_bit <= $PAR_TX_DWID} {incr f2g_tx_out_bit} { 
#####     define_ports -block GBOX_TOP -in f2g_tx_out_$f2g_tx_out_bit
##### } 
define_ports -block GBOX_TOP -in f2g_tx_out_bit9 f2g_tx_out_bit8 f2g_tx_out_bit7 f2g_tx_out_bit6 f2g_tx_out_bit5 \
                                         f2g_tx_out_bit4 f2g_tx_out_bit3 f2g_tx_out_bit2 f2g_tx_out_bit1 f2g_tx_out_bit0
#####
define_ports -block GBOX_TOP -in f2g_in_en 
define_ports -block GBOX_TOP -in f2g_tx_dvalid 
define_ports -block GBOX_TOP -in f2g_tx_clk_en  
define_ports -block GBOX_TOP -in f2g_rx_reset_n 
define_ports -block GBOX_TOP -in f2g_rx_sfifo_reset 
define_ports -block GBOX_TOP -in f2g_rx_dly_ld 
define_ports -block GBOX_TOP -in f2g_rx_dly_adj 
define_ports -block GBOX_TOP -in f2g_rx_dly_inc 
define_ports -block GBOX_TOP -in f2g_rx_bitslip_adj 
define_ports -block GBOX_TOP -in cfg_rx_dpa_mode_bit1 cfg_rx_dpa_mode_bit0
define_ports -block GBOX_TOP -in f2g_rx_dpa_restart 
define_ports -block GBOX_TOP -in fast_clk_sync_in 
define_ports -block GBOX_TOP -in fast_cdr_clk_sync_in 
#####
set PAR_TX_DWID_PEER 5
##### for {set peer_data_in_bit 0} {$peer_data_in_bit <= $PAR_TX_DWID_PEER} {incr peer_data_in_bit} { 
#####     define_ports -block GBOX_TOP -in peer_data_in_$peer_data_in_bit} 
##### } 
define_ports -block GBOX_TOP -in peer_data_in_bit4 peer_data_in_bit3 \
                                         peer_data_in_bit2 peer_data_in_bit1 peer_data_in_bit0
######################################
define_ports -block GBOX_TOP -out fast_clk_sync_out 
define_ports -block GBOX_TOP -out fast_cdr_clk_sync_out 
define_ports -block GBOX_TOP -out g2i_tx_out 
define_ports -block GBOX_TOP -out g2i_tx_oe 
define_ports -block GBOX_TOP -out g2i_tx_clk 
define_ports -block GBOX_TOP -out g2i_ie 
define_ports -block GBOX_TOP -out cdr_clk 
##### for {set cfg_tx_dly_bit 0} {$cfg_tx_dly_bit <= $PAR_TWID} {incr cfg_tx_dly_bit} { 
#####     define_ports -block GBOX_TOP -out g2f_tx_dly_tap_$cfg_tx_dly_bit 
##### } 
define_ports -block GBOX_TOP -out g2f_tx_dly_tap_bit5 g2f_tx_dly_tap_bit4 g2f_tx_dly_tap_bit3 \
                                          g2f_tx_dly_tap_bit2 g2f_tx_dly_tap_bit1 g2f_tx_dly_tap_bit0
define_ports -block GBOX_TOP -out g2f_core_clk 
define_ports -block GBOX_TOP -out g2f_rx_cdr_core_clk 
define_ports -block GBOX_TOP -out g2f_rx_dpa_lock 
define_ports -block GBOX_TOP -out g2f_rx_dpa_error 
##### for {set TMP_WID 0} {$TMP_WID <= 2} {incr TMP_WID} { 
#####     define_ports -block GBOX_TOP -out g2f_rx_dpa_phase_$TMP_WID 
##### } 
### define_ports -block GBOX_TOP -out [2:0] g2f_rx_dpa_phase }
define_ports -block GBOX_TOP -out g2f_rx_dpa_phase_bit2 g2f_rx_dpa_phase_bit1 g2f_rx_dpa_phase_bit0 }

##### for {set cfg_tx_dly_bit 0} {$cfg_tx_dly_bit <= $PAR_TWID} {incr cfg_tx_dly_bit} { 
#####     define_ports -block GBOX_TOP -out g2f_rx_dly_tap_$cfg_tx_dly_bit 
##### } 
### define_ports -block GBOX_TOP -out [PAR_TWID-1:0] g2f_rx_dly_tap }
define_ports -block GBOX_TOP -out g2f_rx_dly_tap_bit5 g2f_rx_dly_tap_bit4 g2f_rx_dly_tap_bit3 \

set PAR_RX_DWID 10
##### for {set g2f_rx_in_bit 0} {$g2f_rx_in_bit <= $PAR_RX_DWID} {incr g2f_rx_in_bit} { 
#####     define_ports -block GBOX_TOP -out g2f_rx_in_$g2f_rx_in_bit
##### } 
### define_ports -block GBOX_TOP -out [PAR_RX_DWID-1:0] g2f_rx_in }
define_ports -block GBOX_TOP -out g2f_rx_in_bit9 g2f_rx_in_bit8 g2f_rx_in_bit7 g2f_rx_in_bit6 g2f_rx_in_bit5 \
                                          g2f_rx_in_bit4 g2f_rx_in_bit3 g2f_rx_in_bit2 g2f_rx_in_bit1 g2f_rx_in_bit0
define_ports -block GBOX_TOP -out g2f_rx_dvalid 

######################################
# Create IO configuration chain (instance of a chain)
set START_LOGICAL_ADDRESS 0x00000
set END_LOGICAL_ADDRESS   0x0F000
create_chain_instance -type ICB_CHAIN -name GBX_IO_CHAIN -start_address $START_LOGICAL_ADDRESS -end_address $END_LOGICAL_ADDRESS

#for {set gbx_instance_id $GBX_START_ID} {$gbx_instance_id <= $GBX_END_ID} {incr gbx_instance_id} { 
#   append_instance_to_chain -chain “GBX_IO_CHAIN” -inst [get_instance_by_id {$gbx_instance_id}] 
#} 

######################################
####  TX_DLY has 64 enuname
####   &   or  combined operation
####   define_ports
