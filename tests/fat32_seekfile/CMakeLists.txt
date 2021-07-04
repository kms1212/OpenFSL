cmake_minimum_required(VERSION 3.1)
project("fat32_seekfile")

add_executable(fat32_seekfile main.cpp)
target_include_directories(fat32_seekfile PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(fat32_seekfile openfsl)

add_test(NAME FAT32_SeekFile
    COMMAND "${CMAKE_CURRENT_BINARY_DIR}/fat32_seekfile"
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")