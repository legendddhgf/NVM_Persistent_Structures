#include "Vector.h"

// returns maxcapacity of current vector, assumes invariant of NULL at end
// doesn't include last element of NULL in count
uintptr_t VectorGetCapacity(Vector *v) {
  if (v == NULL) {
    fprintf(stderr, "Passed NULL vector reference to vectorGetCapacity\n");
    exit(-1);
  }
  GenericArray arr = (GenericArray) *v;
  return (uintptr_t) arr[0]; // capacity written into first index
}

// frees old array and set vector to a new array of double the size
void VectorDoubleCapacity(Vector *v, uintptr_t oldCap) {
  if (v == NULL) {
    fprintf(stderr, "Passed NULL vector reference to vectorDoubleCapacity\n");
    exit(-1);
  }
  GenericArray oldArr = (GenericArray) *v;
  uintptr_t newCap = oldCap * 2;
  GenericArray newArr = NULL;
  commonMalloc(&newArr, (newCap + 1) * sizeof(Generic),
      Generic_TYPE); //+1 for last item
  newArr[0] = (Generic) newCap; // write cap to first index
  for (uintptr_t i = 1; i <= oldCap; i++) {
    newArr[i] = oldArr[i];
  }
  for (uintptr_t i = oldCap + 1; i <= newCap; i++) {
    newArr[i] = VECTORINITVAL; // unused indices;
  }
  commonPersist(newArr, (newCap + 1) * sizeof(Generic));
  *v = (Vector) newArr; // one pointer move to persistently update the vector
  commonFree(&oldArr);
}

// declares a block of size VECTORINITCAP + 1 since the last index is NULL
void VectorInit(Vector *v) {
  if (v == NULL) {
    fprintf(stderr, "Passed NULL vector reference to vectorInit\n");
    exit(-1);
  }
  GenericArray arr = NULL;
  commonMalloc(&arr, (VECTORINITCAP + 1) * sizeof(Generic), Generic_TYPE);
  arr[0] = (Generic) VECTORINITCAP; // write cap to first index
  for (int32_t i = 1; i <= VECTORINITCAP; i++) {
    arr[i] = VECTORINITVAL; // empty fields
  }
  // persist end marker as well
  commonPersist(arr, sizeof(Generic) * (VECTORINITCAP + 1));
  *v = (Vector) arr; // point of persistence
}

void VectorDestroy(Vector *v) {
  if (v == NULL || *v == NULL) {
    return;
  }
  commonFree(v); // &*v
  *v = NULL;
}

// Inserts data into index (range 0 to n-1) of the vector
// This will increase the size of the vector as necessory to accomodeate index
void VectorInsert(Vector *v, Generic data, int64_t index) {
  if (v == NULL) {
    fprintf(stderr, "Passed NULL vector reference to vectorInsert\n");
    exit(-1);
  }
  if (data == NULL) {
    fprintf(stderr, "Cannot have NULL data in vectorInsert\n");
    exit(-1);
  }
  GenericArray arr = (GenericArray) *v;
  uintptr_t curCap = VectorGetCapacity(v);
  // remember that index = 0 means index 1 in the array
  while ((uintptr_t) index + 1 > curCap) { // double until can fit
      VectorDoubleCapacity(v, curCap);
      arr = (GenericArray) *v;
      curCap = VectorGetCapacity(v);
  }
  // At this point, we can guarantee the vector can hold the data
  arr[index + 1] = data;
  // flush only this index
  commonPersist(&arr[index + 1], sizeof(Generic));
}

// return NULL if element requested is unset or out of current cap range
// Returns the item if location at index has been set and NULL otherwise
Generic VectorGetElement(Vector *v, int64_t index) {
  if (v == NULL) {
    fprintf(stderr, "Passed NULL vector reference to vectorGetElement\n");
    exit(-1);
  }
  GenericArray arr = (GenericArray) *v;
  // Note that there technically isn't a sense of out of bounds since any index
  // (not just those adjacent to already existing ones) can be written / read
  if (index < 0 || (uintptr_t) index > VectorGetCapacity(v)) {
    return NULL;
  }
  // if unset, return NULL instead
  return arr[index + 1] == VECTORINITVAL ? NULL : arr[index + 1];
}

// returns number of items set in this vector
uintptr_t VectorGetSize(Vector *v) {
  if (v == NULL) {
    fprintf(stderr, "Passed NULL vector reference to vectorGetSize\n");
    exit(-1);
  }
  GenericArray arr = (GenericArray) *v;
  uintptr_t sizeCount = 0;
  uintptr_t cap = VectorGetCapacity(v);
  for (uintptr_t i = 1; i <= cap; i++) {
    if (arr[i] != VECTORINITVAL) sizeCount++; // only if not VECTORINITVAL
  }
  return sizeCount;
}

uintptr_t VectorGetMemSize(Vector *v) {
  if (v == NULL) {
    fprintf(stderr, "Passed NULL vector reference to VectorGetMemSize\n");
    exit(-1);
  }
  // capacity plus 1 (index holding the capacity) times size of each index
  GenericArray arr = (GenericArray) *v;
  return ((uintptr_t) arr[0] + 1) * sizeof(Generic);
}

void VectorPrint(FILE *out, Vector *v) {
  if (v == NULL) {
    fprintf(stderr, "Passed NULL vector reference to VectorPrint\n");
    exit(-1);
  }
  GenericArray arr = (GenericArray) *v;
  fprintf(out, "Vector Contains: ");
  uintptr_t cap = VectorGetCapacity(v);
  for (uintptr_t iter = 1; iter <= cap; iter++) {
    fprintf(out, " ");
    GenericPrint(out, arr[iter]);
  }
  fprintf(out, "\n");
}
