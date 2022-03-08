/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#ifndef OPENFSL_FSLTIME_H_
#define OPENFSL_FSLTIME_H_

#ifndef __cplusplus
#error C++ compiler required.
#endif

#include <ctime>

namespace openfsl {
/**
 *
 * @brief openfsl time
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
typedef struct Time {
    int       time_millis = 0;
    int       time_sec = 0;
    int       time_min = 0;
    int       time_hour = 0;
    int       time_day = 0;
    int       time_month = 0;
    int       time_year = 0;
} Time;

Time convertToFSLTime(const struct tm* time);

const bool operator== (const Time& a, const Time& b);
const bool operator!= (const Time& a, const Time& b);
}  // namespace openfsl

#endif  // OPENFSL_FSLTIME_H_
