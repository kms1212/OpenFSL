# Copyright (c) 2021. kms1212(Minsu Kwon)
# This file is part of OpenFSL.
# 
# OpenFSL and its source code is published over BSD 3-Clause License.
# Check the full BSD-3-Clause license for more details.
# <https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

cmake_minimum_required(VERSION 3.12)

listsubdir(SUBDIRS ${CMAKE_SOURCE_DIR}/fs)
foreach(SUBDIR ${SUBDIRS})
	include(${CMAKE_SOURCE_DIR}/fs/${SUBDIR}/PackageConfig.cmake)
endforeach()