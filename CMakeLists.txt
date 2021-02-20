cmake_minimum_required(VERSION 3.0)
project("OpenFSL")

file(GLOB libSources src/*.cpp)
add_library(openfsl STATIC ${libSources})