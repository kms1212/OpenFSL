cmake_minimum_required(VERSION 3.1)
project("fat32_getfileinformation")

include_directories(${CMAKE_SOURCE_DIR}/include)
add_executable(fat32_getfileinformation fat32_getfileinformation.cpp)

target_link_libraries(fat32_getfileinformation openfsl)
add_test(NAME FAT32_GetFileInformation 
    COMMAND "${CMAKE_CURRENT_BINARY_DIR}/fat32_getfileinformation" 
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")