# Copyright (c) 2021. kms1212(Minsu Kwon)
# This file is part of OpenFSL.
# 
# OpenFSL and its source code is published over BSD 3-Clause License.
# Check the full BSD-3-Clause license for more details.
# <https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

cmake_minimum_required(VERSION 3.12)

listsubdir(SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR})
foreach(SUBDIR ${SUBDIRS})
	add_subdirectory(${SUBDIR})
endforeach()

if (BUILD_TESTING)
	include (CTest)
endif()