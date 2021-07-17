cmake_minimum_required(VERSION 3.4)
project("OpenFSL")

include(ExternalProject)

set(NO_BUILD false CACHE BOOL "Skip build?")
set(BUILD_TESTING true CACHE BOOL "Build test file?")
set(BUILD_FAT32 true CACHE BOOL "Build fat32 filesystem?")
set(DOXYGEN_GENERATE false CACHE BOOL "Generate doxygen file?")
set(PACKAGE_TYPE "NIGHTLY" CACHE STRING "Package type (NIGHTLY, BETA, RELEASE)")
set(PACKAGE_GENERATE false CACHE BOOL "Generate package?")
set(PACKAGE_GENERATOR "ZIP" CACHE STRING "Package generator")
set(TARGET_ARCHITECTURE "AMD64" CACHE STRING "Target system architecture(AMD64, x86)")

if (DOXYGEN_GENERATE)
    find_package(Doxygen)
    if (DOXYGEN_FOUND)
        find_package(Doxygen REQUIRED dot)
    else (DOXYGEN_FOUND)
        message(WARNING "Doxygen is not found. Creating documents will be disabled.")
        set (DOXYGEN_GENERATE false)
    endif(DOXYGEN_FOUND)

    if (DOXYGEN_FOUND)
        add_custom_target(doc_doxygen ALL
            COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_SOURCE_DIR}/Doxyfile
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMENT "Generating library documentation with Doxygen"
            VERBATIM )
    endif (DOXYGEN_FOUND)
endif (DOXYGEN_GENERATE)
    
if (NOT NO_BUILD)
    #########################
    # BUILD
    #########################
    set(CMAKE_CXX_STANDARD 14)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        if (CMAKE_BUILD_TYPE STREQUAL "Debug")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")
        endif()
        if (TARGET_ARCHITECTURE STREQUAL "AMD64")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64")
        endif()
        if (TARGET_ARCHITECTURE STREQUAL "x86")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")
        endif()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        if (CMAKE_BUILD_TYPE STREQUAL "Debug")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")
        endif()
        if (TARGET_ARCHITECTURE STREQUAL "AMD64")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64")
        endif()
        if (TARGET_ARCHITECTURE STREQUAL "x86")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")
        endif()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
        if (CMAKE_BUILD_TYPE STREQUAL "Debug")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")
        endif()
        if (TARGET_ARCHITECTURE STREQUAL "AMD64")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64")
        endif()
        if (TARGET_ARCHITECTURE STREQUAL "x86")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")
        endif()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc /W3 /wd5045 /wd4668 /wd4514")
        set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS true)
    endif()
    
    string(TOLOWER ${CMAKE_PROJECT_NAME} PROJECT_TARGET)
    
    file(GLOB LIBRARY_SOURCES ${CMAKE_SOURCE_DIR}/src/*/*)
    file(GLOB LIBRARY_SOURCES ${CMAKE_SOURCE_DIR}/src/*)
    add_library(${PROJECT_TARGET} SHARED ${LIBRARY_SOURCES})
    target_include_directories(${PROJECT_TARGET} PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/>)
    set_target_properties(${PROJECT_TARGET} PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
        
    if (WIN32)
        set(PROJECT_OUTPUT_FILES "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${PROJECT_TARGET}.dll" 
             "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${PROJECT_TARGET}.exp" 
             "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${PROJECT_TARGET}.lib")
    elseif(UNIX)
        set(PROJECT_OUTPUT_FILES "${CMAKE_BINARY_DIR}/lib${PROJECT_TARGET}.so")
    elseif(APPLE)
        set(PROJECT_OUTPUT_FILES "${CMAKE_BINARY_DIR}/lib${PROJECT_TARGET}.dylib")
    endif()
    
    add_custom_command(POST_BUILD VERBATIM
        TARGET ${PROJECT_TARGET}
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/output/${PROJECT_TARGET}"
        COMMENT "Copying output files to temp directory...")
    
    foreach(FILENAME ${PROJECT_OUTPUT_FILES})
        add_custom_command(POST_BUILD VERBATIM
            TARGET ${PROJECT_TARGET}
            COMMAND ${CMAKE_COMMAND} -E copy "${FILENAME}" "${CMAKE_BINARY_DIR}/output/${PROJECT_TARGET}"
            COMMENT "${FILENAME} ")
    endforeach()
        
    ## Submodule configurations ##############################################
    if (BUILD_FAT32)
        add_subdirectory(fs/fat32)
    endif()
    
    #########################
    # TEST
    #########################
    if (BUILD_TESTING)
        enable_testing()
        include (CTest)
    
        add_subdirectory(tests)
        
        ## Submodule configurations ##############################################
        if (BUILD_FAT32)
            add_subdirectory(fs/fat32/tests)
        endif()
    endif (BUILD_TESTING)
    
    #########################
    # PACK
    #########################
    if(PACKAGE_GENERATE)
        STRING(TIMESTAMP TODAY "%Y%m%d")
        
        if (PACKAGE_TYPE STREQUAL "NIGHTLY")
            set(PACKAGE_VERSION "${TODAY}nr")
            
            if(PACKAGE_GENERATOR STREQUAL "DEB")
                set(CPACK_DEBIAN_PACKAGE_DEPENDS "")
            endif()
            
            set(CPACK_GENERATOR ${PACKAGE_GENERATOR}) # Package informations
            set(CPACK_RPM_COMPONENT_INSTALL ON)
            set(CPACK_COMPONENTS_IGNORE_GROUPS 1)
            set(CPACK_COMPONENTS_ALL_IN_ONE_PACKAGE 1)
            set(CPACK_PACKAGE_NAME "${PROJECT_TARGET}")
            set(CPACK_PACKAGE_VENDOR "kms1212")
            set(CPACK_PACKAGE_MAINTAINER "kms1212")
            set(CPACK_PACKAGE_CONTACT "kms1212g@gmail.com")
            set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "OpenFSL - An open source filesystem library")
            set(CPACK_PACKAGE_VERSION ${PACKAGE_VERSION})
            set(CPACK_PACKAGE_INSTALL_DIRECTORY "OpenFSL")
            if (MSVC)
                set(CPACK_PACKAGE_FILE_NAME "${PROJECT_TARGET}-${CPACK_PACKAGE_VERSION}-${CMAKE_SYSTEM_NAME}.${CMAKE_GENERATOR_PLATFORM}")
            else()
                set(CPACK_PACKAGE_FILE_NAME "${PROJECT_TARGET}-${CPACK_PACKAGE_VERSION}-${CMAKE_SYSTEM_NAME}.${TARGET_ARCHITECTURE}")
            endif()
            set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
            set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README.md")
            
            install(DIRECTORY "${CMAKE_BINARY_DIR}/output/${PROJECT_TARGET}" # Add files to archive
                    DESTINATION lib
                    COMPONENT OFSL_BINARY)
            install(DIRECTORY "${CMAKE_SOURCE_DIR}/include/${PROJECT_TARGET}"
                    DESTINATION include
                    COMPONENT OFSL_DEVEL)
                    
            if (DOXYGEN_GENERATE)
                install(DIRECTORY "${CMAKE_SOURCE_DIR}/docs"
                        DESTINATION "share/doc/${PROJECT_TARGET}"
                        COMPONENT OFSL_DOC)
            endif (DOXYGEN_GENERATE)
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
            set(CPACK_COMPONENT_OFSL_BINARY_DESCRIPTION "OpenFSL base library binarie")
            set(CPACK_COMPONENT_OFSL_BINARY_REQUIRED true)
            set(CPACK_COMPONENT_OFSL_BINARY_INSTALL_TYPES Binary Full)
            
            set(CPACK_COMPONENT_OFSL_DEVEL_GROUP GROUP_DEVEL) 
            set(CPACK_COMPONENT_OFSL_DEVEL_DISPLAY_NAME "Base library")
            set(CPACK_COMPONENT_OFSL_DEVEL_DESCRIPTION "OpenFSL base library development kit")
            set(CPACK_COMPONENT_OFSL_DEVEL_DEPENDS OFSL_BINARY)
            set(CPACK_COMPONENT_OFSL_DEVEL_INSTALL_TYPES Full)
            
            set(CPACK_COMPONENT_OFSL_DOC_DISPLAY_NAME "Documents")
            set(CPACK_COMPONENT_OFSL_DOC_DESCRIPTION "OpenFSL documents.")
            set(CPACK_COMPONENT_OFSL_DOC_INSTALL_TYPES Full)
            
            list(APPEND CPACK_COMPONENTS_ALL OFSL_BINARY OFSL_DEVEL OFSL_DOC)
            
            ## Submodule configurations ##############################################
            if(BUILD_FAT32)
                install(DIRECTORY "${CMAKE_BINARY_DIR}/output/openfsl_fat32/"
                        DESTINATION lib/openfsl
                        COMPONENT OFSL_FAT32_BINARY)
                install(DIRECTORY "${CMAKE_SOURCE_DIR}/fs/fat32/include/${PROJECT_TARGET}"
                        DESTINATION include
                        COMPONENT OFSL_FAT32_DEVEL)
                        
                set(CPACK_COMPONENT_OFSL_FAT32_BINARY_GROUP GROUP_LIBRARIES) 
                set(CPACK_COMPONENT_OFSL_FAT32_BINARY_DISPLAY_NAME "FAT32 module") # Component informations
                set(CPACK_COMPONENT_OFSL_FAT32_BINARY_DESCRIPTION "OpenFSL fat32 module binaries")
                set(CPACK_COMPONENT_OFSL_FAT32_DEVEL_DEPENDS OFSL_BINARY)
                set(CPACK_COMPONENT_OFSL_FAT32_BINARY_INSTALL_TYPES Binary Full)
                
                set(CPACK_COMPONENT_OFSL_FAT32_DEVEL_GROUP GROUP_DEVEL) 
                set(CPACK_COMPONENT_OFSL_FAT32_DEVEL_DISPLAY_NAME "FAT32 module")
                set(CPACK_COMPONENT_OFSL_FAT32_DEVEL_DESCRIPTION "OpenFSL fat32 development kit")
                set(CPACK_COMPONENT_OFSL_FAT32_DEVEL_DEPENDS OFSL_FAT32_BINARY)
                set(CPACK_COMPONENT_OFSL_FAT32_DEVEL_INSTALL_TYPES Full)
                
                list(APPEND CPACK_COMPONENTS_ALL OFSL_FAT32_BINARY OFSL_FAT32_DEVEL)
            endif()
            
            include(CPack)
        elseif (PACKAGE_TYPE STREQUAL "BETA")
        elseif (PACKAGE_TYPE STREQUAL "RELEASE")
        endif()
    endif()
endif (NOT NO_BUILD)