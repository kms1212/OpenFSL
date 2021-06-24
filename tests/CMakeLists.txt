cmake_minimum_required(VERSION 3.1)
project("test")

include(ExternalProject)

add_subdirectory(fat32_init)
add_subdirectory(fat32_chdir)
add_subdirectory(fat32_getdirlist)
add_subdirectory(fat32_getfileinformation)
add_subdirectory(fat32_openclosefile)
add_subdirectory(fat32_seekfile)
add_subdirectory(fat32_mkdir)