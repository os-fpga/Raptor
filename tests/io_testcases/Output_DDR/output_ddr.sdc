# Timing constraints
create_clock -name CLK -period 5 [get_ports CLK]
set_input_delay -clock CLK -max 3 [get_ports D*]
set_input_delay -clock CLK -min 1 [get_ports D*]
set_output_delay -clock CLK -max 3 [get_ports DDR_OUT] ;
set_output_delay -clock CLK -min 2 [get_ports DDR_OUT] ;
set_output_delay -clock CLK -max 3 [get_ports DDR_OUT] -clock_fall -add_delay;
set_output_delay -clock CLK -min 2 [get_ports DDR_OUT] -clock_fall -add_delay;

# Location Constraints (May not be provided early in design cycle)
#Pin Constraints
set_property PIN_LOC AH26 [get_ports D[0]]   # D[0] pin constrained to pin AH26
set_property PIN_LOC AH27 [get_ports D[1]]   # D[1] pin constrained to pin AH27
set_property PIN_LOC AG21 [get_ports CLK] # CLK pin constrained to clock-capable pin AG21
set_property PIN_LOC AA17 [get_ports DDR_OUT]   # DDR_OUT pin constrained to pin AA17
#Fabric (Optional)
#set_property LOC <FLE_register_coordinate> [get_cells q_reg]   # Inferred regsiter is assigned to a specific register location in the fabric

# Specify I/O Standard (If not specified, use default)
set_property IOSTANDARD LVCMOS_18_HR [get_ports D*]
set_property IOSTANDARD LVCMOS_18_HR [get_ports CLK]
set_property IOSTANDARD LVCMOS_18_HR [get_ports DDR_OUT]

# Specify DRIVE_STRENGTH and SLEW_RATE for LVCMOS Output (If not specified, use default)
set_property DRIVE_STRENGTH 12 [get_ports DDR_OUT]  # Values are: 2,4,6,8,12,16 
set_property SLEW_RATE FAST [get_ports DDR_OUT]   # Values are: SLOW,FAST

