#Copyright (c) 2021-2022 RapiSilicon

# Use bash as the default shell
SHELL := /bin/bash

XVFB = xvfb-run --auto-servernum --server-args="-screen 0, 1280x1024x24"

ifdef $(LC_ALL)
	undefine LC_ALL
endif

ifeq ($(CPU_CORES),)
	CPU_CORES := $(shell nproc)
	ifeq ($(CPU_CORES),)
		CPU_CORES := $(shell sysctl -n hw.physicalcpu)
	endif
	ifeq ($(CPU_CORES),)
		CPU_CORES := 2  # Good minimum assumption
	endif
endif

PREFIX ?= /usr/local
ADDITIONAL_CMAKE_OPTIONS ?=
# make MONACO_EDITOR=0 enables the QScintilla based Editor in place of the WebEngine based Monaco Editor
MONACO_EDITOR ?= 1

# If 'on', then the progress messages are printed. If 'off', makes it easier
# to detect actual warnings and errors  in the build output.
RULE_MESSAGES ?= on

ifeq ($(PRODUCTION_BUILD),1)
release: run-cmake-release
	cmake --build build -j $(CPU_CORES)
else
release: run-cmake-release
	cmake --build build -j $(CPU_CORES)
endif

release_no_tcmalloc: run-cmake-release_no_tcmalloc
	cmake --build build -j $(CPU_CORES)

debug: run-cmake-debug
	cmake --build dbuild -j $(CPU_CORES)

run-cmake-release:
	cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$(PREFIX) -DCPU_CORES=$(CPU_CORES) -DCMAKE_RULE_MESSAGES=$(RULE_MESSAGES) -DPRODUCTION_BUILD=$(PRODUCTION_BUILD) -DUPDATE_SUBMODULES=$(UPDATE_SUBMODULES) -DPRODUCTION_DEVICES=${PRODUCTION_DEVICES} -DSTICK_RELEASE_VERSION=$(STICK_RELEASE_VERSION) -DUSE_DE_SRC=$(USE_DE_SRC)  $(ADDITIONAL_CMAKE_OPTIONS) -DMONACO_EDITOR=$(MONACO_EDITOR) -S . -B build

run-cmake-release_no_tcmalloc:
	cmake -DNO_TCMALLOC=On -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$(PREFIX) -DCPU_CORES=$(CPU_CORES) -DCMAKE_RULE_MESSAGES=$(RULE_MESSAGES) -DUPDATE_SUBMODULES=$(UPDATE_SUBMODULES) -DSTICK_RELEASE_VERSION=$(STICK_RELEASE_VERSION)  $(ADDITIONAL_CMAKE_OPTIONS) -DMONACO_EDITOR=$(MONACO_EDITOR) -S . -B build

run-cmake-debug:
	cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=$(PREFIX) -DCPU_CORES=$(CPU_CORES) -DCMAKE_RULE_MESSAGES=$(RULE_MESSAGES) -DPRODUCTION_BUILD=$(PRODUCTION_BUILD) -DNO_TCMALLOC=On -DUPDATE_SUBMODULES=$(UPDATE_SUBMODULES) -DSTICK_RELEASE_VERSION=$(STICK_RELEASE_VERSION)  $(ADDITIONAL_CMAKE_OPTIONS) -DMONACO_EDITOR=$(MONACO_EDITOR) -S . -B dbuild

run-cmake-coverage:
	cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=$(PREFIX) -DCPU_CORES=$(CPU_CORES) -DCMAKE_RULE_MESSAGES=$(RULE_MESSAGES) -DMY_CXX_WARNING_FLAGS="--coverage" -DUPDATE_SUBMODULES=$(UPDATE_SUBMODULES) -DSTICK_RELEASE_VERSION=$(STICK_RELEASE_VERSION)  $(ADDITIONAL_CMAKE_OPTIONS) -DMONACO_EDITOR=$(MONACO_EDITOR) -S . -B coverage-build

test/unittest: release
	cmake --build build --target unittest -j $(CPU_CORES)
	pushd build && ctest --output-on-failure && popd

test/unittest-d: run-cmake-debug
	cmake --build dbuild --target unittest -j $(CPU_CORES)
	pushd dbuild && ctest --output-on-failure && popd

test/unittest-coverage: run-cmake-coverage
	cmake --build coverage-build --target unittest -j $(CPU_CORES)
	pushd coverage-build && ctest --output-on-failure && popd

coverage-build/raptor_gui.coverage: test/unittest-coverage
	lcov --no-external --exclude "*_test.cpp" --capture --directory coverage-build/CMakeFiles/raptor_gui.dir --base-directory src --output-file coverage-build/raptor_gui.coverage

coverage-build/html: raptor_gui-build/raptor_gui.coverage
	genhtml --output-directory coverage-build/html $^
	realpath coverage-build/html/index.html

test/regression: run-cmake-release

test/valgrind: run-cmake-debug
	$(XVFB) valgrind --tool=memcheck --log-file=valgrind.log ./dbuild/bin/raptor --compiler dummy --replay tests/TestGui/gui_foedag.tcl
	grep "ERROR SUMMARY: 0" valgrind.log


test: test/unittest test/regression

test-parallel: release test/unittest

regression: release


clean:
	$(RM) -r build dbuild coverage-build dist tests/TestInstall/build

ifeq ($(PRODUCTION_BUILD),1)
install: release
	cmake --install build
	$(PREFIX)/share/envs/litex/bin/python3 gen_rel_device.py --production_devices ${PRODUCTION_DEVICES} --xml_filepath $(PREFIX)/share/raptor/etc/device.xml --devices_dirs_path $(PREFIX)/share/raptor/etc/devices --examples_path $(PREFIX)/share/raptor/examples
	mv $(PREFIX)/share/raptor/etc/settings/messages/suppress-rel.json $(PREFIX)/share/raptor/etc/settings/messages/suppress.json
	rm -rf $(PREFIX)/share/raptor/sim_models/rapidsilicon/genesis3/RS_PRIMITIVES/IO/IO_MODELS
else
install: release
	cmake --install build
endif

test_install_mac:
	find /Users/runner/work/Raptor/ -name "*QtWidgets*" -print
	otool -L $(PREFIX)/bin/raptor
	install_name_tool -change @rpath/QtWidgets.framework/Versions/5/QtWidgets /Users/runner/work/Raptor/Qt/5.15.2/clang_64/lib/QtWidgets.framework/QtWidgets $(PREFIX)/bin/raptor
	install_name_tool -change @rpath/QtCore.framework/Versions/5/QtCore /Users/runner/work/Raptor/Qt/5.15.2/clang_64/lib/QtCore.framework/QtCore $(PREFIX)/bin/raptor
	install_name_tool -change @rpath/QtGui.framework/Versions/5/QtGui /Users/runner/work/Raptor/Qt/5.15.2/clang_64/lib/QtGui.framework/QtGui $(PREFIX)/bin/raptor
	install_name_tool -change @rpath/QtXml.framework/Versions/5/QtXml /Users/runner/work/Raptor/Qt/5.15.2/clang_64/lib/QtXml.framework/QtXml $(PREFIX)/bin/raptor
	install_name_tool -change @rpath/QtQuick.framework/Versions/5/QtQuick /Users/runner/work/Raptor/Qt/5.15.2/clang_64/lib/QtQuick.framework/QtQuick $(PREFIX)/bin/raptor
	install_name_tool -change @rpath/QtQmlModels.framework/Versions/5/QtQmlModels /Users/runner/work/Raptor/Qt/5.15.2/clang_64/lib/QtQmlModels.framework/QtQmlModels $(PREFIX)/bin/raptor
	install_name_tool -change @rpath/QtQml.framework/Versions/5/QtQml /Users/runner/work/Raptor/Qt/5.15.2/clang_64/lib/QtQml.framework/QtQml $(PREFIX)/bin/raptor
	install_name_tool -change @rpath/QtNetwork.framework/Versions/5/QtNetwork /Users/runner/work/Raptor/Qt/5.15.2/clang_64/lib/QtNetwork.framework/QtNetwork $(PREFIX)/bin/raptor
	$(PREFIX)/bin/raptor --compiler dummy --batch --script tests/Testcases/trivial/test.tcl

test_install:
ifeq ($(RAPTOR_PUB),1)
else
	$(PREFIX)/bin/raptor --batch --mute --script $(PREFIX)/share/raptor/tcl_examples/oneff_verilog/run_raptor.tcl
	$(PREFIX)/bin/raptor --batch --mute --script $(PREFIX)/share/raptor/tcl_examples/counter_vhdl/run_raptor.tcl
	$(PREFIX)/bin/raptor --batch --mute --script $(PREFIX)/share/raptor/tcl_examples/counter_verilog/run_raptor.tcl
	$(PREFIX)/bin/raptor --batch --mute --script $(PREFIX)/share/raptor/tcl_examples/aes_decrypt_verilog/run_raptor.tcl
# Disable until Verilog read in VPR	$(PREFIX)/bin/raptor --batch --mute --script $(PREFIX)/share/raptor/tcl_examples/aes_decrypt_gate/run_raptor.tcl
	$(PREFIX)/bin/raptor --batch --mute --script $(PREFIX)/share/raptor/tcl_examples/sasc_testcase/run_raptor.tcl
	$(PREFIX)/bin/raptor --batch --mute --script $(PREFIX)/share/raptor/tcl_examples/and2_verilog/run_raptor.tcl
endif

test/gui:
ifeq ($(RAPTOR_PUB),1)
	$(XVFB) ./dbuild/bin/raptor --compiler dummy --replay tests/TestGui/gui_foedag.tcl
	$(XVFB) ./dbuild/bin/raptor --script tests/TestGui/gtkwave_cmds.tcl || (cat raptor.log; exit 1)
else
	$(XVFB) ./dbuild/bin/raptor --compiler dummy --replay tests/TestGui/gui_foedag.tcl
	$(XVFB) ./dbuild/bin/raptor --script tests/TestGui/gtkwave_cmds.tcl || (cat raptor.log; exit 1)
	$(XVFB) ./dbuild/bin/raptor --script tests/TestGui/gui_run_incr_comp_project.tcl
endif

test/rs: run-cmake-release
	./build/bin/raptor --batch --script tests/Testcases/aes_decrypt_fpga/aes_decrypt.tcl

test/rs_gui: run-cmake-release
	./build/bin/raptor --script tests/Testcases/aes_decrypt_fpga/aes_decrypt.tcl

test/openfpga: run-cmake-release
	./build/bin/raptor --batch --script tests/Testcases/aes_decrypt_fpga/aes_decrypt.tcl

test/openfpga_gui: run-cmake-release
	./build/bin/raptor --script tests/Testcases/aes_decrypt_fpga/aes_decrypt.tcl

test/gui_mac: run-cmake-debug
#	$(XVFB) ./dbuild/bin/raptor --replay tests/TestGui/gui_start_stop.tcl
# Tests hanging on mac
#	$(XVFB) ./dbuild/bin/newproject --replay tests/TestGui/gui_new_project.tcl
#	$(XVFB) ./dbuild/bin/projnavigator --replay tests/TestGui/gui_project_navigator.tcl
#	$(XVFB) ./dbuild/bin/texteditor --replay tests/TestGui/gui_text_editor.tcl
#	$(XVFB) ./dbuild/bin/newfile --replay tests/TestGui/gui_new_file.tcl

test/batch: run-cmake-release
ifeq ($(RAPTOR_PUB),1)
else
	./build/bin/raptor --batch  --script tests/Testcases/and2_verilog/run_raptor.tcl
	./build/bin/raptor --batch --mute --script tests/Testcases/and2_compact/raptor.tcl
	./build/bin/raptor --batch --mute --script tests/Testcases/counter16/counter16.tcl
# Needs https://github.com/chipsalliance/systemverilog-plugin/issues/1892	./build/bin/raptor --batch --mute --script tests/Testcases/vex_soc/raptor_vex_no_carry.tcl
	./build/bin/raptor --batch --mute --script tests/Testcases/keep_test/raptor.tcl 
	./build/bin/raptor --batch --mute --script tests/Testcases/trivial/test.tcl
	./build/bin/raptor --batch --mute --script tests/Jira_Testcase/GEMINIEDA_96/build.tcl
	./build/bin/raptor --batch --mute --script tests/Jira_Testcase/GEMINIEDA_107/dsp_mul_unsigned_reg/raptor.tcl --device 1GE75
	./build/bin/raptor --batch --compiler dummy --mute --script tests/TestBatch/test_compiler_mt.tcl
	./build/bin/raptor --batch --compiler dummy --mute --script tests/TestBatch/test_compiler_batch.tcl
	./build/bin/raptor --batch --mute --script tests/Testcases/and2_gemini/raptor.tcl 
	./build/bin/raptor --batch --mute --script tests/Testcases/and2_gemini_no_pcf/raptor.tcl 
	./build/bin/raptor --batch --mute --script tests/Testcases/device_size_negative/raptor.tcl && exit 1 || (echo "PASSED: Caught negative test")
	./build/bin/raptor --batch --mute --script tests/Testcases/incr_comp/raptor.tcl 
	./build/bin/raptor --batch --mute --script tests/TestIP/axi_ram/v1_0/axi_ram.tcl
	./build/bin/raptor --batch --mute --script tests/TestIP/axi_register/v1_0/axi_register.tcl
	./build/bin/raptor --batch --mute --script tests/TestIP/axis_adapter/v1_0/axis_adapter.tcl
	./build/bin/raptor --batch --mute --script tests/TestIP/axi_cdma/v1_0/axi_cdma.tcl
	./build/bin/raptor --batch --mute --script tests/TestIP/axi_interconnect/v1_0/axi_interconnect.tcl
	./build/bin/raptor --batch --mute --script tests/TestIP/axil_gpio/v1_0/axil_gpio.tcl
	./build/bin/raptor --batch --mute --script tests/TestIP/reset_release/v1_0/reset_release.tcl
	./build/bin/raptor --batch --mute --script tests/TestIP/axi2axilite_bridge/v1_0/axi2axilite_bridge.tcl
	./build/bin/raptor --batch --mute --script tests/Testcases/constant/raptor.tcl --batch
	./build/bin/raptor --batch --mute --script tests/Testcases/double_check/raptor.tcl
	./build/bin/raptor --batch --mute --script etc/devices/gemini_compact_62x44/ric/virgotc_bank.tcl
endif

test/batch_gen2: run-cmake-release
ifeq ($(RAPTOR_PUB),1)
else
	./build/bin/raptor --batch --mute --script tests/Testcases/aes_decrypt_fpga/aes_decrypt.tcl
	./build/bin/raptor --batch --mute --script tests/Testcases/oneff/raptor.tcl
	./build/bin/raptor --batch --mute --script tests/Testcases/counter/counter.tcl
	./build/bin/raptor --batch --mute --script tests/Testcases/counter_vhdl/raptor.tcl
	./build/bin/raptor --batch --mute --script tests/Testcases/counter_mixed/raptor.tcl
	./build/bin/raptor --batch --mute --script tests/TestBatch/oneff_clean/raptor.tcl
	./build/bin/raptor --batch --mute --script tests/Testcases/rom/raptor.tcl
endif
solver/tests: release
ifeq ($(RAPTOR_PUB),1)
else
	./build/bin/raptor --batch  --script tests/Testcases/partitioner_aes_verilog/run_raptor.tcl
endif

lib-only: run-cmake-release
	cmake --build build --target raptor_gui -j $(CPU_CORES)

format:
	.github/bin/run-clang-format.sh

help:
	build/bin/raptor --help > docs/source/help/help.txt

doc:
	cd docs && make html
	cd -

uninstall:
	$(RM) -r $(PREFIX)/bin/raptor
	$(RM) -r $(PREFIX)/bin/raptor.exe
	$(RM) -r $(PREFIX)/lib/raptor
	$(RM) -r $(PREFIX)/include/raptor
	$(RM) -r $(PREFIX)/share/raptor
	$(RM) -r $(PREFIX)/bin/gtkwave

