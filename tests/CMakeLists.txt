cmake_minimum_required(VERSION 3.4)

include(ExternalProject)
include(${CMAKE_SOURCE_DIR}/cmakeModules/FindZIP.cmake)

if(NOT EXISTS ${CMAKE_BINARY_DIR}/base_test.img)
	file(DOWNLOAD 
		https://github.com/kms1212/OpenFSL-TestFiles/raw/main/base_test.zip
		${CMAKE_BINARY_DIR}/base_test.zip)
	ZIP_UNPACK_TARGET(zipfile_test_base 
		${CMAKE_BINARY_DIR}/ 
		${CMAKE_BINARY_DIR}/base_test.zip)
	#file(ARCHIVE_EXTRACT 
	#    INPUT ${CMAKE_BINARY_DIR}/base_test.zip
	#    DESTINATION ${CMAKE_BINARY_DIR}/)
endif()

add_subdirectory(ofsl_base)
