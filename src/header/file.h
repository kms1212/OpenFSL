/* Copyright (c) 2021z. kms1212(Min Su Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#ifndef __FS_FILE_H
#define __FS_FILE_H

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
}

#endif