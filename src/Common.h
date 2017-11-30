#ifndef COMMON_H
#define COMMON_H

#include "stdint.h"
#include <stdio.h>
#include <stdlib.h>

#define _MACROVALUE(MACRO) MACRO
#define MACROVALUE(MACRO) _MACROVALUE(MACRO)

// assumes 2's complement representation
#define MAXINT ((1 << (INTEGERBITWIDTH - 1)) - 1)
#define MININT (-1 * (1 << (INTEGERBITWIDTH - 1)))
#define MAXUINT ((1 << INTEGERBITWIDTH) - 1)
#define MINUINT (0)

typedef void * Generic;

#endif
