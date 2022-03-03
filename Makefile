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

# If 'on', then the progress messages are printed. If 'off', makes it easier
# to detect actual warnings and errors  in the build output.
RULE_MESSAGES ?= on

release: run-cmake-release
	cmake --build build -j $(CPU_CORES)

release_no_tcmalloc: run-cmake-release_no_tcmalloc
	cmake --build build -j $(CPU_CORES)

debug: run-cmake-debug
	cmake --build dbuild -j $(CPU_CORES)

run-cmake-release:
	cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$(PREFIX) -DCMAKE_RULE_MESSAGES=$(RULE_MESSAGES) $(ADDITIONAL_CMAKE_OPTIONS) -S . -B build

run-cmake-release_no_tcmalloc:
	cmake -DNO_TCMALLOC=On -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$(PREFIX) -DCMAKE_RULE_MESSAGES=$(RULE_MESSAGES) $(ADDITIONAL_CMAKE_OPTIONS) -S . -B build

run-cmake-debug:
	cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=$(PREFIX) -DCMAKE_RULE_MESSAGES=$(RULE_MESSAGES) -DNO_TCMALLOC=On $(ADDITIONAL_CMAKE_OPTIONS) -S . -B dbuild

run-cmake-coverage:
	cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=$(PREFIX) -DCMAKE_RULE_MESSAGES=$(RULE_MESSAGES) -DMY_CXX_WARNING_FLAGS="--coverage" $(ADDITIONAL_CMAKE_OPTIONS) -S . -B coverage-build

test/unittest: run-cmake-release
	cmake --build build --target UnitTests -j $(CPU_CORES)
	pushd build && ctest --output-on-failure && popd

test/unittest-d: run-cmake-debug
	cmake --build dbuild --target UnitTests -j $(CPU_CORES)
	pushd dbuild && ctest --output-on-failure && popd

test/unittest-coverage: run-cmake-coverage
	cmake --build coverage-build --target UnitTests -j $(CPU_CORES)
	pushd coverage-build && ctest --output-on-failure && popd

coverage-build/raptor_gui.coverage: test/unittest-coverage
	lcov --no-external --exclude "*_test.cpp" --capture --directory coverage-build/CMakeFiles/raptor_gui.dir --base-directory src --output-file coverage-build/raptor_gui.coverage

coverage-build/html: raptor_gui-build/raptor_gui.coverage
	genhtml --output-directory coverage-build/html $^
	realpath coverage-build/html/index.html

test/regression: run-cmake-release

test/valgrind: run-cmake-debug
#	valgrind --tool=memcheck --log-file=valgrind.log dbuild/bin/raptor_gui --noqt --script tests/TestBatch/hello.tcl ; 
#	grep "ERROR SUMMARY: 0" valgrind.log
#	$(XVFB) valgrind --tool=memcheck --log-file=valgrind_gui.log dbuild/bin/raptor_gui --replay tests/TestGui/gui_start_stop.tcl;
#	grep "ERROR SUMMARY: 0" valgrind_gui.log 
#	$(XVFB) valgrind --tool=memcheck --log-file=valgrind_gui.log dbuild/bin/newproject --replay tests/TestGui/gui_new_project.tcl
#	grep "ERROR SUMMARY: 0" valgrind_gui.log
#	$(XVFB) valgrind --tool=memcheck --log-file=valgrind_gui.log dbuild/bin/projnavigator --replay tests/TestGui/gui_project_navigator.tcl
#	grep "ERROR SUMMARY: 0" valgrind_gui.log
#	$(XVFB) valgrind --tool=memcheck --log-file=valgrind_gui.log dbuild/bin/texteditor --replay tests/TestGui/gui_text_editor.tcl
#	grep "ERROR SUMMARY: 0" valgrind_gui.log
#	$(XVFB) valgrind --tool=memcheck --log-file=valgrind_gui.log dbuild/bin/newfile --replay tests/TestGui/gui_new_file.tcl
#	grep "ERROR SUMMARY: 0" valgrind_gui.log
#	$(XVFB) valgrind --tool=memcheck --log-file=valgrind_gui.log dbuild/bin/console_test --replay tests/TestGui/gui_console.tcl
#	grep "ERROR SUMMARY: 0" valgrind_gui.log


test: test/unittest test/regression

test-parallel: release test/unittest
#	cmake -E make_directory build/tests
#	cmake -E remove_directory build/test
#	cmake -E make_directory build/test
#	cmake -S build/test -B build/test/build
#	pushd build && cmake --build test/build -j $(CPU_CORES) && popd


regression: release
#	cmake -E make_directory build/tests
#	cmake -E remove_directory build/test
#	cmake -E make_directory build/test
#	cmake -S build/test -B build/test/build
#	pushd build && cmake --build test/build -j $(CPU_CORES) && popd

clean:
	$(RM) -r build dbuild coverage-build dist tests/TestInstall/build

install: release
	cmake --install build

test_install:
	cmake -DCMAKE_BUILD_TYPE=Release -DINSTALL_DIR=$(PREFIX) -S tests/TestInstall -B tests/TestInstall/build
	cmake --build tests/TestInstall/build -j $(CPU_CORES)

test/gui: run-cmake-debug
#	$(XVFB) ./dbuild/bin/console_test --replay tests/TestGui/gui_console.tcl
#	$(XVFB) ./dbuild/bin/console_test --replay tests/TestGui/gui_console_negative_test.tcl && exit 1 || (echo "PASSED: Caught negative test")
#	$(XVFB) ./dbuild/bin/raptor_gui --replay tests/TestGui/gui_start_stop.tcl
#	$(XVFB) ./dbuild/bin/newproject --replay tests/TestGui/gui_new_project.tcl
#	$(XVFB) ./dbuild/bin/projnavigator --replay tests/TestGui/gui_project_navigator.tcl
#	$(XVFB) ./dbuild/bin/texteditor --replay tests/TestGui/gui_text_editor.tcl
#	$(XVFB) ./dbuild/bin/newfile --replay tests/TestGui/gui_new_file.tcl

test/gui_mac: run-cmake-debug
#	$(XVFB) ./dbuild/bin/raptor_gui --replay tests/TestGui/gui_start_stop.tcl
# Tests hanging on mac
#	$(XVFB) ./dbuild/bin/newproject --replay tests/TestGui/gui_new_project.tcl
#	$(XVFB) ./dbuild/bin/projnavigator --replay tests/TestGui/gui_project_navigator.tcl
#	$(XVFB) ./dbuild/bin/texteditor --replay tests/TestGui/gui_text_editor.tcl
#	$(XVFB) ./dbuild/bin/newfile --replay tests/TestGui/gui_new_file.tcl

test/batch: run-cmake-release
#	./build/bin/compiler_test --noqt --script tests/TestBatch/test_compiler_mt.tcl
#	./build/bin/compiler_test --noqt --script tests/TestBatch/test_compiler_batch.tcl

lib-only: run-cmake-release
	cmake --build build --target raptor_gui -j $(CPU_CORES)

format:
	.github/bin/run-clang-format.sh

help:
	build/bin/raptor_gui --help > docs/source/help/help.txt

doc:
	cd docs && make html
	cd -

uninstall:
	$(RM) -r $(PREFIX)/bin/raptor_gui
	$(RM) -r $(PREFIX)/lib/raptor_gui
	$(RM) -r $(PREFIX)/include/raptor_gui

