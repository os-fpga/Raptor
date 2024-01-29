set script_path [ file dirname [ file normalize [ info script ] ] ]
source $script_path/copy.tcl

set projName incr_comp
set tempFile $projName/tmp.v

create_design $projName
target_device GEMINI_COMPACT_10x8

file copy -force $script_path/and2.v $tempFile

add_design_file $tempFile
set_top_module and2
# cleanup before test
synthesize clean
synthesize

set blifPath $projName/run_1/synth_1_1/synthesis
file copy -force $blifPath/incr_comp_post_synth.eblif $blifPath/incr_comp_post_synth.eblif1
# wait 1 sec since mtime will be the same for $tempFile and *.blif
after 1000
copyContent $script_path/and2_modified.v $tempFile
synthesize
file copy -force $blifPath/incr_comp_post_synth.eblif $blifPath/incr_comp_post_synth.eblif2
set diffresult [catch {exec sh -c "diff $blifPath/incr_comp_post_synth.eblif1 $blifPath/incr_comp_post_synth.eblif2"} diffresult] 
if {$diffresult == 0} {
    error "Synthesis didn't resynthesized"
}
