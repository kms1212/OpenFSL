/* Copyright (c) 2021z. kms1212(Min Su Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#ifndef __FILE_H
#define __FILE_H

#include <iostream>

namespace openFSL {

    /**
     *
     * @brief openFSL time
     * @author kms1212
     * @details
     * Name        | Description                   
     * ------------|-------------------------
     * time_millis | 1/1000 second
     * time_sec    | Second
     * time_min    | Minute
     * time_hour   | Hour
     * time_day    | Day
     * time_month  | Month
     * time_year   | Year
     *
     */
    typedef struct fileTimeStruct {
        int       time_millis;
        int       time_sec;
        int       time_min;
        int       time_hour;
        int       time_day;
        int       time_month;
        int       time_year;
    } FSL_Time;
    
    /**
     *
     * @brief File open mode
     * @author kms1212
     * @details
     * Mode      | Description
     * ----------|-----------------------------------------------
     * read      | Read file
     * write     | Write file
     * append    | Append to the end of the file. (WRITE enabled)
     * trunc     | Truncate file if existts. (WRITE enabled)
     * nocreate  | Fail if file not exists. (WRITE enabled)
     * noreplace | Fail if file exists. (WRITE enabled)
     * binary    | Binary mode
     *
     */
    enum FSL_OpenMode{
        read = 0x0001,
        write = 0x0002,
        append = 0x0006,    // 0x4 + 0x2
        trunc = 0x0000A,    // 0x8 + 0x2
        nocreate = 0x0012,  // 0x10 + 0x2
        noreplace = 0x0022, // 0x20 + 0x2
        binary = 0x0040
    };

    /**
     *
     * @brief OpenFSL file
     * @details Filesystem call wrapper for file control.
     * @author kms1212
     * @param T Filesystem class
     */
    template <class T> class FSL_File {
    private:
        class T::FILE* fileController;
        
    public:
        FSL_File<T>(T* fileSystem_, class T::FileInfo fileInfo_, FSL_OpenMode openMode_);
        ~FSL_File();
        
        class T::FILE* getFileController();
		int seek(size_t loc);
        size_t read(void* ptr, size_t size, size_t count);
    };
}

#include "openfsl/file.tpp"

#endif