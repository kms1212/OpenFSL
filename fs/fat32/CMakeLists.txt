# Copyright (c) 2021. kms1212(Minsu Kwon)
# This file is part of OpenFSL.
# 
# OpenFSL and its source code is published over BSD 3-Clause License.
# Check the full BSD-3-Clause license for more details.
# <https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

cmake_minimum_required(VERSION 3.12)

include("${CMAKE_CURRENT_SOURCE_DIR}/ModuleConfig.cmake")

project("OpenFSL_${FILESYSTEM_NAME}")
    
set(BUILD_${FILESYSTEM_NAME} true CACHE BOOL "Build ${FILESYSTEM_NAME} filesystem?")

if(BUILD_${FILESYSTEM_NAME})
    string(TOLOWER ${PROJECT_NAME} PROJECT_TARGET)
    
    file(GLOB_RECURSE LIBRARY_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*)
    add_library(${PROJECT_TARGET} ${LIBRARY_TYPE} ${LIBRARY_SOURCES})
    target_link_libraries(${PROJECT_TARGET} openfsl)
    target_include_directories(${PROJECT_TARGET} PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/>
        $<INSTALL_INTERFACE:include/>)
    configure_target_options(${PROJECT_TARGET} LIB)
    configure_build_directory(${PROJECT_TARGET} "${CMAKE_BINARY_DIR}/output/${PROJECT_TARGET}")
    add_dependencies(${PROJECT_TARGET} openfsl)
    
    if(BUILD_TESTING)
        include (CTest)
    
        add_subdirectory(tests)
    endif (BUILD_TESTING)
endif()