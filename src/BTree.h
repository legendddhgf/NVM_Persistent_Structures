#ifndef BTREE_H
#define BTREE_H

#include "Common.h"

#define BTREECHILDLOWERLIMIT 2
#define BTREECHILDUPPERLIMIT 3
#define BTREEKEYLOWERLIMIT (BTREECHILDLOWERLIMIT - 1)
#define BTREEKEYUPPERLIMIT (BTREECHILDUPPERLIMIT - 1)

typedef struct BTreeNode *BTree;

BTree *newBTree(BTree *bt);

#endif
