#include "BTree.h"

typedef struct BTreeNode {
  Generic keys[BTREEKEYUPPERLIMIT];
  // TODO: should there be values for each key?
  // we could just make this the hash of the value
  struct BTreeNode *children[BTREECHILDUPPERLIMIT];
} BTNode;

// TODO: go find some paper that describes a K/V variation of a BTree
uint32_t BTreeHash(Generic Data) {
  return *((uint32_t *) Data); // TODO: just do it
}

BTree *BTreeInit(BTree *bt) {
  *bt = NULL;
  return bt;
}

BTree *BTreeInsert(BTree *bt, Generic Data, int32_t index) {
  if (index) {} // tree insertion based off key hash
  if (Data || bt) {} // TODO
  return NULL;
}
