# Timing constraints
create_clock -name CLK -period 5 [get_ports CLK]  # This is input to PLL, Other clocks will be auto-generated including pll_fabric_clock
set_input_delay -clock CLK -max 2 [get_ports D];
set_input_delay -clock CLK -min 3 [get_ports D];
set_input_delay -clock CLK -max 2 [get_ports D] -clock_fall -add_delay;
set_input_delay -clock CLK -min 3 [get_ports D] -clock_fall -add_delay;
set_input_delay -clock CLK -max 3 [get_ports IO_RST]
set_input_delay -clock CLK -min 1 [get_ports IO_RST]
set_output_delay -clock CLK -max 2.5 [get_ports PLL_LOCK]
set_output_delay -clock CLK -min 0.5 [get_ports PLL_LOCK]
set_output_delay -clock CLK -max 2.5 [get_ports DATA_VALID]
set_output_delay -clock CLK -min 0.5 [get_ports DATA_VALID]
set_output_delay -clock CLK -max 2.5 [get_ports Q*]
set_output_delay -clock CLK -min 0.5 [get_ports Q*]

# Location Constraints (May not be provided early in design cycle)
#Pin Constraints
set_property PIN_LOC AH26 [get_ports D]
set_property PIN_LOC AG21 [get_ports CLK]
set_property PIN_LOC AF20 [get_ports IO_RST]
set_property PIN_LOC AA17 [get_ports Q[0]]
set_property PIN_LOC AB27 [get_ports Q[1]]
set_property PIN_LOC AB28 [get_ports Q[2]]
set_property PIN_LOC AB29 [get_ports Q[3]]
#Fabric (Optional)
#set_property LOC <FLE_register_coordinate> [get_cells q_reg*]   # Inferred regsiter is assigned to a specific register location in the fabric
# Push Register into I/O (If not specified, place register in FLE)
#set_property IO_REG TRUE [get_ports Q*] # In conflict with fabric propertly LOC so use this or the other (or none)

# Specify I/O Standard (If not specified, use default)
set_property IOSTANDARD LVCMOS_18_HR [get_ports *]

# Specify DRIVE_STRENGTH and SLEW_RATE for LVCMOS Output (If not specified, use default)
set_property DRIVE_STRENGTH 12 [all_outputs]  # Values are: 2,4,6,8,12,16 
set_property SLEW_RATE SLOW [all_outputs]   # Values are: SLOW,FAST

# Optionally, you can specify I_SERDES DIFF_MODE and CHANNEL_MASTER in this file if not specified in netlist
set_property DIFF_MODE "SINGLE_ENDED" [get_cells iserdes_4_inst]
set_property CHANNEL_MASTER "MASTER" [get_cells iserdes_4_inst]

