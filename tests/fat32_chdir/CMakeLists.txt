cmake_minimum_required(VERSION 3.1)
project("fat32_chdir")

include_directories(${CMAKE_SOURCE_DIR}/include)
add_executable(fat32_chdir fat32_chdir.cpp)

TARGET_LINK_LIBRARIES(fat32_chdir ${CMAKE_BINARY_DIR}/libopenfsl.a)