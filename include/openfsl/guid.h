/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#ifndef OPENFSL_GUID_H_
#define OPENFSL_GUID_H_

#ifndef __cplusplus
#error C++ compiler required.
#endif

#include <cstdint>
#include <string>

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
    GUID(const uint32_t guid1, const uint16_t guid2, const uint16_t guid3, const uint16_t guid4, const uint64_t guid5);
    GUID(const uint8_t* guidByteArray);
    GUID(const char* guidString);

    /**
     * @brief GUID class destructor
     */
    ~GUID();
    
    void toString(std::string* guidString, const bool msFormat);
    void toByteArray(uint8_t* guidByteArray);
    void toIntRecord(uint32_t* guid1, uint16_t* guid2, uint16_t* guid3, uint16_t* guid4, uint64_t* guid5);

    bool equals(GUID& guid);

    static GUID generateGuid4();

    inline bool operator== (const GUID& tgt);
    inline bool operator!= (const GUID& tgt);
};
}  // namespace openfsl

#endif  // OPENFSL_GUID_H_
