# Copyright (c) 2021. kms1212(Minsu Kwon)
# This file is part of OpenFSL.
# 
# OpenFSL and its source code is published over BSD 3-Clause License.
# Check the full BSD-3-Clause license for more details.
# <https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

macro(listsubdir RESULT DIR)
    file(GLOB CHILDREN RELATIVE ${DIR} ${DIR}/*)
    set(DIRS "")
    foreach(CHILD ${CHILDREN})
        if(IS_DIRECTORY ${DIR}/${CHILD})
            list(APPEND DIRS ${CHILD})
        endif()
    endforeach()
    set(${RESULT} ${DIRS})
endmacro()