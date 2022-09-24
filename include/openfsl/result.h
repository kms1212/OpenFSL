/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
Check the full BSD-3-Clause license for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#ifndef OPENFSL_RESULT_H_
#define OPENFSL_RESULT_H_

#ifndef __cplusplus
#error C++ compiler required.
#endif

#include <cstdint>

#include "openfsl/nullable.h"
#include "openfsl/error.h"

namespace openfsl {

template <typename T> class Result {
 private:
    Nullable<T> data = Nullable<T>();
    error_t error = OPENFSL_SUCCESS;
    bool warning = false;

 public:
    explicit Result(T data) {
        this->data = data;
    }

    explicit Result(error_t error, bool warning = false) {
        this->error = error;
        this->warning = warning;
    }

    T getValue() {
        return data;
    }

    error_t getError() {
        return error;
    }

    bool isSuccess() {
        return error == OPENFSL_SUCCESS;
    }

    bool isWarning() {
        return error != OPENFSL_SUCCESS && warning;
    }

    static T tryResult(Result result) {
        if (result.getError()) {
            throw std::runtime_error("Error: " + geterrorstr(result.getError()));
        }
        return result.data;
    }
};

}  // namespace openfsl

#endif  // OPENFSL_RESULT_H_
