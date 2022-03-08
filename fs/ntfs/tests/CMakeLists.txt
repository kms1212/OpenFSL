cmake_minimum_required(VERSION 3.12)

if(NOT EXISTS ${CMAKE_BINARY_DIR}/ntfs.zip)
	file(DOWNLOAD 
		https://github.com/kms1212/OpenFSL-TestFiles/raw/main/ntfs.zip
		${CMAKE_BINARY_DIR}/ntfs.zip)
endif()
ZIP_UNPACK_TARGET(zipfile_test_ntfs
	${CMAKE_BINARY_DIR}/ 
	${CMAKE_BINARY_DIR}/ntfs.zip)

listsubdir(SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR})
foreach(SUBDIR ${SUBDIRS})
	add_subdirectory(${SUBDIR})
endforeach()