# -name is used for creating virtual clock and for actual clock -name option will not be used
create_clock -period 2.5 -name clk
set_input_delay 1 -clock clk [get_ports {*}]
set_output_delay 1 -clock clk [get_ports {*}]

# Stamp begin
# Date: 2022/10/01
# add a fake mode (Mode_GPIO) to verify the set_mode command functionaltiy
# this mode is actaully a replacement of "Usable" in current gemini pin table (July release)
# this will be changed again once new gemini pin table is offically released from HW team
# Stamp end
set_pin_loc a Bank_H_1_12
set_mode Mode_GPIO Bank_H_1_12
set_pin_loc b Bank_H_1_14
set_mode Mode_GPIO Bank_H_1_14
set_pin_loc c Bank_H_1_15
set_mode Mode_GPIO Bank_H_1_15

