cmake_minimum_required(VERSION 3.12)

if(NOT EXISTS ${CMAKE_BINARY_DIR}/fat32.zip)
	file(DOWNLOAD 
		https://github.com/kms1212/OpenFSL-TestFiles/raw/main/fat32.zip
		${CMAKE_BINARY_DIR}/fat32.zip)
endif()
ZIP_UNPACK_TARGET(zipfile_test_fat32 
	${CMAKE_BINARY_DIR}/ 
	${CMAKE_BINARY_DIR}/fat32.zip)

if(NOT EXISTS ${CMAKE_BINARY_DIR}/2kfat32.zip)
	file(DOWNLOAD 
		https://github.com/kms1212/OpenFSL-TestFiles/raw/main/2kfat32.zip
		${CMAKE_BINARY_DIR}/2kfat32.zip)
endif()
ZIP_UNPACK_TARGET(zipfile_test_2kfat32 
	${CMAKE_BINARY_DIR}/ 
	${CMAKE_BINARY_DIR}/2kfat32.zip)

if(NOT EXISTS ${CMAKE_BINARY_DIR}/mbr.zip)
	file(DOWNLOAD 
		https://github.com/kms1212/OpenFSL-TestFiles/raw/main/mbr.zip
		${CMAKE_BINARY_DIR}/mbr.zip)
endif()
ZIP_UNPACK_TARGET(zipfile_test_mbr
	${CMAKE_BINARY_DIR}/ 
	${CMAKE_BINARY_DIR}/mbr.zip)

listsubdir(SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR})
foreach(SUBDIR ${SUBDIRS})
	add_subdirectory(${SUBDIR})
endforeach()