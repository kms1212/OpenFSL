cmake_minimum_required(VERSION 3.12)
project("fat32_non512_test")

add_executable(${PROJECT_NAME} main.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/fs/fat32/include)

target_link_libraries(${PROJECT_NAME} openfsl)
target_link_libraries(${PROJECT_NAME} openfsl_fat32)
add_dependencies(${PROJECT_NAME} openfsl)
add_dependencies(${PROJECT_NAME} openfsl_fat32)

add_dependencies(${PROJECT_NAME} zipfile_test_2kfat32)

configure_target_options(${PROJECT_NAME} TEST)
configure_build_directory(${PROJECT_NAME} "${CMAKE_CURRENT_BINARY_DIR}")
copy_lib(${PROJECT_NAME}
         "${CMAKE_BINARY_DIR}/output/openfsl/\*<>${CMAKE_BINARY_DIR}/output/openfsl_fat32/\*"
         "openfsl;openfsl_fat32")
configure_test(${PROJECT_NAME} 
                 "ALL" 
                 "${CMAKE_BINARY_DIR}/output/openfsl/\*<>${CMAKE_BINARY_DIR}/output/openfsl_fat32/\*" 
                 "${CMAKE_BINARY_DIR}/2kfat32.img" 
                 "openfsl;openfsl_fat32"
                 "OpenFSL FAT32(Non 512bps) test")