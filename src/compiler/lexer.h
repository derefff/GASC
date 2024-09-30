#ifndef LEXER_H
#define LEXER_H

#include<stdlib.h>
#include<stdint.h>

typedef struct {
  uint32_t cursor;
  const char *content;
  char current_char;
  size_t content_len;
  uint32_t line;
  uint32_t line_start;
} Lexer;


Lexer* init_lexer(const char* content, long content_size);
void advance_lexer(Lexer *lexer);
char peek(Lexer* lexer);

#endif