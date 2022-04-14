# Copyright (c) 2021. kms1212(Minsu Kwon)
# This file is part of OpenFSL.
# 
# OpenFSL and its source code is published over BSD 3-Clause License.
# Check the full BSD-3-Clause license for more details.
# <https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

function(CONFIGURE_BUILD_DIRECTORY TARGET BUILD_DIR)
	set_target_properties(${TARGET} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${BUILD_DIR}")
    set_target_properties(${TARGET} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${BUILD_DIR}")
    set_target_properties(${TARGET} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${BUILD_DIR}")
    foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
        string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
        set_target_properties(${TARGET} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${BUILD_DIR}")
        set_target_properties(${TARGET} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${BUILD_DIR}")
        set_target_properties(${TARGET} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${BUILD_DIR}")
    endforeach(OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES)
endfunction(CONFIGURE_BUILD_DIRECTORY)