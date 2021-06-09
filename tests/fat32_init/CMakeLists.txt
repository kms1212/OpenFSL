cmake_minimum_required(VERSION 3.1)
project("fat32_init")

include_directories(${CMAKE_SOURCE_DIR}/include)
add_executable(fat32_init fat32_init.cpp)

target_link_libraries(fat32_init openfsl)