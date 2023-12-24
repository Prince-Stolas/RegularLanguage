#include <stdlib.h>
#include <stdio.h>

void exitWErr(char* msg, int rtnCode) {
  fprintf(stderr, "ERROR: %s\n", msg); exit(rtnCode);
}

void exitWErrRC(int row, int col, char* msg, int rtnCode) {
  fprintf(stderr, "%d:%d | ERROR: %s\n", row, col, msg); exit(rtnCode);
}

