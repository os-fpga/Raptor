default:
	echo "make {synth,timing}"

dirs:
	mkdir -p WORK
	mkdir -p report

ql_clkmux: dirs
	script --flush --command 'dc_shell-t -f ql_clkmux.tcl' report/$(shell date +"%Y_%m_%d_%I_%M_%p").log

synth: dirs
	script --flush --command 'dc_shell-t -f synth.tcl' report/$(shell date +"%Y_%m_%d_%I_%M_%p").log

timing: dirs
	script --flush --command 'dc_shell-t -f timing.tcl' report/$(shell date +"%Y_%m_%d_%I_%M_%p").log

grid_bram: dirs
	script --flush --command 'dc_shell-t -f grid_bram.tcl' report/$(shell date +"%Y_%m_%d_%I_%M_%p").log

clean:
	rm -rf WORK/* report/*
