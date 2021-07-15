cmake_minimum_required(VERSION 3.4)
project("OpenFSL")

include(${CMAKE_SOURCE_DIR}/cmakeModules/FindZIP.cmake)

set(BUILD_TESTING true CACHE BOOL "Build test file?")
set(DOXYGEN_GENERATE false CACHE BOOL "Generate doxygen file?")
set(NO_BUILD false CACHE BOOL "Skip build?")
set(PACKAGE_GENERATE true CACHE BOOL "Generate package?")
set(PACKAGE_TYPE "NIGHTLY" CACHE STRING "Package type (NIGHTLY, BETA, RELEASE)")
set(PACKAGE_GENERATOR "ZIP" CACHE STRING "Package generator")

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
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        if (CMAKE_BUILD_TYPE STREQUAL "Debug")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")
        endif()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
        if (CMAKE_BUILD_TYPE STREQUAL "Debug")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")
        endif()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc /Wall /wd5045 /wd4668")
        set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS true)
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    endif()
    
    string(TOLOWER ${CMAKE_PROJECT_NAME} PROJECT_TARGET)
    
    file(GLOB LIBRARY_SOURCES ${CMAKE_SOURCE_DIR}/src/*/*)
    add_library(${PROJECT_TARGET} SHARED ${LIBRARY_SOURCES})
    target_include_directories(${PROJECT_TARGET} PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/>
        $<INSTALL_INTERFACE:include/>)
        
    
    #########################
    # TEST
    #########################
    if (BUILD_TESTING)
        if(NOT EXISTS ${CMAKE_BINARY_DIR}/fat32.img)
            file(DOWNLOAD 
                https://github.com/kms1212/OpenFSL-TestFiles/raw/main/fat32.zip
                ${CMAKE_BINARY_DIR}/fat32.zip)
            ZIP_UNPACK_TARGET(zipfile 
                ${CMAKE_BINARY_DIR}/ 
                ${CMAKE_BINARY_DIR}/fat32.zip)
#           file(ARCHIVE_EXTRACT 
#               INPUT ${CMAKE_BINARY_DIR}/fat32.zip
#               DESTINATION ${CMAKE_BINARY_DIR}/)
        endif()
        
        enable_testing()
        include (CTest)
    
        add_subdirectory(tests)
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
            set(CPACK_PACKAGE_NAME "${CMAKE_PROJECT_NAME}")
            set(CPACK_PACKAGE_VENDOR "kms1212")
            set(CPACK_PACKAGE_MAINTAINER "kms1212")
            set(CPACK_PACKAGE_CONTACT "kms1212g@gmail.com")
            set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "OpenFSL - An open source filesystem library")
            set(CPACK_PACKAGE_VERSION ${PACKAGE_VERSION})
            set(CPACK_PACKAGE_INSTALL_DIRECTORY "OpenFSL")
			if (MSVC)
			    set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CMAKE_SYSTEM_NAME}.${CMAKE_SYSTEM_PROCESSOR}")
			else()
			    set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CMAKE_SYSTEM_NAME}.${CMAKE_GENERATOR_PLATFORM}")
			endif()
            set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
            set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README.md")
            
            install(TARGETS ${PROJECT_TARGET} # Add files to archive
                    DESTINATION lib
                    COMPONENT COMP_OFSL_BINARY)
            install(DIRECTORY "${CMAKE_SOURCE_DIR}/include/${PROJECT_TARGET}"
                    DESTINATION include
                    COMPONENT COMP_OFSL_DEVEL)
                    
            if (DOXYGEN_GENERATE)
                install(DIRECTORY "${CMAKE_SOURCE_DIR}/docs"
                        DESTINATION "share/doc/${PROJECT_TARGET}"
                        COMPONENT COMP_OFSL_DOC)
            endif (DOXYGEN_GENERATE)
            install(FILES "${CMAKE_SOURCE_DIR}/LICENSE" "${CMAKE_SOURCE_DIR}/README.md"
                    DESTINATION "share/doc/${PROJECT_TARGET}"
                    COMPONENT COMP_OFSL_DOC)
                    
            set(CPACK_COMPONENT_COMP_OFSL_BINARY_GROUP GROUP_LIBRARIES) # Component informations
            set(CPACK_COMPONENT_COMP_OFSL_BINARY_DISPLAY_NAME "OpenFSL")
            set(CPACK_COMPONENT_COMP_OFSL_BINARY_DESCRIPTION "OpenFSL library binaries.")
            set(CPACK_COMPONENT_COMP_OFSL_BINARY_REQUIRED true)
            
            set(CPACK_COMPONENT_COMP_OFSL_DEVEL_GROUP GROUP_DEVEL)
            set(CPACK_COMPONENT_COMP_OFSL_DEVEL_DISPLAY_NAME "OpenFSL")
            set(CPACK_COMPONENT_COMP_OFSL_DEVEL_DESCRIPTION "Files to develop with OpenFSL.")
            set(CPACK_COMPONENT_COMP_OFSL_DEVEL_DEPENDS "COMP_OFSL_BINARY")
            
            set(CPACK_COMPONENT_COMP_OFSL_DOC_GROUP GROUP_DOCUMENTS)
            set(CPACK_COMPONENT_COMP_OFSL_DOC_DISPLAY_NAME "OpenFSL")
            set(CPACK_COMPONENT_COMP_OFSL_DOC_DESCRIPTION "OpenFSL documents.")
            
            set(CPACK_COMPONENT_GROUP_GROUP_LIBRARIES_DESCRIPTION "List of libraries provided by this installer.")
            set(CPACK_COMPONENT_GROUP_GROUP_LIBRARIES_EXPANDED true)
            set(CPACK_COMPONENT_GROUP_GROUP_LIBRARIES_BOLD_TITLE true)
            
            set(CPACK_COMPONENT_GROUP_GROUP_DEVEL_DESCRIPTION "Header files for development.")
            
            set(CPACK_COMPONENT_GROUP_GROUP_DOCUMENTS_DESCRIPTION "List of documents about package.")
            
            #########################
            # PACKAGE STRUCTURE
            #########################
            # GROUP_LIBRARIES
            #   COMP_OFSL_BINARY
            # GROUP_DEVEL
            #   COMP_OFSL_DEVEL
            # GROUP_DOCUMENTS
            #   COMP_OFSL_DOC
            
            set(CPACK_COMPONENTS_ALL COMP_OFSL_BINARY COMP_OFSL_BINARY COMP_OFSL_DOC)
            
            include(CPack)
        elseif (PACKAGE_TYPE STREQUAL "BETA")
        elseif (PACKAGE_TYPE STREQUAL "RELEASE")
        endif()
    endif()
endif (NOT NO_BUILD)