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


puts "RUN PROJECT START" ; flush stdout ; gui_start
puts "AES_DECRYPT RUN" ; flush stdout ; run_project examples/AES_DECRYPT/AES_DECRYPT.ospr
puts "and2_gemini RUN" ; flush stdout ; run_project examples/and2_gemini/and2_gemini.ospr
puts "incr_comp RUN" ; flush stdout ; run_project examples/incr_comp/incr_comp.ospr
puts "sasc_testcase RUN" ; flush stdout ; run_project examples/sasc_testcase/sasc_testcase.ospr
puts "RUN PROJECT STOP"  ; flush stdout ; gui_stop


