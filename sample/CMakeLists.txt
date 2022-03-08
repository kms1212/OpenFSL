# Copyright (c) 2021. kms1212(Minsu Kwon)
# This file is part of OpenFSL.
# 
# OpenFSL and its source code is published over BSD 3-Clause License.
# See the BSD-3-Clause for more details.
# <https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

cmake_minimum_required(VERSION 3.12)
project("ofsl_sample")

set(BUILD_SAMPLE false CACHE BOOL "Build sample executable?")

if(BUILD_SAMPLE)
    set(BUILD_SAMPLE_GUI false CACHE BOOL "Build GUI sample?")


    if (BUILD_SAMPLE_GUI)
        file(GLOB PROGRAM_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/gui/src/*")

        if (MSVC)
            add_executable(${PROJECT_NAME} WIN32 ${PROGRAM_SOURCES})
        else()
            add_executable(${PROJECT_NAME} ${PROGRAM_SOURCES})
        endif()
        include_directories(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/gui/include")

        # wxWidgets library for gui
        find_package(wxWidgets REQUIRED gl core base)
        include(${wxWidgets_USE_FILE})
        include_directories(${wxWidgets_INCLUDE_DIRS})
        target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES})
    else()
        file(GLOB PROGRAM_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/cli/src/*")
        add_executable(${PROJECT_NAME} ${PROGRAM_SOURCES})
        include_directories(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/cli/include")
    endif()

    # Link base library
    target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/include)
    target_link_libraries(${PROJECT_NAME} openfsl)
    add_dependencies(${PROJECT_NAME} openfsl)
    configure_target_options(${PROJECT_NAME} LIB)
    configure_build_directory(${PROJECT_NAME} "${CMAKE_CURRENT_BINARY_DIR}")
    set(COPY_LIST "${CMAKE_BINARY_DIR}/output/openfsl/\*")
    list(APPEND DEP_LIST "openfsl")

    if (BUILD_FAT32) # Link FAT32 library if built
        target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/fs/fat32/include)
        target_link_libraries(${PROJECT_NAME} openfsl_fat32)
        target_compile_definitions(${PROJECT_NAME} PUBLIC FAT32_BUILD)
        add_dependencies(${PROJECT_NAME} openfsl_fat32)
        set(COPY_LIST "${COPY_LIST}<>${CMAKE_BINARY_DIR}/output/openfsl_fat32/\*")
        list(APPEND DEP_LIST "openfsl_fat32")
    endif()

    if (BUILD_NTFS) # Link NTFS library if built
        target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/fs/ntfs/include)
        target_link_libraries(${PROJECT_NAME} openfsl_ntfs)
        target_compile_definitions(${PROJECT_NAME} PUBLIC NTFS_BUILD)
        add_dependencies(${PROJECT_NAME} openfsl_ntfs)
        set(COPY_LIST "${COPY_LIST}<>${CMAKE_BINARY_DIR}/output/openfsl_ntfs/\*")
        list(APPEND DEP_LIST "openfsl_ntfs")
    endif()

    if (BUILD_ISO9660) # Link ISO9660 library if built
        target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/fs/iso9660/include)
        target_link_libraries(${PROJECT_NAME} openfsl_iso9660)
        target_compile_definitions(${PROJECT_NAME} PUBLIC ISO9660_BUILD)
        add_dependencies(${PROJECT_NAME} openfsl_iso9660)
        set(COPY_LIST "${COPY_LIST}<>${CMAKE_BINARY_DIR}/output/openfsl_iso9660/\*")
        list(APPEND DEP_LIST "openfsl_iso9660")
    endif()

    copy_lib(${PROJECT_NAME}
            "${COPY_LIST}"
            "${DEP_LIST}")
endif()