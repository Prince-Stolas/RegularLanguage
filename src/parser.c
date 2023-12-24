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
  if (!tok.val) return tok;

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
  
  if (!isRight) {
    free(tok.val);
    tok.val = NULL;
  }
  return tok;
}

void exitWrongTok(struct Token tok) {
  exitWErrRC(tok.loc.row, tok.loc.col, "TOKEN not usable in this context!", 1);
}

void expectTokWErr(int cnt, ...) {
  va_list arg_list;
  va_start(arg_list, cnt);
  
  struct Token tok = nextToken();
  if (!tok.val) exitWrongTok(tok);

  bool isRight = false;
  for (int i=0;i<cnt;i++) {
    if (strcmp(tok.kind, va_arg(arg_list, char*)) == 0) {
      isRight = true;
    }
  }

  va_end(arg_list);

  if (!isRight) exitWrongTok(tok);
  free(tok.val);
}

struct Expr parseExpr() {
  struct Expr expr;
  struct Token tok;
  do {
    tok = expectTok(1, "TOKEN_NAME");
  } while (!tok.val && tok.kind);
  if (!tok.val) exitWrongTok(tok);

  if (strcmp(tok.val, "print") == 0) {
    expectTokWErr(1, "TOKEN_PARANO");

    struct Token nextTok = expectTok(3, "TOKEN_NAME", "TOKEN_INT", "TOKEN_STRING");
    if (strcmp(nextTok.kind, "TOKEN_STRING") == 0) {
      expectTokWErr(1, "TOKEN_PARANC");
      expectTokWErr(2, "TOKEN_NEWLINE", "TOKEN_SEMICOL");
      struct Expr* argv = (struct Expr*) malloc(sizeof(struct Expr));
      argv[0] = (struct Expr){
	.kind = LIT_STR,
	.val.litVal = nextTok.val
      };
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
  }
  
  struct Token nextTok = expectTok(1, "TOKEN_EQUALS");
  if (nextTok.val) {
    struct Token varVal = expectTok(2, "TOKEN_INT", "TOKEN_STRING");
    if (!varVal.val) exitWrongTok(varVal);
    expr = (struct Expr){
      .kind = ASSIGN_VAR,
      .val.var = {
	.kind = strcmp(varVal.kind, "TOKEN_INT") == 0 ? INT : STR,
        .name = tok.val,
        .val = varVal.val
      }
    };
  } else exitWrongTok(nextTok);

  free(nextTok.val);

  return expr;
}

struct Expr* parse() {
  struct Expr expr = parseExpr();
  struct Expr* exprs = (struct Expr*) malloc(sizeof(struct Expr));
  exprs[0] = expr;
  return exprs;
}
