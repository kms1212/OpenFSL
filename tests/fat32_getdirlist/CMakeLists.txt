cmake_minimum_required(VERSION 3.1)
project("fat32_getdirlist")

include_directories(${CMAKE_SOURCE_DIR}/include)
add_executable(fat32_getdirlist fat32_getdirlist.cpp)

target_link_libraries(fat32_getdirlist openfsl)