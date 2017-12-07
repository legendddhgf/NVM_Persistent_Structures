#ifndef BTREE_H
#define BTREE_H

#include "Common.h"

#define BTREEORDER 1
#define BTREECHILDLOWERLIMIT (BTREEORDER + 1)
#define BTREECHILDUPPERLIMIT (2 * BTREEORDER + 1)
#define BTREEKEYLOWERLIMIT (BTREEORDER)
#define BTREEKEYUPPERLIMIT (2 * BTREEORDER)

typedef struct BTreeNode *BTree;

BTree *newBTree(BTree *bt);

#endif
