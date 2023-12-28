#ifndef PARSER_H
#define PARSER_H

#include <stdbool.h>

struct FunCall {
  char* funName;
  int argc;
  struct Expr* argv;
};

struct FunDef {
  char* funName;
  int argc;
  struct Expr* argv;
  struct Expr* body;
};

struct Var {
  enum {
    INT,
    STR,
    READ,
    CPY_VAR
  } kind;
  char* name;
  char* val;
  bool isRef;
};

struct Expr {
  enum {
    FUN_CALL,
    FUN_DEF,
    LIT_INT,
    LIT_STR,
    READ_VAR,
    ASSIGN_VAR,
    STMT_RET,
    EXPR_EOF
  } kind;
  struct {
    struct FunCall funCall;
    struct FunDef funDev;
    char* litVal;
    struct Var var;
  } val;
};

struct Expr* parse();

#endif
