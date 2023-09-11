# Copyright 2021 The Foedag team

# GPL License

# Copyright (c) 2021 The Open-Source FPGA Foundation

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

set (project raptor-programmer-gui)
project(${project} LANGUAGES CXX)

if (PRODUCTION_BUILD)
  message("Production Build type set to ON")
  set (PRODUCTION_BUILD_FLAG "-DPRODUCTION_BUILD=1")
endif(PRODUCTION_BUILD)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
else()
  set(CMAKE_CXX_FLAGS_DEBUG
  "${CMAKE_CXX_FLAGS_DEBUG} ${TCMALLOC_COMPILE_OPTIONS} -Werror -Wall -O0 -g ${MSYS_COMPILE_OPTIONS} ${MY_CXX_WARNING_FLAGS} ${MEM_SANITIZER_FLAGS} ${PRODUCTION_BUILD_FLAG}")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${PRODUCTION_BUILD_FLAG} -Werror -Wall")
endif()

include_directories(${PROJECT_SOURCE_DIR}/../../src ${PROJECT_SOURCE_DIR}/.. ${CMAKE_CURRENT_BINARY_DIR}/../../include/)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../../lib)

set (SRC_CPP_LIST
  main.cpp
)

if(MSVC)
  add_executable(${project} ${SRC_CPP_LIST})
  set_property(TARGET ${project} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
  set_property(TARGET ${project} PROPERTY COMPILER_FLAGS /DSTATIC_BUILD)
else()
  add_executable(${project} ${SRC_CPP_LIST})
endif()
set_target_properties(${project} PROPERTIES OUTPUT_NAME programmer)
target_link_libraries(${project} programmer-gui raptor_gui foedagcore)
