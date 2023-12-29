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

int searchVarPos(char* name) {
  for (int i=0;i<varAmt;i++) {
    if (strcmp(vars[i].name, name) == 0) return i;
  }
  return -1;
}

void interpret(struct Expr* exprs) {
  vars = (struct Var*) malloc(0);
  int i = 0;
  while (exprs[i].kind != EXPR_EOF) {
    switch (exprs[i].kind) {
    case ASSIGN_VAR:
      if (exprs[i].val.var.kind == CPY_VAR) {
	struct Var var = searchVarWErr(exprs[i].val.var.val);
	//free(exprs[i].val.var.val);
	if (!exprs[i].val.var.isRef) {
	  exprs[i].val.var.val = realloc(exprs[i].val.var.val, strlen(var.val)+1);
	  strcpy(exprs[i].val.var.val, var.val);
	} else {
	  free(exprs[i].val.var.val);
	  exprs[i].val.var.val = var.val;
	}
	exprs[i].val.var.kind = var.kind;
      }
      int replaceVar = searchVarPos(exprs[i].val.var.name);
      if (replaceVar >= 0) {
	free(exprs[i].val.var.name);
	vars[replaceVar].val = realloc(vars[replaceVar].val, strlen(exprs[i].val.var.val)+1);
	strcpy(vars[replaceVar].val, exprs[i].val.var.val);
	free(exprs[i].val.var.val);
      } else {
	varAmt++;
	vars = realloc(vars, sizeof(struct Var)*varAmt);
	vars[varAmt-1] = exprs[i].val.var;
      }
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
    if (!vars[j].isRef) free(vars[j].val);
  }
  free(vars);
  free(exprs);
}
