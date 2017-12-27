#ifndef BTREE_H
#define BTREE_H

#include "Common.h"

#define BTREEORDER 1
#define BTREECHILDLOWERLIMIT (BTREEORDER + 1)
#define BTREECHILDUPPERLIMIT (2 * BTREEORDER + 1)
#define BTREEKEYLOWERLIMIT (BTREEORDER)
#define BTREEKEYUPPERLIMIT (2 * BTREEORDER)

typedef struct BTreeNode * BTree;

void BTreeInit(BTree *bt);

void BTreeDestroy(BTree *bt);

void BTreeInsert(BTree *bt, Generic data, int32_t index);

Generic BTreeGetElement(BTree *bt, int32_t key);

void BTreePrint(FILE *out, BTree *bt);

#endif
