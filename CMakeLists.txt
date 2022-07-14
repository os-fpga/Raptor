# -*- mode:cmake -*-

# Copyright 2021-2022 The RapidSilicon team

cmake_minimum_required(VERSION 3.15)

if (PRODUCTION_BUILD)
  message("Production Build type set to ON")
  set (PRODUCTION_BUILD_FLAG "-DPRODUCTION_BUILD=1")
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

add_subdirectory(FOEDAG_rs)
if (APPLE)
    option(BUILD_YOSYS_PLUGINS OFF) # Currently yosys-plugins doesn't build on MacOS
endif()
add_subdirectory(yosys_verific_rs)
if (NOT APPLE) # Currently OpenFPGA doesn't build on MacOS
    add_subdirectory(OpenFPGA_RS)
endif()

# Raptor version
set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_PATCH 16)

execute_process(COMMAND git config --global --add safe.directory ${CMAKE_CURRENT_SOURCE_DIR}
  COMMAND git rev-parse --short HEAD
  COMMAND tr -d '\n'
  OUTPUT_VARIABLE GIT_HASH
  ERROR_VARIABLE GIT_HASH_ERR
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

message(STATUS "GIT_HASH = ${GIT_HASH}")
configure_file(raptor_version.h.in ${CMAKE_CURRENT_BINARY_DIR}/FOEDAG_rs/FOEDAG/include/foedag_version.h)

# Require swig 3
find_package(SWIG 3.0 REQUIRED)

# Check system 
message("CMAKE_SYSTEM_NAME: ${CMAKE_SYSTEM_NAME}")

# Find Qt packages and error out if not found
find_package(Qt5 COMPONENTS Core Gui Widgets Quick Xml REQUIRED)
if (NOT Qt5Widgets_FOUND)
    message(STATUS "Failed to find Qt5Widgets required (on debian/ubuntu try 'sudo apt install qt5-default')")
elseif (NOT Qt5Gui_FOUND)
    message(STATUS "Failed to find Qt5Gui required (on debian/ubuntu try 'sudo apt install qt5-default')")
elseif (NOT Qt5Core_FOUND)
    message(STATUS "Failed to find Qt5Core required (on debian/ubuntu try 'sudo apt install qt5-default')")
elseif (NOT Qt5Quick_FOUND)
    message(STATUS "Failed to find Qt5Core required (on debian/ubuntu try 'sudo apt install libkf5qqc2desktopstyle-dev')")
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
  add_executable(raptor-bin ${PROJECT_SOURCE_DIR}/src/Main/main.cpp  ${PROJECT_SOURCE_DIR}/FOEDAG_rs/src/Compiler/CompilerRS.cpp ${QRC_MAIN_WINDOW})
  set_property(TARGET raptor-bin PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
  set_property(TARGET raptor-bin PROPERTY COMPILER_FLAGS /DSTATIC_BUILD)
else()
  add_executable(raptor-bin ${PROJECT_SOURCE_DIR}/src/Main/main.cpp ${PROJECT_SOURCE_DIR}/FOEDAG_rs/src/Compiler/CompilerRS.cpp ${QRC_MAIN_WINDOW})
endif()
set_target_properties(raptor-bin PROPERTIES OUTPUT_NAME raptor)



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
add_dependencies(raptor-bin yosys)
add_dependencies(raptor-bin abc)
add_dependencies(raptor-bin de)

if (WIN OR APPLE)
else ()
  # Guaranties all the executables are built before final copy in build directory 
  add_dependencies(raptor-bin libopenfpga)
  add_dependencies(raptor-bin openfpga) 
  add_dependencies(raptor-bin vpr)
endif()

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

target_link_libraries(raptor-bin PUBLIC raptor foedag  )
target_link_libraries(raptor PUBLIC  Qt5::Widgets Qt5::Core Qt5::Gui Qt5::Xml Qt5::Quick)

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
    DIRECTORY etc/ DESTINATION  ${CMAKE_INSTALL_DATAROOTDIR}/raptor/etc/)
install(
      DIRECTORY tests/Testcases/aes_decrypt_fpga/ DESTINATION  ${CMAKE_INSTALL_DATAROOTDIR}/raptor/examples/aes_decrypt_fpga/)
install(
      DIRECTORY tests/Testcases/aes_decrypt_gate/ DESTINATION  ${CMAKE_INSTALL_DATAROOTDIR}/raptor/examples/aes_decrypt_gate/)
install(
      DIRECTORY tests/Testcases/sasc_testcase/ DESTINATION  ${CMAKE_INSTALL_DATAROOTDIR}/raptor/examples/sasc_testcase/)
install(
      DIRECTORY tests/Testcases/and2_testcase/ DESTINATION  ${CMAKE_INSTALL_DATAROOTDIR}/raptor/examples/and2_testcase/)
install(
      DIRECTORY tests/Testcases/and2_testcase/ DESTINATION  ${CMAKE_INSTALL_DATAROOTDIR}/raptor/examples/and2_gemini/)      
install(
      DIRECTORY tests/Testcases/counter/ DESTINATION  ${CMAKE_INSTALL_DATAROOTDIR}/raptor/examples/counter/)
install(
      FILES README.md DESTINATION  ${CMAKE_INSTALL_DATAROOTDIR}/raptor/doc/)

#install(
#  EXPORT Raptor
#  FILE Raptor.cmake
#  DESTINATION cmake)
include(CMakePackageConfigHelpers)


add_custom_target(raptor_link_target ALL
                  COMMAND ${CMAKE_COMMAND} -E create_symlink
                  build/compile_commands.json ../compile_commands.json)

set(YOSYS_BUILD_PATH "$<$<CONFIG:Debug>:debug-install>$<$<CONFIG:Release>:install>") 

add_custom_command(TARGET raptor-bin POST_BUILD
      COMMAND echo "Copying arch and example files in share dir" 
      COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/share/raptor/etc/
      COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/share/raptor/etc/settings/
      COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/share/raptor/examples/aes_decrypt_fpga/
      COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/share/raptor/examples/sasc_testcase/
      COMMAND ${CMAKE_COMMAND} -E copy_directory 
          ${PROJECT_SOURCE_DIR}/etc/    
          ${CMAKE_CURRENT_BINARY_DIR}/share/raptor/etc/
      COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${PROJECT_SOURCE_DIR}/tests/Testcases/aes_decrypt_fpga
          ${CMAKE_CURRENT_BINARY_DIR}/share/raptor/examples/aes_decrypt_fpga/
      COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${PROJECT_SOURCE_DIR}/tests/Testcases/aes_decrypt_gate
          ${CMAKE_CURRENT_BINARY_DIR}/share/raptor/examples/aes_decrypt_gate/
      COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${PROJECT_SOURCE_DIR}/tests/Testcases/sasc_testcase
          ${CMAKE_CURRENT_BINARY_DIR}/share/raptor/examples/sasc_testcase/
      COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${PROJECT_SOURCE_DIR}/tests/Testcases/and2_testcase
          ${CMAKE_CURRENT_BINARY_DIR}/share/raptor/examples/and2_testcase/
      COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${PROJECT_SOURCE_DIR}/tests/Testcases/and2_gemini
          ${CMAKE_CURRENT_BINARY_DIR}/share/raptor/examples/and2_gemini/          
      COMMAND echo "Copying Yosys, ABC, DE in bin dir"     
      COMMAND ${CMAKE_COMMAND} -E copy 
          ${PROJECT_SOURCE_DIR}/yosys_verific_rs/yosys/${YOSYS_BUILD_PATH}/bin/yosys        
          ${CMAKE_CURRENT_BINARY_DIR}/bin/
      COMMAND ${CMAKE_COMMAND} -E copy 
          ${PROJECT_SOURCE_DIR}/yosys_verific_rs/yosys/${YOSYS_BUILD_PATH}/bin/abc        
          ${CMAKE_CURRENT_BINARY_DIR}/bin/
      COMMAND ${CMAKE_COMMAND} -E copy 
          ${PROJECT_SOURCE_DIR}/yosys_verific_rs/yosys/${YOSYS_BUILD_PATH}/bin/de    
          ${CMAKE_CURRENT_BINARY_DIR}/bin/)

if (WIN OR APPLE)
else ()
  install(
      FILES ${PROJECT_SOURCE_DIR}/OpenFPGA_RS/pin_constraint/pin_c  DESTINATION ${CMAKE_INSTALL_BINDIR} PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE )
  install(
      FILES ${PROJECT_SOURCE_DIR}/OpenFPGA_RS/vpr/libvpr.so  DESTINATION ${CMAKE_INSTALL_BINDIR} PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE )
  add_custom_command(TARGET raptor-bin POST_BUILD
      COMMAND echo "Copying VPR and OpenFPGA in bin dir"
      COMMAND ${CMAKE_COMMAND} -E copy 
          ${PROJECT_SOURCE_DIR}/OpenFPGA_RS/vpr/vpr
          ${CMAKE_CURRENT_BINARY_DIR}/bin/
      COMMAND ${CMAKE_COMMAND} -E copy 
          ${PROJECT_SOURCE_DIR}/OpenFPGA_RS/openfpga/openfpga
          ${CMAKE_CURRENT_BINARY_DIR}/bin/
      COMMAND ${CMAKE_COMMAND} -E copy 
          ${PROJECT_SOURCE_DIR}/OpenFPGA_RS/pin_constraint/pin_c
          ${CMAKE_CURRENT_BINARY_DIR}/bin/    
     )
endif()
