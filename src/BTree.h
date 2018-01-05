#ifndef BTREE_H
#define BTREE_H

#include "Common.h"

#ifndef BTREEORDER /* pass through for makefile */
#define BTREEORDER (1)
#endif

#define BTREECHILDLOWERLIMIT (BTREEORDER + 1)
#define BTREECHILDUPPERLIMIT (2 * BTREEORDER + 1)
#define BTREEKEYLOWERLIMIT (BTREEORDER)
#define BTREEKEYUPPERLIMIT (2 * BTREEORDER)

typedef struct BTreeNode * BTree;

void BTreeInit(BTree *bt);

void BTreeDestroy(BTree *bt);

void BTreeInsert(BTree *bt, Generic data, int64_t index);

Generic BTreeGetElement(BTree *bt, int64_t key);

uintptr_t BTreeGetMemSize(BTree *bt);

void BTreePrint(FILE *out, BTree *bt);

#endif
