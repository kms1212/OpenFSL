cmake_minimum_required(VERSION 3.1)
project("fat32_getfileinformation")

include_directories(${CMAKE_SOURCE_DIR}/include)
add_executable(fat32_getfileinformation fat32_getfileinformation.cpp)

target_link_libraries(fat32_getfileinformation openfsl)