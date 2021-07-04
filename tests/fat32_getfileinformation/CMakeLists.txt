cmake_minimum_required(VERSION 3.1)
project("fat32_getfileinformation")

add_executable(fat32_getfileinformation main.cpp)
target_include_directories(fat32_getfileinformation PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(fat32_getfileinformation openfsl)

add_test(NAME FAT32_GetFileInformation 
    COMMAND "${CMAKE_CURRENT_BINARY_DIR}/fat32_getfileinformation" 
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")