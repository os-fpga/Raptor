default:
	echo "make {synth,timing}"

dirs:
	mkdir -p WORK
	mkdir -p report

synth: dirs
	script --flush --command 'dc_shell-t -f synth.tcl' report/$(shell date +"%Y_%m_%d_%I_%M_%p").log

grid_dsp: dirs
	script --flush --command 'dc_shell-t -f grid_dsp.tcl' report/$(shell date +"%Y_%m_%d_%I_%M_%p").log

fabric_top: dirs
	script --flush --command 'dc_shell-t -f fabric_top.tcl' report/$(shell date +"%Y_%m_%d_%I_%M_%p").log

timing: dirs
	script --flush --command 'dc_shell-t -f timing.tcl' report/$(shell date +"%Y_%m_%d_%I_%M_%p").log

clean:
	rm -rf WORK/*
	rm -rf report/*
