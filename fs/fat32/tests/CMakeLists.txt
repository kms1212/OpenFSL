cmake_minimum_required(VERSION 3.4)

include(${CMAKE_SOURCE_DIR}/cmakeModules/FindZIP.cmake)

if(NOT EXISTS ${CMAKE_BINARY_DIR}/fat32.img)
	file(DOWNLOAD 
		https://github.com/kms1212/OpenFSL-TestFiles/raw/main/fat32.zip
		${CMAKE_BINARY_DIR}/fat32.zip)
	ZIP_UNPACK_TARGET(zipfile_test_fat32 
		${CMAKE_BINARY_DIR}/ 
		${CMAKE_BINARY_DIR}/fat32.zip)
	#file(ARCHIVE_EXTRACT 
	#    INPUT ${CMAKE_BINARY_DIR}/fat32.zip
	#    DESTINATION ${CMAKE_BINARY_DIR}/)
endif()

add_subdirectory(ofsl_fat32)