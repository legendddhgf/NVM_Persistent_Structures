#include "SLL.h"

#include <stdio.h>
#include <stdlib.h>

// creates a new node with data and the specified next node (can pass in NULL)
SLLNode *newSLLNode(Generic data, SLLNode *next) {
  SLLNode *node = NULL;
  commonMalloc(&node, sizeof(SLLNode), SLLNode_TYPE);
  if (node == NULL) {
    fprintf(stderr, "Unable to allocate memory for a new SLLNode\n");
    exit(-1);
  }
  node->data = data;
  node->next = next;
  return node;
}

void SLLInit(SLL *sll) {
  *sll = NULL; // initialized here
}

void SLLDestroy(SLL *sll) {
  if (sll == NULL || *sll == NULL) {
    return;
  }
  SLLNode *root = *sll;
  *sll = NULL;
  SLLDestroy(&root->next);
  commonFree(&root);
}

// provide the address of the SLL of the main program.
// insert into index: pushing previous data from >= index, up by one.
// index of 0 means insert at 0, while 1 to size - 1 means insert elsewhere.
//
// index of -1 means append (I am indeed aware that this is limited by the
// datatype of index)
void SLLInsert(SLL *sll, Generic data, int64_t index) {
  if (sll == NULL) {
    fprintf(stderr, "Null SLL reference passed to SLLInsert\n");
    exit(-1);
  }
  SLLNode *sllHead = *sll;
  // insert into empty list
  if (sllHead == NULL) {
    if (index != 0 && index != -1) { // only valid insertion index is 0,-1
      fprintf(stderr, "Invalid index passed to SLLInsert\n");
      exit(-1);
    }
    sllHead = newSLLNode(data, NULL); //overwrite head of original list
    commonPersist(sllHead, sizeof(SLLNode));
    *sll = sllHead;
    return;
  }
  // insert into non-empty list
  SLLNode *iter = sllHead; // start at front
  int64_t indexCount = 0;
  while (1) { // broken from loop manually
    if (indexCount == index) break; // break when find insertion point
    indexCount++;
    if (iter->next == NULL) break; // will result in an append
    iter = iter->next;
  }

  // perform insertion here: everything up till this point is a shadow update
  if (index == 0) { // head insertion
    sllHead = newSLLNode(data, sllHead); //make next be old head/overwrite orig head
    commonPersist(sllHead, sizeof(SLLNode));
    *sll = sllHead;
  } else if (indexCount == index || index == -1) {
    SLLNode *sllNode = newSLLNode(data, iter->next); // place directly after iter
    commonPersist(sllNode, sizeof(SLLNode));
    iter->next = sllNode;
    commonPersist(iter, sizeof(SLLNode)); // point of persistence
  } else { // gave an index that was out of bounds
      fprintf(stderr, "Invalid index passed to SLLInsert\n");
      exit(-1);
  }
}

// returns the data in index (range from 0 to n - 1).
// index -1 is last item.
//
// program dies if item isn't found so return is undefined in this case
Generic SLLGetElement(SLL *sll, int64_t index) {
  if (sll == NULL) {
    fprintf(stderr, "Null SLL reference passed to SLLGet\n");
    exit(-1);
  }
  SLLNode *sllHead = *sll;
  SLLNode *iter = sllHead; // start at front
  int64_t indexCount = 0;
  while (iter != NULL) {
    if (indexCount == index) return iter->data; // found item
    indexCount++;
    if (iter->next == NULL && index == -1) return iter->data;
    iter = iter->next;
  }
  fprintf(stderr, "Invalid index passed to SLLGet\n");
  exit(-1);
}

uintptr_t SLLGetMemSize(SLL *sll) {
  if (sll == NULL) {
    fprintf(stderr, "Null SLL reference passed to SLLGetMemSize\n");
    exit(-1);
  }
  uintptr_t memSize = 0;
  // size of all nodes
  for (SLLNode *iter = *sll; iter != NULL; iter = iter->next) {
    memSize += sizeof(SLLNode);
  }
  return memSize;
}

void SLLPrint(FILE *out, SLL *sll) {
  if (sll == NULL) {
    fprintf(stderr, "Null SLL reference passed to SLLPrint\n");
    exit(-1);
  }
  fprintf(out, "List Contains:");
  for (SLLNode *iter = *sll; iter != NULL; iter = iter->next) {
    fprintf(out, " ");
    GenericPrint(out, iter->data);
  }
  fprintf(out, "\n");
}
