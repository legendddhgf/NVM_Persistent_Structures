#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <x86intrin.h>
#include <immintrin.h>

#ifdef VALGRIND // valgrind doesn't believe clflushopt is a real instruction
#define _mm_clflushopt(x)
#endif

#define _MACROVALUE(MACRO) MACRO
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

void GenericPrint(FILE* out, Generic data);

#endif
