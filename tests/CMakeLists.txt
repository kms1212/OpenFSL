cmake_minimum_required(VERSION 3.1)
project("test")

include(ExternalProject)

if(NOT EXISTS "${CMAKE_BINARY_DIR}/fat32.img")
	file(DOWNLOAD 
		ftp://kms1212.kro.kr/fsimage/fat32.img
		${CMAKE_BINARY_DIR}/fat32.img
		SHOW_PROGRESS
	)
endif()

add_subdirectory(fat32_init)
add_subdirectory(fat32_chdir)
add_subdirectory(fat32_getdirlist)
add_subdirectory(fat32_getfileinformation)