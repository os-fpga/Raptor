# Timing constraints
create_clock -name CLK -period 5 [get_ports CLK] //  This clock is th einput to the PLL, all other clocks will be automatically calculated
set_input_delay -clock CLK -max 3 [get_ports D*]
set_input_delay -clock CLK -min 1 [get_ports D*]
set_input_delay -clock CLK -max 3 [get_ports IO_RST]
set_input_delay -clock CLK -min 1 [get_ports IO_RST]
set_input_delay -clock CLK -max 3 [get_ports LOAD_WORD]
set_input_delay -clock CLK -min 1 [get_ports LOAD_WORD]
set_false_path -to [get_ports PLL_LOCK] ;
set_output_delay -clock CLK -max 3 [get_ports Q] ;
set_output_delay -clock CLK -min 2 [get_ports Q] ;
set_output_delay -clock CLK -max 3 [get_ports Q] -clock_fall -add_delay;
set_output_delay -clock CLK -min 2 [get_ports Q] -clock_fall -add_delay;

# Location Constraints (May not be provided early in design cycle)
#Pin Constraints
set_property PIN_LOC AH26 [get_ports D[0]]  
set_property PIN_LOC AH27 [get_ports D[1]] 
set_property PIN_LOC AH29 [get_ports D[2]]  
set_property PIN_LOC AH30 [get_ports D[3]]  
set_property PIN_LOC AG20 [get_ports IO_RST]  
set_property PIN_LOC AG21 [get_ports CLK] # CLK pin constrained to clock-capable pin AG21
set_property PIN_LOC AA17 [get_ports DDR_OUT]   # DDR_OUT pin constrained to pin AA17
set_property PIN_LOC AA18 [get_ports PLL_LOCK] 

# Specify I/O Standard (If not specified, use default)
set_property IOSTANDARD LVCMOS_18_HR [get_ports *]

# Specify DRIVE_STRENGTH and SLEW_RATE for LVCMOS Output (If not specified, use default)
set_property DRIVE_STRENGTH 12 [all_outputs]  # Values are: 2,4,6,8,12,16 
set_property SLEW_RATE FAST [all_outputs]   # Values are: SLOW,FAST

# Optionally, you can specify I_SERDES DIFF_MODE and CHANNEL_MASTER in this file if not specified in netlist
set_property DIFF_MODE "SINGLE_ENDED" [get_cells oserdes_4_inst]
set_property CHANNEL_MASTER "MASTER" [get_cells oserdes_4_inst]

