#include "SLL.h"
#include "Vector.h"
#include "BTree.h"

#define TESTRANGE (400)

#define LISTHEADINSERT (0)
#define LISTTAILINSERT (-1)

int main(int argc, char **argv) {
  fprintf(stdout, "You're attempting to run %s with %d arguments, nice...\n",
      argv[0], argc);

  fprintf(stdout, "Testing SLL:\n");
  SLL sll;
  SLLInit(&sll);
  // unfortunately pointer types don't like having 0 as a value
  for (uintptr_t i = 1; i <= TESTRANGE; i++) {
    SLLInsert(&sll, (Generic) i, LISTTAILINSERT);
  }

  for (uintptr_t i = TESTRANGE; i >= 1; i--) {
    if (SLLGetElement(&sll, i - 1) != (Generic) i) {
      fprintf(stderr, "List failed GetElement at index = %lu\nTracing:\n",
          i - 1);
      SLLPrint(stderr, &sll);
      exit(-1);
    }
  }

  SLLDestroy(&sll);
  fprintf(stdout, "\tBasic SLL tests PASSED\n");


  fprintf(stdout, "Testing Vector:\n");

  Vector v;
  VectorInit(&v);

  for (uintptr_t i = 1; i <= TESTRANGE; i++) {
    VectorInsert(&v, (Generic) i, 2 * i - 1);
  }
  for (uintptr_t i = TESTRANGE; i >= 1; i--) {
    if (VectorGetElement(&v, 2 * i - 1) != (Generic) i) {
      fprintf(stderr, "Vector failed GetElement at index = %lu\nTracing:\n",
          2 * i - 1);
      VectorPrint(stderr, &v);
      exit(-1);
    }
  }

  VectorDestroy(&v);
  fprintf(stdout, "\tBasic Vector tests PASSED\n");

  fprintf(stdout, "Testing HTable:\n");

  fprintf(stdout, "Testing BTree:\n");
  BTree bt;
  BTreeInit(&bt);

  for (uintptr_t i = 1; i <= TESTRANGE/4; i++) {
    // index doesn't matter
    // this is basically having mixed ranges of inserts instead of only seq.
    BTreeInsert(&bt, (Generic) (5 * i), 0);
    BTreeInsert(&bt, (Generic) (5 * (i + TESTRANGE / 2)), 0);
    BTreeInsert(&bt, (Generic) (5 * (i + TESTRANGE / 4)), 0);
    BTreeInsert(&bt, (Generic) (5 * (i + 3 * TESTRANGE / 4)), 0);
  }
  for (uintptr_t i = TESTRANGE; i >= 1; i--) {
    if (BTreeGetElement(&bt, 5 * i) != (Generic) (5 * i)) {
      fprintf(stderr, "BTree failed GetElement at index = %lu\nTracing:\n",
          5 * i);
      BTreePrint(stderr, &bt);
      exit(-1);
    }
  }

  BTreeDestroy(&bt);
  fprintf(stdout, "\tBasic BTree tests PASSED\n");

  return 0;
}
