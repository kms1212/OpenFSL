#ifndef __VINT_H
#define __VINT_H

#include <stdint.h>

#if defined(__x86_64__) || defined(__amd64__) || defined(_M_X64) || defined(_M_AMD64)
typedef uint64_t vint_arch;
#elif defined(i386) || defined(__i386__) || defined(_M_IX86)
typedef uint32_t vint_arch;
#endif

#endif