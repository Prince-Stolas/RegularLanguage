#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

#include "lexer.h"

// #define TOKEN_NAME "TOKEN_NAME"
// #define TOKEN_NAME "TOKEN_INT"
// #define TOKEN_NAME "TOKEN_STRING"
// #define TOKEN_NAME "TOKEN_ARROW"
// #define TOKEN_PARANO "TOKEN_PARANO"
// #define TOKEN_PRAANC "TOKEN_PARANC"
// #define TOKEN_EQUALS "TOKEN_EQUALS"
// #define TOKEN_PLUS "TOKEN_PLUS"
// #define TOKEN_MINUS "TOKEN_MINUS" 
// #define TOKEN_STAR "TOKEN_STAR"
// #define TOKEN_SLASH "TOKEN_SLASH" 
// #define TOKEN_GREATER "TOKEN_GREATER"
// #define TOKEN_LESS "TOKEN_LESS"
// #define TOKEN_COMMA "TOKEN_COMMA" 
// #define TOKEN_SEMICOL "TOKEN_SEMICOL"
// #define TOKEN_DOT "TOKEN_DOT"
// #define TOKEN_NEWLINE "TOKEN_NEWLINE"

int row = 1;
int col = 1;
char* source;

struct Token token(char* kind, char* val, struct Loc loc) {
  struct Token tok = {
    .kind = kind,
    .val = val,
    .loc = loc,
    .isEnd = false,
    .isWrong = false
  };

  return tok;
}

bool eof() {
  return *source == '\0';
}

struct Loc getLoc(int colNegOffs) {
  struct Loc loc = {
    .row = row,
    .col = col-colNegOffs
  };
  
  return loc;
}

void setSource(char* src) {
  source = src;
}

void skipChar() {
  if (eof()) return;

  if (*source == '\n') {
    row++;
    col = 0;
  }

  source++;
  col++;
}

void skipLine() {
  if (eof()) return;
  
  while (*source != '\n' && !eof()) skipChar();
  if (!eof()) skipChar();
}

void trimLeft() {
  if (eof()) return;

  while (isspace(*source) && !eof() && *source != '\n') skipChar();
}

char* subStr(char* src, int len) {
  char* buffer = (char*) malloc(len+1);
  memcpy(buffer, src, len);
  buffer[len] = '\0';
  return buffer;
}

struct Token nextToken() {
  struct Token endTok = {
    .kind = "TOKEN_EOF",
    .isEnd = true
  };

  trimLeft();
  while (!eof()) {
    if (strncmp("//", source, 2) != 0) break;
    skipLine();
    trimLeft();
  }

  if (eof()) return endTok;
  
  char* start = source;
  int len = 0;

  if (isalpha(*source)) {
    while (isalnum(*source) && !eof()) {
      skipChar(); len++;
    }
    char* buffer = subStr(start, len);
    return token("TOKEN_NAME", buffer, getLoc(len));
  }

  if (isdigit(*source)) {
    len = 0;
    while (isdigit(*source) && !eof()) {
      skipChar(); len++;
    }
    char* buffer = subStr(start, len);
    return token("TOKEN_INT", buffer, getLoc(len));
  }

  if (*source == '"') {
    len = 0;
    skipChar();
    start++;
    while (*source != '"' && !eof()) {
      skipChar(); len++;
    }
    skipChar();
    char* buffer = subStr(start, len);
    return token("TOKEN_STRING", buffer, getLoc(len));
  }

  if (strncmp("->", source, 2) == 0) {
    skipChar(); skipChar();
    char* buffer = subStr(start, 2);
    return token("TOKEN_ARROW", buffer, getLoc(2));
  }

  if (strchr("()=+-*/<>,;.\n", *source) != NULL) {
    char* oneCharTok;
    switch (*source) {
      case '(': oneCharTok = "TOKEN_PARANO"; break;
      case ')': oneCharTok = "TOKEN_PARANC"; break;
      case '=': oneCharTok = "TOKEN_EQUALS"; break;
      case '+': oneCharTok = "TOKEN_PLUS"; break;
      case '-': oneCharTok = "TOKEN_MINUS"; break;
      case '*': oneCharTok = "TOKEN_STAR"; break;
      case '/': oneCharTok = "TOKEN_SLASH"; break;
      case '>': oneCharTok = "TOKEN_GREATER"; break;
      case '<': oneCharTok = "TOKEN_LESS"; break;
      case ',': oneCharTok = "TOKEN_COMMA"; break;
      case ';': oneCharTok = "TOKEN_SEMICOL"; break;
      case '.': oneCharTok = "TOKEN_DOT"; break;
      case '!': oneCharTok = "TOKEN_EXCLEM"; break;
      case '\n':oneCharTok = "TOKEN_NEWLINE"; break;
    }
    skipChar();
    char* buffer = subStr(start, 1);
    return token(oneCharTok, buffer, getLoc(1));
  }

  return endTok;
}
