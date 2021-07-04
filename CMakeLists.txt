cmake_minimum_required(VERSION 3.1)
project("OpenFSL")

option(BUILD_TESTING "Build test file?" true)
option(DOXYGEN_GENERATE "Generate doxygen file?" false)
option(NO_BUILD "Skip build?" false)

find_package(Doxygen)
if (DOXYGEN_FOUND)
    find_package(Doxygen REQUIRED dot)
else (DOXYGEN_FOUND)
    message(WARNING "Doxygen is not found. Creating documents will be disabled.")
    set (DOXYGEN_GENERATE false)
endif(DOXYGEN_FOUND)
    
if (DOXYGEN_FOUND AND DOXYGEN_GENERATE)
    add_custom_target( doc_doxygen ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_SOURCE_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Generating library documentation with Doxygen"
        VERBATIM )
endif (DOXYGEN_FOUND AND DOXYGEN_GENERATE)
    
if (NOT NO_BUILD)
    
    set(CMAKE_CXX_STANDARD 14)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
    
    
    file(GLOB libSources "${CMAKE_SOURCE_DIR}/src/*/*.cpp")
    add_library(openfsl STATIC ${libSources})
    target_include_directories(openfsl PRIVATE "${CMAKE_SOURCE_DIR}/include/")

    if (BUILD_TESTING)
        if(NOT EXISTS "${CMAKE_BINARY_DIR}/fat32.img")
            file(DOWNLOAD 
                ftp://kms1212.kro.kr/fsimage/fat32.img
                ${CMAKE_BINARY_DIR}/fat32.img)
        
        endif()
        
        enable_testing()
        include (CTest)
    
        add_subdirectory(tests)
    endif (BUILD_TESTING)
endif (NOT NO_BUILD)