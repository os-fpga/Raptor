# Timing constraints
create_clock -name CLK -period 5 [get_ports CLK]
set_input_delay -clock CLK -max 2 [get_ports D];
set_input_delay -clock CLK -min 3 [get_ports D];
set_input_delay -clock CLK -max 2 [get_ports D] -clock_fall -add_delay;
set_input_delay -clock CLK -min 3 [get_ports D] -clock_fall -add_delay;
set_output_delay -clock CLK -max 2.5 [get_ports Q*]
set_output_delay -clock CLK -min 0.5 [get_ports Q*]

# Location Constraints (May not be provided early in design cycle)
#Pin Constraints
set_property PIN_LOC AH26 [get_ports D]   # D pin constrained to pin AH26
set_property PIN_LOC AG21 [get_ports CLK] # CLK pin constrained to clock-capable pin AG21
set_property PIN_LOC AF20 [get_ports OE] # CLK pin constrained to clock-capable pin AG21
set_property PIN_LOC AA17 [get_ports Q[0]]   # Q pin constrained to pin AA17
set_property PIN_LOC AB27 [get_ports Q[1]]   # Q pin constrained to pin AB27
#Fabric (Optional)
#set_property LOC <FLE_register_coordinate> [get_cells q_reg*]   # Inferred regsiter is assigned to a specific register location in the fabric
# Push Register into I/O (If not specified, place register in FLE)
#set_property IO_REG TRUE [get_ports Q*] # In conflict with fabric propertly LOC so use this or the other (or none)

# Specify I/O Standard (If not specified, use default)
set_property IOSTANDARD LVCMOS_18_HR [get_ports D]
set_property IOSTANDARD LVCMOS_18_HR [get_ports CLK]
set_property IOSTANDARD LVCMOS_18_HR [get_ports Q*]

# Specify DRIVE_STRENGTH and SLEW_RATE for LVCMOS Output (If not specified, use default)
set_property DRIVE_STRENGTH 12 [get_ports Q*]  # Values are: 2,4,6,8,12,16 
set_property SLEW_RATE SLOW [get_ports Q*]   # Values are: SLOW,FAST

