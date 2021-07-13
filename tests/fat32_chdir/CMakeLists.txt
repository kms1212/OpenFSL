cmake_minimum_required(VERSION 3.1)
project("fat32_chdir")

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(fat32_chdir main.cpp)
target_include_directories(fat32_chdir PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(fat32_chdir openfsl)

if(MSVC)
    add_test(NAME FAT32_ChangeDirectory
         COMMAND ${CMAKE_BINARY_DIR}/fat32_chdir
         WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")
else()
    add_test(NAME FAT32_ChangeDirectory
         COMMAND ${CMAKE_CURRENT_BINARY_DIR}/fat32_chdir
         WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")
endif()