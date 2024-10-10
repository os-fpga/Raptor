#Copyright (c) 2021-2024 Rapid Silicon

# All these are internal test target. They can't be invoked directly
# When creating new target here, please respect the pattern that test/int_something

PRIVATE_TARGETS := $(shell cat $(MAKEFILE_LIST) | grep -E '^test/int_[a-zA-Z0-9_-]+:' | sed 's/:.*//')
.ONESHELL:
.SILENT:

test/int_unittest:
	cmake --build build --target unittest -j $(CPU_CORES)
	pushd build && ctest --output-on-failure && popd

test/int_unittest-d:
	cmake --build dbuild --target unittest -j $(CPU_CORES)
	pushd dbuild && ctest --output-on-failure && popd

test/int_unittest-coverage: 
	cmake --build coverage-build --target unittest -j $(CPU_CORES)
	pushd coverage-build && ctest --output-on-failure && popd

test/int_valgrind:
	$(XVFB) valgrind --tool=memcheck --log-file=valgrind.log ./dbuild/bin/raptor --compiler dummy --replay tests/TestGui/gui_foedag.tcl
	grep "ERROR SUMMARY: 0" valgrind.log

test/int_install:
ifeq ($(RAPTOR_PUB),1)
	$(PREFIX)/bin/raptor --batch --script tests/tcl_examples/and2_verilog/run_raptor.tcl --device MPW1
else
	$(PREFIX)/bin/raptor --batch --script $(PREFIX)/share/raptor/tcl_examples/oneff_verilog/run_raptor.tcl
	$(PREFIX)/bin/raptor --batch --mute --script $(PREFIX)/share/raptor/tcl_examples/counter_vhdl/run_raptor.tcl
	$(PREFIX)/bin/raptor --batch --mute --script $(PREFIX)/share/raptor/tcl_examples/counter_verilog/run_raptor.tcl
	$(PREFIX)/bin/raptor --batch --mute --script $(PREFIX)/share/raptor/tcl_examples/aes_decrypt_verilog/run_raptor.tcl
	$(PREFIX)/bin/raptor --batch --mute --script $(PREFIX)/share/raptor/tcl_examples/aes_decrypt_gate/run_raptor.tcl
	$(PREFIX)/bin/raptor --batch --mute --script $(PREFIX)/share/raptor/tcl_examples/sasc_testcase/run_raptor.tcl
	$(PREFIX)/bin/raptor --batch --mute --script $(PREFIX)/share/raptor/tcl_examples/and2_verilog/run_raptor.tcl
endif

test/int_dgui:
ifeq ($(RAPTOR_PUB),1)
	$(XVFB) ./dbuild/bin/raptor --compiler dummy --replay tests/TestGui/gui_foedag.tcl
	$(XVFB) ./dbuild/bin/raptor --script tests/TestGui/gtkwave_cmds.tcl || (cat raptor.log; exit 1)
else
	$(XVFB) ./dbuild/bin/raptor --script tests/TestGui/gui_run_incr_comp_project.tcl
endif

test/int_dgui2:
ifeq ($(RAPTOR_PUB),1)
	$(XVFB) ./dbuild/bin/raptor --compiler dummy --replay tests/TestGui/gui_foedag.tcl
	$(XVFB) ./dbuild/bin/raptor --script tests/TestGui/gtkwave_cmds.tcl || (cat raptor.log; exit 1)
else
endif

test/int_raptor_gui:
	./build/bin/raptor --script tests/tcl_examples/and2_verilog/run_raptor.tcl --device MPW1

test/int_openfpga:
	./build/bin/raptor --batch --script tests/Testcases/aes_decrypt_fpga/aes_decrypt.tcl

test/int_openfpga_gui:
	./build/bin/raptor --script tests/Testcases/aes_decrypt_fpga/aes_decrypt.tcl

test/int_gui_mac:
#	$(XVFB) ./dbuild/bin/raptor --replay tests/TestGui/gui_start_stop.tcl
# Tests hanging on mac
#	$(XVFB) ./dbuild/bin/newproject --replay tests/TestGui/gui_new_project.tcl
#	$(XVFB) ./dbuild/bin/projnavigator --replay tests/TestGui/gui_project_navigator.tcl
#	$(XVFB) ./dbuild/bin/texteditor --replay tests/TestGui/gui_text_editor.tcl
#	$(XVFB) ./dbuild/bin/newfile --replay tests/TestGui/gui_new_file.tcl

# Do not invoke
test/int_batch:
	mkdir -p run_tests
	pushd run_tests
ifeq ($(RAPTOR_PUB),1)
	../build/bin/raptor --batch --script ../tests/tcl_examples/and2_verilog/run_raptor.tcl --device MPW1
else
	../build/bin/raptor --batch --mute --script ../tests/Testcases/and2_verilog/run_raptor.tcl
	../build/bin/raptor --batch --mute --script ../tests/Testcases/auto_testbench/raptor_lec.tcl
	../build/bin/raptor --batch --mute --script ../tests/Testcases/gen_clk/run_raptor.tcl
	../build/bin/raptor --batch --mute --script ../tests/Testcases/and2_2clks/run_raptor.tcl
	../build/bin/raptor --batch --mute --script ../tests/Testcases/and2_wio/run_raptor.tcl
	../build/bin/raptor --batch --mute --script ../tests/Testcases/and2_vec/run_raptor.tcl
	../build/bin/raptor --batch --mute --script ../tests/Testcases/oneff_wio/run_raptor.tcl
	../build/bin/raptor --batch --mute --script ../tests/Testcases/and2_compact/raptor.tcl
	../build/bin/raptor --batch --mute --script ../tests/Testcases/counter16/counter16.tcl
	../build/bin/raptor --batch --mute --script ../tests/Testcases/keep_test/raptor.tcl 
	../build/bin/raptor --batch --mute --script ../tests/Testcases/trivial/test.tcl
	../build/bin/raptor --batch --mute --script ../tests/Jira_Testcase/GEMINIEDA_96/build.tcl
	../build/bin/raptor --batch --mute --script ../tests/Jira_Testcase/GEMINIEDA_107/dsp_mul_unsigned_reg/raptor.tcl --device 1VG28
	../build/bin/raptor --batch --compiler dummy --mute --script ../tests/TestBatch/test_compiler_mt.tcl
	../build/bin/raptor --batch --compiler dummy --mute --script ../tests/TestBatch/test_compiler_batch.tcl
	../build/bin/raptor --batch --mute --script ../tests/Testcases/and2_gemini/raptor.tcl 
	../build/bin/raptor --batch --mute --script ../tests/Testcases/and2_gemini_no_pcf/raptor.tcl 
	../build/bin/raptor --batch --mute --script ../tests/Testcases/device_size_negative/raptor.tcl && exit 1 || (echo "PASSED: Caught negative test")
	../build/bin/raptor --batch --mute --script ../tests/Testcases/incr_comp/raptor.tcl 
	../build/bin/raptor --batch --mute --script ../tests/TestIP/axi_ram/v1_0/axi_ram.tcl
	../build/bin/raptor --batch --mute --script ../tests/TestIP/axi_register/v1_0/axi_register.tcl
	../build/bin/raptor --batch --mute --script ../tests/TestIP/axis_adapter/v1_0/axis_adapter.tcl
	../build/bin/raptor --batch --mute --script ../tests/TestIP/axi_cdma/v1_0/axi_cdma.tcl
	../build/bin/raptor --batch --mute --script ../tests/TestIP/axi_interconnect/v1_0/axi_interconnect.tcl
	../build/bin/raptor --batch --mute --script ../tests/TestIP/axil_gpio/v1_0/axil_gpio.tcl
	../build/bin/raptor --batch --mute --script ../tests/TestIP/reset_release/v1_0/reset_release.tcl
	../build/bin/raptor --batch --mute --script ../tests/TestIP/axi2axilite_bridge/v1_0/axi2axilite_bridge.tcl
	../build/bin/raptor --batch --mute --script ../tests/Testcases/constant/raptor.tcl
	../build/bin/raptor --batch --mute --script ../tests/Testcases/double_check/raptor.tcl
	../build/bin/raptor --batch --mute --script ../etc/devices/gemini_compact_62x44/ric/periphery.tcl
endif
	popd


test/int_batch_gen2:
	mkdir -p run_tests
	pushd run_tests
ifeq ($(RAPTOR_PUB),1)
else
	../build/bin/raptor --batch --mute --script ../tests/Testcases/aes_decrypt_fpga/aes_decrypt.tcl
	../build/bin/raptor --batch --mute --script ../tests/Testcases/aes_decrypt_gate/aes_decrypt_gate.tcl
	../build/bin/raptor --batch --mute --script ../tests/Testcases/oneff/raptor.tcl
	../build/bin/raptor --batch --mute --script ../tests/Testcases/counter/counter.tcl
	../build/bin/raptor --batch --mute --script ../tests/Testcases/counter_vhdl/raptor.tcl
	../build/bin/raptor --batch --mute --script ../tests/Testcases/counter_mixed/raptor.tcl
	../build/bin/raptor --batch --mute --script ../tests/TestBatch/oneff_clean/raptor.tcl
	../build/bin/raptor --batch --mute --script ../tests/Testcases/rom/raptor.tcl
endif
	popd

test/int_batch_gen3:
ifeq ($(RAPTOR_PUB),1)
else
	cd tests/Testcases/and2_bitstream; rm -rf and2; ../../../build/bin/raptor --batch --mute --script raptor.tcl; cd -
	cd tests/Testcases/up5bit_counter_dual_clock_bitstream; rm -rf up5bit_counter_dual_clock; ../../../build/bin/raptor --batch --mute --script raptor.tcl
endif


test/int_gjc_tests:
ifeq ($(RAPTOR_PUB),1)
else
	export PATH=$$PWD/build/bin:$$PATH && abspath=$$PWD && \
	cd tests/Testcases && rm -rf Validation && git clone --filter=blob:none --no-checkout https://github.com/os-fpga/Validation.git && cd Validation && \
	git sparse-checkout set RTL_testcases/GJC-IO-Testcases && git checkout @ && cd RTL_testcases/GJC-IO-Testcases && \
	for d in GJC*/; do \
		cd $$abspath/tests/Testcases/Validation/RTL_testcases/GJC-IO-Testcases/$$d; \
		if [[ -f "disabled.txt" ]]; then \
			echo "Skipping testcase: $$d"; \
		else \
			echo "Running testcase: $$d"; \
			./raptor_run.sh > /dev/null; \
			if grep -q '"status": "Fail"' results_dir/CGA_Result.json; then \
				echo "Test failed in $$d, exiting..."; \
				exit 1; \
			fi; \
		fi; \
	done
endif

test/int_solver:
	mkdir -p run_tests
	pushd run_tests
ifeq ($(RAPTOR_PUB),1)
else
	../build/bin/raptor --batch --mute --script ../tests/Testcases/partitioner_aes_verilog/run_raptor.tcl
endif
	popd

test/int_install_mac:
	$(PREFIX)/bin/raptor --compiler dummy --batch --script tests/Testcases/trivial/test.tcl

test/int_production:
	d_test=`echo $(PRODUCTION_DEVICES) | cut -d ',' -f 1` && \
	echo "Production device is $$d_test" && \
	./build/OPENLM_DIR/licensecc/extern/license-generator/src/license_generator/lccgen license issue -p projects/Raptor -f Raptor,$$d_test,DE -o build/bin/raptor.lic && \
	export LICENSE_LOCATION=$$PWD/build/bin/raptor.lic && \
	./build/bin/raptor --batch --script tests/tcl_examples/and2_verilog/run_raptor.tcl --device $$d_test

ifneq ($(MAKECMDGOALS), test/batch_all)
  ifneq ($(filter $(MAKECMDGOALS), $(PRIVATE_TARGETS)),)
    ifneq ($(CI), true)
      $(error You cannot invoke '$(MAKECMDGOALS)' directly. Please see Makefile for proper test target)
    endif
  endif
endif

