#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

  /*struct Token token;
  while (!(token = nextToken()).isEnd) {
    printf("%10d:%-10d | %-14s | %s\n", token.loc.row, token.loc.col, token.kind, !(strcmp(token.val, "\n")==0)?token.val:"\\n");
    free(token.val);
    }*/

  interpret(parse());
  
  /*struct Expr* exprs = parse();

  int i = 0;
  while (exprs[i].kind != EXPR_EOF) {
    printf("%s\n", exprs[i].val.funCall.argv[0].val.litVal);

    free(exprs[i].val.funCall.funName);
    free(exprs[i].val.funCall.argv[0].val.litVal);
    free(exprs[i].val.funCall.argv);
    i++;
  }
  free(exprs);*/
  free(orgSource);
  return 0;
}
