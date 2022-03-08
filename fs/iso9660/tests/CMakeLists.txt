cmake_minimum_required(VERSION 3.12)

listsubdir(SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR})
foreach(SUBDIR ${SUBDIRS})
	add_subdirectory(${SUBDIR})
endforeach()