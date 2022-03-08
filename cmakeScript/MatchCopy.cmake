# Copyright (c) 2021. kms1212(Minsu Kwon)
# This file is part of OpenFSL.
# 
# OpenFSL and its source code is published over BSD 3-Clause License.
# See the BSD-3-Clause for more details.
# <https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

string(REPLACE "<>" ";" ARG_SRCFILE ${ARG_SRCFILE})

set(FILES "")
file(GLOB_RECURSE FILES ${ARG_SRCFILE})

execute_process(COMMAND ${CMAKE_COMMAND} -E copy ${FILES} ${ARG_DESTDIR})