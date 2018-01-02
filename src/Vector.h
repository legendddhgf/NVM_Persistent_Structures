#ifndef VECTOR_H
#define VECTOR_H

#include "Common.h"

#define VECTORINITCAP (2)
#define VECTORINITVAL ((Generic) ~0)


typedef Generic * GenericArray;

// the contiguous block of data constituting the vector
typedef GenericArray * Vector;

void VectorInit(Vector *v);

void VectorDestroy(Vector *v);

void VectorInsert(Vector *v, Generic data, int64_t index);

Generic VectorGetElement(Vector *v, int64_t index);

uintptr_t VectorGetSize(Vector *v);

// actual memory size
uintptr_t VectorGetMemSize(Vector *v);

void VectorPrint(FILE *out, Vector *v);

#endif
