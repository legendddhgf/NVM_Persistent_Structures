#include "BTree.h"

typedef struct BTreeNode {
  uint32_t *keys; // contain BTREEKEYUPPERLIMIT
  Generic *values; // ""
  struct BTreeNode **children; // contain BTREECHILDUPPERLIMIT
} BTNode;

// TODO: add the clflushes

// TODO: go find some paper that describes a K/V variation of a BTree like this
uint32_t BTreeHash(Generic data) {
  return (uintptr_t) data;  // TODO: can probably find a better hashing
                                // function
}

// initial call should be BTreeHeight(bt, 0)
// returns height of tree where 1 means only root exists
uint32_t BTreeHeight(BTree bt, uint32_t currentDepth) {
  BTNode *root = (BTNode *) bt;
  if (root == NULL) return currentDepth;
  return BTreeHeight(root->children[0], currentDepth + 1);
}

BTNode *newBTNode() {
  BTNode *n = malloc(sizeof(BTNode));
  n->keys = calloc(BTREEKEYUPPERLIMIT, sizeof(uint32_t));
  n->values = calloc(BTREEKEYUPPERLIMIT, sizeof(Generic));
  n->children = calloc(BTREECHILDUPPERLIMIT, sizeof(BTNode *));
  return n;
}

// it's ok to copy references because we will be replacing n anyway
BTNode *BTNodeCopy(BTNode *n) {
  if (n == NULL) {
    return NULL;
  }
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

void BTNodeDestroy(BTNode *n) {
  free(n->keys);
  free(n->values);
  free(n->children);
  free(n);
}

// provide address of BTree from separate instance
// initializes BTree to an empty one
// TODO: rethink init potentially here and at other structures
void BTreeInit(BTree *bt) {
  *bt = NULL;
}

// returns a copy of this BTree (note that keys and values will be value copies)
BTree BTreeCopy(BTree bt) {
  BTree cp = BTNodeCopy((BTNode *) bt);
  BTNode *copyRoot = cp;
  BTNode *root = bt;
  if (copyRoot == NULL) return NULL;
  for (int i = 0; i < BTREECHILDUPPERLIMIT; i++) {
    copyRoot->children[i] = (BTNode *) BTreeCopy((BTree) root->children[i]);
  }
  return cp;
}

// provide address of BTree from separate instance
// deletes all nodes in tree
void BTreeDestroy(BTree *bt) {
  BTNode *root = *bt;
  if (root == NULL) return;
  *bt = NULL; // point of persistence
  // the rest of this code frees the memory
  for (int i = 0; i < BTREECHILDUPPERLIMIT; i++) {
    BTreeDestroy((BTree *) &root->children[i]); // NULL calls handled above
  }
  BTNodeDestroy(root);
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
uint32_t BTNodeNextKeyLocation(BTNode *n, uint32_t key) {
  if (n == NULL) {
    fprintf(stderr, "Null BTNode reference passed to BTNodeNextKeyLocation\n");
    exit(-1);
  }
  // remember that we assume this node is full
  for (int i = 0; i < BTREEKEYUPPERLIMIT; i++) {
    if (n->values[i] == NULL || key < n->keys[i]) {
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
  uint32_t keyInsertLoc = 0;
  while (keyInsertLoc < BTREEKEYUPPERLIMIT - 1) { // there is a spot for sure
    if (n->values[keyInsertLoc] == NULL) break; // found next empty spot
    if (key < n->keys[keyInsertLoc]) break; // our key goes before this one
    keyInsertLoc++;
  }
  return keyInsertLoc;
}

// returns the index of the median value key
// an index of BTREEKEYLOWERLIMIT + 1 means key is median
uint32_t BTNodeMedianIndex(BTNode *n, uint32_t key) {
  if (key < n->keys[BTREEKEYLOWERLIMIT] &&
      key > n->keys[BTREEKEYLOWERLIMIT - 1]) {
    return BTREEKEYLOWERLIMIT + 1; // key is median
  } else if (key < n->keys[BTREEKEYLOWERLIMIT - 1]) {
    return BTREEKEYLOWERLIMIT - 1; // numbers shifted over to make this median
  }
  return BTREEKEYLOWERLIMIT; // this is median by default
}

// This function is basically a helper to BTNodeSplit
// This function should only be called if a split is necessary for insert
// determines and returns the highest node that will have to be split
BTNode *BTNodeHighestSplit(BTNode *root, uint32_t key) {
  BTNode *highestSplitNode = root;
  printf("test1\n");
  if (BTNodeIsLeaf(highestSplitNode)) {
    return highestSplitNode;
  }
  uint32_t iter = 0;
  for (; iter < BTREEKEYUPPERLIMIT; iter++) {
    if (root->values[iter] == NULL || key < root->keys[iter]) {
      break;
    }
  }
  // find highestSplitNode from only the appropriate child
  highestSplitNode = BTNodeHighestSplit(root->children[iter], key);
  // if need to split child and already have max children or max entries
  // then we also need to split this node
  if (highestSplitNode == root->children[iter] &&
      (root->children[BTREECHILDUPPERLIMIT - 1] != NULL ||
       root->values[BTREEKEYUPPERLIMIT - 1] != NULL)) {
    highestSplitNode = root;
  }
  return highestSplitNode;
}

// this is basically a helper function to BTNodeSplitHelper
// call node to split, and a k/v entry to insert
// recursively splits from bottom to top
// returns the node resulting from the split
BTNode *BTNodeSplitHelper(BTNode *child1, uint32_t insertKey,
    Generic insertValue) {
  BTNode *child2 = NULL;
  // only do a recursive call if this node isn't a leaf
  printf("test2\n");
  if (!BTNodeIsLeaf(child1)) {
    uint32_t iter = 0;
    for (; iter < BTREEKEYUPPERLIMIT; iter++) {
      if (insertKey < child1->keys[iter]) {
        break;
      }
    }
    child2 = BTNodeSplitHelper(child1->children[iter], insertKey,
        insertValue);
  }
  BTNode *split = newBTNode();
  // this is the k/v entry attempting to be stored at this node
  // Although it may be possible it is pushed to parent instead
  uint32_t thisKey = 0; // will be overwritten in following if-else
  Generic thisValue = NULL; // ""

  // if child1 isn't a leaf, a node will have been returned called child2
  // child2 is the split node of child1->children[iter] from above
  // we also ensure properties in various locations of child2
  // - keys[BTREEKEYLOWERLIMIT] : key to push to child1/split
  // - values[BTREEKEYLOWERLIMIT] : value to push to child1/split
  // - child2 : child to push to child1/split
  // note that the first two fields should be reset for child2 afterwards

  printf("test3\n");
  if (!BTNodeIsLeaf(child1)) {
    thisKey = child2->keys[BTREEKEYLOWERLIMIT];
    thisValue = child2->values[BTREEKEYLOWERLIMIT];
    child2->keys[BTREEKEYLOWERLIMIT] = 0; // reset fields
    child2->values[BTREEKEYLOWERLIMIT] = NULL; // ""
  }
  // in this case we use key and value passed from highest recursive call
  // since this is the intended placement of these parameters
  // all other levels intend to pass values to be pushed to parents
  else {
    thisKey = insertKey;
    thisValue = insertValue;
  }
  printf("Calling split helper with key: %u\n", thisKey);

  // get median index to push to parent
  uint32_t medianIndex = BTNodeMedianIndex(child1, thisKey);
  // find next location for entry pushed to parent / new child pointer
  // note that at worst case, last spot is available because preconditions

  // we are pushing median key entry to parent
  uint32_t parentKey = 0; // information to be sent to parent via split node
  Generic parentValue = NULL; // ""
  if (medianIndex == BTREEKEYLOWERLIMIT + 1) { // key is median
    parentKey = thisKey;
    parentValue = thisValue;
  } else { // key location within child1
    parentKey = child1->keys[medianIndex];
    parentValue = child1->values[medianIndex];

    // for child1: remove medianIndex and then figure out where to put key

    // first delete median index and shift everything over left to fill hole
    for (uint32_t i = medianIndex; i < BTREEKEYUPPERLIMIT - 1; i++) {
      child1->keys[i] = child1->keys[i + 1];
      child1->values[i] = child1->values[i + 1];
    }
    // need to manually clear so this is seen as empty by BTNodeAddLocation
    child1->values[BTREEKEYUPPERLIMIT - 1] = NULL;

    // then find location to insert key, shift things to make space, and insert
    uint32_t keyInsertLoc = BTNodeAddLocation(child1, thisKey);
    for (uint32_t i = BTREEKEYUPPERLIMIT - 2; i >= keyInsertLoc; i--) {
      child1->keys[i + 1] = child1->keys[i];
      child1->values[i + 1] = child1->values[i];
    }
    child1->keys[keyInsertLoc] = thisKey;
    child1->values[keyInsertLoc] = thisValue;
  }

  // first half in child1, second half in split
  for(int i = 0; i < BTREEKEYUPPERLIMIT; i++) {
    if (i < BTREEKEYLOWERLIMIT) { // fill values
      split->keys[i] = child1->keys[BTREEKEYLOWERLIMIT + i];
      split->values[i] = child1->values[BTREEKEYLOWERLIMIT + i];
      // BTREECHILDLOWERLIMIT nodes in child1, BTREECHILDLOWERLIMIT - 1 here
      // if !leaf, will get another child from another call to BTNodeSplitThis
      split->children[i] = child1->children[BTREECHILDLOWERLIMIT + i];
    } else { // pad 0's
      child1->values[i] = NULL;
      child1->children[1 + i] = NULL;
      split->values[i] = NULL;
      split->children[1 + i] = NULL;
    }
  }
  // finally, write values to split at BTREEKEYLOWERLIMIT as described above
  // these are pushed to parent / cleared from split in the return of this call
  split->keys[BTREEKEYLOWERLIMIT] = parentKey;
  split->values[BTREEKEYLOWERLIMIT] = parentValue;

  return split; // this is the split of child1
}

// This function is basically a helper to BTreeInsert
// This function should only be called if a split is neccessary for insert
// determines and performs split based off key
// also inserts key/value
// Note this all happens as a shadow update in nodes not pointed to by tree yet
// returns: new root
BTNode *BTNodeSplit(BTNode *root, uint32_t key, Generic value) {
  BTNode *newRoot = (BTNode *) BTreeCopy((BTree) root);
  // from this point on only use newRoot as part of a shadow update
  BTNode *highestSplitNode = BTNodeHighestSplit(newRoot, key);
  BTNode *leafSplitNode = newRoot; // the leaf needing to be split
  BTNode *highestSplitParent = NULL; // parent of highestSplitNode
  printf("test4\n");
  while (!BTNodeIsLeaf(leafSplitNode)) { // set targetSplitNode correctly
    uint32_t iter = 0;
    for (; iter < BTREEKEYUPPERLIMIT; iter++) {
      if (leafSplitNode->values[iter] == NULL || key < leafSplitNode->keys[iter]) {
        break;
      }
    }
    // set the parent of the highestSplitNode while iterating
    if (leafSplitNode->children[iter] == highestSplitNode) {
      highestSplitParent = leafSplitNode;
    }
    leafSplitNode = leafSplitNode->children[iter];
  }
  // if need to split root, create parent for it first
  if (highestSplitNode == newRoot) {
    highestSplitParent = newBTNode();
    highestSplitParent->children[0] = newRoot; // fulfill preconditions
    highestSplitNode = newRoot;
    // the root is the parent of the old root
    newRoot = highestSplitParent;
  }
  // split all the way down and get the split node of this node
  BTNode *split = BTNodeSplitHelper(highestSplitNode, key, value);

  // parent: find the location to insert the k/v entry and split as child
  uint32_t keyInsertLoc = BTNodeAddLocation(highestSplitParent, key);

  for (int32_t i = BTREEKEYUPPERLIMIT - 2; i >= (int32_t) keyInsertLoc; i--) {
    highestSplitParent->keys[i + 1] = highestSplitParent->keys[i];
    highestSplitParent->values[i + 1] = highestSplitParent->values[i];
  }
  highestSplitParent->keys[keyInsertLoc] = split->keys[BTREEKEYLOWERLIMIT];
  highestSplitParent->values[keyInsertLoc] = split->values[BTREEKEYLOWERLIMIT];
  // note highestSplitParent->children[keyInsertLoc] should be highestSplitNode
  highestSplitParent->children[keyInsertLoc + 1] = split;

  // don't forget to clear the values passed along from split
  split->keys[BTREEKEYLOWERLIMIT] = 0;
  split->values[BTREEKEYLOWERLIMIT] = NULL;

  // note the frees are handled where this function returns
  return newRoot; // return the newRoot the program will have
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
  printf("test5\n");
  while (!BTNodeIsLeaf(iter)) { // find insertion node
    printf("test6\n");
    parent = iter;
    childIndex = BTNodeNextKeyLocation(iter, key);
    iter = iter->children[childIndex]; // next child
    printf("get index of %d which is %s for child\n", childIndex, iter != NULL ?
        "Valid" : "Not Valid");
  }
  BTNode *newIter = NULL; // keeping track in case of later changing root
  // just insert it into the node
  if (!BTNodeIsFull(iter)) {
    uint32_t keyIndex = BTNodeAddLocation(iter, key); // get insert location
    newIter = BTNodeCopy(iter);

    // shift things right once from keyIndex to BTREEKEYUPPERLIMIT - 1
    for (int32_t i = BTREEKEYUPPERLIMIT - 2; i >= (int32_t) keyIndex; i--) {
      newIter->keys[i + 1] = newIter->keys[i];
      newIter->values[i + 1] = newIter->values[i];
    }
    newIter->keys[keyIndex] = key;
    newIter->values[keyIndex] = data;
    // note we don't have to shift children since this is a leaf
    if (parent != NULL) { // if it's the root, handled later
      parent->children[childIndex] = newIter;
    } else {
      *bt = newIter;
    }
    // a point of persistence
    free(iter); // it was replaced
    return;
  }
  // else, split current node into two: put median in parent and left/right in
  // respective split nodes
  // note that this has to be a leaf node that we are splitting
  *bt = BTNodeSplit(root, key, data); // root is now result of split's root
  // point of persistence
  BTreeDestroy((BTree *) &root); // replaced entire tree so free old one
}

// returns value for key or NULL if value not found
Generic BTreeGetElement(BTree *bt, int32_t key) {
  if (key < 0) {
    fprintf(stderr, "Invalid key provided to BTreeGetElement\n");
    exit(-1);
  }
  BTNode *root = *bt;
  if (root == NULL) {
    return NULL;
  }
  uint32_t iter = 0;
  for (iter = 0; iter < BTREEKEYUPPERLIMIT; iter++) {
    if (key < (int32_t) root->keys[iter]) {
      break;
    }
    if ((int32_t) root->keys[iter] == key) {
      return root->values[iter];
    }
  }
  return BTreeGetElement(&root->children[iter], key);
}

// call from BTreePrint and recursively does depth first traversal
// prints out in sorted order
void BTreePrintHelper(FILE* out, BTNode *root) {
  if (root == NULL) {
    return;
  }
  for (int i = 0; i < BTREECHILDUPPERLIMIT; i++) {
    BTreePrintHelper(out, root->children[i]);
  }
  for (int i = 0; i < BTREEKEYUPPERLIMIT; i++) {
    if (root->values[i] == NULL) break; // done printing keys in this node
    fprintf(out, " ");
    GenericPrint(out, root->values[i]);
  }
}

void BTreePrint(FILE *out, BTree *bt) {
  fprintf(out, "B-Tree contains:");
  BTreePrintHelper(out, (BTNode *) bt);
  fprintf(out, "\n");
}
