/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
Check the full BSD-3-Clause license for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#ifndef OPENFSL_GUID_H_
#define OPENFSL_GUID_H_

#ifndef __cplusplus
#error C++ compiler required.
#endif

#include <cstdint>
#include <string>

#include "openfsl/endian.h"

namespace openfsl {
/**
 *
 * @brief Sector buffer
 * @details Sector buffer with uint8_t(byte) array.
 *
 */
class GUID {
 private:
    uint32_t guid1;
    uint16_t guid2;
    uint16_t guid3;
    uint16_t guid4;
    uint64_t guid5;

 public:
    /**
     *
     * @brief GUID class contructor
     *
     */
    GUID();
    GUID(const uint32_t guid1,
        const uint16_t guid2,
        const uint16_t guid3,
        const uint16_t guid4,
        const uint64_t guid5);
    explicit GUID(const uint8_t* guidByteArray);
    explicit GUID(const char* guidString);

    /**
     * @brief GUID class destructor
     */
    ~GUID();

    void setGUID(const uint32_t guid1,
        const uint16_t guid2,
        const uint16_t guid3,
        const uint16_t guid4,
        const uint64_t guid5);
    void setGUID(const uint8_t* guidByteArray);
    void setGUID(const char* guidString);

    void clear();

    void toString(std::string* guidString, const bool msFormat);
    void toByteArray(uint8_t* guidByteArray);
    void toIntRecord(uint32_t* guid1,
        uint16_t* guid2,
        uint16_t* guid3,
        uint16_t* guid4,
        uint64_t* guid5);

    bool equals(const GUID& guid);

    static size_t hash(const GUID& guid);

    static GUID generateGuid4();

    bool operator== (const GUID& tgt) const;
    bool operator!= (const GUID& tgt) const;
};

struct GUIDHash {
    size_t operator()(const GUID& guid) const;
};
}  // namespace openfsl


#endif  // OPENFSL_GUID_H_
