cmake_minimum_required(VERSION 3.1)
project("OpenFSL")

option(BUILD_TESTING "Build test file?" true)
option(DOXYGEN_GENERATE "Generate doxygen file?" true)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

file(GLOB libSources "${CMAKE_SOURCE_DIR}/src/fs_*/*.cpp" "${CMAKE_SOURCE_DIR}/src/library/*.cpp" "${CMAKE_SOURCE_DIR}/src/table/*.cpp")
add_library(openfsl STATIC ${libSources})

file(GLOB INCLUDE_FILES ${CMAKE_SOURCE_DIR}/src/header/*.h)
file(COPY ${INCLUDE_FILES} DESTINATION ${CMAKE_SOURCE_DIR}/include/openfsl/)
file(GLOB INCLUDE_FILES_FS ${CMAKE_SOURCE_DIR}/src/header/fs_*/*.h)
file(COPY ${INCLUDE_FILES_FS} DESTINATION ${CMAKE_SOURCE_DIR}/include/openfsl/fs/)

find_package(Doxygen)
if (DOXYGEN_FOUND)
	find_package(Doxygen REQUIRED dot)
else (DOXYGEN_FOUND)
	message(WARNING "Doxygen is not found. Creating documents will be disabled.")
	set(DOXYGEN_GENERATE false)
endif (DOXYGEN_FOUND)

if (DOXYGEN_FOUND AND DOXYGEN_GENERATE)
    add_custom_target( doc_doxygen ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_SOURCE_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM )
endif (DOXYGEN_FOUND AND DOXYGEN_GENERATE)

if (BUILD_TESTING)
    if(NOT EXISTS "${CMAKE_BINARY_DIR}/fat32.img")
        file(DOWNLOAD 
            ftp://kms1212.kro.kr/fsimage/fat32.img
            ${CMAKE_BINARY_DIR}/fat32.img
        )
    endif()

    add_subdirectory(tests)
    
    enable_testing()
    include (CTest)
    
    add_test("FAT32_Initialization" "${CMAKE_BINARY_DIR}/tests/fat32_init/fat32_init" "${CMAKE_BINARY_DIR}")
    add_test("FAT32_Chdir" "${CMAKE_BINARY_DIR}/tests/fat32_chdir/fat32_chdir" "${CMAKE_BINARY_DIR}")
    add_test("FAT32_GetDirList" "${CMAKE_BINARY_DIR}/tests/fat32_getdirlist/fat32_getdirlist" "${CMAKE_BINARY_DIR}")
    add_test("FAT32_GetFileInformation" "${CMAKE_BINARY_DIR}/tests/fat32_getfileinformation/fat32_getfileinformation" "${CMAKE_BINARY_DIR}")
    add_test("FAT32_openCloseFile" "${CMAKE_BINARY_DIR}/tests/fat32_openclosefile/fat32_openclosefile" "${CMAKE_BINARY_DIR}")
    add_test("FAT32_seekFile" "${CMAKE_BINARY_DIR}/tests/fat32_seekfile/fat32_seekfile" "${CMAKE_BINARY_DIR}")
endif (BUILD_TESTING)