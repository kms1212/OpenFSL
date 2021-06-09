cmake_minimum_required(VERSION 3.1)
project("OpenFSL")

option(BUILD_TESTING "Build test file?" true)
option(DOXYGEN_GENERATE "Generate doxygen file?" true)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

if (MSVC)
    add_compile_options(/std:c++14 /permissive-)
else (MSVC)
endif (MSVC)

file(GLOB libSources "${CMAKE_SOURCE_DIR}/src/fs_*/*.cpp" "${CMAKE_SOURCE_DIR}/src/library/*.cpp" "${CMAKE_SOURCE_DIR}/src/table/*.cpp")
add_library(openfsl STATIC ${libSources})

file(GLOB INCLUDE_FILES ${CMAKE_SOURCE_DIR}/src/header/*.h)
file(COPY ${INCLUDE_FILES} DESTINATION ${CMAKE_SOURCE_DIR}/include/openfsl/)
file(GLOB INCLUDE_FILES_FS ${CMAKE_SOURCE_DIR}/src/header/fs_*/*.h)
file(COPY ${INCLUDE_FILES_FS} DESTINATION ${CMAKE_SOURCE_DIR}/include/openfsl/fs/)

find_package(Doxygen)
if (DOXYGEN_FOUND)
    find_package(Doxygen REQUIRED dot)
else (DOXYGEN_FOUND)
    message(WARNING "Doxygen is not found. Creating documents will be disabled.")
    set(DOXYGEN_GENERATE false)
endif (DOXYGEN_FOUND)

if (DOXYGEN_FOUND AND DOXYGEN_GENERATE)
    add_custom_target( doc_doxygen ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_SOURCE_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM )
endif (DOXYGEN_FOUND AND DOXYGEN_GENERATE)

if (BUILD_TESTING)
    if(NOT EXISTS "${CMAKE_BINARY_DIR}/fat32.img")
        file(DOWNLOAD 
            ftp://kms1212.kro.kr/fsimage/fat32.img
            ${CMAKE_BINARY_DIR}/fat32.img
        )
    endif()
    
    enable_testing()
    include (CTest)

    add_subdirectory(tests)
endif (BUILD_TESTING)