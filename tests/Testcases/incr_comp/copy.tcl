# copy content for fileSource into fileDest. Files must exist
proc copyContent {fileSource fileDest} {
	set fp [open $fileSource r]
	set file_data [read $fp]
	close $fp
	set fo [open $fileDest w]
	puts $fo $file_data
	close $fo
}
