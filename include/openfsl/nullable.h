/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
Check the full BSD-3-Clause license for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#ifndef OPENFSL_NULLABLE_H_
#define OPENFSL_NULLABLE_H_

#ifndef __cplusplus
#error C++ compiler required.
#endif

#include <cstdint>

#include <stdexcept>

namespace openfsl {

template <typename T> class Nullable {
 private:
    T* dataptr;

 public:
    Nullable() {
        dataptr = nullptr;
    }

    explicit Nullable(T data) {
        dataptr = new T(data);
    }

    ~Nullable() {
        if (dataptr != nullptr) {
            delete dataptr;
        }
    }

    bool isNull() {
        return dataptr == nullptr;
    }

    T& operator=(T data) {
        if (dataptr != nullptr) {
            delete dataptr;
        }
        dataptr = new T(data);
        return *dataptr;
    }

    T& operator!() {
        if (dataptr == nullptr) {
            throw std::runtime_error("Nullable<T> is null.");
        }
        return *dataptr;
    }

    operator T() {
        if (dataptr == nullptr) {
            throw std::runtime_error("Nullable<T> is null.");
        }
        return *dataptr;
    }
};

}  // namespace openfsl

#endif  // OPENFSL_NULLABLE_H_
