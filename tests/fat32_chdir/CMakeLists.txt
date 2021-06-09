cmake_minimum_required(VERSION 3.1)
project("fat32_chdir")

include_directories(${CMAKE_SOURCE_DIR}/include)
add_executable(fat32_chdir fat32_chdir.cpp)

target_link_libraries(fat32_chdir openfsl)
add_test(NAME FAT32_Chdir
    COMMAND "${CMAKE_CURRENT_BINARY_DIR}/fat32_chdir" --config $<CONFIG>
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")