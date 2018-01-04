// This is a wrapper that allows the calling of C code from C++

//#include <pilot/libpilot.h>
#include <stdio.h>
#include <stdlib.h>
#include "Bench.h"

#define MAXRUNNERTIME_us (60)
#define MAXRUNNERTIME_s (MAXRUNNERTIME_us * 1e6)

/*
using namespace pilot;

void run_test(int (*func)(void)) {
  simple_runner(func);
}
*/

int main (int argc, char **argv) {
  benchInit(argc, argv);
  benchRun(argc, argv);
  benchDestroy(argc, argv);
  return 0;
}
