#include "Common.h"

/* TODO
static PMEMobjpool *pop;

void pmemInit() {

  const char *path = "/mnt/pmemdir";

  pop = NULL;

  if (file_exists(path) != 0) {
    if ((pop = pmemobj_create(path, POBJ_LAYOUT_NAME(array),
            PMEMOBJ_MIN_POOL, CREATE_MODE_RW)) == NULL) {
      printf("failed to create pool\n");
      return 1;
    }
  } else {
    if ((pop = pmemobj_open(path, POBJ_LAYOUT_NAME(array)))
        == NULL) {
      printf("failed to open pool\n");
      return 1;
    }
  }
}

void *pmemMalloc() {

}

void pmemFree() {

}
*/

// since Generic is using unsigned integers, hash function just returns their
// value
uint32_t GenericHash(Generic data) {
  return (uintptr_t) data;
}

void GenericPrint(FILE* out, Generic data) {
  if (data == NULL) {
    fprintf(out, "NULL");
  } else { // hardcoded routine that treats data as a number (regardless)
    fprintf(out, "%lu", (uintptr_t) data);
  }
}
