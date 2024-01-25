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

puts "GUI START" ; flush stdout ; gui_start
puts "GUI STOP"  ; flush stdout ; gui_stop
puts "GUI START" ; flush stdout ; gui_start
#puts "TEXT EDITOR GUI OPENFILE"  ; flush stdout ; openfile tests/TestGui/test.v
#puts "TEXT EDITOR GUI OPENFILE"  ; flush stdout ; openfile tests/TestGui/test.v

puts "NEW PROJECT START" ; flush stdout ; newproject_gui_open
puts "NEXT" ; flush stdout ; next
puts "NEXT" ; flush stdout ; next
puts "NEXT" ; flush stdout ; next
puts "NEXT" ; flush stdout ; next
puts "NEXT" ; flush stdout ; next
puts "NEW PROJECT STOP"  ; flush stdout ; newproject_gui_close

puts "GUI STOP"  ; flush stdout ; gui_stop


