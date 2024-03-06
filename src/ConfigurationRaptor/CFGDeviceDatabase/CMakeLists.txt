# -*- mode:cmake -*-

# Copyright 2022 The Foedag team

# GPL License

# Copyright (c) 2022 The Open-Source FPGA Foundation

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.15)
set(subsystem cfgdevicedatabase)
set(subsystem_exe cfg_device_database)
set(subsystem_file ${CFG_BUILD_ROOT_DIR}/share/raptor/configuration/devices.ddb)
project(${subsystem} LANGUAGES CXX)
add_custom_target(${subsystem}
	COMMAND echo "***********************"
  COMMAND echo "* This is to generate configuration device database"
  COMMAND echo "***********************"
)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
else()
  set(CMAKE_CXX_FLAGS_DEBUG
  "${CMAKE_CXX_FLAGS_DEBUG} ${TCMALLOC_COMPILE_OPTIONS} -Werror -Wall -O0 -g ${MSYS_COMPILE_OPTIONS} ${MY_CXX_WARNING_FLAGS} ${MEM_SANITIZER_FLAGS}")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Werror")
endif()

#################################################################################################################
#
# Three elements will be built
#
# 1. subsystem executable
#
# 2. generate device database file
#
#################################################################################################################

###################
#
# Executable
#
###################
add_executable(
  ${subsystem_exe}
  CFGDeviceDatabase.cpp
)
target_link_libraries(${subsystem_exe} cfgcommon)
target_link_libraries(${subsystem_exe} cfgcommonrs)
target_link_libraries(${subsystem_exe} cfgobject)
add_dependencies(${subsystem_exe} foedag_nlohmann_json)
target_include_directories(${subsystem_exe} PUBLIC
    ${CFG_PROJECT_ROOT_DIR}
    ${CFG_PROJECT_ROOT_DIR}/FOEDAG_rs/FOEDAG/src/Configuration
    ${CFG_PROJECT_ROOT_DIR}/FOEDAG_rs/src/ConfigurationRS
    ${CFG_PROJECT_ROOT_DIR}/src/ConfigurationRaptor
    ${CFG_BUILD_ROOT_DIR}/FOEDAG_rs/FOEDAG/src
    ${CFG_BUILD_ROOT_DIR}/FOEDAG_rs/FOEDAG/src/Configuration
    ${CFG_BUILD_ROOT_DIR}/FOEDAG_rs/src
    ${CFG_BUILD_ROOT_DIR}/FOEDAG_rs/src/ConfigurationRS
    ${CFG_BUILD_ROOT_DIR}/src
    ${CFG_BUILD_ROOT_DIR}/src/ConfigurationRaptor
    ${CFG_BUILD_ROOT_DIR}/FOEDAG/include
    ${CFG_BUILD_ROOT_DIR}/FOEDAG_rs/FOEDAG/include
    ${CFG_BUILD_ROOT_DIR}/include
)

###################
#
# generate database file
#
###################
add_custom_target(subsystem_file ALL
  DEPENDS ${CFG_PROJECT_ROOT_DIR}/etc/devices ${subsystem_exe}
  COMMAND python3 ${PROJECT_SOURCE_DIR}/CFGDeviceDatabase.py ${CFG_PROJECT_ROOT_DIR}/etc ${CMAKE_CURRENT_BINARY_DIR}/configuration.json
  COMMAND ${subsystem_exe} ${CMAKE_CURRENT_BINARY_DIR}/configuration.json ${subsystem_file}
)

add_dependencies(${subsystem} ${subsystem_exe})
add_dependencies(${subsystem} subsystem_file)

###################
#
# install 
#   - for header file(s), only put those that we really need to export
#
###################
set(
  SRC_H_INSTALL_LIST
)

set(
  SRC_LIB_INSTALL_LIST
  ${subsystem}
)

foreach(file ${SRC_H_INSTALL_LIST})
  install(
    FILES ${PROJECT_SOURCE_DIR}/${file}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/foedag/CFGCommonRS
  )
endforeach()
