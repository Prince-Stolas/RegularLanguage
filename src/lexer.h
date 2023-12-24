#ifndef LEXER_H
#define LEXER_H

#include "loc.h"

struct Token {
  char* kind;
  char* val;
  struct Loc loc;
};

struct Token nextToken();
void setSource(char*);

#endif
