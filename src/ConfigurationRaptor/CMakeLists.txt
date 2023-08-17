cmake_minimum_required(VERSION 3.15)

set(cfg_path ${CMAKE_CURRENT_SOURCE_DIR})
project(configurationRaptor)

add_custom_target(configurationRaptor
	COMMAND
		echo "This is top level build for ConfigurationRaptor subsystem. It does not really build any target but it defines all subsystem or subdirectory and their dependencies"
	)

add_custom_target(pre-configurationRaptor
	COMMAND
		echo "This is top level build for Pre-ConfigurationRaptor subsystem. It does not really build any target but it defines all subsystem or subdirectory that need to be built in configuration first"
	)

# Define the subsystem here
list(APPEND SUBSYSTEMS CFGDeviceDatabase)

# set parent scope directory definition before subdirectory is added so that it can be inherited
#	in each subdirectory avoid using using ".." relative path -- it is very hard to manage if we have multiple layer
set(CFG_PROJECT_ROOT_DIR ${PROJECT_SOURCE_DIR}/../..)
set(CFG_FOEDAG_PROJECT_ROOT_DIR ${CFG_PROJECT_ROOT_DIR}/FOEDAG_rs/FOEDAG/src/Configuration)
set(CFG_FOEDAGRS_PROJECT_ROOT_DIR ${CFG_PROJECT_ROOT_DIR}/FOEDAG_rs/src/ConfigurationRS)
set(CFG_RAPTOR_PROJECT_ROOT_DIR ${CFG_PROJECT_ROOT_DIR}/src/ConfigurationRaptor)
set(CFG_BUILD_ROOT_DIR ${CMAKE_CURRENT_BINARY_DIR}/../..)
message("ConfigurationRaptor source dirctory is ${PROJECT_SOURCE_DIR}")
message("ConfigurationRaptor binary dirctory is ${CMAKE_CURRENT_BINARY_DIR}")
message("ConfigurationRaptor set project root directory as ${CFG_PROJECT_ROOT_DIR}")
message("ConfigurationRaptor set build root directory as ${CFG_BUILD_ROOT_DIR}")

# Determine the library type for configuration
set(CFG_LIB_TYPE STATIC)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CFG_BUILD_ROOT_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CFG_BUILD_ROOT_DIR}/bin)

# subdirectory
foreach(SUBSYSTEM ${SUBSYSTEMS})
	add_subdirectory(${SUBSYSTEM})
endforeach()

# default dependencies on configuraiton (with outside world)
# 	at top-level CMakeLists.txt, we can define configurationRS dependencies
#   this default dependencis make sure all the subsystem inherit configurationRS dependencies
foreach(SUBSYSTEM ${SUBSYSTEMS})
	string(TOLOWER ${SUBSYSTEM} SUBSYSTEM)
	add_dependencies(configurationRaptor ${SUBSYSTEM})
endforeach()
