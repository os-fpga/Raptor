# Timing constraints
create_clock -name CLK -period 5 [get_ports CLK]
set_input_delay -clock CLK -max 3 [get_ports BIDIR_IN]
set_input_delay -clock CLK -min 1 [get_ports BIDIR_IN]
set_input_delay -clock CLK -max 3 [get_ports BIDIR]
set_input_delay -clock CLK -min 1 [get_ports BIDIR]
set_output_delay -clock CLK -max 2.5 [get_ports BIDIR_OUT]
set_output_delay -clock CLK -min 0.5 [get_ports BIDIR_OUT]
set_output_delay -clock CLK -max 2.5 [get_ports BIDIR]
set_output_delay -clock CLK -min 0.5 [get_ports BIDIR]
set_max_delay 3 -from [get_ports OE] -to [get_ports BIDIR]

# Location Constraints (May not be provided early in design cycle)
#Pin Constraints
set_property PIN_LOC AH26 [get_ports BIDIR_IN]   # D pin constrained to pin AH26
set_property PIN_LOC AG21 [get_ports CLK] # CLK pin constrained to clock-capable pin AG21
set_property PIN_LOC AF20 [get_ports OE] 
set_property PIN_LOC AA17 [get_ports BIDIR_OUT] 
set_property PIN_LOC AA18 [get_ports BIDIR]  
#Fabric (Optional)
#set_property LOC <FLE_register_coordinate> [get_cells q_reg]   # Inferred regsiter is assigned to a specific register location in the fabric
# Push Register into I/O (If not specified, place register in FLE)
#set_property IO_REG TRUE [get_ports BIDIR_IN] # In conflict with fabric propertly LOC so use this or the other (or none)
#set_property IO_REG TRUE [get_ports BIDIR_OUT] # In conflict with fabric propertly LOC so use this or the other (or none)



# Specify I/O Standard (If not specified, use default)
set_property IOSTANDARD LVCMOS_18_HR [get_ports *]

# Specify DRIVE_STRENGTH and SLEW_RATE for LVCMOS Output (If not specified, use default)
set_property DRIVE_STRENGTH 12 [all_outputs]  # Values are: 2,4,6,8,12,16 
set_property SLEW_RATE SLOW [all_outputs]   # Values are: SLOW,FAST

