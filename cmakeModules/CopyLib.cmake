# Copyright (c) 2021. kms1212(Minsu Kwon)
# This file is part of OpenFSL.
# 
# OpenFSL and its source code is published over BSD 3-Clause License.
# See the BSD-3-Clause for more details.
# <https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

function(COPY_LIB TESTNAME REQUIRED_FILE_LIST DEPENDS)
if ("${LIBRARY_TYPE}" STREQUAL "SHARED")
    add_custom_target(${TESTNAME}_COPY_LIB ALL
                    DEPENDS ${DEPENDS}
                    COMMAND ${CMAKE_COMMAND} 
                            -DARG_SRCFILE="${REQUIRED_FILE_LIST}"
                            -DARG_DESTDIR="${CMAKE_CURRENT_BINARY_DIR}"
                            -P "${CMAKE_SOURCE_DIR}/cmakeScript/MatchCopy.cmake"
                    COMMENT "Copying required files to test binary directory...")
endif()
endfunction(COPY_LIB)