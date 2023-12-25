#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "interpreter.h"
#include "parser.h"
#include "error.h"

struct Var* vars;
int varAmt = 0;

struct Var searchVarWErr(char* name) {
  for (int i=0;i<varAmt;i++) {
    if (strcmp(vars[i].name, name) == 0) return vars[i];
  }
  exitWErr("Trying to use undeclared variable!", 1);
}

void interpret(struct Expr* exprs) {
  vars = (struct Var*) malloc(0);
  int i = 0;
  while (exprs[i].kind != EXPR_EOF) {
    switch (exprs[i].kind) {
    case ASSIGN_VAR:
      varAmt++;
      vars = realloc(vars, sizeof(struct Var)*varAmt);
      vars[varAmt-1] = exprs[i].val.var;
      break;
    case FUN_CALL:
      char* funName = exprs[i].val.funCall.funName;
      if (strcmp(funName, "print") == 0) {
	if (exprs[i].val.funCall.argv[0].kind == LIT_INT ||
	    exprs[i].val.funCall.argv[0].kind == LIT_STR) {
	  printf("%s\n", exprs[i].val.funCall.argv[0].val.litVal);

	  free(exprs[i].val.funCall.argv[0].val.litVal);
	} else {
	  struct Var argv = searchVarWErr(exprs[i].val.funCall.argv[0].val.var.name);

	  printf("%s\n", argv.val);

	  free(exprs[i].val.funCall.argv[0].val.var.name);
	}
	free(exprs[i].val.funCall.argv);
	free(funName);
	break;
      }
    }

    i++;
  }

  for (int j=0;j<varAmt;j++) {
    free(vars[j].name);
    free(vars[j].val);
  }
  free(vars);
  free(exprs);
}
