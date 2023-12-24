#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "lexer.h"
#include "loc.h"
#include "error.h"
#include "readSrc.h"
#include "parser.h"

int main(int argc, char** argv) {
  if (argc < 2) exitWErr("No input file provided!", 1);
  
  char* source = readSource(argv[1]);
  char* orgSource = source;

  setSource(source);

  //struct Token token;
  //while ((token = nextToken()).kind) {
  //  printf("%10d:%-10d | %-14s | %s\n", token.loc.row, token.loc.col, token.kind, !(strcmp(token.val, "\n")==0)?token.val:"\\n");
  //  free(token.val);
  //}
  struct Expr* exprs = parse();
  //printf("%d\n", exprs[0].kind);
  //printf("%s\n", exprs[0].val.funCall.funName);
  //printf("%d\n", exprs[0].val.funCall.argc);
  printf("%s\n", exprs[0].val.funCall.argv[0].val.litVal);

  free(exprs[0].val.funCall.funName);
  free(exprs[0].val.funCall.argv[0].val.litVal);
  free(exprs[0].val.funCall.argv);
  free(exprs);
  free(orgSource);
  return 0;
}
