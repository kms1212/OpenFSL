/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
Check the full BSD-3-Clause license for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/fat32/fs_fat32.h"

openfsl::Time openfsl::FAT32::toFslTime(const openfsl::FAT32::Time fatTime) {
    openfsl::Time fslTime;
    fslTime.time_sec = fatTime.second * 2;
    fslTime.time_min = fatTime.minute;
    fslTime.time_hour = fatTime.hour;
    fslTime.time_day = 0;
    fslTime.time_month = 0;
    fslTime.time_year = 0;
    return fslTime;
}

openfsl::Time openfsl::FAT32::toFslTime(const openfsl::FAT32::Date fatDate) {
    openfsl::Time fslTime;
    fslTime.time_sec = 0;
    fslTime.time_min = 0;
    fslTime.time_hour = 0;
    fslTime.time_day = fatDate.day;
    fslTime.time_month = fatDate.month;
    fslTime.time_year = fatDate.year + 1980;
    return fslTime;
}

openfsl::Time openfsl::FAT32::toFslTime(const openfsl::FAT32::Time fatTime, const openfsl::FAT32::Date fatDate) {
    openfsl::Time fslTime;
    openfsl::Time tmp;
    fslTime = toFslTime(fatTime);
    tmp = toFslTime(fatDate);

    fslTime.time_day = tmp.time_day;
    fslTime.time_month = tmp.time_month;
    fslTime.time_year = tmp.time_year;

    return fslTime;
}

openfsl::FAT32::Time openfsl::FAT32::toFatTime(const openfsl::Time fslTime) {
    openfsl::FAT32::Time fatTime;
    fatTime.second = fslTime.time_sec / 2;
    fatTime.minute = fslTime.time_min;
    fatTime.hour = fslTime.time_hour;
    return fatTime;
}

openfsl::FAT32::Date openfsl::FAT32::toFatDate(const openfsl::Time fslTime) {
    openfsl::FAT32::Date fatDate;
    fatDate.day = fslTime.time_day;
    fatDate.month = fslTime.time_month;
    fatDate.year = fslTime.time_year - 1980;
    return fatDate;
}
