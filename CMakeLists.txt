# -*- mode:cmake -*-

# Copyright 2021-2024 The Rapid Silicon team

cmake_minimum_required(VERSION 3.15)

option(ENABLE_VERIFIC "Enable Verific frontend" OFF)
option(ENABLE_OPENSTA "Enable OpenSTA" OFF)

set (BUILD_TYPE_STRING Engineering)
if (PRODUCTION_BUILD)
  message("Production Build type set to ON")
  set (PRODUCTION_BUILD_FLAG "-DPRODUCTION_BUILD=1")
  add_definitions(-DPRODUCTION_BUILD)
  set (BUILD_TYPE_STRING Production)
  message(STATUS "Given Production devices are ${PRODUCTION_DEVICES}")
else()
  set (PRODUCTION_DEVICES "")
endif(PRODUCTION_BUILD)

# Detect build type, fallback to release and throw a warning if use didn't
# specify any
if(NOT CMAKE_BUILD_TYPE)
  message(WARNING "Build type not set, falling back to Release mode.
 To specify build type use:
 -DCMAKE_BUILD_TYPE=<mode> where <mode> is Debug or Release.")
  set(CMAKE_BUILD_TYPE
      "Release"
      CACHE STRING "Choose the type of build, options are: Debug Release."
            FORCE)
endif(NOT CMAKE_BUILD_TYPE)

option(
  WITH_LIBCXX
  "Building with clang++ and libc++(in Linux). To enable with: -DWITH_LIBCXX=On"
  On)

project(RAPTOR)
set(RAPTOR ON)

# Augment VPR with a Verilog reader (Verific based)
set(ADD_READ_VERILOG_TO_VPR 1)

#TODO Embedded common and uboot
list(APPEND RAPTOR_RECURSIVE_SUBSYTEMS
  tests/Jira_Testcase
  litex_reference_designs
  zephyr-rapidsi-dev
  Raptor_Tools
  )

list(APPEND RAPTOR_NON_RECURSIVE_SUBSYTEMS
  yosys_verific_rs
  FOEDAG_rs
  IP_Catalog
  Backend
  )

list(APPEND RAPTOR_PARTIAL_RECURSIVE_SUBSYTEMS
  Backend/OpenFPGA
  Backend/OpenFPGA/vtr-verilog-to-routing
  Backend/OpenFPGA/vtr-verilog-to-routing/libs/EXTERNAL/libcatch2
)

if(PRODUCTION_BUILD)
 list(APPEND RAPTOR_RECURSIVE_SUBSYTEMS
   FOEDAG_rs
     )	
endif()

find_package(Git QUIET)

if (NOT UPDATE_SUBMODULES)
  set(UPDATE_SUBMODULES OFF)
endif()

function(checkout_subsystems SUBSYSTEM_LIST WORK_DIR RECURSIVE UPDATE PARTIAL)
    set(ORGNL_RECEV ${WORK_DIR})
    message("###########################")
    message(${SUBSYSTEM_LIST})
    message("###########################")
  foreach(GIVEN_SUBMODULE IN LISTS ${SUBSYSTEM_LIST})
    if(PARTIAL)
        set(WORK_DIR ${ORGNL_RECEV})
        get_filename_component(SUBMODULE "${GIVEN_SUBMODULE}" NAME)
        get_filename_component(extract_dir "${GIVEN_SUBMODULE}" DIRECTORY)
        set(WORK_DIR ${WORK_DIR}/${extract_dir})
    else()
        set(SUBMODULE ${GIVEN_SUBMODULE})    
    endif()
    file(GLOB README ${WORK_DIR}/${SUBMODULE}/README*)
    if (NOT EXISTS "${README}" OR ${UPDATE})
      message("Updating ${SUBMODULE} submodule...")
      execute_process(
        COMMAND ${GIT_EXECUTABLE} submodule update --init ${RECURSIVE} ${SUBMODULE}
        WORKING_DIRECTORY ${WORK_DIR}
        RESULT_VARIABLE GIT_SUBMOD_RESULT
        )
      if  (NOT GIT_SUBMOD_RESULT EQUAL "0")
        message(FATAL_ERROR "\"${GIT_EXECUTABLE} submodule update --init ${RECURSIVE} ${SUBMODULE}\" failed with ${GIT_SUBMOD_RESULT}, please checkout submodule manually")
      endif()
    endif()
  endforeach()
endfunction(checkout_subsystems)

checkout_subsystems(RAPTOR_RECURSIVE_SUBSYTEMS ${CMAKE_CURRENT_SOURCE_DIR} "--recursive" ${UPDATE_SUBMODULES} OFF)
checkout_subsystems(RAPTOR_NON_RECURSIVE_SUBSYTEMS ${CMAKE_CURRENT_SOURCE_DIR} "" ${UPDATE_SUBMODULES} OFF)
checkout_subsystems(RAPTOR_PARTIAL_RECURSIVE_SUBSYTEMS ${CMAKE_CURRENT_SOURCE_DIR} "" ${UPDATE_SUBMODULES} ON)

if (PRODUCTION_BUILD)
  #get_filename_component(FLEX_LM_SRC_DIR "Raptor_Tools/Flex_LM"
  #      REALPATH BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
  #add_subdirectory("Raptor_Tools/Flex_LM")
  get_filename_component(OPENLM_DIR "Raptor_Tools/OpenLM"
         REALPATH BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
  add_subdirectory(${OPENLM_DIR} OPENLM_DIR)
  message(STATUS "OPENLM_DIR: "  ${OPENLM_DIR})
  #dump openlm license file env variable default value
  file(READ "${CMAKE_CURRENT_SOURCE_DIR}/etc/config.json" JSON_DATA)
  string(JSON prefixPath GET ${JSON_DATA} general license-path)
  file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/bin/default_lic_path" "default_path=${prefixPath}\n")
  file(APPEND "${CMAKE_CURRENT_BINARY_DIR}/bin/default_lic_path" "export LICENSE_LOCATION=$default_path\\;$LICENSE_LOCATION\n")
  file(APPEND "${CMAKE_CURRENT_BINARY_DIR}/bin/default_lic_path" "RAPTOR_EXE_DEBUG=0\n")
endif()


if(ENABLE_VERIFIC)
  get_filename_component(VERIFIC_HOME "Raptor_Tools/verific_rs"
      REALPATH BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}" CACHE)
  message("VERIFIC_HOME: ${VERIFIC_HOME}")
  if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    execute_process(
  	  COMMAND ${CMAKE_MAKE_PROGRAM} clean
  	  WORKING_DIRECTORY ${VERIFIC_HOME}
        )

    add_subdirectory(${VERIFIC_HOME})
  endif()
endif()

get_filename_component(VERIFY_SYNTH_PORTS_SRC_DIR "Raptor_Tools/gatelevel_readers/verify_synth_ports"
REALPATH BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}" CACHE)
add_subdirectory(${VERIFY_SYNTH_PORTS_SRC_DIR})

#TODO enable them 
#get_filename_component(VERI_PRUNE_SRC_DIR "Raptor_Tools/gatelevel_readers/veri_prune"
#REALPATH BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}" CACHE)
#add_subdirectory(${VERI_PRUNE_SRC_DIR})
#get_filename_component(READ_VERILOG_SRC_DIR "Raptor_Tools/gatelevel_readers/read_verilog"
#REALPATH BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}" CACHE)
#add_subdirectory(${READ_VERILOG_SRC_DIR})


get_filename_component(READ_EDIF_SRC_DIR "Raptor_Tools/gatelevel_readers/read_edif"
REALPATH BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}" CACHE)
add_subdirectory(${READ_EDIF_SRC_DIR})

add_subdirectory(Raptor_Tools/gatelevel_readers/reconstruct_verilog)

if (ENABLE_OPENSTA)
  add_subdirectory("Raptor_Tools/psta-rel")
endif()

add_subdirectory("Raptor_Tools/python_tools")
add_subdirectory("Raptor_Tools/HDL_simulator")

add_subdirectory(FOEDAG_rs)
# add mt-kahpar after FOEDAG as we need to provide googletest
add_subdirectory("Raptor_Tools/graph_partition")

if (APPLE)
    option(BUILD_YOSYS_PLUGINS OFF) # Currently yosys-plugins doesn't build on MacOS
endif()

if (NOT APPLE)
    # Disable yosys_verific_rs till EDA-799 is resolved.
    add_subdirectory(yosys_verific_rs)
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/src/de/CMakeLists.txt)
    	add_subdirectory(src/de) 
    	add_dependencies(de_actual de)
    endif()
endif()

add_subdirectory("Raptor_Tools/parser_plugins")

include_directories(SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/Raptor_Tools/libs/external/boost)

if (NOT APPLE) # Currently OpenFPGA doesn't build on MacOS
   set(OPENFPGA_WITH_YOSYS OFF CACHE BOOL "Disable building Yosys in OpenFPGA" FORCE)
   set(OPENFPGA_WITH_TEST OFF CACHE BOOL "Disable OpenFPGA tests" FORCE)
   set(WITH_ABC OFF CACHE BOOL "Enable building ABC in Verilog-to-Routing" FORCE)
   add_subdirectory(Backend)
endif()

#Submodules for litex_reference_designs and IP_Catalog

add_subdirectory(litex_reference_designs)
add_subdirectory(IP_Catalog)


# Raptor version
set(VERSION_MAJOR 1)
set(VERSION_MINOR 0)
set(VERSION_PATCH 11)

execute_process(COMMAND git config --global --add safe.directory ${CMAKE_CURRENT_SOURCE_DIR}
  COMMAND git rev-parse --short HEAD
  COMMAND tr -d '\n'
  OUTPUT_VARIABLE GIT_HASH
  ERROR_VARIABLE GIT_HASH_ERR
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

message(STATUS "GIT_HASH = ${GIT_HASH}")
if(STICK_RELEASE_VERSION STREQUAL "")
  string(TIMESTAMP CURRENT_DATE "%Y-%m-%d" UTC)
  string(SUBSTRING "${CURRENT_DATE}" 0 4 RELEASE_YEAR)
  string(SUBSTRING "${CURRENT_DATE}" 5 2 RELEASE_MONTH)
  set(RELEASE_VERSION ${RELEASE_YEAR}.${RELEASE_MONTH})
else()
  set(RELEASE_VERSION ${STICK_RELEASE_VERSION})
endif()
message(STATUS "The version is ::: ${RELEASE_VERSION}")
# DO NOT CHANGE THE NUMBERS BELOW, THEY DENOTE THE PROJECT VERSION REQUIRING MIGRATION
set(VERSION_MAJOR_COM 0)
set(VERSION_MINOR_COM 8)
set(VERSION_PATCH_COM 55)
configure_file(raptor_version.h.in ${CMAKE_CURRENT_BINARY_DIR}/FOEDAG_rs/FOEDAG/include/foedag_version.h)

# Require swig 3
find_package(SWIG 3.0 REQUIRED)

# Check system
message("CMAKE_SYSTEM_NAME: ${CMAKE_SYSTEM_NAME}")

# Find Qt packages and error out if not found
#find_package(Qt6 COMPONENTS Core Gui Widgets Quick Xml REQUIRED)
find_package(Qt6 COMPONENTS Core Gui Widgets Xml REQUIRED)
if (NOT Qt6Widgets_FOUND)
    message(STATUS "Failed to find Qt6Widgets required (on debian/ubuntu try 'sudo apt install qt6-default')")
elseif (NOT Qt6Gui_FOUND)
    message(STATUS "Failed to find Qt6Gui required (on debian/ubuntu try 'sudo apt install qt6-default')")
elseif (NOT Qt6Core_FOUND)
    message(STATUS "Failed to find Qt6Core required (on debian/ubuntu try 'sudo apt install qt6-default')")
#elseif (NOT Qt5Quick_FOUND)
#    message(STATUS "Failed to find Qt5Core required (on debian/ubuntu try 'sudo apt install libkf5qqc2desktopstyle-dev')")
else()
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTOUIC ON)
    set(CMAKE_AUTORCC ON)
endif()

# NOTE: Policy changes has to happen before adding any subprojects
cmake_policy(SET CMP0091 NEW)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# NOTE: Set the global output directories after the subprojects have had their go at it
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)


if(NOT NO_TCMALLOC)
  find_library(TCMALLOC_LIBRARY NAMES tcmalloc)
  if(TCMALLOC_LIBRARY)
    set(TCMALLOC_COMPILE_OPTIONS
        "-fno-builtin-malloc -fno-builtin-calloc -fno-builtin-realloc -fno-builtin-free"
    )
  endif()
endif()

set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} ${TCMALLOC_COMPILE_OPTIONS} ${MY_CXX_WARNING_FLAGS}")

if(MSVC)
  add_subdirectory(third_party/zlib) # Do not use EXCLUDE_FROM_ALL else unit tests will fail for this subsystem

  add_compile_definitions(_CRT_NONSTDC_NO_WARNINGS)

  set(CMAKE_CXX_FLAGS_DEBUG
      "${CMAKE_CXX_FLAGS_DEBUG} ${TCMALLOC_COMPILE_OPTIONS} /Zc:__cplusplus /W0 /bigobj /Zl /GL- /MT /DSTATIC_BUILD ${MY_CXX_WARNING_FLAGS}"
  )
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO
      "${CMAKE_CXX_FLAGS_RELEASE} ${TCMALLOC_COMPILE_OPTIONS} /Zc:__cplusplus /W0 /bigobj /Zl /GL- /MT /DSTATIC_BUILD ${MY_CXX_WARNING_FLAGS}"
  )
  set(CMAKE_CXX_FLAGS_RELEASE
      "${CMAKE_CXX_FLAGS_RELEASE} ${TCMALLOC_COMPILE_OPTIONS} /Zc:__cplusplus /W0 /bigobj /Zl /GL- /MT /DSTATIC_BUILD ${MY_CXX_WARNING_FLAGS}"
  )
  set(CMAKE_EXE_LINKER_FLAGS /STACK:8388608)  # 8MB stack size
else()
  if(DEFINED ENV{MSYSTEM})
    # Under MSYS some files are too large to build without additional flags
    set(MSYS_COMPILE_OPTIONS "-m64 -Wa,-mbig-obj")
  endif()

  if (SANITIZE)
    set(MEM_SANITIZER_FLAGS "-fsanitize=address -fno-omit-frame-pointer")
  endif()

  set(CMAKE_CXX_FLAGS_DEBUG
      "${CMAKE_CXX_FLAGS_DEBUG} ${TCMALLOC_COMPILE_OPTIONS} -Werror -Wall -O0 -g ${MSYS_COMPILE_OPTIONS} ${MY_CXX_WARNING_FLAGS} ${MEM_SANITIZER_FLAGS} ${PRODUCTION_BUILD_FLAG}"
  )
  set(CMAKE_CXX_FLAGS_RELEASE
      "${CMAKE_CXX_FLAGS_RELEASE} ${TCMALLOC_COMPILE_OPTIONS} -Werror -Wall -O3 ${MSYS_COMPILE_OPTIONS} -DNDEBUG ${MY_CXX_WARNING_FLAGS} ${PRODUCTION_BUILD_FLAG}"
  )
endif()

include_directories(${PROJECT_SOURCE_DIR}/src
                    ${PROJECT_SOURCE_DIR}/FOEDAG_rs/src
                    ${PROJECT_SOURCE_DIR}/FOEDAG_rs/FOEDAG/src
                    ${CMAKE_CURRENT_BINARY_DIR}/include/
                    ${CMAKE_CURRENT_BINARY_DIR}/FOEDAG_rs/include/
                    ${CMAKE_CURRENT_BINARY_DIR}/FOEDAG_rs/FOEDAG/include/)

# Put source code here, files that are generated at build time in
# raptor_generated_SRC

set(QRC_MAIN_WINDOW
  FOEDAG_rs/FOEDAG/src/NewProject/newproject.qrc
  FOEDAG_rs/FOEDAG/src/MainWindow/main_window_resource.qrc
)

set(raptor_SRC
  ${QRC_MAIN_WINDOW}
)

add_library(raptor STATIC ${raptor_SRC})

#set_target_properties(raptor PROPERTIES PUBLIC_HEADER )
target_include_directories(raptor PRIVATE
  ${CMAKE_CURRENT_BINARY_DIR}/include
  FOEDAG_rs/FOEDAG/third_party/googletest/googletest/include
  FOEDAG_rs/FOEDAG/third_party/googletest/googlemock/include)
target_include_directories(raptor PUBLIC $<INSTALL_INTERFACE:include/raptor>)

if(MSVC)
  add_executable(raptor-bin ${PROJECT_SOURCE_DIR}/src/Main/main.cpp ${QRC_MAIN_WINDOW})
  set_property(TARGET raptor-bin PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
  set_property(TARGET raptor-bin PROPERTY COMPILER_FLAGS /DSTATIC_BUILD)
else()
  add_executable(raptor-bin ${PROJECT_SOURCE_DIR}/src/Main/main.cpp src/Main/MPW1Loader.h src/Main/MPW1Loader.cpp ${QRC_MAIN_WINDOW})
endif()
set_target_properties(raptor-bin PROPERTIES OUTPUT_NAME raptor)
if (PRODUCTION_BUILD)
  add_dependencies(raptor-bin rs_licenseManager)
  add_dependencies(licensecc_static openssl_build)
  add_dependencies(lcc_base openssl_build)
  add_dependencies(license_generator_lib openssl_build)
  #target_include_directories(raptor-bin PUBLIC ${FLEX_LM_SRC_DIR})
  target_include_directories(raptor-bin PUBLIC ${OPENLM_DIR})
  target_link_libraries(raptor-bin PUBLIC rs_licenseManager)
  link_libraries(raptor rs_licenseManager)
endif()

add_subdirectory(src/ProgrammerGui)
add_subdirectory(src/ConfigurationRaptor)

link_libraries(raptor-bin raptor_gui)
link_libraries(raptor raptor_gui)
add_dependencies(raptor-bin verify_synth_ports)

if (MSVC)
  message("WINDOWS MODE")
  set(TCL_STUBB_LIB tclstub86.lib)
  set(TCL_STATIC_LIB tcl86ts.lib)
  set(ZLIB_STATIC_LIB zlib.lib)

  add_library(tcl_static STATIC IMPORTED )
  set_target_properties(tcl_static PROPERTIES
    IMPORTED_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/FOEDAG_rs/FOEDAG/lib/${TCL_STATIC_LIB})

else()
  set(TCL_STATIC_LIB libtcl8.6.so)
  set(TCL_STUBB_LIB libtclstub8.6.a)
  set(ZLIB_STATIC_LIB libz.a)
  if(APPLE)
    set(TCL_STATIC_LIB libtcl8.6.dylib)
  endif()

  link_directories(${CMAKE_CURRENT_BINARY_DIR}/FOEDAG_rs/FOEDAG/lib/)

  if(CMAKE_SYSTEM_NAME MATCHES "MSYS")
    message("MSYS MODE")
    set(TCL_STATIC_LIB libtcl8.6.a)
    add_library(tcl_static STATIC IMPORTED )
    set_target_properties(tcl_static PROPERTIES
      IMPORTED_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/FOEDAG_rs/FOEDAG/lib/${TCL_STATIC_LIB})

  else()
    message("LINUX MODE")
    get_filename_component(buildDirRelFilePath ${TCL_STATIC_LIB}
                         REALPATH BASE_DIR ${CMAKE_CURRENT_BINARY_DIR}/FOEDAG_rs/FOEDAG/lib)

    add_library(tcl_static SHARED IMPORTED )
    find_library(tcl_static PATHS ${CMAKE_CURRENT_BINARY_DIR}/FOEDAG_rs/FOEDAG/lib/)
    set_target_properties(tcl_static PROPERTIES
      IMPORTED_LOCATION ${buildDirRelFilePath} IMPORTED_NO_SONAME ON)

  endif()

endif()

add_library(tcl_stubb STATIC IMPORTED )
set_target_properties(tcl_stubb PROPERTIES
  IMPORTED_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/FOEDAG_rs/FOEDAG/lib/${TCL_STUBB_LIB})

if(MSVC)
  set_target_properties(tcl_static PROPERTIES
    COMPILE_OPTIONS "$<$<CONFIG:Debug>:/MTd>$<$<CONFIG:Release>:/MT>"
  )

  set_target_properties(tcl_stubb PROPERTIES
    COMPILE_OPTIONS "$<$<CONFIG:Debug>:/MTd>$<$<CONFIG:Release>:/MT>"
  )

  # Do not add zlib library, it is imported above

  set_target_properties(raptor PROPERTIES
    COMPILE_OPTIONS "$<$<CONFIG:Debug>:/MTd>$<$<CONFIG:Release>:/MT>"
  )
  target_link_libraries(raptor-bin PUBLIC Netapi32)
  target_link_libraries(raptor  PUBLIC Netapi32)

else()
  add_library(zlib STATIC IMPORTED)
  set_target_properties(zlib PROPERTIES
    IMPORTED_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/FOEDAG_rs/FOEDAG/lib/${ZLIB_STATIC_LIB})
endif()

# Copy the init.tcl file from source to build directory
add_custom_command(TARGET raptor POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy
          ${CMAKE_CURRENT_SOURCE_DIR}/FOEDAG_rs/FOEDAG/third_party/tcl8.6.12/library/init.tcl
          ${CMAKE_CURRENT_BINARY_DIR}/lib/tcl8.6/init.tcl)

# Explicit lib build order
add_dependencies(raptor raptor_gui)
if (NOT APPLE)
    add_dependencies(raptor-bin yosys)
    add_dependencies(raptor-bin abc)
    add_dependencies(raptor-bin de)
endif()

if (ENABLE_OPENSTA)
  add_dependencies(raptor-bin sta)
endif()

if (WIN OR APPLE)
else ()
  # Guaranties all the executables are built before final copy in build directory
  add_dependencies(raptor-bin libopenfpga)
  add_dependencies(raptor-bin openfpga)
  add_dependencies(raptor-bin vpr)
  add_dependencies(raptor-bin pin_c)
  add_dependencies(raptor-bin stars)
endif()

# Make sure openocd_cmake build is done before raptor
add_dependencies(raptor rs_openocd_extract_config_and_openocd)

add_dependencies(synlig_build yosys)

if (APPLE)
  # In macOS, it is necessary to add the correct @rpath to the executable for
  # finding python dynamic libraries ref: https://gitlab.kitware.com/cmake/cmake/-/issues/21293
  # https://gitlab.kitware.com/cmake/cmake/-/issues/21947
  # Python3_LINK_OPTIONS is variable available from cmake 3.19, update cmake using homebrew
  # if can't update cmake use:
  # set_target_properties(raptor-bin PROPERTIES BUILD_WITH_INSTALL_RPATH TRUE
  # INSTALL_RPATH "/Library/Developer/CommandLineTools/Library/Frameworks/")
  # if you installed python with hombrew. Or if you install python with Xcode:
  # set_target_properties(raptor-bin PROPERTIES BUILD_WITH_INSTALL_RPATH TRUE
  # INSTALL_RPATH "/Applications/Xcode.app/Contents/Developer/Library/Frameworks/")
  target_link_libraries(raptor-bin PUBLIC raptor "-framework CoreFoundation")
  target_link_libraries(raptor PUBLIC "-framework CoreFoundation")
endif()

if(MSVC OR WIN32)
  # We have two files named "raptor.lib" and both getting generated in the lib folder
  # One is the raptor.lib generated by the raptor target and the other is the one generated
  # because of /IMPLIB option when linking the executable. Unfortunately, there is no documented
  # way to disable the latter in CMake. So, moving the library to the bin directory (right next to the exe)
  set_target_properties(raptor-bin PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
endif()

target_link_libraries(raptor-bin PUBLIC raptor raptor_gui foedag  )
#target_link_libraries(raptor PUBLIC  Qt6::Widgets Qt6::Core Qt6::Gui Qt6::Xml Qt6::Quick)
target_link_libraries(raptor PUBLIC  Qt6::Widgets Qt6::Core Qt6::Gui Qt6::Xml)

if(NOT NO_TCMALLOC)
  find_library(TCMALLOC_LIBRARY NAMES tcmalloc)
  if(TCMALLOC_LIBRARY)
    target_link_libraries(raptor PRIVATE tcmalloc)
  endif()
endif()

if(NOT NO_TCMALLOC)
  find_library(TCMALLOC_LIBRARY NAMES tcmalloc)
  if(TCMALLOC_LIBRARY)
    set(TCMALLOC_COMPILE_OPTIONS
        "-fno-builtin-malloc -fno-builtin-calloc -fno-builtin-realloc -fno-builtin-free"
    )
  endif()
endif()

set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} ${TCMALLOC_COMPILE_OPTIONS} ${MY_CXX_WARNING_FLAGS}  -std=c++11 -pthread")

if (UNIX)
  target_link_libraries(raptor PRIVATE dl)
  target_link_libraries(raptor PRIVATE util)
  target_link_libraries(raptor PRIVATE m)
  target_link_libraries(raptor PRIVATE pthread)
endif()

if (CMAKE_SYSTEM_NAME MATCHES "Linux")
  target_link_libraries(raptor PRIVATE stdc++fs)
  target_link_libraries(raptor PRIVATE rt)
endif()

# Unit tests
enable_testing()
include(GoogleTest)

if(MSVC)
  # Microsoft reports the value of __cplusplus wrong and gmock/gtest pulls in the
  # string_view implementation based on it's value. Microsoft's solution is to
  # provide additional flags to make the value correct. More info can be found here -
  #
  #   https://docs.microsoft.com/en-us/cpp/build/reference/zc-cplusplus?view=msvc-160
  #   https://devblogs.microsoft.com/cppblog/msvc-now-correctly-reports-__cplusplus/
  target_compile_options(gmock PRIVATE /Zc:__cplusplus)
  target_compile_options(gmock_main PRIVATE /Zc:__cplusplus)
  target_compile_options(gtest PRIVATE /Zc:__cplusplus)
  target_compile_options(gtest_main PRIVATE /Zc:__cplusplus)
endif()


# All unit-tests are registered with this custom target as dependency, so
# just `make RaptorUnitTests` will build them all.
add_custom_target(RaptorUnitTests)

# Concise way to register a new google test
# call with register_gtest(path/to/UnitTestFile.cpp)
function(register_gtests)
  foreach(gtest_cc_file IN LISTS ARGN)
    # We create the binary name and test prefix from the cpp-filepath
    get_filename_component(test_bin ${gtest_cc_file} NAME_WE)
    get_filename_component(test_prefix ${gtest_cc_file} DIRECTORY)

    # Build binary, link all relevant libs and extract tests
    add_executable(${test_bin} EXCLUDE_FROM_ALL ${gtest_cc_file})

    target_include_directories(${test_bin} PRIVATE
    )
    # For simplicity, we link the test with libraptor, but there is of
    # course a lot unnecessary churn if headers are modified.
    # Often it is sufficient to just have a few depeendencies.

    if (APPLE)
      target_link_libraries(${test_bin} raptor gtest gmock gtest_main "-framework CoreFoundation")
    elseif (CMAKE_SYSTEM_NAME MATCHES "Darwin")
      target_link_libraries(${test_bin} raptor gtest gmock gtest_main "-framework CoreFoundation")
    else()
      target_link_libraries(${test_bin} raptor gtest gmock gtest_main)
    endif()

    gtest_discover_tests(${test_bin} TEST_PREFIX "${test_prefix}/")

    # Now, add this binary to our UnitTests target that it builds this
    add_dependencies(UnitTests ${test_bin})
  endforeach()
endfunction()

register_gtests(

)

if (WIN OR APPLE)
else ()
# The test works, the CI running headlessly does not
#  register_gtests(src/Main/GuiMain_test.cpp)
endif()

# Installation target
install(
  TARGETS raptor-bin
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(
  TARGETS raptor
  EXPORT Raptor
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/raptor
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/raptor)

install (
  FILES ${CMAKE_CURRENT_BINARY_DIR}/FOEDAG_rs/FOEDAG/lib/${TCL_STATIC_LIB}
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/raptor/lib)
install (
  FILES ${CMAKE_CURRENT_BINARY_DIR}/FOEDAG_rs/FOEDAG/lib/${TCL_STUBB_LIB}
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/raptor/lib)
install (
  FILES ${CMAKE_CURRENT_BINARY_DIR}/FOEDAG_rs/FOEDAG/lib/${ZLIB_STATIC_LIB}
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/raptor/lib)


if (WIN32 AND $<CONFIG:Debug>)
  if (RAPTOR_WITH_PYTHON)
    install(
      FILES $<TARGET_PDB_FILE:raptor-bin>
            ${Python3_RUNTIME_LIBRARY_DIRS}/python${Python3_VERSION_MAJOR}${Python3_VERSION_MINOR}$<$<CONFIG:Debug>:_d>.dll
      DESTINATION ${CMAKE_INSTALL_BINDIR})
  endif()
  install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/raptor.dir/raptor.pdb
          ${TCL_BINARY_DIR}/runtime/CMakeFiles/tcl_static.dir/tcl_static.pdb
          ${TCL_BINARY_DIR}/runtime/CMakeFiles/tcl_stubb.dir/tcl_stubb.pdb
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/raptor)
endif()

install(
  FILES ${CMAKE_CURRENT_BINARY_DIR}/FOEDAG_rs/FOEDAG/include/tcl.h
        ${CMAKE_CURRENT_BINARY_DIR}/FOEDAG_rs/FOEDAG/include/tclDecls.h
        ${CMAKE_CURRENT_BINARY_DIR}/FOEDAG_rs/FOEDAG/include/tclPlatDecls.h
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/raptor/include)

install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/bin/raptor.exe
    DESTINATION ${CMAKE_INSTALL_BINDIR}
    PERMISSIONS
    OWNER_READ OWNER_EXECUTE OWNER_WRITE
    GROUP_READ GROUP_EXECUTE
    WORLD_READ WORLD_EXECUTE)

install(
    DIRECTORY etc/ DESTINATION  ${CMAKE_INSTALL_DATAROOTDIR}/raptor/etc/)
install(
    DIRECTORY licenses/ DESTINATION  ${CMAKE_INSTALL_DATAROOTDIR}/raptor/licenses/)
install(
    DIRECTORY examples/ DESTINATION  ${CMAKE_INSTALL_DATAROOTDIR}/raptor/examples)
install(
      DIRECTORY tests/tcl_examples/ DESTINATION  ${CMAKE_INSTALL_DATAROOTDIR}/raptor/tcl_examples)
install(
        DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/share/raptor/IP_Catalog DESTINATION  ${CMAKE_INSTALL_DATAROOTDIR}/raptor/)
install(
        DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin/HDL_simulator DESTINATION  ${CMAKE_INSTALL_BINDIR}
	USE_SOURCE_PERMISSIONS
	)
install(
        DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/share/envs DESTINATION  ${CMAKE_INSTALL_DATAROOTDIR}
        USE_SOURCE_PERMISSIONS
        )
install(
        DIRECTORY litex_reference_designs DESTINATION  ${CMAKE_INSTALL_DATAROOTDIR})
install(
      FILES README.md DESTINATION  ${CMAKE_INSTALL_DATAROOTDIR}/raptor/doc/)
install(
      DIRECTORY FOEDAG_rs/FOEDAG/third_party/tcl8.6.12/library/ DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/raptor/tcl8.6.12/library/)
       
install(
      FILES ${CMAKE_CURRENT_BINARY_DIR}/FOEDAG_rs/FOEDAG/bin/tclsh8.6 DESTINATION ${CMAKE_INSTALL_BINDIR}
      PERMISSIONS
    OWNER_READ OWNER_EXECUTE OWNER_WRITE
    GROUP_READ GROUP_EXECUTE
    WORLD_READ WORLD_EXECUTE)

install(
      FILES  ${CMAKE_CURRENT_BINARY_DIR}/bin/programmer_gui DESTINATION ${CMAKE_INSTALL_BINDIR}
      PERMISSIONS
    OWNER_READ OWNER_EXECUTE OWNER_WRITE
    GROUP_READ GROUP_EXECUTE
    WORLD_READ WORLD_EXECUTE)    

install(
      FILES  ${CMAKE_CURRENT_BINARY_DIR}/bin/finalize DESTINATION ${CMAKE_INSTALL_BINDIR}
      PERMISSIONS
    OWNER_READ OWNER_EXECUTE OWNER_WRITE
    GROUP_READ GROUP_EXECUTE
    WORLD_READ WORLD_EXECUTE)

if (NOT PRODUCTION_BUILD)
  # Genesis Simulation models
  install(
      DIRECTORY ${PROJECT_SOURCE_DIR}/yosys_verific_rs/yosys-rs-plugin/genesis/
      DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/raptor/sim_models/rapidsilicon/genesis/)
  install(
      FILES ${PROJECT_SOURCE_DIR}/yosys_verific_rs/yosys/share/simlib.v
      DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/raptor/sim_models/rapidsilicon/genesis/)
  install(
      FILES ${PROJECT_SOURCE_DIR}/lib/pnr/primitives.v
      DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/raptor/sim_models/rapidsilicon/genesis/)
  install(
      FILES ${PROJECT_SOURCE_DIR}/yosys_verific_rs/yosys-rs-plugin/genesis/cell_sim_blackbox.v
      DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/raptor/sim_models/rapidsilicon/genesis/)    
endif()


# Genesis3 Simulation models
install(
      DIRECTORY ${PROJECT_SOURCE_DIR}/yosys_verific_rs/yosys-rs-plugin/genesis3/
      DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/raptor/sim_models/rapidsilicon/genesis3/)
install(
      FILES ${PROJECT_SOURCE_DIR}/yosys_verific_rs/yosys/share/simlib.v
      DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/raptor/sim_models/rapidsilicon/genesis3/)
install(
      FILES ${PROJECT_SOURCE_DIR}/lib/pnr/primitives.v
      DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/raptor/sim_models/rapidsilicon/genesis3/)

install(
      FILES src/raptorenv_lin64.sh
      src/.raptorenv_lin64.sh
      DESTINATION  ${CMAKE_INSTALL_BINDIR}/..
      PERMISSIONS
      OWNER_READ OWNER_EXECUTE OWNER_WRITE
      GROUP_READ GROUP_EXECUTE
      WORLD_READ WORLD_EXECUTE
      )
#install(
#      FILES Raptor_Tools/Flex_LM/
#      Raptor_Tools/Flex_LM/
#      Raptor_Tools/Flex_LM/
#      DESTINATION  ${CMAKE_INSTALL_BINDIR}/Flex_LM
#      PERMISSIONS
#      OWNER_READ OWNER_EXECUTE OWNER_WRITE
#      GROUP_READ GROUP_EXECUTE
#      WORLD_READ WORLD_EXECUTE
#      )
#install OpenLM  customer end
if (PRODUCTION_BUILD)
  install(
      PROGRAMS
          ${CMAKE_CURRENT_SOURCE_DIR}/Raptor_Tools/OpenLM/licensecc/install/bin/Raptor/lccinspector
      DESTINATION ${CMAKE_INSTALL_BINDIR}/OpenLM
  )
  install(
      PROGRAMS
          ${CMAKE_CURRENT_BINARY_DIR}/bin/default_lic_path
      DESTINATION ${CMAKE_INSTALL_BINDIR}
  )
endif()

install(
  DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin/gtkwave DESTINATION ${CMAKE_INSTALL_BINDIR} USE_SOURCE_PERMISSIONS)
install(
  DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/share/raptor/configuration DESTINATION  ${CMAKE_INSTALL_DATAROOTDIR}/raptor USE_SOURCE_PERMISSIONS)
install(
    PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/bin/openocd${CMAKE_EXECUTABLE_SUFFIX} 
    DESTINATION ${CMAKE_INSTALL_BINDIR} 
)

install(CODE "execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink  ../../../../bin/gtkwave/bin/lib/tcl8.6/tk8.6   ${CMAKE_INSTALL_PREFIX}/share/raptor/tcl8.6.12/library/tk8.6)")

#install(
#  EXPORT Raptor
#  FILE Raptor.cmake
#  DESTINATION cmake)
include(CMakePackageConfigHelpers)


add_custom_target(raptor_link_target ALL
                  COMMAND ${CMAKE_COMMAND} -E create_symlink
                  build/compile_commands.json ../compile_commands.json)

set(YOSYS_BUILD_PATH "$<$<CONFIG:Debug>:debug-install>$<$<CONFIG:Release>:install>")

if (PRODUCTION_BUILD)
   add_custom_command(TARGET raptor-bin POST_BUILD
      COMMAND echo "Create dirs in share dir"
      COMMAND echo "Create etc dirs in share dir"
      COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/share/raptor/etc/
      COMMAND echo "Create etc/settings dirs in share dir"
      COMMAND echo "Copying arch and example files in share dir"
      COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/share/raptor/etc/settings/
      COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${PROJECT_SOURCE_DIR}/etc/
          ${CMAKE_CURRENT_BINARY_DIR}/share/raptor/etc/
      COMMAND python3 ${PROJECT_SOURCE_DIR}/gen_rel_device.py --production_devices "${PRODUCTION_DEVICES}" --xml_filepath ${CMAKE_CURRENT_BINARY_DIR}/share/raptor/etc/device.xml --devices_dirs_path ${CMAKE_CURRENT_BINARY_DIR}/share/raptor/etc/devices --examples_path  ${CMAKE_CURRENT_BINARY_DIR}/share/raptor/examples
      COMMAND ${CMAKE_COMMAND} -E rename ${CMAKE_CURRENT_BINARY_DIR}/share/raptor/etc/settings/messages/suppress-rel.json
      ${CMAKE_CURRENT_BINARY_DIR}/share/raptor/etc/settings/messages/suppress.json)
else()
  add_custom_command(TARGET raptor-bin POST_BUILD
      COMMAND echo "Create dirs in share dir"
      COMMAND echo "Create etc dirs in share dir"
      COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/share/raptor/etc/
      COMMAND echo "Create etc/settings dirs in share dir"
      COMMAND echo "Copying arch and example files in share dir"
      COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/share/raptor/etc/settings/
      COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${PROJECT_SOURCE_DIR}/etc/
          ${CMAKE_CURRENT_BINARY_DIR}/share/raptor/etc/)
endif()

if (ENABLE_OPENSTA)
  add_custom_command(TARGET raptor-bin POST_BUILD
      COMMAND echo "Copy OpenSTA"
      COMMAND ${CMAKE_COMMAND} -E copy
          ${PROJECT_SOURCE_DIR}/Raptor_Tools/psta-rel/app/sta
          ${CMAKE_CURRENT_BINARY_DIR}/bin/
      )
endif()

add_custom_command(TARGET raptor-bin POST_BUILD
      COMMAND echo "Create dirs in share dir"
      COMMAND echo "Create bin/Flex_LM dirs in share dir"
      COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${PROJECT_SOURCE_DIR}/licenses/
          ${CMAKE_CURRENT_BINARY_DIR}/share/raptor/licenses/
      COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/bin/Flex_LM
      COMMAND echo "Copying IP Catalog in share dir"
      COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${PROJECT_SOURCE_DIR}/IP_Catalog/rapidsilicon
          ${CMAKE_CURRENT_BINARY_DIR}/share/raptor/IP_Catalog/rapidsilicon
      COMMAND echo "Copying litex_reference_designs to share dir"
      COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${PROJECT_SOURCE_DIR}/litex_reference_designs/
          ${CMAKE_CURRENT_BINARY_DIR}/share/litex_reference_designs
      COMMAND echo "Copying tests to share dir"
      COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${PROJECT_SOURCE_DIR}/examples
          ${CMAKE_CURRENT_BINARY_DIR}/share/raptor/examples/
      COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${PROJECT_SOURCE_DIR}/tests/tcl_examples/
          ${CMAKE_CURRENT_BINARY_DIR}/share/raptor/tcl_examples/
   #Genesis Sim Model
      COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${PROJECT_SOURCE_DIR}/yosys_verific_rs/yosys-rs-plugin/genesis/
          ${CMAKE_CURRENT_BINARY_DIR}/share/raptor/sim_models/rapidsilicon/genesis/
      COMMAND ${CMAKE_COMMAND} -E copy
          ${PROJECT_SOURCE_DIR}/yosys_verific_rs/yosys/share/simlib.v
          ${CMAKE_CURRENT_BINARY_DIR}/share/raptor/sim_models/rapidsilicon/genesis/
      COMMAND ${CMAKE_COMMAND} -E copy
          ${PROJECT_SOURCE_DIR}/lib/pnr/primitives.v
          ${CMAKE_CURRENT_BINARY_DIR}/share/raptor/sim_models/rapidsilicon/genesis/
      COMMAND ${CMAKE_COMMAND} -E copy
          ${PROJECT_SOURCE_DIR}/yosys_verific_rs/yosys-rs-plugin/genesis/cell_sim_blackbox.v
          ${CMAKE_CURRENT_BINARY_DIR}/share/raptor/sim_models/rapidsilicon/genesis    
   #Genesis3 Sim Model
      COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${PROJECT_SOURCE_DIR}/yosys_verific_rs/yosys-rs-plugin/genesis3/
          ${CMAKE_CURRENT_BINARY_DIR}/share/raptor/sim_models/rapidsilicon/genesis3/
      COMMAND ${CMAKE_COMMAND} -E copy
          ${PROJECT_SOURCE_DIR}/yosys_verific_rs/yosys/share/simlib.v
          ${CMAKE_CURRENT_BINARY_DIR}/share/raptor/sim_models/rapidsilicon/genesis3/
      COMMAND ${CMAKE_COMMAND} -E copy
          ${PROJECT_SOURCE_DIR}/lib/pnr/primitives.v
          ${CMAKE_CURRENT_BINARY_DIR}/share/raptor/sim_models/rapidsilicon/genesis3/
)

if (WIN OR APPLE)
else ()
  add_custom_command(TARGET raptor-bin POST_BUILD
      COMMAND echo "Copying Yosys, ABC, DE, sta in bin dir"
      COMMAND ${CMAKE_COMMAND} -E copy
          ${PROJECT_SOURCE_DIR}/yosys_verific_rs/yosys/${YOSYS_BUILD_PATH}/bin/yosys
          ${CMAKE_CURRENT_BINARY_DIR}/bin/
      COMMAND ${CMAKE_COMMAND} -E copy
          ${PROJECT_SOURCE_DIR}/yosys_verific_rs/yosys/${YOSYS_BUILD_PATH}/bin/abc
          ${CMAKE_CURRENT_BINARY_DIR}/bin/
      COMMAND ${CMAKE_COMMAND} -E copy
          ${CMAKE_CURRENT_BINARY_DIR}/Raptor_Tools/gatelevel_readers/verify_synth_ports/verify_synth_ports
          ${CMAKE_CURRENT_BINARY_DIR}/bin/)

  if (ENABLE_VERIFIC)
     add_custom_command(TARGET raptor-bin POST_BUILD
          COMMAND echo "Copying Analyze in bin dir"
          COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_CURRENT_BINARY_DIR}/yosys_verific_rs/analyze/bin/analyze
            ${CMAKE_CURRENT_BINARY_DIR}/bin/
          COMMAND echo "Copying vhdl_packages in share dir"
          COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_BINARY_DIR}/yosys_verific_rs/analyze/share/verific/vhdl_packages
            ${CMAKE_CURRENT_BINARY_DIR}/share/verific/vhdl_packages)
  endif()
  

  add_custom_command(TARGET raptor-bin POST_BUILD
      COMMAND echo "Copying VPR and OpenFPGA in bin dir"
      COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_BINARY_DIR}/Backend/OpenFPGA/vtr-verilog-to-routing/vpr/vpr
        ${CMAKE_CURRENT_BINARY_DIR}/bin/
      COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_BINARY_DIR}/Backend/OpenFPGA/openfpga/openfpga
        ${CMAKE_CURRENT_BINARY_DIR}/bin/
      COMMAND ${CMAKE_COMMAND} -E copy
        ${PROJECT_SOURCE_DIR}/Backend/pin_c/pin_c
        ${CMAKE_CURRENT_BINARY_DIR}/bin/
      COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_BINARY_DIR}/Backend/stars/stars
        ${CMAKE_CURRENT_BINARY_DIR}/bin/)
  add_custom_command(TARGET raptor-bin POST_BUILD
        COMMAND echo "rename raptor binary as raptor.exe"
        COMMAND ${CMAKE_COMMAND} -E rename
          ${CMAKE_CURRENT_BINARY_DIR}/bin/raptor
          ${CMAKE_CURRENT_BINARY_DIR}/bin/raptor.exe)
  add_custom_command(TARGET raptor-bin POST_BUILD
        COMMAND echo "copy to_be_raptor_lnx as raptor"
        COMMAND ${CMAKE_COMMAND} -E copy
          ${PROJECT_SOURCE_DIR}/src/to_be_raptor_linx
          ${CMAKE_CURRENT_BINARY_DIR}/bin/raptor
        COMMAND ${CMAKE_COMMAND} -E copy
          ${PROJECT_SOURCE_DIR}/src/.raptorenv_lin64.sh
          ${CMAKE_CURRENT_BINARY_DIR})
  add_custom_command(TARGET raptor-bin POST_BUILD
        COMMAND echo "copy FOEDAG tclsh8.6 to bin"
        COMMAND ${CMAKE_COMMAND} -E copy
          ${CMAKE_CURRENT_BINARY_DIR}/FOEDAG_rs/FOEDAG/bin/tclsh8.6
          ${CMAKE_CURRENT_BINARY_DIR}/bin/
        )
  add_custom_command(TARGET raptor-bin POST_BUILD
        COMMAND echo "copy FOEDAG tclsh8.6.so to GTKwave"
        COMMAND ${CMAKE_COMMAND} -E copy
          ${CMAKE_CURRENT_BINARY_DIR}/FOEDAG_rs/FOEDAG/lib/libtcl8.6.so
           ${CMAKE_CURRENT_BINARY_DIR}/bin/gtkwave/bin/lib
	      )
  add_custom_command(TARGET raptor-bin POST_BUILD
        COMMAND echo "remove GTKwave tcl8.6"
        COMMAND ${CMAKE_COMMAND} -E remove_directory
           ${CMAKE_CURRENT_BINARY_DIR}/bin/gtkwave/bin/lib/tcl8.6
        COMMAND echo "copy FOEDAG tcl8.6 to GTKwave"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
             ${PROJECT_SOURCE_DIR}/FOEDAG_rs/FOEDAG/third_party/tcl8.6.12/library
              ${CMAKE_CURRENT_BINARY_DIR}/bin/gtkwave/bin/lib/tcl8.6
        COMMAND echo "copy GTKwave tk to GTKwave tcl8.6"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
              ${CMAKE_CURRENT_BINARY_DIR}/bin/gtkwave/bin/lib/tk8.6
                 ${CMAKE_CURRENT_BINARY_DIR}/bin/gtkwave/bin/lib/tcl8.6
           )       
  if (PRODUCTION_BUILD AND (NOT ENABLE_OPENSTA))
     add_custom_command(TARGET raptor-bin POST_BUILD
        COMMAND echo "Remove OpenSTA from Production build"
        COMMAND ${CMAKE_COMMAND} -E rm -f
          ${CMAKE_CURRENT_BINARY_DIR}/bin/sta
        )
  endif()
endif()
