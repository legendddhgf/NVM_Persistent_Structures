#include "BTree.h"

typedef struct BTreeNode {
  uint32_t *keys; // contain BTREEKEYUPPERLIMIT
  Generic *values; // ""
  struct BTreeNode **children; // contain BTREECHILDUPPERLIMIT
} BTNode;

// TODO: go find some paper that describes a K/V variation of a BTree like this
uint32_t BTreeHash(Generic data) {
  return *((uint32_t *) data);  // TODO: can probably find a better hashing
                                // function
}

BTNode *newBTNode() {
  BTNode *n = malloc(sizeof(BTNode));
  n->keys = calloc(BTREEKEYUPPERLIMIT, sizeof(uint32_t));
  n->values = calloc(BTREEKEYUPPERLIMIT, sizeof(Generic));
  n->children = calloc(BTREECHILDUPPERLIMIT, sizeof(BTNode *));
  return n;
}

BTree *BTreeInit(BTree *bt) {
  *bt = NULL;
  return bt;
}

// returns whether n is a leaf
uint32_t BTNodeIsLeaf(BTNode *n) {
  if (n == NULL) {
    fprintf(stderr, "Null BTNode reference passed to BTNodeIsLeaf\n");
    exit(-1);
  }
  // leaf if has no children (first child pointer is still NULL)
  return n->children[0] == NULL ? TRUE : FALSE;
}

// returns whether n is full
uint32_t BTNodeIsFull(BTNode *n) {
  if (n == NULL) {
    fprintf(stderr, "Null BTNode reference passed to BTNodeIsFull\n");
    exit(-1);
  }
  // full if last value in Node is filled (not NULL)
  return n->values[BTREEKEYUPPERLIMIT - 1] != NULL ? TRUE : FALSE;
}

// return the index of the child node to attempt to place key into
// This expects n to be full, otherwise you wouldn't bother inserting in a child
uint32_t BTNodeNextKeyLocation(BTNode *n, uint32_t key) {
  if (n == NULL) {
    fprintf(stderr, "Null BTNode reference passed to BTNodeNextKeyLocation\n");
    exit(-1);
  }
  if (!BTNodeIsFull(n)) {
    fprintf(stderr, "Non-Full BTNode passed to BTNodeNextKeyLocation\n");
    exit(-1);
  }
  // remember that we assume this node is full
  for (int i = 0; i < BTREEKEYUPPERLIMIT; i++) {
    if (key < n->keys[i]) {
      return i; // child node 0 to BTREECHILDUPPERLIMIT - 2
    }
  }
  return BTREEKEYUPPERLIMIT; // the last child node (BTREECHILDUPPERLIMIT - 1)
}

// returns location to add an entry to a non-full node
uint32_t BTNodeAddLocation(BTNode *n, uint32_t key) {
  if (n == NULL) {
    fprintf(stderr, "Null BTNode reference passed to BTNodeAddEntry\n");
    exit(-1);
  }
  if (BTNodeIsFull(n)) {
    fprintf(stderr, "Full BTNode passed to BTNodeAddEntry\n");
    exit(-1);
  }
  int keyInsertLoc = 0;
  while (keyInsertLoc < BTREEKEYUPPERLIMIT - 1) { // there is a spot for sure
    if (n->values[keyInsertLoc] == NULL) break; // found next empty spot
    if (key < n->keys[keyInsertLoc]) break; // our key goes before this one
    keyInsertLoc++;
  }
  return keyInsertLoc;
}

// it's ok to copy references because we will be replacing n anyway
BTNode *BTNodeCopy(BTNode *n) {
  BTNode *btn = newBTNode();
  for (uint32_t i = 0; i < BTREECHILDUPPERLIMIT; i++) {
    btn->children[i] = n->children[i];
  }
  for (uint32_t i = 0; i < BTREEKEYUPPERLIMIT; i++) {
    btn->keys[i] = n->keys[i];
    btn->values[i] = n->values[i];
  }
  return btn;
}

// TODO;
// returns the highest node to split based off key and prespecified lowest split
// targer node
BTNode *BTNodeToSplit(BTNode *root, BTNode *split, uint32_t key) {
  if (1) return split; // TODO:
  BTNode *iter = BTNodeToSplit(root, split, key); // TODO: this was an idea that needs sleep
  return iter; // TODO
}

// Performs insertion of data into bt by creating a key by hashing data and
// inserting data as a k/v entry. Index exists as a parameter to support the
// insert functions implemented in other data structures in this project.
void BTreeInsert(BTree *bt, Generic data, int32_t index) {
  if (index) {} // tree insertion based off key hash
  if (bt == NULL) {
    fprintf(stderr, "Null BTree reference passed to BTreeInsert\n");
    exit(-1);
  }
  BTNode *root = *bt;
  BTNode *n = newBTNode();
  uint32_t key = BTreeHash(data);
  if (root == NULL) { // tree is empty
    n->keys[0] = key; // to the first location
    n->values[0] = data; // ""
    *bt = n; // the point of persistent change to the BTree
    return;
  }
  BTNode *iter = root;
  BTNode *parent = NULL;
  uint32_t childIndex = 0; // index of iter relative to parent
  while (!BTNodeIsLeaf(iter) && BTNodeIsFull(iter)) { // find insertion node
    parent = iter;
    childIndex = BTNodeNextKeyLocation(iter, key);
    iter = iter->children[childIndex]; // next child
  }
  BTNode *newIter = NULL; // keeping track in case of later changing root
  // just insert it into the node
  if (!BTNodeIsFull(iter)) {
    uint32_t keyIndex = BTNodeAddLocation(iter, key); // get insert location
    newIter = BTNodeCopy(iter);

    // shift things right once from keyIndex to BTREEKEYUPPERLIMIT - 1
    for (uint32_t i = BTREEKEYUPPERLIMIT - 2; i >= keyIndex; i--) {
      newIter->keys[i + 1] = newIter->keys[i];
      newIter->values[i + 1] = newIter->values[i];
    }
    newIter->keys[keyIndex] = key;
    newIter->values[keyIndex] = data;
    if (parent != NULL) { // if it's the root, handled later
      parent->children[childIndex] = newIter; // a point of persistence
    }
  }
  // else, must split current node into this and BTREECHILDLOWERLIMIT children
  // then fill current node and children accordingly to maintain ordering
  else { // TODO:
    //BTNode *splitNode = BTNodeToSplit(root, iter, key);
    newIter = BTNodeCopy(iter);
  }
  free(iter); // it was replaced
  if (parent == NULL) { // this is the new root
    *bt = newIter;
  }
  // point of persistence
}

void BTreePrint(FILE *out, BTree *bt) {
  fprintf(out, "%s\n", bt == NULL ? "NULL" : "NAH"); // TODO
}
