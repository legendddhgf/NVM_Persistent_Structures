#include "Vector.h"

// returns maxcapacity of current vector, assumes invariant of NULL at end
// doesn't include last element of NULL in count
uint32_t VectorGetCapacity(Vector *v) {
  if (v == NULL) {
    fprintf(stderr, "Passed NULL vector reference to vectorGetCapacity\n");
    exit(-1);
  }
  GenericArray arr = (GenericArray) *v;
  for (int32_t i = 0; ; i++) {
    if (arr[i] == NULL) return i; // end of arr
  }
  return ~0; // error, shouldn't happen
}

// frees old array and set vector to a new array of double the size
void VectorDoubleCapacity(Vector *v, uint32_t oldCap) {
  if (v == NULL) {
    fprintf(stderr, "Passed NULL vector reference to vectorDoubleCapacity\n");
    exit(-1);
  }
  GenericArray oldArr = (GenericArray) *v;
  uint32_t newCap = oldCap * 2;
  GenericArray newArr = calloc(newCap + 1, sizeof(Generic)); //+1 for last item
  for (uint32_t i = 0; i < oldCap; i++) {
    newArr[i] = oldArr[i];
  }
  for (uint32_t i = oldCap; i < newCap; i++) {
    newArr[i] = VECTORINITVAL; // unused indices;
  }
  newArr[newCap] = VECTORENDMARK;
  _mm_clflush(newArr);
  *v = (Vector) newArr; // one pointer move to persistently update the vector
  _mm_clflush(v);
  free(oldArr); // No promise that there are no memory leaks, only persistence
}

// declares a block of size VECTORINITCAP + 1 since the last index is NULL
void VectorInit(Vector *v) {
  if (v == NULL) {
    fprintf(stderr, "Passed NULL vector reference to vectorInit\n");
    exit(-1);
  }
  GenericArray arr = calloc(VECTORINITCAP + 1, sizeof(Generic));
  for (int32_t i = 0; i < VECTORINITCAP; i++) {
    arr[i] = VECTORINITVAL; // empty fields
  }
  arr[VECTORINITCAP] = VECTORENDMARK; // mark all 1's at end of array
  _mm_clflush(arr);
  *v = (Vector) arr; // point of persistence
  _mm_clflush(v);
}

void VectorDestroy(Vector *v) {
  if (v == NULL || *v == NULL) {
    return;
  }
  free(*v);
  *v = NULL;
}

// Inserts data into index (range 0 to n-1) of the vector
// This will increase the size of the vector as necessory to accomodeate index
void VectorInsert(Vector *v, Generic data, int32_t index) {
  if (v == NULL) {
    fprintf(stderr, "Passed NULL vector reference to vectorInsert\n");
    exit(-1);
  }
  if (data == NULL) {
    fprintf(stderr, "Cannot have NULL data in vectorInsert\n");
    exit(-1);
  }
  int32_t indexCount = 0;
  GenericArray arr = (GenericArray) *v;
  while (indexCount - 1 < index) { // checked 0 to index, thus cap is sufficient
    if (arr[indexCount] == VECTORENDMARK) {
      // at this point, indexCount == cap
      VectorDoubleCapacity(v, indexCount);
      arr = (GenericArray) *v;
    } else {
      indexCount++;
    }
  }
  // At this point, we can guarantee the vector can hold the data
  arr[index] = data;
  _mm_clflush(arr + index); // only need to flush from this index onwards
}

// TODO: error if index >= cap? Currently return NULL in this case
// Returns the item if location at index has been set and NULL otherwise
Generic VectorGetElement(Vector *v, int32_t index) {
  if (v == NULL) {
    fprintf(stderr, "Passed NULL vector reference to vectorGetElement\n");
    exit(-1);
  }
  GenericArray arr = (GenericArray) *v;
  // Note that there technically isn't a sense of out of bounds since any index
  // (not just those adjacent to already existing ones) can be written / read
  if (index < 0 || index >= (int32_t) VectorGetCapacity(v) ||
      arr[index] == VECTORINITVAL) {
    return NULL;
  }
  return arr[index];
}

uint32_t VectorGetSize(Vector *v) {
  if (v == NULL) {
    fprintf(stderr, "Passed NULL vector reference to vectorGetSize\n");
    exit(-1);
  }
  GenericArray arr = (GenericArray) *v;
  uint32_t sizeCount = 0;
  for (uint32_t i = 0; ; i++) {
    if (arr[i] == VECTORENDMARK) return sizeCount; // last item means done
    if (arr[i] != VECTORINITVAL) sizeCount++; // only if not VECTORINITVAL
  }
  return ~0; // error, shouldn't happen
}

void VectorPrint(FILE *out, Vector *v) {
  if (v == NULL) {
    fprintf(stderr, "Passed NULL vector reference to VectorPrint\n");
    exit(-1);
  }
  GenericArray arr = (GenericArray) *v;
  fprintf(out, "Vector Contains: ");
  for (uint32_t iter = 0; arr[iter] != VECTORENDMARK; iter++) {
    fprintf(out, " ");
    GenericPrint(out, arr[iter]);
  }
  fprintf(out, "\n");
}
