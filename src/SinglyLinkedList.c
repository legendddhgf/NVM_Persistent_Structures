#include "SinglyLinkedList.h"
//TODO: create ExtraUtitlities library and use it for better debug statements

#include <stdio.h>
#include <stdlib.h>

// creates a new node with data and the specified next node (can pass in NULL)
SLLNode *newSLL(int data, SLLNode *next) {
  SLLNode *node = malloc(sizeof(SLLNode));
  if (node == NULL) {
    fprintf(stderr, "Unable to allocate memory for a new SLLNode\n");
    exit(-1);
  }
  node->data = data;
  node->next = next;
  return node;
}

// provide the address of the (SLLNode *) of the main program for sll.
// insert into index: pushing previous data from >= index, up by one.
// index of 0 means insert at head, 1 to size - 1 means insert elsewhere.
//
// index of -1 means append (I am indeed aware that this is limited by the
// datatype of index)
void insert(SLLNode **sllHead, int data, int index) {
  if (sllHead == NULL) {
    fprintf(stderr, "Null SLL reference passed to SLLNode::insert\n");
    exit(-1);
  }
  // insert into empty list
  if (*sllHead == NULL) {
    if (index != 0) { // only valid insertion index is 0
      fprintf(stderr, "Invalid index passed to SLLNode::insert\n");
      exit(-1);
    }
    *sllHead = newSLL(data, NULL); //overwrite head of original list
    return;
  }
  // insert into non-empty list
  SLLNode *iter = *sllHead; // start at front
  int indexCount = 0;
  while (1) { // broken from loop manually
    if (indexCount == index) break; // break when find insertion point
    indexCount++;
    if (iter->next == NULL) break; // will result in an append
    iter = iter->next;
  }
  // perform insertion
  if (index == 0) { // head insertion
    *sllHead = newSLL(data, *sllHead); //overwrite original head/ make next be old head
  } else if (indexCount == index || index == -1) {
    iter->next = newSLL(data, iter->next);
  } else { // gave an index that was out of bounds
      fprintf(stderr, "Invalid index passed to SLLNode::insert\n");
      exit(-1);
  }
}

// returns the data in index (range from 0 to n - 1).
// index -1 is last item.
//
// program dies if item isn't found so return is undefined in this case
int get(SLLNode **sllHead, int index) {
  if (sllHead == NULL) {
    fprintf(stderr, "Null SLL reference passed to SLLNode::get\n");
    exit(-1);
  }
  SLLNode *iter = *sllHead; // start at front
  int indexCount = 0;
  while (iter != NULL) {
    if (indexCount == index) return iter->data; // found item
    indexCount++;
    iter = iter->next;
  }
  fprintf(stderr, "Invalid index passed to SLLNode::get\n");
  exit(-1);
}
