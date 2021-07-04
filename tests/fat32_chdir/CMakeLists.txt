cmake_minimum_required(VERSION 3.1)
project("fat32_chdir")

add_executable(fat32_chdir main.cpp)
target_include_directories(fat32_chdir PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(fat32_chdir openfsl)

add_test(NAME FAT32_Chdir
    COMMAND "${CMAKE_CURRENT_BINARY_DIR}/fat32_chdir" --config $<CONFIG>
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")