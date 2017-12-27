#include "Common.h"

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
