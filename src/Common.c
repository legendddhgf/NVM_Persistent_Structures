#include "Common.h"

static PMEMobjpool *pop;

void commonInit() {
#ifdef PMEM
  pmemInit();
#endif
}

void commonDestroy() {
#ifdef PMEM
  pmemDestroy();
#endif
}

// initialized the persistent memory pool
void pmemInit() {
  /* From pmdk examples*/
  const char *path = PMEMFS_POOL_PATH;
  pop = NULL;
  if (!file_exists(path)) {
    pop = pmemobj_create(path, COMMON_LAYOUT, PMEMFS_POOL_SIZE,
            S_IWUSR | S_IRUSR);
    if (pop == NULL) {
      fprintf(stderr, "failed to create pool\n");
      exit(-1);
    }
  } else {
    pop = pmemobj_open(path, COMMON_LAYOUT);
    if (pop == NULL) {
      fprintf(stderr, "failed to open pool\n");
      exit(-1);
    }
  }
  /*end pmdk examples*/
}

// takes a SIZE and TYPE_NUM to allocate
// sets *POBJ to the direct object pointer
// example: pmemMalloc(&node, sizeof(SLLNode), SLLNode_TYPE)
// where node is SLLNode *
void pmemMalloc(void **POBJ, size_t SIZE, uintptr_t TYPE_NUM) { \
  PMEMoid OID;
  int ret = pmemobj_zalloc(pop, &OID, SIZE, TYPE_NUM);
  if (ret == -1) {
    perror("pmemobj_zalloc failed");
    exit(-1);
  }
  /*Set the object to point to the new memory*/
  *POBJ = pmemobj_direct(OID);
}

// frees data pointed to by *POBJ
// then sets *POBJ = NULL
// example: pmemFree(&node) where node is SLLNode *
void pmemFree(void **POBJ) {
  PMEMoid OID = pmemobj_oid(*POBJ);
  if (OID_IS_NULL(OID)) {
    fprintf(stderr, "Invalid object pointer provided to pmemFree\n");
    exit(-1);
  }
  pmemobj_free(&OID);
}

void pmemDestroy() {
  pmemobj_close(pop);
}

void pmemPersist(void *OBJ, uintptr_t LEN) {
  pmemobj_persist(pop, OBJ, LEN); \
}

// return 1 if file denoted by string path exists and 0 otherwise
uint8_t file_exists(const char *path) {
  if (path == NULL) {
    fprintf(stderr, "NULL file provided to file_exists()\n");
    exit(-1);
  }
  FILE *fp = fopen(path, "r");
  if (fp != NULL) {
    fclose(fp);
    return 1;
  }
  return 0;
}

// since Generic is using unsigned integers, hash function just returns their
// value
uintptr_t GenericHash(Generic data) {
  return (uintptr_t) data;
}

void GenericPrint(FILE* out, Generic data) {
  if (data == NULL) {
    fprintf(out, "NULL");
  } else { // hardcoded routine that treats data as a number (regardless)
    fprintf(out, "%lu", (uintptr_t) data);
  }
}
