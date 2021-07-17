cmake_minimum_required(VERSION 3.4)
project("OpenFSL_FAT32")
    
if(NOT NO_BUILD)
    set(CMAKE_CXX_STANDARD 14)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        if(CMAKE_BUILD_TYPE STREQUAL "Debug")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")
        endif()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        if(CMAKE_BUILD_TYPE STREQUAL "Debug")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")
        endif()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
        if(CMAKE_BUILD_TYPE STREQUAL "Debug")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")
        endif()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc /W3 /wd5045 /wd4668 /wd4514")
        set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS true)
    endif()
    
    string(TOLOWER ${PROJECT_NAME} PROJECT_TARGET)
    
    file(GLOB LIBRARY_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*)
    add_library(${PROJECT_TARGET} SHARED ${LIBRARY_SOURCES})
    target_link_libraries(${PROJECT_TARGET} openfsl)
    target_include_directories(${PROJECT_TARGET} PRIVATE ${CMAKE_SOURCE_DIR}/include/openfsl)
    target_include_directories(${PROJECT_TARGET} PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>)
    set_target_properties(${PROJECT_TARGET} PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
        
    if(WIN32)
        set(PROJECT_OUTPUT_FILES "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${PROJECT_TARGET}.dll" 
             "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${PROJECT_TARGET}.exp" 
             "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${PROJECT_TARGET}.lib")
    elseif(APPLE)
        set(PROJECT_OUTPUT_FILES "${CMAKE_BINARY_DIR}/lib${PROJECT_TARGET}.dylib")
    elseif(UNIX)
        set(PROJECT_OUTPUT_FILES "${CMAKE_BINARY_DIR}/lib${PROJECT_TARGET}.so")
    endif()
    
    add_custom_command(POST_BUILD VERBATIM
        TARGET ${PROJECT_TARGET}
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/output/${PROJECT_TARGET}"
        COMMENT "Copying output files to temp directory...")
    
    foreach(FILENAME ${PROJECT_OUTPUT_FILES})
        add_custom_command(POST_BUILD VERBATIM
            TARGET ${PROJECT_TARGET}
            COMMAND ${CMAKE_COMMAND} -E copy "${FILENAME}" "${CMAKE_BINARY_DIR}/output/${PROJECT_TARGET}"
            COMMENT "${FILENAME} ")
    endforeach()
endif(NOT NO_BUILD)