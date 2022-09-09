set script_path [ file dirname [ file normalize [ info script ] ] ]
source $script_path/copy.tcl

set projName incr_comp
set tempFile $projName/tmp.v

create_design $projName
target_device GEMINI

file copy -force $script_path/and2.v $tempFile

add_design_file $tempFile
set_top_module and2
# cleanup before test
synthesize clean
synthesize

file copy -force $projName/incr_comp_post_synth.blif $projName/incr_comp_post_synth.blif1
# wait 1 sec since mtime will be the same for $tempFile and *.blif
after 1000
copyContent $script_path/and2_modified.v $tempFile
synthesize
file copy -force $projName/incr_comp_post_synth.blif $projName/incr_comp_post_synth.blif2
set diffresult [catch {exec sh -c "diff $projName/incr_comp_post_synth.blif1 $projName/incr_comp_post_synth.blif2"} diffresult] 
if {$diffresult == 0} {
    error "Synthesis didn't resynthesized"
}
