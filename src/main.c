#include <stdlib.h>

#include "lexer.h"
#include "error.h"
#include "readSrc.h"
#include "parser.h"
#include "interpreter.h"

int main(int argc, char** argv) {
  if (argc < 2) exitWErr("No input file provided!", 1);
  
  char* source = readSource(argv[1]);
  char* orgSource = source;

  setSource(source);
  interpret(parse());
  
  free(orgSource);
  return 0;
}
