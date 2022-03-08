/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#ifndef OPENFSL_CONTAINER_H_
#define OPENFSL_CONTAINER_H_

#ifndef __cplusplus
#error C++ compiler required.
#endif

#include <cstdint>
#include <cstddef>

namespace openfsl {
/**
 *
 * @brief Data container
 * @details Byte size container
 *
 */
class Container {
 private:
    size_t size;

 protected:
    void* data;

 public:
    /**
     * @brief Class constructor
     * @param size_: Container size
     */
    explicit Container(const size_t size_);

    ~Container();

    size_t getSize();

    /**
     * @brief Data buffer getter
     * @return void*: Data buffer pointer
     */
    void* getData();
    void* getData(size_t offset);

    /**
     * @brief Data buffer getter
     * @param T: Type name to static cast
     * @return void*: Data buffer pointer
     */
    template<class T> inline T* getDataCast()
        { return static_cast<T*>(data); }
    template<class T> inline T* getDataCast(size_t offset)
        { return reinterpret_cast<T*>(static_cast<uint8_t*>(data) + offset); }

    void* operator[](const size_t index);
    void* operator[](const int index);
};
}  // namespace openfsl

#endif  // OPENFSL_CONTAINER_H_
