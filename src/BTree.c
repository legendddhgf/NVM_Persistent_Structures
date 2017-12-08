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

// it's ok to copy references because we will be replacing n anyway
BTNode *BTNodeCopy(BTNode *n) {
  if (n == NULL) return NULL;
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
void BTreeInit(BTree *pbt) {
  *pbt = NULL;
}

// returns a copy of this BTree (note that keys and values will be value copies)
BTree BTreeCopy(BTree bt) {
  BTree cp = BTNodeCopy((BTNode *) bt);
  BTNode *copyRoot = cp;
  BTNode *root = bt;
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
  int keyInsertLoc = 0;
  while (keyInsertLoc < BTREEKEYUPPERLIMIT - 1) { // there is a spot for sure
    if (n->values[keyInsertLoc] == NULL) break; // found next empty spot
    if (key < n->keys[keyInsertLoc]) break; // our key goes before this one
    keyInsertLoc++;
  }
  return keyInsertLoc;
}

// returns the index of the median value key
// an index of BTREEKEYLOWERLIMIT + 1 means key is median
uint32_t BTNodeMedianValue(BTNode *n, uint32_t key) {
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
  if (BTNodeIsLeaf(highestSplitNode)) {
    return highestSplitNode;
  }
  uint32_t iter = 0;
  for (; iter < BTREEKEYUPPERLIMIT; iter++) {
    if (key < root->keys[iter]) {
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

// This function is basically a helper to BTNodeSplit
// This function should be called when parent is guaranteed to be able to
// accomodate one additional node and entry (target and the node it splits into)
// The function splits target (including its keys and values)
// It assumes target is already a child of parent and adds the split node as a
// taget as well.
// Doesn't return anything since parent remains intact
void BTNodeSplitThis(BTNode *parent, BTNode *target, uint32_t key,
    Generic value) {
  BTNode *split = BTNodeCopy(target);
  uint32_t medianIndex = BTNodeMedianValue(target, key);
  uint32_t iter = 0;
  // find next location for entry pushed to parent / new child pointer
  // note that at worst case, last spot is available because preconditions
  for (iter = 0; iter < BTREEKEYUPPERLIMIT - 1; iter++) {
    if (parent->values[iter] == NULL) break; // found k/v entry location
  }
  if (medianIndex == BTREEKEYLOWERLIMIT + 1) { // key is median
    parent->keys[iter] = key;
    parent->values[iter] = value;
  } else { // actual index of median
    parent->keys[iter] = target->keys[medianIndex];
    parent->values[iter] = target->values[medianIndex];
  }
  // append split as a child pointer to parent
  parent->children[iter + 1] = split; // parent->children[iter] already target

  // TODO: ensure appropriate entries and children are in target and split
  //uint32_t keyIndex = BTNodeAddLocation(target, key);
  /* TODO: sleep then look at this again
  for (iter = 0; iter < BTREEKEYUPPERLIMIT; iter++) {
    // TODO
  }
  */
}

// TODO: so close!
// This function is basically a helper to BTreeInsert
// This function should only be called if a split is neccessary for insert
// determines and performs split based off key
// also inserts key/value
// Note this all happens as a shadow update in nodes not pointed to by tree yet
// returns: new root
BTNode *BTNodeSplit(BTNode *root, uint32_t key, Generic value) {
  BTNode *newRoot = (BTNode *) BTreeCopy((BTree) root);
  // from this point on only use newRoot as a shadow update
  BTNode *highestSplitNode = BTNodeHighestSplit(newRoot, key);
  BTNode *targetSplitNode = newRoot; // the leaf needing to be split
  BTNode *highestSplitParent = NULL; // parent of highestSplitNode
  while (!BTNodeIsLeaf(targetSplitNode)) { // set targetSplitNode correctly
    uint32_t iter = 0;
    for (; iter < BTREEKEYUPPERLIMIT; iter++) {
      if (key < targetSplitNode->keys[iter]) {
        break;
      }
    }
    // set the parent of the highestSplitNode while iterating anyway
    if (targetSplitNode->children[iter] == highestSplitNode) {
      highestSplitParent = targetSplitNode;
    }
    targetSplitNode = targetSplitNode->children[iter];
  }
  // splitting all the way till root is even more fun
  if (highestSplitParent == NULL) {
    highestSplitParent = newBTNode();
    highestSplitParent->children[0] = newRoot; // fulfill BTNodeSplitThis precon
    BTNodeSplitThis(highestSplitParent, newRoot, key, value);
    highestSplitParent = newRoot;
  }
  // at this point can assume root no longer needs to be split
  //
  // TODO: please split all nodes from highestSplitParent to targetSplitNode
  // should basically be a loop of calling BTNodeSplitThis() which handles
  // everything assuming parent at any given time doesn't need to be split

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
  while (!BTNodeIsLeaf(iter)) { // find insertion node
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

void BTreePrint(FILE *out, BTree *bt) {
  fprintf(out, "%s\n", bt == NULL ? "NULL" : "NAH"); // TODO
}
