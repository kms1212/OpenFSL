cmake_minimum_required(VERSION 3.1)
project("fat32_init")

include_directories(${CMAKE_SOURCE_DIR}/include)
add_executable(fat32_init fat32_init.cpp)

TARGET_LINK_LIBRARIES(fat32_init ${CMAKE_BINARY_DIR}/libopenfsl.a)