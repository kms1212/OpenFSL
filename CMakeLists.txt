cmake_minimum_required(VERSION 3.1)
project("OpenFSL")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB libSources "src/fs_*/*.cpp" "src/library/*.cpp" "src/table/*.cpp")
add_library(openfsl STATIC ${libSources})

file(GLOB INCLUDE_FILES "src/header/*.h")
file(COPY ${INCLUDE_FILES} DESTINATION "include/openfsl/")

SET (LIB_PKG
    spdlog
)

find_package(${LIB_PKG} REQUIRED)
target_link_libraries(openfsl ${LIB_PKG})