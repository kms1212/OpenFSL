/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include <iomanip>
#include <iostream>

#include "openfsl/fslutils.h"

void openfsl::fsl_strtokenize(const std::string& str,
    std::vector<std::string>* tokens, const std::string& delimiters) {
    std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    std::string::size_type pos     = str.find_first_of(delimiters, lastPos);

    while (std::string::npos != pos || std::string::npos != lastPos) {
        tokens->push_back(str.substr(lastPos, pos - lastPos));
        lastPos = str.find_first_not_of(delimiters, pos);
        pos = str.find_first_of(delimiters, lastPos);
    }
}

std::tm openfsl::fsl_localtime(const std::time_t time) {
    std::tm tmTime;
#if _LOCALTIME64_S_EXIST == 1
    __time64_t time64 = (__time64_t)time;
    _localtime64_s(&tmTime, &time64);
#elif LOCALTIME_R_EXIST == 1
    localtime_r(&time, &tmTime);
#else
    std::tm* temp = std::localtime(&time);
    memcpy(&tmTime, temp, sizeof(std::tm));
#endif
    return tmTime;
}

openfsl::lba48_t openfsl::convertCHSToLBA(const CHS chs,
    const size_t sectPerTra, const size_t headPerCyl) {
    return (chs.cyl * headPerCyl + chs.head) * sectPerTra + (chs.sect - 1);
}

int openfsl::convertLBAToCHS(const lba48_t lba, CHS* chs,
    const uint8_t sectPerTra, const uint8_t headPerCyl) {
    if (lba > 16515135)
        return OPENFSL_ERROR_CANNOT_CONVERT_TO_CHS;
    chs->cyl = lba / (headPerCyl * sectPerTra);
    chs->head = (lba / sectPerTra) % headPerCyl;
    chs->sect = (lba % sectPerTra) + 1;
    return 0;
}

void __hexdump(const uint8_t* p, size_t offset, size_t len) {
    std::ios init(NULL);
    init.copyfmt(std::cout);

    size_t address = 0;
    size_t row = 0;

    std::cout << std::hex << std::setfill('0');
    while (1) {
        if (address >= len) break;
        size_t nread = ((len - address) > 16) ? 16 : (len - address);

        // Show the address
        std::cout << std::right << std::setw(8) << address + offset;

        // Show the hex codes
        for (size_t i = 0; i < 16; i++) {
            if (i % 8 == 0) std::cout << ' ';
            if (i < nread)
                std::cout << ' ' << std::setw(2) <<
                    static_cast<int>(p[16 * row + i + offset]);
            else
                std::cout << "   ";
        }

        // Show printable characters
        std::cout << "  ";
        for (size_t i = 0; i < nread; i++) {
            uint8_t ch = p[16 * row + i + offset];
            if (ch < 32 || ch > 126) std::cout << '.';
            else
                std::cout << ch;
        }

        std::cout << "\n";
        address += 16;
        row++;
    }

    std::cout.flush();
    std::cout.copyfmt(init);
}
