cmake_minimum_required(VERSION 3.0)
function(BriningCommercialDevices)
    set(DEVICES_DIR "${CMAKE_CURRENT_LIST_DIR}/etc/devices")
    if(EXISTS "${DEVICES_DIR}")
        file(RENAME "${DEVICES_DIR}" "${CMAKE_CURRENT_LIST_DIR}/etc/devices_osfpga")
        message(STATUS "Renamed previous devices directory to devices_osfpga")
    else()
        message(FATAL_ERROR "Devices directory not found: ${DEVICES_DIR}")
    endif()
    message(STATUS "Fetching Assests from ${REPO}")
    execute_process(
        COMMAND git clone --filter=blob:none --no-checkout https://${TOKEN}@github.com/${REPO}.git ${SRC}
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
        RESULT_VARIABLE result_clone
        OUTPUT_VARIABLE output_clone
        ERROR_VARIABLE error_clone
    )
    if(NOT result_clone EQUAL 0)
        message(WARNING "Error during assest fetch. Please contact maintainer")
    else()
        execute_process(
            COMMAND git sparse-checkout set ${SRC}
            WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/${SRC}
            RESULT_VARIABLE result_sparse
            OUTPUT_VARIABLE output_sparse
            ERROR_VARIABLE error_sparse
        )
        if(NOT result_sparse EQUAL 0)
            message(FATAL_ERROR "Something gone wrong on populating the fetch assest. Please contact maintainer")
        endif()
        message(STATUS "Setting up the Assets")
        file(GLOB files_to_move "${CMAKE_CURRENT_LIST_DIR}/${SRC}/${SRC}/*")
        if(NOT files_to_move STREQUAL "")
            foreach(file ${files_to_move})
                #message("Moved ${file} to ${CMAKE_CURRENT_LIST_DIR}/${SRC}")
                file(COPY ${file} DESTINATION "${CMAKE_CURRENT_LIST_DIR}/${SRC}")
            endforeach()
        else()
            message("No files found to Setup")
        endif()
    endif()
endfunction()

if(NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/etc/devices/etc/devices")

    # Set the path to the .ini file in $HOME/.local
    set(INI_FILE "$ENV{HOME}/.local/repator-dev.ini")

    # Check if the .ini file exists
    if(NOT EXISTS "${INI_FILE}")
        message(WARNING "Config file not found: ${INI_FILE}")
    else()
        # Read the contents of the .ini file
        file(READ "${INI_FILE}" ini_content)

        # Extract TOKEN, REPO, and SRC using regex
        string(REGEX MATCH "TOKEN=([a-zA-Z0-9_]+)" token_match "${ini_content}")
        string(REGEX REPLACE "TOKEN=([a-zA-Z0-9_]+)" "\\1" TOKEN "${token_match}")

        string(REGEX MATCH "REPO=([a-zA-Z0-9/_-]+)" repo_match "${ini_content}")
        string(REGEX REPLACE "REPO=([a-zA-Z0-9/_-]+)" "\\1" REPO "${repo_match}")

        string(REGEX MATCH "SRC=([a-zA-Z0-9/_-]+)" src_match "${ini_content}")
        string(REGEX REPLACE "SRC=([a-zA-Z0-9/_-]+)" "\\1" SRC "${src_match}")

        # Output the extracted values (for debug purposes)
        #message("GitHub Token: ${TOKEN}")
        #message("GitHub Repo: ${REPO}")
        #message("Source Path: ${SRC}")
        # call function to populate Commercial devices
        BriningCommercialDevices()
    endif()

endif()