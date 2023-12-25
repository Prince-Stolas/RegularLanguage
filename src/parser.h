#ifndef PARSER_H
#define PARSER_H

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
    READ
  } kind;
  char* name;
  char* val;
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
