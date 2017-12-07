#ifndef VECTOR_H
#define VECTOR_H

#include "Common.h"

#define VECTORINITCAP (2)
// The end of GenericArray is marked with a NULL
#define VECTORENDMARK ((Generic) NULL)
#define VECTORINITVAL ((Generic) ~0)


typedef Generic * GenericArray;

// the contiguous block of data constituting the vector
typedef GenericArray * Vector;

Vector *VectorInit(Vector *v);

void VectorInsert(Vector *v, Generic data, int32_t index);

Generic VectorGetElement(Vector *v, int32_t index);

uint32_t VectorGetSize(Vector *v);

// actual memory size
uint32_t VectorGetMemSize(Vector *v);

void VectorPrint(FILE *out, Vector *v);

#endif
