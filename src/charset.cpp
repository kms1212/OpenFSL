/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
Check the full BSD-3-Clause license for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include <codecvt>
#include <locale>

#include "openfsl/charset.h"

std::u16string openfsl::u8ToUCS2(const std::string src) {
    std::wstring_convert<std::codecvt_utf8<char16_t>, char16_t> convert;

    try {
        return convert.from_bytes(src);
    }
    catch (...) {
        return nullptr;
    }
    return nullptr;
}

std::string openfsl::ucs2ToU8(const std::u16string src) {
    std::wstring_convert<std::codecvt_utf8<char16_t>, char16_t> convert;

    try {
        return convert.to_bytes(src);
    }
    catch (...) {
        return nullptr;
    }
    return nullptr;
}