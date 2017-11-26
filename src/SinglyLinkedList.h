#ifndef SINGLYLINKEDLIST_H
#define SINGLYLINKEDLIST_H

// Note the following:
//
// -can't have a previous/last pointer or size since they couldn't be updated in
// such a way that the list is guaranteed to be valid
//
// -a NULL SLLNode is an empty list

typedef struct SinglyLinkedListNode {
  struct SinglyLinkedListNode *next;
  int data;
} SLLNode;

void insert(SLLNode **sllHead, int data, int index);

int get(SLLNode **sllHead, int index);

#endif
