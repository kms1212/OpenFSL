# Copyright (c) 2021. kms1212(Minsu Kwon)
# This file is part of OpenFSL.
# 
# OpenFSL and its source code is published over BSD 3-Clause License.
# Check the full BSD-3-Clause license for more details.
# <https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

function(CONFIGURE_TARGET_OPTIONS TARGET TYPE)
    set_property(TARGET ${TARGET} PROPERTY C_STANDARD 11)
    set_property(TARGET ${TARGET} PROPERTY CXX_STANDARD 11)

    set_target_properties(${TARGET} PROPERTIES COMPILE_OPTIONS "")
    set_target_properties(${TARGET} PROPERTIES LINK_OPTIONS "")
    
    target_compile_options(${TARGET} BEFORE PUBLIC $<$<COMPILE_LANGUAGE:C>:${C_OPTIONS_GLOB}>)
    target_compile_options(${TARGET} BEFORE PUBLIC $<$<COMPILE_LANGUAGE:CXX>:${CXX_OPTIONS_GLOB}>)
    target_link_options(${TARGET} BEFORE PUBLIC ${LD_OPTIONS_GLOB})

    target_compile_options(${TARGET} BEFORE PUBLIC $<$<COMPILE_LANGUAGE:C>:${C_OPTIONS_${TYPE}}>)
    target_compile_options(${TARGET} BEFORE PUBLIC $<$<COMPILE_LANGUAGE:CXX>:${CXX_OPTIONS_${TYPE}}>)
    target_link_options(${TARGET} BEFORE PUBLIC ${LD_OPTIONS_${TYPE}})
endfunction(CONFIGURE_TARGET_OPTIONS)