cmake_minimum_required(VERSION 3.12)
project("base_test")

add_executable(${PROJECT_NAME} main.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME} openfsl)
add_dependencies(${PROJECT_NAME} openfsl)

add_dependencies(${PROJECT_NAME} zipfile_test_base)

configure_target_options(${PROJECT_NAME} TEST)
configure_build_directory(${PROJECT_NAME} "${CMAKE_CURRENT_BINARY_DIR}")
copy_lib(${PROJECT_NAME}
         "${CMAKE_BINARY_DIR}/output/openfsl/\*"
         "openfsl")
configure_test(${PROJECT_NAME} 
                 "CONTAINER_CONTAINER;CONTAINER_SECTOR;UTIL_STRTOK;UTIL_CHS;BLOCKDEVICE;FILEBLOCKDEVICE;MEMCHARDEVICE;MEMBLOCKDEVICE"
                 "${CMAKE_BINARY_DIR}/output/openfsl/\*" 
                 "${CMAKE_BINARY_DIR}/base_test.img" 
                 "openfsl" 
                 "OpenFSL base test")