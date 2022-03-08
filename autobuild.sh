#!/bin/bash

# Copyright (c) 2021. kms1212(Minsu Kwon)
# This file is part of OpenFSL.
# 
# OpenFSL and its source code is published over BSD 3-Clause License.
# See the BSD-3-Clause for more details.
# <https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

set -e

###################################
# Check superuser privileges
###################################
if [ ! "$UID" -eq 0 ]; then
    echo "This script requires superuser privileges."
    exit 1
fi

###################################
# Detect system package manager
###################################
echo "Detecting package manager..."

if [ -f /etc/os-release ]; then
    . /etc/os-release
fi

declare -A osInfo;
set +e
osInfo[/etc/redhat-release]="$(which yum) install -y"
osInfo[/etc/alpine-release]="$(which apk) --update add"
osInfo[/etc/arch-release]="$(which pacman) -S --noconfirm";
osInfo[/etc/SuSE-release]="$(which zypper) install -y"
osInfo[/etc/debian_version]="$(which apt) install -y"
set -e

for f in ${!osInfo[@]}
do
    if [[ -f $f ]];then
        PKGINST=${osInfo[$f]}
    fi
done

if [ "$ID" = "freebsd" ]; then
    PKGINST="$(which pkg) install -y"
fi

###################################
# Configure Build Method
###################################
read -p "Specify build directory(Default: $(dirname "$0")/build): " BUILD_DIR
if [ "$BUILD_DIR" = "" ]; then
    BUILD_DIR="$(dirname "$0")/build"
fi
mkdir -p "$BUILD_DIR"


echo "Default build configurations:"
echo "  CMAKE_BUILD_TYPE=Release"
echo "  TARGET_ARCHITECTURE=native"
echo "  TARGET_TRIPLET=x86_64-unknown-linux-gnu"
echo "  NO_BUILD=false"
echo "  BUILD_TESTING=true"
echo "  BUILD_SAMPLE=true"
echo "  BUILD_FAT32=true"
echo "  BUILD_NTFS=true"
echo "  TEST_COVERAGE=false"
echo "  DOXYGEN_GENERATE=false"
echo "  PACKAGE_GENERATE=false"
echo "  PACKAGE_GENERATOR=ZIP"
echo "  PACKAGE_TYPE=NIGHTLY"
read -p "Will you use default configuration?(Y/n): " CONFIG_TYPE
if [ "$CONFIG_TYPE" = "N" ] || [ "$CONFIG_TYPE" = "n" ]; then
    TARGET_ARCHITECTURE="native"
    TARGET_TRIPLET="x86_64-unknown-linux-gnu"
    CMAKE_BUILD_TYPE="Release"
    NO_BUILD="false"
    BUILD_TESTING="true"
    BUILD_SAMPLE="true"
    BUILD_FAT32="true"
    BUILD_NTFS="true"
    TEST_COVERAGE="false"
    DOXYGEN_GENERATE="false"
    PACKAGE_GENERATE="false"
    PACKAGE_TYPE="NIGHTLY"
    PACKAGE_GENERATOR="ZIP"
    
    while true
    do
        read -p "Specify build type(Release or Debug, Default Release): " CMAKE_BUILD_TYPE
        if [ "$CMAKE_BUILD_TYPE" = "" ]; then
            CMAKE_BUILD_TYPE="Release"
        fi
        if [ "$CMAKE_BUILD_TYPE" = "Release" ] || [ "$CMAKE_BUILD_TYPE" = "Debug" ]; then
            break
        fi
    done
    
    read -p "Specify target architecture for gcc(Default native): " TARGET_MENU_INPUT
    
    if [ ! "$TARGET_MENU_INPUT" = "" ]; then
        TARGET_ARCHITECTURE="$TARGET_MENU_INPUT"
    fi
    
    read -p "Specify target tuplet for clang(Default x86_64-unknown-linux-gnu): " TARGET_MENU_INPUT
    
    if [ ! "$TARGET_MENU_INPUT" = "" ]; then
        TARGET_TRIPLET="$TARGET_MENU_INPUT"
    fi
    
    read -p "Will you do not build binaries?(y/N): " CONF_ANSWER
    if [ "$CONF_ANSWER" = "Y" ] || [ "$CONF_ANSWER" = "y" ]; then
        NO_BUILD="true"
    fi
    
    read -p "Will you build test projects?(Y/n): " CONF_ANSWER
    if [ "$CONF_ANSWER" = "N" ] || [ "$CONF_ANSWER" = "n" ]; then
        BUILD_TESTING="false"
    fi
    
    read -p "Will you build sample executable?(Y/n): " CONF_ANSWER
    if [ "$CONF_ANSWER" = "N" ] || [ "$CONF_ANSWER" = "n" ]; then
        BUILD_SAMPLE="false"
    fi
    
    read -p "Will you build FAT32 library?(Y/n): " CONF_ANSWER
    if [ "$CONF_ANSWER" = "N" ] || [ "$CONF_ANSWER" = "n" ]; then
        BUILD_FAT32="false"
    fi
    
    read -p "Will you build NTFS library?(Y/n): " CONF_ANSWER
    if [ "$CONF_ANSWER" = "N" ] || [ "$CONF_ANSWER" = "n" ]; then
        BUILD_NTFS="false"
    fi
    
    read -p "Will you test coverage?(y/N): " CONF_ANSWER
    if [ "$CONF_ANSWER" = "Y" ] || [ "$CONF_ANSWER" = "y" ]; then
        TEST_COVERAGE="true"
    fi
    
    read -p "Will you generate documents?(y/N): " CONF_ANSWER
    if [ "$CONF_ANSWER" = "Y" ] || [ "$CONF_ANSWER" = "y" ]; then
        DOXYGEN_GENERATE="true"
    fi
    
    read -p "Will you generate packages?(y/N): " CONF_ANSWER
    if [ "$CONF_ANSWER" = "Y" ] || [ "$CONF_ANSWER" = "y" ]; then
        PACKAGE_GENERATE="true"
        
        read -p "Specify package type(NIGHTLY or BETA or RELEASE): " PACKAGE_TYPE
        
        echo "Visit https://gitlab.kitware.com/cmake/community/-/wikis/doc/cpack/PackageGenerators to see the generator list."
        read -p "Specify package generator: " PACKAGE_GENERATOR
    fi
else
    TARGET_ARCHITECTURE="native"
    CMAKE_BUILD_TYPE="Release"
    NO_BUILD="false"
    BUILD_TESTING="true"
    BUILD_SAMPLE="true"
    BUILD_FAT32="true"
    BUILD_NTFS="true"
    TEST_COVERAGE="false"
    DOXYGEN_GENERATE="false"
    PACKAGE_GENERATE="false"
    PACKAGE_TYPE="NIGHTLY"
    PACKAGE_GENERATOR="ZIP"
fi

###################################
# Install dependencies
###################################
echo "Installing dependencies..."

$PKGINST nasm cmake binutils zip jq python
if [ ! "$ID" = "freebsd" ]; then
    $PKGINST gcc g++ make
else
    $PKGINST gmake
fi

if [ "$DOXYGEN_GENERATE" = "true" ]; then
    $PKGINST doxygen graphviz
fi

###################################
# Configure CMake
###################################
CMAKE=$(which cmake)

$CMAKE -G "Unix Makefiles"\
       -S$(dirname "$0") -B"$BUILD_DIR"\
       -DCMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE\
       -DTARGET_ARCHITECTURE=$TARGET_ARCHITECTURE\
       -DTARGET_TRIPLET=$TARGET_TRIPLET\
       -DNO_BUILD=$NO_BUILD\
       -DBUILD_TESTING=$BUILD_TESTING\
       -DBUILD_SAMPLE=$BUILD_SAMPLE\
       -DBUILD_FAT32=$BUILD_FAT32\
       -DBUILD_NTFS=$BUILD_NTFS\
       -DTEST_COVERAGE=$TEST_COVERAGE\
       -DDOXYGEN_GENERATE=$DOXYGEN_GENERATE\
       -DPACKAGE_GENERATE=$PACKAGE_GENERATE\
       -DPACKAGE_TYPE=$PACKAGE_TYPE\
       -DPACKAGE_GENERATOR=$PACKAGE_GENERATOR\

###################################
# Build project
###################################
$CMAKE --build "$BUILD_DIR" --config $CMAKE_BUILD_TYPE --target all

###################################
# Test project
###################################
if [ "$BUILD_TESTING" = "true" ]; then
    cd "$BUILD_DIR"
    CTEST=$(which ctest)
    $CTEST --output-on-failure
fi

cd $(dirname "$0")

###################################
# Test coverage
###################################
if [ "$TEST_COVERAGE" = "true" ]; then
    cd "$BUILD_DIR"
    CTEST=$(which ctest)
    $CTEST -T Coverage
fi

cd $(dirname "$0")

###################################
# Pack project
###################################
if [ "$PACKAGE_GENERATE" = "true" ]; then
    cd "$BUILD_DIR"
    CPACK=$(which cpack)
    $CPACK
fi

cd $(dirname "$0")
