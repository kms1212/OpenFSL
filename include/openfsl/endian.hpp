/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
Check the full BSD-3-Clause license for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#ifndef OPENFSL_ENDIAN_HPP_
#define OPENFSL_ENDIAN_HPP_

#ifndef __cplusplus
#error C++ compiler required.
#endif

#ifndef OPENFSL_ENDIAN_H_TEMPLATE_
#error endian.hpp has to be included by endian.h.
#endif

#ifdef _MSC_VER
#include <cstdlib>

#pragma warning(disable: 4244)
#endif

template<typename T> T openfsl::leToSystem(T leValue) {
    T ret;

    if (!isEndianChecked())
        checkEndian();

    // Flip byte order if system is big endian.
    if (!isLittleEndian())
        ret = flipEndian<T>(leValue);
    else
        ret = leValue;

    return ret;
}

template<typename T> T openfsl::beToSystem(T beValue) {
    T ret;

    if (!isEndianChecked())
        checkEndian();

    // Flip byte order if system is big endian.
    if (isLittleEndian())
        ret = flipEndian<T>(beValue);
    else
        ret = beValue;

    return ret;
}

template<typename T> T openfsl::systemToLe(T value) {
    return leToSystem<T>(value);
}

template<typename T> T openfsl::systemToBe(T value) {
    return beToSystem<T>(value);
}

template<typename T> T openfsl::flipEndian(T value) {
    T ret;

    // Flip byte order.
#if defined(__GNUC__) || defined(_MSC_VER)
    switch (sizeof(T)) {
#if defined(__GNUC__)
     case 2:
        ret = __builtin_bswap16(value);
        break;
     case 4:
        ret = __builtin_bswap32(value);
        break;
     case 8:
        ret = __builtin_bswap64(value);
        break;
#if defined(__uint128_t)
     case 16:
        ret = __builtin_bswap128();
        break;
#endif
#elif defined(_MSC_VER)
     case sizeof(unsigned short):
        ret = _byteswap_ushort(value);
        break;
     case sizeof(unsigned long):
        ret = _byteswap_ulong(value);
        break;
     case sizeof(unsigned long long):
        ret = _byteswap_uint64(value);
        break;
#endif
     default:
        for (size_t i = 0; i < sizeof(T); i++) {
            ((uint8_t*)&ret)[i] = ((uint8_t*)&value)[sizeof(T) - i - 1];
        }
    }
#else
    for (size_t i = 0; i < sizeof(T); i++) {
        ((uint8_t*)&ret)[i] = ((uint8_t*)&value)[sizeof(T) - i - 1];
    }
#endif

    return ret;
}

#ifdef _MSC_VER
#pragma warning(default: 4244)
#endif

#endif  // OPENFSL_ENDIAN_HPP_
