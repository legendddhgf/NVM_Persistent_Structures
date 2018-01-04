#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <x86intrin.h>
#include <immintrin.h>
#include <libpmemobj.h>

// this is one way to get the value out of a macro
#define __MACROVALUE(MACRO) (MACRO)
#define _MACROVALUE(MACRO) __MACROVALUE(MACRO)
#define MACROVALUE(MACRO) _MACROVALUE(MACRO)

#ifndef INTEGERBITWIDTH
#define INTEGERBITWIDTH 32
#endif

// assumes 2's complement representation
#define MAXINT ((1 << (INTEGERBITWIDTH - 1)) - 1)
#define MININT (-1 * (1 << (INTEGERBITWIDTH - 1)))
#define MAXUINT ((1 << INTEGERBITWIDTH) - 1)
#define MINUINT (0)

#define TRUE 1
#define FALSE 0

#define PMEMFS_POOL_PATH "/mnt/pmemdir/pmempool"
#define PMEMFS_POOL_SIZE 1e8 /* 100 MB */
#define COMMON_LAYOUT "COMMON_LAYOUT"

enum TYPE_NUMS {
  Generic_TYPE, SLLNode_TYPE, GenericArray_TYPE, LSM_TYPE, LSMNode_TYPE,
  LSMINode_TYPE, BTNode_TYPE, PBTNode_TYPE, uintptr_t_TYPE, NUM_TYPES,
};

void commonInit(void);
void commonDestroy(void);

void pmemInit(void);
void pmemMalloc(void **POBJ, size_t SIZE, uintptr_t TYPE_NUM);
void pmemFree(void **POBJ);
void pmemDestroy(void);
void pmemPersist(void *OBJ, uintptr_t LEN);

uint8_t file_exists(const char *path);

// chooses whether to use standard malloc or persistent based malloc
// example: pmemMalloc(&node, sizeof(SLLNode), SLLNode_TYPE)
// where node is SLLNode *
#ifdef PMEM
#define commonMalloc(POBJ, SIZE, TYPE_NUM) { \
  pmemMalloc((void **) POBJ, SIZE, TYPE_NUM); \
}
#else
#define commonMalloc(POBJ, SIZE, TYPE_NUM) { \
  /*because SIZE is what you would already pass to malloc but want zeroed*/\
  *POBJ = calloc(1, SIZE); \
}
#endif

// chooses whether to use standard free or persistent based free
// example: pmemFree(&node)
#ifdef PMEM
#define commonFree(POBJ) { \
  pmemFree((void **) POBJ); \
}
#else
#define commonFree(POBJ) { \
  free(*POBJ); \
  *POBJ = NULL; \
}
#endif

// chooses whether to persist with clflush and msync or pmemobj_persist
// example: commonPersist(node, sizeof(SLLNode)) where node is SLLNode *
#ifdef PMEM
#define commonPersist(OBJ, LEN) { \
  pmemPersist((void *) OBJ, LEN); \
}
#else
#define commonPersist(OBJ, LEN) { \
  _mm_clflush((void *) OBJ); \
}
#endif



typedef void * Generic;

uintptr_t GenericHash(Generic data);
void GenericPrint(FILE* out, Generic data);

#endif
