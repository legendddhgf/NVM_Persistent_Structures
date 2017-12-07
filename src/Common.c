#include "Common.h"

void GenericPrint(FILE* out, Generic data) {
  if (data == NULL) {
    fprintf(out, "NULL");
  } else { // hardcoded routine that treats data as a number (regardless)
    fprintf(out, "%p", data);
  }
}
