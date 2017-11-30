#ifndef VECTOR_H
#define VECTOR_H

#include "Common.h"

#define INITVECTORCAP 2

// The end of GenericArray is marked with a block with value ~0 (all 1 bitstr)

typedef Generic * GenericArray;

// the contiguous block of data constituting the vector
typedef GenericArray * Vector;

Vector *vectorInit(Vector *v);

void vectorInsert(Vector *v, Generic data, int32_t index);

Generic vectorGet(Vector *v, int32_t index);

uint32_t VectorGetSize(Vector *v);

// actual memory size
uint32_t VectorGetMemSize(Vector *v);

#endif
