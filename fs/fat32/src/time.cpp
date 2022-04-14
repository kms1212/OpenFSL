/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
Check the full BSD-3-Clause license for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/fat32/fs_fat32.h"

uint16_t openfsl::FAT32::__dateToWord(const Time date) {
    uint16_t ret;
    ret = (date.time_day & 0x001F);
    ret += (date.time_month & 0x000F) << 5;
    ret += ((date.time_year - 1980) & 0x07F) << 9;

    return ret;
}

uint16_t openfsl::FAT32::__timeToWord(const Time time) {
    uint16_t ret;
    ret = ((time.time_sec / 2) & 0x1F);
    ret += (time.time_min & 0x3F) << 5;
    ret += (time.time_hour & 0x1F) << 11;

    return ret;
}

openfsl::Time* openfsl::FAT32::__wordToDate(Time* d, const uint16_t date) {
    d->time_day = (date & 0x001F);
    d->time_month = (date & 0x01E0) >> 5;
    d->time_year = ((date & 0xFE00) >> 9) + 1980;

    return d;
}

openfsl::Time* openfsl::FAT32::__wordToTime(Time* t, const uint16_t time) {
    t->time_sec = (time & 0x001F) * 2;
    t->time_min = (time & 0x07E0) >> 5;
    t->time_hour = (time & 0xF800) >> 11;

    return t;
}
