cmake_minimum_required(VERSION 3.1)
project("fat32_seekfile")

include_directories(${CMAKE_SOURCE_DIR}/include)
add_executable(fat32_seekfile fat32_seekfile.cpp)

TARGET_LINK_LIBRARIES(fat32_seekfile ${CMAKE_BINARY_DIR}/libopenfsl.a)