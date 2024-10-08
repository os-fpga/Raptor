#Copyright (c) 2021-2024 Rapid Silicon

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
IS_WSL_OR_DOCKER := $(shell \
    if [[ -s /etc/wsl.conf ]] || \
       grep -qi "wsl2" /proc/version || \
       uname -r | grep -qi "wsl2" || \
       [ -n "$$WSL_DISTRO_NAME" ] || \
       which powershell.exe &> /dev/null || \
       [ -f "/.dockerenv" ]; then \
        echo 0; \
    else \
        echo 1; \
    fi \
)
MONACO_EDITOR ?= $(IS_WSL_OR_DOCKER)

IPA ?= 1

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
	cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$(PREFIX) -DCPU_CORES=$(CPU_CORES) -DCMAKE_RULE_MESSAGES=$(RULE_MESSAGES) -DPRODUCTION_BUILD=$(PRODUCTION_BUILD) -DUPDATE_SUBMODULES=$(UPDATE_SUBMODULES) -DPRODUCTION_DEVICES=${PRODUCTION_DEVICES} -DSTICK_RELEASE_VERSION=$(STICK_RELEASE_VERSION) -DUSE_DE_SRC=$(USE_DE_SRC)  $(ADDITIONAL_CMAKE_OPTIONS) -DMONACO_EDITOR=$(MONACO_EDITOR) -DIPA=$(IPA) -S . -B build

run-cmake-release_no_tcmalloc:
	cmake -DNO_TCMALLOC=On -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$(PREFIX) -DCPU_CORES=$(CPU_CORES) -DCMAKE_RULE_MESSAGES=$(RULE_MESSAGES) -DUPDATE_SUBMODULES=$(UPDATE_SUBMODULES) -DSTICK_RELEASE_VERSION=$(STICK_RELEASE_VERSION)  $(ADDITIONAL_CMAKE_OPTIONS) -DMONACO_EDITOR=$(MONACO_EDITOR) -DIPA=$(IPA) -S . -B build

run-cmake-debug:
	cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=$(PREFIX) -DCPU_CORES=$(CPU_CORES) -DCMAKE_RULE_MESSAGES=$(RULE_MESSAGES) -DPRODUCTION_BUILD=$(PRODUCTION_BUILD) -DNO_TCMALLOC=On -DUPDATE_SUBMODULES=$(UPDATE_SUBMODULES) -DSTICK_RELEASE_VERSION=$(STICK_RELEASE_VERSION)  $(ADDITIONAL_CMAKE_OPTIONS) -DMONACO_EDITOR=$(MONACO_EDITOR) -DIPA=$(IPA) -S . -B dbuild

run-cmake-coverage:
	cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=$(PREFIX) -DCPU_CORES=$(CPU_CORES) -DCMAKE_RULE_MESSAGES=$(RULE_MESSAGES) -DMY_CXX_WARNING_FLAGS="--coverage" -DUPDATE_SUBMODULES=$(UPDATE_SUBMODULES) -DSTICK_RELEASE_VERSION=$(STICK_RELEASE_VERSION)  $(ADDITIONAL_CMAKE_OPTIONS) -DMONACO_EDITOR=$(MONACO_EDITOR) -DIPA=$(IPA) -S . -B coverage-build

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

install_mac:
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

lib-only: run-cmake-release
	cmake --build build --target raptor_gui -j $(CPU_CORES)

include tests.mk

test/unittest: release test/int_unittest

test/unittest-d: run-cmake-debug test/int_unittest-d

test/unittest-coverage: run-cmake-coverage test/int_unittest-coverage

coverage-build/raptor_gui.coverage: test/unittest-coverage
	lcov --no-external --exclude  "*_test.cpp" --capture --directory coverage-build/CMakeFiles/raptor_gui.dir --base-directory src --output-file coverage-build/raptor_gui.coverage

coverage-build/html: raptor_gui-build/raptor_gui.coverage
	genhtml --output-directory coverage-build/html $^
	realpath coverage-build/html/index.html

test/valgrind: run-cmake-debug test/int_valgrind

test: test/unittest

test-parallel: release test/unittest

test/install_mac:  install_mac test/int_install_mac

test_install: install test/int_install

test/gui: release test/int_dgui

test/raptor_batch: test/int_batch

test/raptor_gui: run-cmake-release test/int_raptor_gui

test/openfpga: run-cmake-release test/int_openfpga

test/openfpga_gui: run-cmake-release test/int_openfpga_gui

test/gui_mac: run-cmake-debug test/int_gui_mac

# This target can be invoked directly
test/batch: release test/int_batch

# This target can be invoked directly
test/batch_gen2: release test/int_batch_gen2

# Too large for Github Action
test/batch_gen3: release test/int_batch_gen3

# This target can be invoked directly
solver/tests: release test/int_solver

test/batch_all:
	echo "############################# Raptor checkin tests, all tests must pass! #############################"
	export CI=true && $(MAKE) test/int_batch test/int_batch_gen2 test/int_solver test/batch_gen3
	echo "############################# Success, all tests passed! #############################################"

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

clean:
	$(RM) -r build dbuild coverage-build dist tests/TestInstall/build Raptor_Tools/parser_plugins/synlig/out/
