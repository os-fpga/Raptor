cmake_minimum_required(VERSION 3.0)

project(LicenseManager)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_COMPILER g++)

add_library(rs_licenseManager License_manager.cpp)

target_include_directories(rs_licenseManager
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/x64_lsb
        ${CMAKE_CURRENT_SOURCE_DIR}/machind)

target_link_libraries(rs_licenseManager
    PUBLIC 
       ${CMAKE_CURRENT_SOURCE_DIR}/x64_lsb/lm_new.o
       ${CMAKE_CURRENT_SOURCE_DIR}/x64_lsb/liblmgr_trl.a 
       ${CMAKE_CURRENT_SOURCE_DIR}/x64_lsb/libcrvs.a 
       ${CMAKE_CURRENT_SOURCE_DIR}/x64_lsb/libsb.a 
       ${CMAKE_CURRENT_SOURCE_DIR}/x64_lsb/liblmgr_dongle_stub.a 
       ${CMAKE_CURRENT_SOURCE_DIR}/x64_lsb/activation/lib/libnoact.a
       ${CMAKE_DL_LIBS})

set(CXX_OPTS "-fPIC;-fvisibility=hidden;-no-pie;-lrt;-Wl,--no-as-needed;-ldl;-pthread")
target_compile_options(rs_licenseManager PUBLIC ${CXX_OPTS})
target_link_options(rs_licenseManager PUBLIC ${CXX_OPTS})
