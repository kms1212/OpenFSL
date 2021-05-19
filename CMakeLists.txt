cmake_minimum_required(VERSION 3.0)
project("OpenFSL")

file(GLOB INCLUDE_FILES "src/header/*.h")
file(COPY ${INCLUDE_FILES} DESTINATION "include/openfsl/")

file(GLOB libSources "src/fs/*.cpp" "src/library/*.cpp" "src/table/*.cpp")
add_library(openfsl STATIC ${libSources})

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -nostdlib")
list(REMOVE_ITEM CMAKE_CXX_IMPLICIT_LINK_LIBRARIES stdc++)