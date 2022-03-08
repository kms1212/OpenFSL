# Copyright (c) 2021. kms1212(Minsu Kwon)
# This file is part of OpenFSL.
# 
# OpenFSL and its source code is published over BSD 3-Clause License.
# See the BSD-3-Clause for more details.
# <https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

message("ConfigureVersion")

# Reading version file
file(READ "${SOURCE_DIR}/version.json" JQ_OUTPUT)
	
# Read file and get last build date
execute_process(COMMAND ${CMAKE_COMMAND} -E echo "${JQ_OUTPUT}"
				COMMAND jq -r ".last_build"
				OUTPUT_VARIABLE JQ_VALUE)
set(FILE_LASTBUILD "${JQ_VALUE}")

string(TIMESTAMP TODAY "%Y%m%d" UTC)

# Update last build date
if (NOT ${TODAY} EQUAL "${FILE_LASTBUILD}" AND INCREASE_BUILD_NUMBER)
	message("Last build date is not equal to today's date.")
	message("Build date updated: ${TODAY}")
	execute_process(COMMAND ${CMAKE_COMMAND} -E echo "${JQ_OUTPUT}"
					COMMAND jq ".last_build=${TODAY}"
					COMMAND jq ".version_build_daily=0"
					OUTPUT_VARIABLE JQ_OUTPUT)
endif()
				
# Get major version
execute_process(COMMAND ${CMAKE_COMMAND} -E echo "${JQ_OUTPUT}"
				COMMAND jq -r ".version_major"
				OUTPUT_VARIABLE JQ_VALUE)
set(FILE_VERSION_MAJOR ${JQ_VALUE})
string(REGEX REPLACE "\n$" "" FILE_VERSION_MAJOR "${FILE_VERSION_MAJOR}")
				
# Get minor version
execute_process(COMMAND ${CMAKE_COMMAND} -E echo "${JQ_OUTPUT}"
				COMMAND jq -r ".version_minor"
				OUTPUT_VARIABLE JQ_VALUE)
set(FILE_VERSION_MINOR ${JQ_VALUE})
string(REGEX REPLACE "\n$" "" FILE_VERSION_MINOR "${FILE_VERSION_MINOR}")
				
# Get patch version
execute_process(COMMAND ${CMAKE_COMMAND} -E echo "${JQ_OUTPUT}"
				COMMAND jq -r ".version_patch"
				OUTPUT_VARIABLE JQ_VALUE)
set(FILE_VERSION_PATCH ${JQ_VALUE})
string(REGEX REPLACE "\n$" "" FILE_VERSION_PATCH "${FILE_VERSION_PATCH}")
				
# Get total build count
execute_process(COMMAND ${CMAKE_COMMAND} -E echo "${JQ_OUTPUT}"
				COMMAND jq -r ".version_build_total"
				OUTPUT_VARIABLE JQ_VALUE)
set(FILE_BUILDTOTAL ${JQ_VALUE})
string(REGEX REPLACE "\n$" "" FILE_BUILDTOTAL "${FILE_BUILDTOTAL}")

if (INCREASE_BUILD_NUMBER)
	math(EXPR FILE_BUILDTOTAL "${FILE_BUILDTOTAL}+1")

	message("Total build count incresed : ${FILE_BUILDTOTAL}")

	execute_process(COMMAND ${CMAKE_COMMAND} -E echo "${JQ_OUTPUT}"
					COMMAND jq ".version_build_total=${FILE_BUILDTOTAL}"
					OUTPUT_VARIABLE JQ_OUTPUT)
endif ()
				
# Get daily build count
execute_process(COMMAND ${CMAKE_COMMAND} -E echo "${JQ_OUTPUT}"
				COMMAND jq -r ".version_build_daily"
				OUTPUT_VARIABLE JQ_VALUE)
set(FILE_BUILDDAILY ${JQ_VALUE})
string(REGEX REPLACE "\n$" "" FILE_BUILDDAILY "${FILE_BUILDDAILY}")

if (INCREASE_BUILD_NUMBER)
	math(EXPR FILE_BUILDDAILY "${FILE_BUILDDAILY}+1")

	message("Daily build count incresed : ${FILE_BUILDDAILY}")

	execute_process(COMMAND ${CMAKE_COMMAND} -E echo "${JQ_OUTPUT}"
					COMMAND jq ".version_build_daily=${FILE_BUILDDAILY}"
					OUTPUT_VARIABLE JQ_OUTPUT)
endif()
				
# Write changes
if (INCREASE_BUILD_NUMBER)
	file(WRITE "${SOURCE_DIR}/version.json" "${JQ_OUTPUT}")
endif()

# Configure library header
set(PROJECT_VERSION_MAJOR ${FILE_VERSION_MAJOR})
set(PROJECT_VERSION_MINOR ${FILE_VERSION_MINOR})
set(PROJECT_VERSION_PATCH ${FILE_VERSION_PATCH})
set(PROJECT_VERSION_BUILD ${FILE_BUILDTOTAL})
set(PROJECT_VERSION_DATE ${TODAY})

if(PACKAGE_TYPE STREQUAL "RELEASE")
	set(PROJECT_VERSION_TYPE "OPENFSL_VERSION_TYPE_RELEASE")
	set(PROJECT_VERSION_STRING "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")
elseif(PACKAGE_TYPE STREQUAL "BETA")
	set(PROJECT_VERSION_TYPE "OPENFSL_VERSION_TYPE_BETA")
	set(PROJECT_VERSION_STRING "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_BUILD}")
elseif(PACKAGE_TYPE STREQUAL "NIGHTLY")
	set(PROJECT_VERSION_TYPE "OPENFSL_VERSION_TYPE_NIGHTLY")
	set(PROJECT_VERSION_BUILD ${FILE_BUILDDAILY})
	set(PROJECT_VERSION_STRING "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_DATE}NR.${PROJECT_VERSION_BUILD}")
else()
	set(PROJECT_VERSION_TYPE "OPENFSL_VERSION_TYPE_NONE")
	set(PROJECT_VERSION_STRING "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")
endif()
set(PROJECT_FULL_VERSION_STRING "OpenFSL ${PROJECT_VERSION_STRING} (${CXX_COMPILER_ID} ${CXX_COMPILER_VERSION} ${HOST_SYSTEM_PROCESSOR}) Running On ${SYSTEM_NAME}")
configure_file("${SOURCE_DIR}/src/version.cpp.in"
			   "${SOURCE_DIR}/src/version.cpp")

# Write version to file
file(WRITE "${BINARY_DIR}/Version.cmake" "set(OPENFSL_VERSION \"${PROJECT_VERSION_STRING}\")")