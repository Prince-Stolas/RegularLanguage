#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>

#include "parser.h"
#include "lexer.h"
#include "error.h"

struct Token expectTok(int cnt, ...) {
  va_list arg_list;
  va_start(arg_list, cnt);
  
  struct Token tok = nextToken();
  if (tok.isEnd) return tok;

  bool isRight = false;
  for (int i=0;i<cnt;i++) {
    if (strcmp(tok.kind, va_arg(arg_list, char*)) == 0) {
      isRight = true;
    }
  }

  va_end(arg_list);

  /*if (!isRight) {
    char buffer[21+strlen(tok.kind)];
    sprintf(buffer, "%s not usable in this context!", tok.kind);
    exitWErrRC(tok.loc.row, tok.loc.col, buffer, 1);
  }*/
  
  if (!isRight) tok.isWrong = true;
  return tok;
}

void exitWrongTok(struct Token tok) {
  char msg[29+strlen(tok.kind)];
  sprintf(msg, "%s not usable in this context!", tok.kind);
  exitWErrRC(tok.loc.row, tok.loc.col, msg, 1);
}

struct Token expectTokWErr(int cnt, ...) {
  va_list arg_list;
  va_start(arg_list, cnt);
  
  struct Token tok = nextToken();
  if (tok.isEnd) exitWrongTok(tok);

  bool isRight = false;
  for (int i=0;i<cnt;i++) {
    if (strcmp(tok.kind, va_arg(arg_list, char*)) == 0) {
      isRight = true;
    }
  }

  va_end(arg_list);

  if (!isRight) exitWrongTok(tok);
  return tok;
}

struct Expr parseExpr() {
  struct Expr expr;
  struct Token tok = expectTok(1, "TOKEN_NAME");
  while (
      tok.isWrong &&
      !tok.isEnd &&
      strcmp(tok.kind, "TOKEN_NEWLINE") == 0 ||
      strcmp(tok.kind, "TOKEN_SEMICOL") == 0) {
    free(tok.val);
    tok = expectTok(1, "TOKEN_NAME");
  }
  if (tok.isEnd) return (struct Expr){.kind=EXPR_EOF};
  if (tok.isWrong) exitWrongTok(tok);

  if (strcmp(tok.val, "print") == 0) {
    free(expectTokWErr(1, "TOKEN_PARANO").val);

    struct Token nextTok = expectTokWErr(3, "TOKEN_NAME", "TOKEN_INT", "TOKEN_STRING");
    free(expectTokWErr(1, "TOKEN_PARANC").val);
    free(expectTokWErr(2, "TOKEN_NEWLINE", "TOKEN_SEMICOL").val);
    struct Expr* argv = (struct Expr*) malloc(sizeof(struct Expr));
    if (strcmp(nextTok.kind, "TOKEN_STRING") == 0 || strcmp(nextTok.kind, "TOKEN_INT") == 0) {
      argv[0] = (struct Expr){
	.kind = strcmp(nextTok.kind, "TOKEN_STRING") == 0 ? LIT_STR : LIT_INT,
	.val.litVal = nextTok.val
      };
    } else {
      argv[0] = (struct Expr){
	.kind = READ_VAR,
	.val.var = (struct Var){
	  .kind = READ,
	  .name = nextTok.val
	}
      };
    }

    expr = (struct Expr){
      .kind = FUN_CALL,
      .val.funCall = {
        .funName = tok.val,
        .argc = 1,
        .argv = argv
      }
    };

    return expr;
  }

  //variables
  bool ref = false;
  if (strcmp(tok.val, "ref") == 0) ref = true;
  
  struct Token varName;
  if (ref) {
    varName = expectTokWErr(1, "TOKEN_NAME");
    free(tok.val);
  }
  else varName = tok;
  free(expectTokWErr(1, "TOKEN_EQUALS").val);
  struct Token varVal;
  if (!ref) varVal = expectTokWErr(3, "TOKEN_NAME", "TOKEN_INT", "TOKEN_STRING");
  else varVal = expectTokWErr(1, "TOKEN_NAME");
  expr = (struct Expr){
    .kind = ASSIGN_VAR,
    .val.var = {
      .kind = strcmp(varVal.kind, "TOKEN_NAME") == 0 ? CPY_VAR : (strcmp(varVal.kind, "TOKEN_INT") == 0 ? INT : STR),
      .name = varName.val,
      .val = varVal.val,
      .isRef = ref
    }
  };

  return expr;
  //end variables
}

struct Expr* parse() {
  int exprAmt = 0;
  struct Expr* exprs = (struct Expr*) malloc(sizeof(struct Expr));
  struct Expr expr;
  while (true) {
    expr = parseExpr();
    exprs[exprAmt] = expr;
    if (expr.kind == EXPR_EOF) break;
    exprAmt++;
    exprs = (struct Expr*) realloc(exprs, sizeof(struct Expr)*(exprAmt+1));
  }
  return exprs;
}
