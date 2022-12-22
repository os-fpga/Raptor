#Copyright 2022 The Foedag team

#GPL License

#Copyright (c) 2022 The Open-Source FPGA Foundation

#This program is free software: you can redistribute it and/or modify
#it under the terms of the GNU General Public License as published by
#the Free Software Foundation, either version 3 of the License, or
#(at your option) any later version.

#This program is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#GNU General Public License for more details.

#You should have received a copy of the GNU General Public License
#along with this program.  If not, see <http://www.gnu.org/licenses/>.

# Helper Delay function
proc delay { time } { set delayDone 0; after $time set delayDone 1; vwait delayDone; }
# Helper function load and return the contents of raptor.log
proc getLog {} {
    # Read log contents
    set fp [open "raptor.log" r]
    set file_data [read $fp]
    close $fp

    return $file_data
}
# Count the number of times a given regex occurs in raptor.log
proc strCount {regex} { return [regexp -all "$regex" [getLog]] }
# Repeat a given expr command string until the expression is true.
# The loop will delay 3sec between each try
proc repeatOnFalse { exprCmd {retries 5} {delayMs 3000} } {
    set retry 0
    set count 0
    set result 0

    while { !$result && $count < $retries} {
        set result [eval $exprCmd]
        incr count 1
        delay $delayMs
    }

    if { !$result } {
        # failed on last attempt
        puts "repeatOnFalse: Command '$exprCmd' failed $retries times. Retry limit reached."
        # exit 1
    }
    return $result
}

# open gtkwave
wave_open

# ensure GTKWave has started
set result [repeatOnFalse {expr [strCount "GTKWave - Interpreter id is"] == 1}]
if { !$result } {
    puts "FAILED to launch gtkwave process";
    exit 1
}

# try to open gtkwave when it's already open
wave_open

# ensure GTKWave invoke count remains at 1 because gtkwave is already open
set result [repeatOnFalse {expr [strCount "GTKWave - Interpreter id is"] == 1}]
if { !$result } {
    puts "FAILED: a new instance of gtkwave was created when one already existed";
    exit 1
}

# close gtkwave via the menu
wave_cmd gtkwave::/File/Quit

# ensure GTKWave closed
set result [repeatOnFalse {expr [strCount "GTKWave - Exiting"] == 1}]
if { !$result } {
    puts "FAILED: gtkwave failed to close";
    exit 1
}

# re-open gtkwave
wave_open

# ensure GTKWave invoke count is now 2 after closing and re-opening gtkwave
set result [repeatOnFalse {expr [strCount "GTKWave - Interpreter id is"] == 2}]
if { !$result } {
    puts "FAILED: gtkwave invoke count should be 2 at this point";
    exit 1
}

# Close GTKWave again, using repeatOnFalse here just to delay until close occures
wave_cmd gtkwave::/File/Quit
repeatOnFalse {expr [strCount "GTKWave - Exiting"] == 2}

# call wave_cmd to show that other wave_* commands invoke gtkwave as well
wave_cmd
# ensure GTKWave invoke count is now 3 after closing and re-opening gtkwave
set result [repeatOnFalse {expr [strCount "GTKWave - Interpreter id is"] == 3}]
if { !$result } {
    puts "FAILED: gtkwave invoke count should be 3 at this point";
    exit 1
}

# ensure no specific file has been opened yet
# Note: Gtkwave prints start/end times on load so we'll check for "start time"
set count [strCount "start time"]
if { $count != 0 } {
    puts "FAILED: A file should not have been opened yet"
}

# Open a vcd file
wave_open tests/TestGui/test.vcd

# ensure a file has been opened
set result [repeatOnFalse {expr [strCount "start time"] == 1}]
if { !$result } {
    puts "FAILED: wave_open tests/TestGui/test.vcd failed to open a file";
    exit 1
}

# refresh the wave file
wave_refresh

# ensure the file is reloaded
set result [repeatOnFalse {expr [strCount "start time"] == 2}]
if { !$result } {
    puts "FAILED: wave_refresh failed to reload the file";
    exit 1
}

# This will query and return the currently shown signal names in gtkwave
proc getSignals {} {
    # Gtkwave doesn't print all its values so tell it to print a specially formatted
    # string with our result and FOEDAG will parse and store the value which can be
    # access with wave_get_return
    wave_cmd {set signals [gtkwave::getDisplayedSignals];puts "_RETURN_$signals"}
    # give the command time to fire and be parsed before querying the return
    delay 2000
    return [wave_get_return]
}

# Ensure there are no signals currently
if { [expr { [getSignals] != ""}] } {
    puts "FAILED: gtkwave shouldn't be displaying any signals yet";
    exit 1
}

# Add a signal
wave_show top.co_sim_sdp_nsplit_ram_1024x36_R4W4.clk

# Ensure top.co_sim_sdp_nsplit_ram_1024x36_R4W4.clk is now displayed
set result [repeatOnFalse {expr {[getSignals] == "top.co_sim_sdp_nsplit_ram_1024x36_R4W4.clk"}}]
if { !$result } {
    puts "FAILED: wave_show top.co_sim_sdp_nsplit_ram_1024x36_R4W4.clk should have added a signal";
    exit 1
}

# This will query and return the current primary marker time
proc getMarkerTime {} {
    wave_cmd {set time [gtkwave::getMarker];puts "_RETURN_$time"}
    delay 2000
    return [wave_get_return]
}

# Ensure the primary marker is currently unset/-1
set result [repeatOnFalse {expr {[getMarkerTime] == -1}}]
if { !$result } {
    puts "FAILED: wave marker shouldn't be set";
    exit 1
}

# Change the marker time to 3ps
wave_time 3ps

# Ensure the marker time is now 3
set result [repeatOnFalse {expr {[getMarkerTime] == 3}}]
if { !$result } {
    puts "FAILED: wave marker should be set at 3ps";
    exit 1
}

# close gtkwave
wave_cmd gtkwave::/File/Quit

exit 0
