/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
Check the full BSD-3-Clause license for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#ifndef OPENFSL_SYMBOLS_H_
#define OPENFSL_SYMBOLS_H_

#ifndef __cplusplus
#error C++ compiler required.
#endif

#define SHORT_EXIST TRUE
#define INT_EXIST TRUE
#define LONG_EXIST TRUE
#define LONG_LONG_EXIST TRUE
#define DOUBLE_EXIST TRUE
#define SIZE_T_EXIST TRUE
#define UINT8_T_EXIST TRUE
#define UINT16_T_EXIST TRUE
#define UINT32_T_EXIST TRUE
#define UINT64_T_EXIST TRUE
#define TIME_T_EXIST TRUE
#define __TIME64_T_EXIST FALSE
#define STRUCT_TM_EXIST TRUE
#define ERROR_T_EXIST FALSE

#define _LOCALTIME64_S_EXIST 0
#define LOCALTIME_R_EXIST 1

#endif  // OPENFSL_SYMBOLS_H_
