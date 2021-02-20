#!/bin/bash

######################################################
# configure.sh
######################################################

printUsage()
{
    echo "Usage: $0 [option]"
    echo "  [description]"
    echo "      make a Makefile Make build script by cmake"
    echo "      "
    echo "  [option]"
    echo "      (default) : make a Makefile by CMake at ./build/ directory"
    echo "      clean     : remove make a Makefile"
    echo "      help      : print a usage"
}


if [ "$1" = "help" ]; then
    printUsage
    exit
fi


if [ "$1" = "" ] ; then

    if [ -d ./build ] ; then
        echo ""
        echo "[Warning] ./build/ directory is exist.  You should delete ./build/ first. or use this command: "
        echo "    $0 clean "
        echo ""
        exit
    fi 

    mkdir build
    cd build

    cmake -S .. -B .

    echo "-------------"
    echo " How to make"
    echo "-------------"
    echo " cd build"
    echo " make"
    echo "-------------"


elif [ "$1" = "clean" ] ; then
    read -p "Do you want to delete ./build/ ? (y/n) " YesNo
    if [ "$YesNo" = "y" ] ; then
        rm -rf build
    fi 
else
    printUsage
fi