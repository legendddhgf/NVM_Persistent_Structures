#ifndef SLL_H
#define SLL_H

#include "Common.h"

// Note the following:
//
// -can't have a previous/last pointer or size since they couldn't be updated in
// such a way that the list is guaranteed to be persistent
//
// -an empty list is simply when *(SLL *) == NULL
// note that (SLL *) should not be null

typedef struct SinglyLinkedListNode {
  struct SinglyLinkedListNode *next;
  Generic data;
} SLLNode;

typedef SLLNode * SLL; // we define a list to just be a pointer to a head node

void SLLInit(SLL *sll);

void SLLDestroy(SLL *sll);

void SLLInsert(SLL *sll, Generic data, int32_t index);

Generic SLLGetElement(SLL *sll, int32_t index);

uint32_t SLLGetSize(SLL *sll);

// actual memory size
uint32_t SLLGetMemSize(SLL *sll);

void SLLPrint(FILE *out, SLL *sll);

#endif
