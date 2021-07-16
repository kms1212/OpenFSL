cmake_minimum_required(VERSION 3.4)
project("ofsl_fat32")

add_executable(${PROJECT_NAME} main.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/fs/fat32/include)
target_link_libraries(${PROJECT_NAME} openfsl_fat32)

if(MSVC)
	set_target_properties(${PROJECT_NAME} PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
    add_test(NAME ${PROJECT_NAME}
         COMMAND ${CMAKE_BINARY_DIR}/${PROJECT_NAME}
         WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")
else()
    add_test(NAME ${PROJECT_NAME}
         COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}
         WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")
endif()