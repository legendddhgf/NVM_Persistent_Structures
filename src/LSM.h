#ifndef LSM_H
#define LSM_H

#include "Common.h"

typedef uint64_t keyType;
typedef Generic valType;

typedef struct LSMNode {
  keyType key;
  valType val;
} LSMNode;


typedef struct LSM {
  size_t block_size; // This is the number of nodes each block can hold.
  int k; // The LSM tree grows in dimension k.
  int node_size;
  size_t next_empty;
  LSMNode *block;
  char* disk1;
  bool sorted;
} LSM;

typedef struct LSMInternalNode{
  LSMNode *node;
  size_t index;
} LSMINode;

#ifndef LSM_BLOCK_SIZE /* pass through for Makefile */
#define LSM_BLOCK_SIZE 256 /* 256 bytes */
#endif

#ifndef LSM_SORTED
#define LSM_SORTED FALSE
#endif

#ifndef LSM_DISK_PATH
#define LSM_DISK_PATH "/tmp/lsm_storage.txt"
#endif

/* wrappers created to maintain my standard naming conventions */

void LSMInit(LSM **plsm);

void LSMInsert(LSM **plsm, Generic data, int64_t index);

Generic LSMGetElement(LSM **plsm, int64_t key);

void LSMDestroy(LSM **plsm);

void LSMPrint(FILE *out, LSM **plsm);

uintptr_t LSMGetMemSize(LSM **plsm);

/* end my wrappers */

void check_file_ret(FILE* f, int r);

LSM* init_new_lsm(void);

void destruct_lsm(LSM* tree);

void merge(LSMNode *whole, LSMNode *left, int left_size, LSMNode *rght, int right_size);

void merge_sort(LSMNode *block, int n);

LSMINode* search_buffer(const keyType* key, LSM* tree);

LSMINode* search_disk(const keyType* key, LSM* tree);

LSMNode* get(const keyType key, LSM* tree);

int write_to_disk(LSM* tree);

int put(const keyType* key, const valType* val, LSM* tree);

int delete(const keyType* key, LSM* tree);

int update(const keyType* key, const valType* val, LSM* tree);

void print_buffer_data(FILE *out, LSM* tree);

void print_disk_data(FILE *out, LSM* tree);

#endif
