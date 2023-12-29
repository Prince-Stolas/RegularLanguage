#include <stdlib.h>
#include <stdio.h>

#include "readSrc.h"
#include "error.h"

char* readSource(char* path) {
  FILE* sourceFile = fopen(path, "r");
  if (!sourceFile) exitWErr("Could not read file!", 1);

  fseek(sourceFile, 0, SEEK_END);
  int fileSize = ftell(sourceFile);
  char* buffer = (char*) malloc(fileSize+1);
  rewind(sourceFile);

  char c;
  for (int i=0;i<fileSize;i++) {
    c = fgetc(sourceFile);
    buffer[i] = c;
  }

  buffer[fileSize] = '\0';

  fclose(sourceFile);
  return buffer;
}
