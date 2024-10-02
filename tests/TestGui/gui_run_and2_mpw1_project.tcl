#Copyright 2021 The Foedag team

#GPL License

#Copyright (c) 2021 The Open-Source FPGA Foundation

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

puts "RUNNING PROJECT"
run_project examples/and2_gemini/and2_gemini.ospr
set fp [open "raptor.log" r]
set file_data [read $fp]
close $fp
# Report an error if project run wasn't successful
set found [regexp "Project run successful" $file_data]
if { !$found } {
     puts "TEST FAILED: incr_comp.ospr run failed."
     exit 1
}

puts "TEST SUCCEEDED"

exit 0
