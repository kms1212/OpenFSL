/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
Check the full BSD-3-Clause license for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/fsltime.h"

openfsl::Time openfsl::convertToFSLTime(const struct tm* time) {
    Time ret;
    ret.time_millis = 0;
    ret.time_sec = time->tm_sec;
    ret.time_min = time->tm_min;
    ret.time_hour = time->tm_hour;
    ret.time_day = time->tm_mday;
    ret.time_month = time->tm_mon + 1;
    ret.time_year = 1900 + time->tm_year;

    return ret;
}

const bool openfsl::operator== (const Time& a, const Time& b) {
    return ((a.time_year == b.time_year) &&
        (a.time_month == b.time_month) &&
        (a.time_day == b.time_day) &&
        (a.time_hour == b.time_hour) &&
        (a.time_min == b.time_min) &&
        (a.time_sec == b.time_sec) &&
        (a.time_millis == b.time_millis));
}

const bool openfsl::operator!= (const Time& a, const Time& b) {
    return ((a.time_year != b.time_year) ||
        (a.time_month != b.time_month) ||
        (a.time_day != b.time_day) ||
        (a.time_hour != b.time_hour) ||
        (a.time_min != b.time_min) ||
        (a.time_sec != b.time_sec) ||
        (a.time_millis != b.time_millis));
}
