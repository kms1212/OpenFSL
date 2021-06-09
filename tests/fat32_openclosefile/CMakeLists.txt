cmake_minimum_required(VERSION 3.1)
project("fat32_openclosefile")

include_directories(${CMAKE_SOURCE_DIR}/include)
add_executable(fat32_openclosefile fat32_openclosefile.cpp)

TARGET_LINK_LIBRARIES(fat32_openclosefile ${CMAKE_BINARY_DIR}/libopenfsl.a)