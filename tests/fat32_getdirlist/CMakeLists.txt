cmake_minimum_required(VERSION 3.1)
project("fat32_getdirlist")

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(fat32_getdirlist main.cpp)
target_include_directories(fat32_getdirlist PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(fat32_getdirlist openfsl)

add_test(NAME FAT32_GetDirList
     COMMAND "${CMAKE_CURRENT_BINARY_DIR}/fat32_getdirlist"
     WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")