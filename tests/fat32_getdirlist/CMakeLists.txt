cmake_minimum_required(VERSION 3.1)
project("fat32_getdirlist")

include_directories(${CMAKE_SOURCE_DIR}/include)
add_executable(fat32_getdirlist fat32_getdirlist.cpp)

TARGET_LINK_LIBRARIES(fat32_getdirlist ${CMAKE_BINARY_DIR}/libopenfsl.a)