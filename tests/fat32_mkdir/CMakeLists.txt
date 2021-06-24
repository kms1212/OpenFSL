cmake_minimum_required(VERSION 3.1)
project("fat32_mkdir")

include_directories(${CMAKE_SOURCE_DIR}/include)
add_executable(fat32_mkdir fat32_mkdir.cpp)

target_link_libraries(fat32_mkdir openfsl)
add_test(NAME FAT32_MakeDir
    COMMAND "${CMAKE_CURRENT_BINARY_DIR}/fat32_mkdir"
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")