cmake_minimum_required(VERSION 3.1)
project("OpenFSL")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB libSources "src/fs_*/*.cpp" "src/library/*.cpp" "src/table/*.cpp")
add_library(openfsl STATIC ${libSources})

file(GLOB INCLUDE_FILES "src/header/*.h")
file(GLOB INCLUDE_FILES_FS "src/header/fs_*/*.h")
file(COPY ${INCLUDE_FILES} DESTINATION "include/openfsl/")
file(COPY ${INCLUDE_FILES_FS} DESTINATION "include/openfsl/fs/")

option(BUILD_DOC "Build documentation" ON)

find_package(Doxygen)
if (DOXYGEN_FOUND)
    add_custom_target( doc_doxygen ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM )
endif (DOXYGEN_FOUND)