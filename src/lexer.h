#ifndef LEXER_H
#define LEXER_H

#include <stdbool.h>

#include "loc.h"

struct Token {
  char* kind;
  char* val;
  struct Loc loc;
  bool isEnd;
  bool isWrong;
};

struct Token nextToken();
void setSource(char*);

#endif
