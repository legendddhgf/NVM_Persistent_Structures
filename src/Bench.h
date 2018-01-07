#ifndef BENCH_H
#define BENCH_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef NUMTRIALS
#define NUMTRIALS (10000) /* number of times to repeat test for numbers */
#endif

#ifndef TESTRANGE
#define TESTRANGE (40) /* range of operation in tests*/
#endif

#define LISTHEADINSERT (0)
#define LISTTAILINSERT (-1)

// performs any necessary initializations and checks the correctness of the data
// structures
int benchInit(int argc, char **argv);

int benchRun(int argc, char **argv);

int benchDestroy(int argc, char **argv);

#ifdef __cplusplus
}
#endif

#endif
