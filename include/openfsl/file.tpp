/* Copyright (c) 2021z. kms1212(Min Su Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#ifndef __FILE_TPP
#define __FILE_TPP

#ifndef __FILE_H
#error __FILE__ should only be included from file.h
#endif

#include "file.h"

using namespace openFSL;

template<typename T> FSL_File<T>::FSL_File(T* fileSystem_, typename T::FileInfo fileInfo_, FSL_OpenMode openMode_) {
    fileController = new typename T::FILE(fileSystem_, fileInfo_, openMode_);
}

template<typename T> FSL_File<T>::~FSL_File()
{
    delete fileController;
}

template<typename T> typename T::FILE* FSL_File<T>::getFileController()
{
    return fileController;
}

template<typename T> size_t FSL_File<T>::read(void* ptr, size_t size, size_t count)
{
    return fileController->read(ptr, size, count);
}

template<typename T> int FSL_File<T>::seek(size_t loc)
{
    return fileController->seek(loc);
}

/*

template<typename T> uint8_t FSL_File<T>::getc()
{
    return 0;
}

template<typename T> int FSL_File<T>::getw()
{
    return 0;
}

template<typename T> std::string FSL_File<T>::gets()
{
    
    return 0;
}*/

#endif