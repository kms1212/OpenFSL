/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
Check the full BSD-3-Clause license for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#ifndef OPENFSL_OPTIONS_H_
#define OPENFSL_OPTIONS_H_

#ifndef __cplusplus
#error C++ compiler required.
#endif

// #define OPENFSL_FBD_DISABLED
// #define OPENFSL_STDLIB_DISABLED

#if defined(OPENFSL_FBD_DISABLED) && !defined(OPENFSL_STDLIB_DISABLED)
#error Cannot disable FileBlockDevice without disabling standard library.
#endif

#endif  // OPENFSL_OPTIONS_H_
