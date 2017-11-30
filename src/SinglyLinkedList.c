#include "SinglyLinkedList.h"
//TODO: create ExtraUtitlities library and use it for better debug statements

#include <stdio.h>
#include <stdlib.h>

// creates a new node with data and the specified next node (can pass in NULL)
SLLNode *newSLLNode(Generic data, SLLNode *next) {
  SLLNode *node = malloc(sizeof(SLLNode));
  if (node == NULL) {
    fprintf(stderr, "Unable to allocate memory for a new SLLNode\n");
    exit(-1);
  }
  node->data = data;
  node->next = next;
  return node;
}

SLL *SLLInit(SLL *sll) {
  *sll = NULL; // initialized here
  return sll;
}

// provide the address of the SLL of the main program.
// insert into index: pushing previous data from >= index, up by one.
// index of 0 means insert at 0, while 1 to size - 1 means insert elsewhere.
//
// index of -1 means append (I am indeed aware that this is limited by the
// datatype of index)
void SLLInsert(SLL *sll, Generic data, int32_t index) {
  if (sll == NULL) {
    fprintf(stderr, "Null SLL reference passed to SLLInsert\n");
    exit(-1);
  }
  SLLNode *sllHead = *sll;
  // insert into empty list
  if (sllHead == NULL) {
    if (index != 0) { // only valid insertion index is 0
      fprintf(stderr, "Invalid index passed to SLLInsert\n");
      exit(-1);
    }
    *sll = newSLLNode(data, NULL); //overwrite head of original list
    return;
  }
  // insert into non-empty list
  SLLNode *iter = sllHead; // start at front
  int32_t indexCount = 0;
  while (1) { // broken from loop manually
    if (indexCount == index) break; // break when find insertion point
    indexCount++;
    if (iter->next == NULL) break; // will result in an append
    iter = iter->next;
  }
  // perform insertion
  //
  // TODO: mention that it's main program responsibility to track size
  // and ensure that it doesn't go past limits of related datatypes
  if (index == 0) { // head insertion
    *sll = newSLLNode(data, sllHead); //make next be old head/overwrite orig head
  } else if (indexCount == index || index == -1) {
    iter->next = newSLLNode(data, iter->next); // place directly after iter
  } else { // gave an index that was out of bounds
      fprintf(stderr, "Invalid index passed to SLLInsert\n");
      exit(-1);
  }
}

// returns the data in index (range from 0 to n - 1).
// index -1 is last item.
//
// program dies if item isn't found so return is undefined in this case
Generic SLLGetElement(SLL *sll, int32_t index) {
  if (sll == NULL) {
    fprintf(stderr, "Null SLL reference passed to SLLGet\n");
    exit(-1);
  }
  SLLNode *sllHead = *sll;
  SLLNode *iter = sllHead; // start at front
  int32_t indexCount = 0;
  while (iter != NULL) {
    if (indexCount == index) return iter->data; // found item
    indexCount++;
    iter = iter->next;
  }
  fprintf(stderr, "Invalid index passed to SLLGet\n");
  exit(-1);
}
