cmake_minimum_required(VERSION 3.1)
project("fat32_openclosefile")

add_executable(fat32_openclosefile main.cpp)
target_include_directories(fat32_openclosefile PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(fat32_openclosefile openfsl)

add_test(NAME FAT32_OpenCloseFile
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/fat32_openclosefile
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")