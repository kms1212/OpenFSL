/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include <cstring>
#include <cinttypes>
#include <stdexcept>
#include <random>

#include "openfsl/guid.h"

openfsl::GUID::GUID() {
    clear();
}

openfsl::GUID::GUID(
    const uint32_t guid1,
    const uint16_t guid2,
    const uint16_t guid3,
    const uint16_t guid4,
    const uint64_t guid5) {
    setGUID(guid1, guid2, guid3, guid4, guid5);
}

openfsl::GUID::GUID(const uint8_t* guidByteArray) {
    setGUID(guidByteArray);
}

openfsl::GUID::GUID(const char* guidString) {
    setGUID(guidString);
}

openfsl::GUID::~GUID() { }

void openfsl::GUID::setGUID(
    const uint32_t guid1,
    const uint16_t guid2,
    const uint16_t guid3,
    const uint16_t guid4,
    const uint64_t guid5) {
    this->guid1 = guid1;
    this->guid2 = guid2;
    this->guid3 = guid3;
    this->guid4 = guid4;
    this->guid5 = guid5;
}

void openfsl::GUID::setGUID(
    const uint8_t* guidByteArray) {
    this->guid1 = *(reinterpret_cast<const uint32_t*>(guidByteArray));
    this->guid2 = *(reinterpret_cast<const uint16_t*>(guidByteArray + 4));
    this->guid3 = *(reinterpret_cast<const uint16_t*>(guidByteArray + 6));
    this->guid4 = 0;
    this->guid5 = 0;

    for (int i = 0; i < 2; i++) {
        guid4 = guid4 << 8;
        guid4 += guidByteArray[i + 8];
    }

    for (int i = 0; i < 6; i++) {
        guid5 = guid5 << 8;
        guid5 += guidByteArray[i + 10];
    }
}

void openfsl::GUID::setGUID(const char* guidString) {
    char stdGuidString[37];

    if (guidString[0] == '{') {
        if ((strnlen(guidString, 40) != 38) &&
            (guidString[37] != '}')) {
            throw std::invalid_argument("Invalid GUID string format");
        }

        strncpy(stdGuidString, guidString + 1, 36);
    } else {
        if (strnlen(guidString, 40) != 36) {
            throw std::invalid_argument("Invalid GUID string format");
        }

        strncpy(stdGuidString, guidString, 36);
    }

    if ((stdGuidString[8] != '-') ||
        (stdGuidString[13] != '-') ||
        (stdGuidString[18] != '-') ||
        (stdGuidString[23] != '-')) {
        throw std::invalid_argument("Invalid GUID string format");
    }

    stdGuidString[8] = 0;
    stdGuidString[13] = 0;
    stdGuidString[18] = 0;
    stdGuidString[23] = 0;
    stdGuidString[36] = 0;

    for (char c : stdGuidString) {
        if (!(c >= '0' && c <= '9') &&
            !(c >= 'A' && c <= 'F') &&
            !(c >= 'a' && c <= 'f') &&
            c != 0) {
            throw std::invalid_argument("Invalid GUID string format");
        }
    }

    guid1 = (uint32_t)strtoul(stdGuidString, NULL, 16);
    guid2 = (uint16_t)strtoul(stdGuidString + 9, NULL, 16);
    guid3 = (uint16_t)strtoul(stdGuidString + 14, NULL, 16);
    guid4 = (uint16_t)strtoul(stdGuidString + 19, NULL, 16);
    guid5 = (uint64_t)strtoull(stdGuidString + 24, NULL, 16);
}

void openfsl::GUID::clear() {
    this->guid1 = 0;
    this->guid2 = 0;
    this->guid3 = 0;
    this->guid4 = 0;
    this->guid5 = 0;
}

void openfsl::GUID::toString(std::string* guidString, const bool msFormat) {
    char guidCharArray[40] = { 0, };
    std::sprintf(guidCharArray, "%08X-%04X-%04X-%04X-%012" PRIX64 "", guid1, guid2, guid3, guid4, guid5);
    *guidString = (msFormat ? "{" : "") + std::string(guidCharArray) + (msFormat ? "}" : "");
}

void openfsl::GUID::toByteArray(uint8_t* guidByteArray) {
    uint16_t guid4d = guid4;
    uint64_t guid5d = guid5;

    memcpy(guidByteArray, &guid1, 4);
    memcpy(guidByteArray + 4, &guid2, 2);
    memcpy(guidByteArray + 6, &guid3, 2);
    
    for (int i = 1; i > -1; i--) {
        guidByteArray[i + 8] = (uint8_t)guid4d;
        guid4d = guid4d >> 8;
    }
    
    for (int i = 5; i > -1; i--) {
        guidByteArray[i + 10] = (uint8_t)guid5d;
        guid5d = guid5d >> 8;
    }
}

void openfsl::GUID::toIntRecord(uint32_t* guid1, uint16_t* guid2, uint16_t* guid3, uint16_t* guid4, uint64_t* guid5) {
    *guid1 = this->guid1;
    *guid2 = this->guid2;
    *guid3 = this->guid3;
    *guid4 = this->guid4;
    *guid5 = this->guid5;
}

openfsl::GUID openfsl::GUID::generateGuid4() {
    std::random_device rd;
    std::mt19937 rng(rd());
    std::mt19937_64 rng64(rd());

    std::uniform_int_distribution<uint16_t> g3dist(0, 0xFFF);
    uint16_t guid3 = g3dist(rng);
    guid3 += 0x4000;

    std::uniform_int_distribution<uint16_t> g4dist(0, 0x3FFF);
    uint16_t guid4 = g4dist(rng);
    guid4 += 0x8000;
    
    return openfsl::GUID(rng(), rng(), guid3, guid4, rng64());
}

bool openfsl::GUID::operator== (const openfsl::GUID& tgt) const {
    return ((this->guid1 == tgt.guid1) &&
        (this->guid2 == tgt.guid2) &&
        (this->guid3 == tgt.guid3) &&
        (this->guid4 == tgt.guid4) && 
        (this->guid5 == tgt.guid5));
}

bool openfsl::GUID::operator!= (const openfsl::GUID& tgt) const {
    return !((this->guid1 == tgt.guid1) &&
        (this->guid2 == tgt.guid2) &&
        (this->guid3 == tgt.guid3) &&
        (this->guid4 == tgt.guid4) && 
        (this->guid5 == tgt.guid5));
}

size_t openfsl::GUIDHash::operator()(const openfsl::GUID& guid) const {
    std::string str;
    openfsl::GUID* tmp = new openfsl::GUID(guid);
    tmp->toString(&str, false);
    delete tmp;
    return std::hash<std::string>()(str);
}
