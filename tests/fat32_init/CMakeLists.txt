cmake_minimum_required(VERSION 3.1)
project("fat32_init")

add_executable(fat32_init main.cpp)
target_include_directories(fat32_init PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(fat32_init openfsl)

add_test(NAME FAT32_Initialization
    COMMAND "${CMAKE_CURRENT_BINARY_DIR}/fat32_init"
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")