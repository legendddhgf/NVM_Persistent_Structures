#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <x86intrin.h>
#include <immintrin.h>

#ifdef VALGRIND // valgrind doesn't believe _mm_clflushopt is a real instruction
#define _mm_clflush(x) // will use when testing with VRAM
#define _mm_clwb(x)
#endif

// this is one way to get the value out of a macro
#define __MACROVALUE(MACRO) (MACRO)
#define _MACROVALUE(MACRO) __MACROVALUE(MACRO)
#define MACROVALUE(MACRO) _MACROVALUE(MACRO)

#ifndef INTEGERBITWIDTH
#define INTEGERBITWIDTH 32
#endif

// assumes 2's complement representation
#define MAXINT ((1 << (INTEGERBITWIDTH - 1)) - 1)
#define MININT (-1 * (1 << (INTEGERBITWIDTH - 1)))
#define MAXUINT ((1 << INTEGERBITWIDTH) - 1)
#define MINUINT (0)

#define TRUE 1
#define FALSE 0

typedef void * Generic;

uint32_t GenericHash(Generic data);
void GenericPrint(FILE* out, Generic data);

#endif
