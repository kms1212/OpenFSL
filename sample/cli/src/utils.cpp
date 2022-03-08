/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "header.h"

void hexdump(const uint8_t* p, size_t offset, size_t len) {
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
                std::cout << ' ' << std::setw(2)
                    << static_cast<int>(p[16 * row + i + offset]);
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

size_t split(const std::string& txt, std::vector<std::string>* strs, char ch) {
    std::string temp = txt;
    size_t pos = temp.find(ch);
    size_t initialPos = 0;
    strs->clear();

    // Decompose statement
    while (pos != std::string::npos) {
        if (temp.at(pos - 1) != '\\') {
            strs->push_back(temp.substr(initialPos, pos - initialPos));
            initialPos = pos + 1;
        } else {
            temp.erase(temp.begin() + pos - 1);
        }

        pos = temp.find(ch, pos + 1);
    }

    // Add the last one
    strs->push_back(temp.substr(initialPos,
        std::min(pos, temp.size()) - initialPos + 1));

    return strs->size();
}