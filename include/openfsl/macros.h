/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#ifndef OPENFSL_MACROS_H_
#define OPENFSL_MACROS_H_

#ifndef __cplusplus
#error C++ compiler required.
#endif

#if defined(__GNUC__) && (__GNUC__ >= 4)

#ifndef __check_result
#define __check_result __attribute__((warn_unused_result))
#endif

#ifndef __deprecated
#define __deprecated __attribute__((deprecated))
#endif

#ifndef __deprecated_msg
#define __deprecated_msg(msg) __attribute__((deprecated(msg)))
#endif

#ifndef __maybe_unused
#define __maybe_unused __attribute__((unused))
#endif

#elif defined(__clang__)

#ifndef __check_result
#define __check_result __attribute__((warn_unused_result))
#endif

#ifndef __deprecated
#define __deprecated __attribute__(deprecated))
#endif

#ifndef __deprecated_msg
#define __deprecated_msg(msg) __attribute__((deprecated(msg)))
#endif

#ifndef __maybe_unused
#define __maybe_unused __attribute__((unused))
#endif

#elif defined(_MSC_VER) && (_MSC_VER >= 1700)

#include <sal.h>

#ifndef __check_result
#define __check_result _Check_return_
#endif

#ifndef __deprecated
#define __deprecated __declspec(deprecated)
#endif

#ifndef __deprecated_msg
#define __deprecated_msg(msg) __declspec(deprecated(msg))
#endif

#ifndef __maybe_unused
#define __maybe_unused
#endif

#else

#ifndef __check_result
#define __check_result
#endif

#ifndef __deprecated
#define __deprecated
#endif

#ifndef __deprecated_msg
#define __deprecated_msg(msg)
#endif

#ifndef __maybe_unused
#define __maybe_unused
#endif

#endif

#endif  // OPENFSL_MACROS_H_
