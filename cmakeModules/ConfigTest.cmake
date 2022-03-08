# Copyright (c) 2021. kms1212(Minsu Kwon)
# This file is part of OpenFSL.
# 
# OpenFSL and its source code is published over BSD 3-Clause License.
# See the BSD-3-Clause for more details.
# <https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

function(CONFIGURE_TEST TESTNAME TEST_COMMAND_LIST REQUIRED_FILE_LIST IMAGE_FILE DEPENDS LABEL)
    foreach(TEST_COMMAND ${TEST_COMMAND_LIST})
        add_test(NAME ${TESTNAME}_${TEST_COMMAND}
                 COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${TESTNAME} 
                         ${IMAGE_FILE}
                         ${TEST_COMMAND})

        set_property(TEST ${TESTNAME}_${TEST_COMMAND} APPEND PROPERTY 
                     LABELS "${LABEL}")
        set_property(TEST ${TESTNAME}_${TEST_COMMAND} APPEND PROPERTY 
                     TIMEOUT 3)
        set_property(TEST ${TESTNAME}_${TEST_COMMAND} APPEND PROPERTY 
                     SKIP_RETURN_CODE 255)
        set_property(TEST ${TESTNAME}_${TEST_COMMAND} APPEND PROPERTY 
                     RUN_SERIAL true)
        
        message("Test ${TESTNAME}_${TEST_COMMAND} added.")
    endforeach()
endfunction(CONFIGURE_TEST)