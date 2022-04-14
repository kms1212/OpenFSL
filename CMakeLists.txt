# Copyright (c) 2021. kms1212(Minsu Kwon)
# This file is part of OpenFSL.
# 
# OpenFSL and its source code is published over BSD 3-Clause License.
# Check the full BSD-3-Clause license for more details.
# <https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

cmake_minimum_required(VERSION 3.12)

include(ExternalProject)

set(NO_BUILD false CACHE BOOL "Skip build?")
set(DOXYGEN_GENERATE false CACHE BOOL "Generate doxygen file?")

if(DOXYGEN_GENERATE)
    find_package(Doxygen)
    if(DOXYGEN_FOUND)
        find_package(Doxygen REQUIRED dot)
    else(DOXYGEN_FOUND)
        message(WARNING "Doxygen is not found. Creating documents will be disabled.")
        set (DOXYGEN_GENERATE false)
    endif(DOXYGEN_FOUND)

    if(DOXYGEN_FOUND)
        add_custom_target(doc_doxygen ALL
            COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_SOURCE_DIR}/Doxyfile
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMENT "Generating library documentation with Doxygen"
            VERBATIM )
    endif(DOXYGEN_FOUND)
endif(DOXYGEN_GENERATE)
    
if(NOT NO_BUILD)
    set(TARGET_ARCHITECTURE "native" CACHE STRING "Target system architecture for gcc(e.g. x86-64, i686)")
    set(TARGET_TRIPLET "x86_64-unknown-linux-gnu" CACHE STRING "Target system triplet for clang(def. x86_64-unknown-linux-gnu)")
    set(BUILD_TESTING true CACHE BOOL "Build test suite?")
    set(LIBRARY_TYPE "SHARED" CACHE STRING "Library type(SHARED, STATIC)")
    set(TEST_COVERAGE false CACHE BOOL "Build with coverage options")
    set(PACKAGE_GENERATE false CACHE BOOL "Generate package?")
    set(INCREASE_BUILD_NUMBER true CACHE BOOL "Increase build number?")

    if (INCREASE_BUILD_NUMBER)
        execute_process(COMMAND jq --help 
                        RESULT_VARIABLE JQ_RESULT
                        OUTPUT_QUIET)
        if (NOT ${JQ_RESULT} EQUAL 0)
            message(FATAL_ERROR "jq is not found.")
        endif()
    endif()

    find_package(Python3 REQUIRED COMPONENTS Interpreter)

    if ("${LIBRARY_TYPE}" STREQUAL "SHARED")
    elseif ("${LIBRARY_TYPE}" STREQUAL "STATIC")
    else()
        message("${LIBRARY_TYPE}" "Invalid library type.")
    endif()
    
    project("OpenFSL")
    string(TOLOWER ${CMAKE_PROJECT_NAME} PROJECT_TARGET)
    
    #########################
    # MODULES
    #########################
    include(${CMAKE_SOURCE_DIR}/cmakeModules/FindZIP.cmake)
    include(${CMAKE_SOURCE_DIR}/cmakeModules/ListSubDir.cmake)
    include(${CMAKE_SOURCE_DIR}/cmakeModules/ConfigBuildDirectory.cmake)
    include(${CMAKE_SOURCE_DIR}/cmakeModules/ConfigTargetOptions.cmake)
    include(${CMAKE_SOURCE_DIR}/cmakeModules/ConfigTest.cmake)
    include(${CMAKE_SOURCE_DIR}/cmakeModules/CopyLib.cmake)
    
    #########################
    # SCRIPT SETTINGS
    #########################
    enable_language(ASM_NASM)
    
    #########################
    # BUILD OPTIONS
    #########################
    set(CMAKE_CXX_STANDARD 11)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS NO)
    set(CMAKE_C_STANDARD 11)
    set(CMAKE_C_STANDARD_REQUIRED ON)
    set(CMAKE_C_EXTENSIONS NO)
    
    if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        # Flags for building library
        if(CMAKE_BUILD_TYPE STREQUAL "Debug")
            set(CXX_OPTIONS_GLOB "${CXX_OPTIONS_LIB} -g")
            set(C_OPTIONS_GLOB "${C_OPTIONS_LIB} -g")
        else()
        endif()
        if(DEFINED TARGET_TRIPLET)
            set(CXX_OPTIONS_LIB "${CXX_OPTIONS_LIB} --target=${TARGET_TRIPLET}")
            set(C_OPTIONS_LIB "${C_OPTIONS_LIB} --target=${TARGET_TRIPLET}")

            set(CXX_OPTIONS_TEST "${CXX_OPTIONS_TEST} --target=${TARGET_TRIPLET}")
            set(C_OPTIONS_TEST "${C_OPTIONS_TEST} --target=${TARGET_TRIPLET}")
        endif()
        if(TARGET_ARCHITECTURE MATCHES "i?86$")
            set(CXX_OPTIONS_LIB "${CXX_OPTIONS_LIB} -m32")
            set(C_OPTIONS_LIB "${C_OPTIONS_LIB} -m32")
            set(LD_OPTIONS_LIB "${LD_OPTIONS_LIB} -m32")

            set(CXX_OPTIONS_TEST "${CXX_OPTIONS_TEST} -m32")
            set(C_OPTIONS_TEST "${C_OPTIONS_TEST} -m32")
            set(LD_OPTIONS_TEST "${LD_OPTIONS_TEST} -m32")
        endif()

        set(CXX_OPTIONS_TEST "${CXX_OPTIONS_LIB}")
        set(C_OPTIONS_TEST "${C_OPTIONS_LIB}")
        set(LD_OPTIONS_TEST "${LD_OPTIONS_LIB}")
        message("${CXX_OPTIONS_TEST}")

        if(TEST_COVERAGE)
            set(CXX_OPTIONS_LIB "${CXX_OPTIONS_LIB} -fprofile-arcs -ftest-coverage")
            set(C_OPTIONS_LIB "${C_OPTIONS_LIB} -fprofile-arcs -ftest-coverage")
            set(LD_OPTIONS_LIB "${LD_OPTIONS_LIB} --coverage")
        endif()
        
        # Global flags
        set(CXX_OPTIONS_GLOB "${CXX_OPTIONS_GLOB} -Wall")
        set(C_OPTIONS_GLOB "${C_OPTIONS_GLOB} -Wall")
        set(LD_OPTIONS_GLOB "${LD_OPTIONS_GLOB} -pthread")
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        # Flags for building library
        if(CMAKE_BUILD_TYPE STREQUAL "Debug")
            set(CXX_OPTIONS_GLOB "${CXX_OPTIONS_LIB} -g")
            set(C_OPTIONS_GLOB "${C_OPTIONS_LIB} -g")
        endif()
        if(DEFINED TARGET_ARCHITECTURE)
            set(CXX_OPTIONS_LIB "${CXX_OPTIONS_LIB} -march=${TARGET_ARCHITECTURE}")
            set(C_OPTIONS_LIB "${C_OPTIONS_LIB} -march=${TARGET_ARCHITECTURE}")

            set(CXX_OPTIONS_TEST "${CXX_OPTIONS_TEST} -march=${TARGET_ARCHITECTURE}")
            set(C_OPTIONS_TEST "${C_OPTIONS_TEST} -march=${TARGET_ARCHITECTURE}")
        endif()
        if (TARGET_ARCHITECTURE MATCHES "i?86$")
            set(CXX_OPTIONS_LIB "${CXX_OPTIONS_LIB} -m32")
            set(C_OPTIONS_LIB "${C_OPTIONS_LIB} -m32")
            set(LD_OPTIONS_LIB "${LD_OPTIONS_LIB} -m32")

            set(CXX_OPTIONS_TEST "${CXX_OPTIONS_TEST} -m32")
            set(C_OPTIONS_TEST "${C_OPTIONS_TEST} -m32")
            set(LD_OPTIONS_TEST "${LD_OPTIONS_TEST} -m32")
        endif()
        if(TEST_COVERAGE)
            set(CXX_OPTIONS_LIB "${CXX_OPTIONS_LIB} -fprofile-arcs -ftest-coverage")
            set(C_OPTIONS_LIB "${C_OPTIONS_LIB} -fprofile-arcs -ftest-coverage")
            set(LD_OPTIONS_LIB "${LD_OPTIONS_LIB} --coverage -lgcov")
        endif()
        
        # Global flags
        set(CXX_OPTIONS_GLOB "${CXX_OPTIONS_GLOB} -Wall")
        set(C_OPTIONS_GLOB "${C_OPTIONS_GLOB} -Wall")
        set(LD_OPTIONS_GLOB "${LD_OPTIONS_GLOB} -pthread")
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        # Global 
        set(CXX_OPTIONS_GLOB "${CXX_OPTIONS_GLOB} /EHsc /W3 /wd5045 /wd4668 /wd4514 /wd5208 /wd5105 /utf-8")
        set(C_OPTIONS_GLOB "${C_OPTIONS_GLOB} /EHsc /W3 /wd5045 /wd4668 /wd4514 /wd5208 /wd5105 /utf-8")
        set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS true)
    endif()
    string(APPEND CMAKE_ASM_NASM_FLAGS "")
    
    separate_arguments(C_OPTIONS_LIB)
    separate_arguments(CXX_OPTIONS_LIB)
    separate_arguments(LD_OPTIONS_LIB)

    separate_arguments(C_OPTIONS_TEST)
    separate_arguments(CXX_OPTIONS_TEST)
    separate_arguments(LD_OPTIONS_TEST)
    
    separate_arguments(C_OPTIONS_GLOB)
    separate_arguments(CXX_OPTIONS_GLOB)
    separate_arguments(LD_OPTIONS_GLOB)

    #########################
    # VERSION CONFIGURATIONS
    #########################
    execute_process(COMMAND ${CMAKE_COMMAND} 
                            -DSOURCE_DIR=${CMAKE_SOURCE_DIR}
                            -DBINARY_DIR=${CMAKE_BINARY_DIR}
                            -DPACKAGE_TYPE=${PACKAGE_TYPE}
                            -DINCREASE_BUILD_NUMBER=${INCREASE_BUILD_NUMBER}
                            -DCXX_COMPILER_ID=${CMAKE_CXX_COMPILER_ID}
                            -DCXX_COMPILER_VERSION=${CMAKE_CXX_COMPILER_VERSION}
                            -DHOST_SYSTEM_PROCESSOR=${CMAKE_HOST_SYSTEM_PROCESSOR}
                            -DSYSTEM_NAME=${CMAKE_SYSTEM_NAME}
                            -P "${CMAKE_SOURCE_DIR}/cmakeScript/ConfigureVersion.cmake")
    include(${CMAKE_BINARY_DIR}/Version.cmake)
                   
    #########################
    # BUILD UTILITIES
    #########################
    add_subdirectory(buildutils)
    
    #########################
    # BUILD BOOT CODES
    #########################
    add_custom_command(
                      OUTPUT "${CMAKE_BINARY_DIR}/boot/x86/mbrboot.bin"
                      COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/boot/x86/"
                      COMMAND ${CMAKE_ASM_NASM_COMPILER} 
                              ${CMAKE_ASM_NASM_FLAGS} "-fbin" 
                              "-o${CMAKE_BINARY_DIR}/boot/x86/mbrboot.bin"
                              "${CMAKE_SOURCE_DIR}/boot/x86/mbrboot.asm"
                      DEPENDS "${CMAKE_SOURCE_DIR}/boot/x86/mbrboot.asm"
                      COMMENT "Assembling mbrboot assembly"
                      VERBATIM)
    
    
    add_custom_command(
                      OUTPUT "${CMAKE_BINARY_DIR}/boot/x86/vbrboot.bin"
                      COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/boot/x86/"
                      COMMAND ${CMAKE_ASM_NASM_COMPILER}
                              ${CMAKE_ASM_NASM_FLAGS} "-fbin" 
                              "-o${CMAKE_BINARY_DIR}/boot/x86/vbrboot.bin"
                              "${CMAKE_SOURCE_DIR}/boot/x86/vbrboot.asm"
                      DEPENDS "${CMAKE_SOURCE_DIR}/boot/x86/vbrboot.asm"
                      COMMENT "Assembling vbrboot assembly"
                      VERBATIM)
    
    #########################
    # CONVERT BINARY TO C
    #########################
    add_custom_command(
                      OUTPUT "${CMAKE_SOURCE_DIR}/include/openfsl/bootcode.h"
                      COMMAND "${Python3_EXECUTABLE}"
                              "${CMAKE_SOURCE_DIR}/buildutils/bintoc/bintoc.py"
                              "--header=${CMAKE_SOURCE_DIR}/etc/licenseheader.txt"
                              "--incfile"
                              "--incguard=OPENFSL_BOOTCODE_H_"
                              "${CMAKE_SOURCE_DIR}/include/openfsl/bootcode.h"
                              "${CMAKE_BINARY_DIR}/boot/x86/mbrboot.bin"
                              "${CMAKE_BINARY_DIR}/boot/x86/vbrboot.bin"
                      DEPENDS "${CMAKE_BINARY_DIR}/boot/x86/mbrboot.bin"
                              "${CMAKE_BINARY_DIR}/boot/x86/vbrboot.bin"
                      COMMENT "Converting boot record binary to C source")
                      
    add_custom_target(bootcode DEPENDS "${CMAKE_SOURCE_DIR}/include/openfsl/bootcode.h")

    #########################
    # BASE LIBRARY DEPENDENCIES
    #########################
    find_package(OpenSSL REQUIRED)
    find_package(ZLIB REQUIRED)

    #########################
    # CHECK SYMBOLS FOR LIBRARY
    #########################
    include(CheckCXXSymbolExists)
    include(CheckTypeSize)

    string(REPLACE ";" " " CXX_OPTIONS_CONFIGTEST "${CXX_OPTIONS_LIB};${CXX_OPTIONS_GLOB}")
    set(CMAKE_REQUIRED_FLAGS "${CXX_OPTIONS_CONFIGTEST}")
    set(CMAKE_EXTRA_INCLUDE_FILES "time.h")
    check_type_size("short" SHORT)
    check_type_size("int" INT)
    check_type_size("long" LONG)
    check_type_size("long long" LONG_LONG)
    check_type_size("double" DOUBLE)
    check_type_size("size_t" SIZE_T)
    check_type_size("uint8_t" UINT8_T)
    check_type_size("uint16_t" UINT16_T)
    check_type_size("uint32_t" UINT32_T)
    check_type_size("uint64_t" UINT64_T)
    check_type_size("time_t" TIME_T)
    check_type_size("__time64_t" __TIME64_T)
    check_type_size("struct tm" STRUCT_TM)
    check_type_size("error_t" ERROR_T)

    check_cxx_symbol_exists("_localtime64_s" "ctime" _LOCALTIME64_S_EXIST)
    if (_LOCALTIME64_S_EXIST STREQUAL "")
        set(_LOCALTIME64_S_EXIST "0")
    endif()
    check_cxx_symbol_exists("localtime_r" "ctime" LOCALTIME_R_EXIST)
    if (LOCALTIME_R_EXIST STREQUAL "")
        set(LOCALTIME_R_EXIST "0")
    endif()

    configure_file("${CMAKE_SOURCE_DIR}/include/openfsl/symbols.h.in"
                   "${CMAKE_SOURCE_DIR}/include/openfsl/symbols.h")
    
    #########################
    # BUILD BASE LIBRARY
    #########################
    file(GLOB LIBRARY_SOURCES "${CMAKE_SOURCE_DIR}/src/*")
    add_library(${PROJECT_TARGET} 
                ${LIBRARY_TYPE} ${LIBRARY_SOURCES})
    target_include_directories(${PROJECT_TARGET} PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/>
        $<INSTALL_INTERFACE:include/>)
    target_link_libraries(${PROJECT_TARGET} OpenSSL::Crypto)
    target_link_libraries(${PROJECT_TARGET} ZLIB::ZLIB)
    configure_target_options(${PROJECT_TARGET} LIB)
    configure_build_directory(${PROJECT_TARGET} "${CMAKE_BINARY_DIR}/output/${PROJECT_TARGET}")
    add_dependencies(${PROJECT_TARGET} bootcode)
        
    ## Submodule configurations ##############################################
    add_subdirectory(fs)
    
    add_subdirectory(sample)
    
    #########################
    # TEST
    #########################
    if(BUILD_TESTING)
        set_property(GLOBAL PROPERTY CTEST_TARGETS_ADDED 1)
        include (CTest)
    
        add_subdirectory(tests)
    endif (BUILD_TESTING)
    
    #########################
    # PACK
    #########################
    if(PACKAGE_GENERATE)
        set(PACKAGE_TYPE "NIGHTLY" CACHE STRING "Package type (NIGHTLY, BETA, RELEASE)")
        set(PACKAGE_GENERATOR "ZIP" CACHE STRING "Package generator")
        
        if (MSVC)
            set(PACKAGE_VCREDIST_PATH "" CACHE FILEPATH "Microsoft Visual C++ Redistributable package installer path")
        endif()

        set(CPACK_GENERATOR ${PACKAGE_GENERATOR}) # Package informations
        if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
            set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc++1-10")
        elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
            set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6,libstdc++6")
        endif()
        set(CPACK_RPM_COMPONENT_INSTALL ON)
        set(CPACK_COMPONENTS_IGNORE_GROUPS 1)
        set(CPACK_COMPONENTS_ALL_IN_ONE_PACKAGE 1)
        set(CPACK_PACKAGE_NAME "${PROJECT_TARGET}")
        set(CPACK_PACKAGE_VENDOR "kms1212")
        set(CPACK_PACKAGE_MAINTAINER "kms1212")
        set(CPACK_PACKAGE_CONTACT "kms1212g@gmail.com")
        set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "OpenFSL - An open source filesystem library")
        set(CPACK_PACKAGE_VERSION ${OPENFSL_VERSION})
        set(CPACK_PACKAGE_INSTALL_DIRECTORY "OpenFSL")
        if (WIN32)
            if(MSVC)
                string(REPLACE "\\" "/" PACKAGE_VCREDIST_PATH "${PACKAGE_VCREDIST_PATH}")
                    
                if (NOT PACKAGE_VCREDIST_PATH STREQUAL "")
                    install(FILES "${PACKAGE_VCREDIST_PATH}"
                            DESTINATION tmp
                            COMPONENT VCREDIST
                            RENAME "vcredist.exe")
                    set(CPACK_COMPONENT_VCREDIST_REQUIRED true)
                    set(CPACK_COMPONENT_VCREDIST_DISABLED false)
                    set(CPACK_COMPONENT_VCREDIST_DISPLAY_NAME "MSVC Runtime")
                    set(CPACK_COMPONENT_VCREDIST_DESCRIPTION "This build of OpenFSL contains MSVC redistributable package. This will be automatically installed in install time.")
                    get_filename_component(VCREDIST_FILENAME "${PACKAGE_VCREDIST_PATH}" NAME)
                    list(APPEND CPACK_NSIS_EXTRA_PREINSTALL_COMMANDS "
                        ExecWait '\\\"$INSTDIR\\\\tmp\\\\vcredist.exe\\\" /install /quiet /norestart'
                        RMDir /r '$INSTDIR\\\\tmp'
                        ")
                endif()
            endif()
        endif()
        if (${PACKAGE_TYPE} STREQUAL "NIGHTLY")
            set(CPACK_PACKAGE_FILE_NAME
                "${PROJECT_TARGET}-${CMAKE_SYSTEM_NAME}.${TARGET_ARCHITECTURE}-${CMAKE_CXX_COMPILER_ID}-${LIBRARY_TYPE}")
        else()
            set(CPACK_PACKAGE_FILE_NAME
                "${PROJECT_TARGET}-${CPACK_PACKAGE_VERSION}-${CMAKE_SYSTEM_NAME}.${TARGET_ARCHITECTURE}-${CMAKE_CXX_COMPILER_ID}-${LIBRARY_TYPE}")
        endif()

        set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
        set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README.md")
        
        install(DIRECTORY "${CMAKE_BINARY_DIR}/output/${PROJECT_TARGET}" # Add files to archive
                DESTINATION lib
                COMPONENT OFSL_BINARY)
        install(DIRECTORY "${CMAKE_SOURCE_DIR}/include/openfsl"
                DESTINATION include
                COMPONENT OFSL_DEVEL)
                
        if(DOXYGEN_GENERATE)
            install(DIRECTORY "${CMAKE_SOURCE_DIR}/docs"
                    DESTINATION "share/doc/${PROJECT_TARGET}"
                    COMPONENT OFSL_DOC)
        endif(DOXYGEN_GENERATE)
        install(FILES "${CMAKE_SOURCE_DIR}/LICENSE" "${CMAKE_SOURCE_DIR}/README.md"
                DESTINATION "share/doc/${PROJECT_TARGET}"
                COMPONENT OFSL_DOC)
                
        set(CPACK_COMPONENT_GROUP_GROUP_LIBRARIES_DISPLAY_NAME "Libraries")
        set(CPACK_COMPONENT_GROUP_GROUP_LIBRARIES_DESCRIPTION "Binary files of OpenFSL base library and modules")
        set(CPACK_COMPONENT_GROUP_GROUP_LIBRARIES_BOLD_TITLE true)
        set(CPACK_COMPONENT_GROUP_GROUP_LIBRARIES_EXPANDED true)
        
        set(CPACK_COMPONENT_GROUP_GROUP_DEVEL_DISPLAY_NAME "Devel")
        set(CPACK_COMPONENT_GROUP_GROUP_DEVEL_DESCRIPTION "Additional files for developing with OpenFSL")
                
        set(CPACK_COMPONENT_OFSL_BINARY_GROUP GROUP_LIBRARIES) 
        set(CPACK_COMPONENT_OFSL_BINARY_DISPLAY_NAME "Base library") # Component informations
        set(CPACK_COMPONENT_OFSL_BINARY_DESCRIPTION "OpenFSL base library binaries")
        set(CPACK_COMPONENT_OFSL_BINARY_REQUIRED true)
        set(CPACK_COMPONENT_OFSL_BINARY_INSTALL_TYPES Binary Full)
        set(CPACK_COMPONENT_OFSL_BINARY_DISABLED false)
        
        set(CPACK_COMPONENT_OFSL_DEVEL_GROUP GROUP_DEVEL) 
        set(CPACK_COMPONENT_OFSL_DEVEL_DISPLAY_NAME "Base library")
        set(CPACK_COMPONENT_OFSL_DEVEL_DESCRIPTION "OpenFSL base library development kit")
        set(CPACK_COMPONENT_OFSL_DEVEL_DEPENDS OFSL_BINARY)
        set(CPACK_COMPONENT_OFSL_DEVEL_INSTALL_TYPES Full)
        set(CPACK_COMPONENT_OFSL_DEVEL_DISABLED true)
        
        set(CPACK_COMPONENT_OFSL_DOC_DISPLAY_NAME "Documents")
        set(CPACK_COMPONENT_OFSL_DOC_DESCRIPTION "OpenFSL documents.")
        set(CPACK_COMPONENT_OFSL_DOC_INSTALL_TYPES Full)
        set(CPACK_COMPONENT_OFSL_DOC_DISABLED true)
        
        list(APPEND CPACK_COMPONENTS_ALL OFSL_BINARY OFSL_DEVEL OFSL_DOC VCREDIST)

        include(fs/PackageConfig.cmake)
        
        include(CPack)
    endif()
endif (NOT NO_BUILD)
