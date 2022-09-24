/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
Check the full BSD-3-Clause license for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#ifndef OPENFSL_CHARSET_H_
#define OPENFSL_CHARSET_H_

#ifndef __cplusplus
#error C++ compiler required.
#endif

#include <cstdint>
#include <cstddef>

#include <string>

namespace openfsl {

/**
 * @brief UCS-2 string to UTF-8 string
 * @details Converts UCS-2 u16string to UTF-8 string.
 * @param src: source string
 * @return std::string: converted string
 */
std::string ucs2ToU8(const std::u16string src);

/**
 * @brief UTF-8 string to UCS-2 string
 * @details Converts UTF-8 string to UCS-2 u16string.
 * @param src: source string
 * @return std::u16string: converted string
 */
std::u16string u8ToUCS2(const std::string src);

}  // namespace openfsl

#endif  // OPENFSL_CHARSET_H_
