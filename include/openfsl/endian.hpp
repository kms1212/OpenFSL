/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
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

template<typename T> T openfsl::leToSystem(T leValue) {
    T ret;

    if (!isEndianChecked)
        checkEndian();

    // Flip byte order if system is big endian.
    if (!isLittleEndian)
        ret = flipEndian<T>(leValue);
    else
        ret = leValue;

    return ret;
}

template<typename T> T openfsl::beToSystem(T beValue) {
    T ret;

    if (!isEndianChecked)
        checkEndian();

    // Flip byte order if system is big endian.
    if (isLittleEndian)
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
    for (int i = sizeof(T) - 1; i > -1; i--) {
        ((uint8_t*)&ret)[i] = (uint8_t)value;
        value = value >> 8;
    }

    return ret;
}

#endif  // OPENFSL_ENDIAN_HPP_
