cmake_minimum_required(VERSION 3.1)
project("fat32_mkdir")

add_executable(fat32_mkdir main.cpp)
target_include_directories(fat32_mkdir PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(fat32_mkdir openfsl)

add_test(NAME FAT32_MakeDir
    COMMAND "${CMAKE_CURRENT_BINARY_DIR}/fat32_mkdir"
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")