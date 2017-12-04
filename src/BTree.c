#include "BTree.h"

typedef struct BTreeNode {
  Generic keys[BTREEKEYUPPERLIMIT];
  struct BTreeNode *children[BTREECHILDUPPERLIMIT];
} BTNode;
