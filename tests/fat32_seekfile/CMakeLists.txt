cmake_minimum_required(VERSION 3.1)
project("fat32_seekfile")

include_directories(${CMAKE_SOURCE_DIR}/include)
add_executable(fat32_seekfile fat32_seekfile.cpp)

target_link_libraries(fat32_seekfile openfsl)
add_test(NAME FAT32_SeekFile
    COMMAND "${CMAKE_CURRENT_BINARY_DIR}/fat32_seekfile"
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")