gtkwave::loadFile "and2/and2.vcd" 

set cntrl_sigs {reset clk}
set input_sigs {a b}
set delayed_input_sigs {a_delay_by_2 b_delay_by_2}
set output_sigs {c}

set cntrl_facs ""
set input_facs ""
set delayed_input_facs ""
set output_facs ""

foreach sig $cntrl_sigs {
  for {set i 0} {$i<[gtkwave::getNumFacs]} {incr i} {
    set test [split [gtkwave::getFacName $i] .]
    if {[llength $test]==2} {
      if {$sig == [lindex $test end]} {
        lappend cntrl_facs [gtkwave::getFacName $i]
      }
    }
  }
}

foreach sig $input_sigs {
  for {set i 0} {$i<[gtkwave::getNumFacs]} {incr i} {
    set test [split [gtkwave::getFacName $i] .]
    if {[llength $test]==2} {
      if {$sig == [lindex $test end]} {
        lappend input_facs [gtkwave::getFacName $i]
      }
    }
  }
}

foreach sig $delayed_input_sigs {
  for {set i 0} {$i<[gtkwave::getNumFacs]} {incr i} {
    set test [split [gtkwave::getFacName $i] .]
    if {[llength $test]==2} {
      if {$sig == [lindex $test end]} {
        lappend delayed_input_facs [gtkwave::getFacName $i]
      }
    }
  }
}

foreach sig $output_sigs {
  for {set i 0} {$i<[gtkwave::getNumFacs]} {incr i} {
    set test [split [gtkwave::getFacName $i] .]
    if {[llength $test]==2} {
      if {$sig == [lindex $test end]} {
        lappend output_facs [gtkwave::getFacName $i]
      }
    }
  }
}


gtkwave::/Edit/Color_Format/Red
gtkwave::/Edit/Insert_Comment "Control Signals"
gtkwave::addSignalsFromList $cntrl_facs
gtkwave::highlightSignalsFromList $cntrl_facs
gtkwave::/Edit/Color_Format/Red
gtkwave::/Edit/UnHighlight_All
gtkwave::/Edit/Insert_Blank
gtkwave::/Edit/Insert_Comment "Inputs"
gtkwave::addSignalsFromList $input_facs
gtkwave::highlightSignalsFromList $input_facs
gtkwave::/Edit/Color_Format/Green
gtkwave::/Edit/UnHighlight_All
gtkwave::/Edit/Insert_Blank
gtkwave::/Edit/Insert_Comment "Delayed Inputs"
gtkwave::addSignalsFromList $delayed_input_facs
gtkwave::highlightSignalsFromList $delayed_input_facs
gtkwave::/Edit/Color_Format/Yellow
gtkwave::/Edit/UnHighlight_All
gtkwave::/Edit/Insert_Blank
gtkwave::/Edit/Insert_Comment "Output"
gtkwave::addSignalsFromList $output_facs
gtkwave::highlightSignalsFromList $output_facs
gtkwave::/Edit/Color_Format/Blue
gtkwave::/Edit/UnHighlight_All

gtkwave::/Time/Zoom/Zoom_Full

gtkwave::setNamedMarker A 100 "Reset Released"

# gtkwave::setWindowStartTime 100

# gtkwave::setZoomRangeTimes 100 230

# gtkwave::loadFile filename

    # gtkwave::/Edit/Insert_Comment $filter
    # gtkwave::addSignalsFromList $monitorSignals
    # gtkwave::/Edit/Insert_Blank

    # gtkwave::/Edit/Color_Format/$color
    # gtkwave::/Edit/UnHighlight_All

