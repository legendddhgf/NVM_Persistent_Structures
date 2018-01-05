#include <sys/time.h>

#include "Bench.h"

#include "Common.h"
#include "SLL.h"
#include "Vector.h"
#include "LSM.h"
#include "BTree.h"

FILE *resFile;
uint64_t TimeAcc;
struct timeval start, end;

int SLLTestBasic() {

  fprintf(stdout, "Testing SLL:\n");
#ifdef PMEM
  resFile = fopen("PMEMSLLBasicResults.csv", "w");
#else
  resFile = fopen("SLLBasicResults.csv", "w");
#endif
  fprintf(resFile, "insert() runtime [ms],get() runtime [ms],"
      "memory usage [kB],\n");
  uint64_t trial_count = 0;
  while (trial_count++ < NUMTRIALS) {
    SLL sll;
    SLLInit(&sll);
    TimeAcc = 0;
    for (uintptr_t i = TESTRANGE; i >= 1; i--) {
      gettimeofday(&start, NULL);
      SLLInsert(&sll, (Generic) i, LISTHEADINSERT);
      gettimeofday(&end, NULL);
      TimeAcc += (end.tv_sec * 1e6 + end.tv_usec) -
        (start.tv_sec * 1e6 + start.tv_usec);
    }

    printf("\tTrial %lu: Time for %d SLLInsert calls is %lu us\n",
        trial_count, TESTRANGE, TimeAcc);
    fprintf(resFile, "%.3lf,", ((double) TimeAcc) / 1e3);

    TimeAcc = 0;
    for (uintptr_t i = TESTRANGE; i >= 1; i--) {
      gettimeofday(&start, NULL);
      Generic data = SLLGetElement(&sll, i - 1);
      gettimeofday(&end, NULL);
      if (data != (Generic) i) {
        fprintf(stderr, "List failed GetElement at index = %lu\nTracing:\n",
            i - 1);
        SLLPrint(stderr, &sll);
        return 1;
      }
      TimeAcc += (end.tv_sec * 1e6 + end.tv_usec) -
        (start.tv_sec * 1e6 + start.tv_usec);
    }

    printf("\tTrial %lu: Time for %d SLLGetElement calls is %lu us\n",
        trial_count, TESTRANGE, TimeAcc);
    fprintf(resFile, "%.3lf,", ((double) TimeAcc) / 1e3);

    // memory reading
    fprintf(resFile, "%.3lf,", ((double) SLLGetMemSize(&sll)) / 1e3);

    SLLDestroy(&sll);
    fprintf(resFile, "\n"); // end of this trial result
  }

  fprintf(stdout, "\tBasic SLL tests PASSED\n");

  fclose(resFile);

  return 0;
}

int VectorTestBasic() {

  fprintf(stdout, "Testing Vector:\n");
#ifdef PMEM
  resFile = fopen("PMEMVectorBasicResults.csv", "w");
#else
  resFile = fopen("VectorBasicResults.csv", "w");
#endif
  fprintf(resFile, "insert() runtime [ms],get() runtime [ms],"
      "memory usage [kB],\n");

  uint64_t trial_count = 0;
  while (trial_count++ < NUMTRIALS) {
    Vector v;
    VectorInit(&v);

    TimeAcc = 0;
    for (uintptr_t i = 1; i <= TESTRANGE; i++) {
      gettimeofday(&start, NULL);
      VectorInsert(&v, (Generic) i, i);
      gettimeofday(&end, NULL);
      TimeAcc += (end.tv_sec * 1e6 + end.tv_usec) -
        (start.tv_sec * 1e6 + start.tv_usec);
    }

    printf("\tTrial %lu: Time for %d VectorInsert calls is %lu us\n",
        trial_count, TESTRANGE, TimeAcc);
    fprintf(resFile, "%.3lf,", ((double) TimeAcc) / 1e3);

    TimeAcc = 0;
    for (uintptr_t i = 1; i <= TESTRANGE; i++) {
      gettimeofday(&start, NULL);
      Generic data = VectorGetElement(&v, i);
      gettimeofday(&end, NULL);
      if (data != (Generic) i) {
        fprintf(stderr, "Vector failed GetElement at index = %lu\nTracing:\n",
            i);
        VectorPrint(stderr, &v);
        return 1;
      }
      TimeAcc += (end.tv_sec * 1e6 + end.tv_usec) -
        (start.tv_sec * 1e6 + start.tv_usec);
    }

    printf("\tTrial %lu: Time for %d VectorGetElement calls is %lu us\n",
        trial_count, TESTRANGE, TimeAcc);
    fprintf(resFile, "%.3lf,", ((double) TimeAcc) / 1e3);

    // memory reading
    fprintf(resFile, "%.3lf,", ((double) VectorGetMemSize(&v)) / 1e3);

    VectorDestroy(&v);
    fprintf(resFile, "\n"); // end of this trial result
  }
  fprintf(stdout, "\tBasic Vector tests PASSED\n");

  fclose(resFile);

  return 0;
}

int LSMTestBasic() {

  fprintf(stdout, "Testing LSM:\n");
#ifdef PMEM
  resFile = fopen("PMEMLSMBasicResults.csv", "w");
#else
  resFile = fopen("LSMBasicResults.csv", "w");
#endif
  fprintf(resFile, "insert() runtime [ms],get() runtime [ms],"
      "memory usage [kB],\n");

  uint64_t trial_count = 0;
  while (trial_count++ < NUMTRIALS) {
    LSM *lsm;
    LSMInit(&lsm);

    TimeAcc = 0;
    for (uintptr_t i = 1; i <= TESTRANGE; i++) {
      gettimeofday(&start, NULL);
      LSMInsert(&lsm, (Generic) i, 0);
      gettimeofday(&end, NULL);
      TimeAcc += (end.tv_sec * 1e6 + end.tv_usec) -
        (start.tv_sec * 1e6 + start.tv_usec);
    }

    printf("\tTrial %lu: Time for %d LSMInsert calls is %lu us\n",
        trial_count, TESTRANGE, TimeAcc);
    fprintf(resFile, "%.3lf,", ((double) TimeAcc) / 1e3);

    TimeAcc = 0;
    for (uintptr_t i = 1; i <= TESTRANGE; i++) {
      gettimeofday(&start, NULL);
      Generic data = LSMGetElement(&lsm, i);
      gettimeofday(&end, NULL);
      if (data != (Generic) i) {
        fprintf(stderr, "LSM failed GetElement at index = %lu\nTracing:\n",
            i);
        LSMPrint(stderr, &lsm);
        exit(-1);
      }
      TimeAcc += (end.tv_sec * 1e6 + end.tv_usec) -
        (start.tv_sec * 1e6 + start.tv_usec);
    }

    printf("\tTrial %lu: Time for %d LSMGetElement calls is %lu us\n",
        trial_count, TESTRANGE, TimeAcc);
    fprintf(resFile, "%.3lf,", ((double) TimeAcc) / 1e3);

    // memory reading
    fprintf(resFile, "%.3lf,", ((double) LSMGetMemSize(&lsm)) / 1e3);

    LSMDestroy(&lsm);
    fprintf(resFile, "\n"); // end of this trial result
  }
  fprintf(stdout, "\tBasic LSM tests PASSED\n");

  fclose(resFile);
  return 0;
}

int BTreeTestBasic() {

  fprintf(stdout, "Testing BTree:\n");
#ifdef PMEM
  resFile = fopen("PMEMBTreeBasicResults.csv", "w");
#else
  resFile = fopen("BTreeBasicResults.csv", "w");
#endif
  fprintf(resFile, "insert() runtime [ms],get() runtime [ms],"
      "memory usage [kB],\n");

  uint64_t trial_count = 0;
  while (trial_count++ < NUMTRIALS) {
    BTree bt;
    BTreeInit(&bt);

    TimeAcc = 0;
    for (uintptr_t i = 1; i <= TESTRANGE; i++) {
      // index doesn't matter
      // this is basically having mixed ranges of inserts instead of only seq.
      gettimeofday(&start, NULL);
      BTreeInsert(&bt, (Generic) i, 0);
      gettimeofday(&end, NULL);
      TimeAcc += (end.tv_sec * 1e6 + end.tv_usec) -
        (start.tv_sec * 1e6 + start.tv_usec);
    }
    printf("\tTrial %lu: Time for %d BTreeInsert calls is %lu us\n",
        trial_count, TESTRANGE, TimeAcc);
    fprintf(resFile, "%.3lf,", ((double) TimeAcc) / 1e3);


    TimeAcc = 0;
    for (uintptr_t i = 1; i <= TESTRANGE; i++) {
      gettimeofday(&start, NULL);
      Generic data = BTreeGetElement(&bt, i);
      gettimeofday(&end, NULL);
      if (data != (Generic) (i)) {
        fprintf(stderr, "BTree failed GetElement at index = %lu\nTracing:\n",
            i);
        BTreePrint(stderr, &bt);
        exit(-1);
      }
      TimeAcc += (end.tv_sec * 1e6 + end.tv_usec) -
        (start.tv_sec * 1e6 + start.tv_usec);
    }

    printf("\tTrial %lu: Time for %d BTreeGetElement calls is %lu us\n",
        trial_count, TESTRANGE, TimeAcc);
    fprintf(resFile, "%.3lf,", ((double) TimeAcc) / 1e3);

    // memory reading
    fprintf(resFile, "%.3lf,", ((double) BTreeGetMemSize(&bt)) / 1e3);

    BTreeDestroy(&bt);
    fprintf(resFile, "\n"); // end of this trial result

  }
  fprintf(stdout, "\tBasic BTree tests PASSED\n");

  fclose(resFile);

  return 0;
}

int benchInit(int argc, char **argv) {
  fprintf(stdout, "You're attempting to run %s with %d arguments, nice...\n",
      argv[0], argc);

  commonInit();
  return 0;
}

int benchRun(int argc, char **argv) {
  if (argc || argv) {} // quiet compiler

  if (SLLTestBasic()) return 1;

  if (VectorTestBasic()) return 1;

  if (LSMTestBasic()) return 1;

  if (BTreeTestBasic()) return 1;

  return 0;
}

int benchDestroy(int argc, char **argv) {
  if (argc || argv) {} // quiet compiler
  commonDestroy();

  return 0;
}
