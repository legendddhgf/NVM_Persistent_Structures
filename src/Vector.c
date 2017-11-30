#include "Vector.h"

// returns maxcapacity of current vector, assumes invariant of NULL at end
// doesn't include last element of NULL in count
uint32_t vectorGetCapacity(Vector *v) {
  GenericArray arr = (GenericArray) *v;
  for (int32_t i = 0; ; i++) {
    if (arr[i] == NULL) return i; // end of arr
  }
  return ~0; // error, shouldn't happen
}

// frees old array and set vector to a new array of double the size
void vectorDoubleCapacity(Vector *v, uint32_t oldCap) {
  GenericArray oldArr = (GenericArray) *v;
  uint32_t newCap = oldCap * 2;
  GenericArray newArr = calloc(newCap + 1, sizeof(Generic)); //+1 for last item
  for (uint32_t i = 0; i < oldCap; i++) {
    newArr[i] = oldArr[i];
  }
  for (uint32_t i = oldCap; i < newCap; i++) {
    newArr[i] = (Generic) ~0; // unused indices;
  }
  newArr[newCap] = NULL;
  *v = (Vector) newArr; // one pointer move to persistently update the vector
  free(oldArr); // No promise that there are no memory leaks, only persistence
}

// declares a block of size INITVECTORCAP + 1 since the last index is NULL
Vector *vectorInit(Vector *v) {
  GenericArray arr = calloc(INITVECTORCAP + 1, sizeof(Generic));
  for (int32_t i = 0; i < INITVECTORCAP; i++) {
    arr[i] = (Generic) ~0; // empty fields
  }
  arr[INITVECTORCAP] = NULL; // mark all 1's at end of array
  *v = (Vector) arr; // initialized here
  return v;
}

// Inserts data into index (range 0 to n-1) of the vector
// This will increase the size of the vector as necessory to accomodeate index
void vectorInsert(Vector *v, Generic data, int32_t index) {
  if (data == NULL) {
    fprintf(stderr, "Cannot have NULL data in vectorInsert\n");
    exit(-1);
  }
  int32_t indexCount = 0;
  GenericArray arr = (GenericArray) *v;
  while (indexCount + 1 < index) { // checked 0 to index so cap is sufficient
    if (arr[indexCount] == NULL) {
      // at this point, indexCount == cap
      vectorDoubleCapacity(v, indexCount);
    } else {
      indexCount++;
    }
  }
  // At this point, we can guarantee the vector can hold the data
  arr[index] = data;
}

// TODO: error if index unused (still initialized to ~0)?
// FIXME: make this function
/* while not implemented to prevent compilation errors
Generic vectorGet(Vector *v, int32_t index) {
  return NULL;
}
*/

uint32_t vectorGetSize(Vector *v) {
  GenericArray arr = (GenericArray) *v;
  uint32_t sizeCount = 0;
  for (uint32_t i = 0; ; i++) {
    if (arr[i] == NULL) return sizeCount; // last item means we have seen all
    if (arr[i] != (Generic) ~0) sizeCount++; // only increment if has been set to not ~0
  }
  return ~0; // error, shouldn't happen
}
