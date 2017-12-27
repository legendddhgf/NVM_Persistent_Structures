#include <sys/time.h>

#include "SLL.h"
#include "Vector.h"
#include "BTree.h"

#define NUMTESTS (160000)

#define LISTHEADINSERT (0)
#define LISTTAILINSERT (-1)

int main(int argc, char **argv) {
  fprintf(stdout, "You're attempting to run %s with %d arguments, nice...\n",
      argv[0], argc);

  fprintf(stdout, "Testing SLL:\n");
  SLL sll;
  SLLInit(&sll);
  uint64_t TimeAcc = 0;
  struct timeval start, end;
  // insert index + 1 at index for NUMTESTS different items
  for (uintptr_t i = NUMTESTS; i >= 1; i--) {
    gettimeofday(&start, NULL);
    SLLInsert(&sll, (Generic) i, LISTHEADINSERT);
    gettimeofday(&end, NULL);
    TimeAcc += (end.tv_sec * 1e6 + end.tv_usec) -
      (start.tv_sec * 1e6 + start.tv_usec);
  }

  printf("\tTime for %d SLLInsert calls is %lu us\n",
      NUMTESTS, TimeAcc);

  TimeAcc = 0;
  // check that above insertion is valid but in reverse order
  for (uintptr_t i = NUMTESTS; i >= 1; i--) {
    gettimeofday(&start, NULL);
    Generic data = SLLGetElement(&sll, i - 1);
    gettimeofday(&end, NULL);
    if (data != (Generic) i) {
      fprintf(stderr, "List failed GetElement at index = %lu\nTracing:\n",
          i - 1);
      SLLPrint(stderr, &sll);
      exit(-1);
    }
    TimeAcc += (end.tv_sec * 1e6 + end.tv_usec) -
      (start.tv_sec * 1e6 + start.tv_usec);
  }

  printf("\tTime for %d SLLGetElement calls is %lu us\n",
      NUMTESTS, TimeAcc);

  SLLDestroy(&sll);
  fprintf(stdout, "\tBasic SLL tests PASSED\n");


  fprintf(stdout, "Testing Vector:\n");

  Vector v;
  VectorInit(&v);

  TimeAcc = 0;
  for (uintptr_t i = 1; i <= NUMTESTS; i++) {
    gettimeofday(&start, NULL);
    VectorInsert(&v, (Generic) i, 2 * i - 1);
    gettimeofday(&end, NULL);
    TimeAcc += (end.tv_sec * 1e6 + end.tv_usec) -
      (start.tv_sec * 1e6 + start.tv_usec);
  }

  printf("\tTime for %d VectorInsert calls is %lu us\n",
      NUMTESTS, TimeAcc);

  TimeAcc = 0;
  for (uintptr_t i = NUMTESTS; i >= 1; i--) {
    gettimeofday(&start, NULL);
    Generic data = VectorGetElement(&v, 2 * i - 1);
    gettimeofday(&end, NULL);
    if (data != (Generic) i) {
      fprintf(stderr, "Vector failed GetElement at index = %lu\nTracing:\n",
          2 * i - 1);
      VectorPrint(stderr, &v);
      exit(-1);
    }
    TimeAcc += (end.tv_sec * 1e6 + end.tv_usec) -
      (start.tv_sec * 1e6 + start.tv_usec);
  }

  printf("\tTime for %d VectorGetElement calls is %lu us\n",
      NUMTESTS, TimeAcc);

  VectorDestroy(&v);
  fprintf(stdout, "\tBasic Vector tests PASSED\n");

  fprintf(stdout, "Testing HTable:\n");

  fprintf(stdout, "Testing BTree:\n");
  BTree bt;
  BTreeInit(&bt);

  TimeAcc = 0;
  for (uintptr_t i = 1; i <= NUMTESTS/4; i++) {
    // index doesn't matter
    // this is basically having mixed ranges of inserts instead of only seq.
    gettimeofday(&start, NULL);
    BTreeInsert(&bt, (Generic) (5 * i), 0);
    BTreeInsert(&bt, (Generic) (5 * (i + NUMTESTS / 2)), 0);
    BTreeInsert(&bt, (Generic) (5 * (i + NUMTESTS / 4)), 0);
    BTreeInsert(&bt, (Generic) (5 * (i + 3 * NUMTESTS / 4)), 0);
    gettimeofday(&end, NULL);
    TimeAcc += (end.tv_sec * 1e6 + end.tv_usec) -
      (start.tv_sec * 1e6 + start.tv_usec);
  }

  printf("\tTime for %d BTreeInsert calls is %lu us\n",
      NUMTESTS, TimeAcc);

  TimeAcc = 0;
  for (uintptr_t i = NUMTESTS; i >= 1; i--) {
    gettimeofday(&start, NULL);
    Generic data = BTreeGetElement(&bt, 5 * i);
    gettimeofday(&end, NULL);
    if (data != (Generic) (5 * i)) {
      fprintf(stderr, "BTree failed GetElement at index = %lu\nTracing:\n",
          5 * i);
      BTreePrint(stderr, &bt);
      exit(-1);
    }
    TimeAcc += (end.tv_sec * 1e6 + end.tv_usec) -
      (start.tv_sec * 1e6 + start.tv_usec);
  }

  printf("\tTime for %d BTreeGetElement calls is %lu us\n",
      NUMTESTS, TimeAcc);

  BTreeDestroy(&bt);
  fprintf(stdout, "\tBasic BTree tests PASSED\n");

  return 0;
}
