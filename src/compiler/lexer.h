#ifndef LEXER_H
#define LEXER_H

#include<stdlib.h>
#include<stdint.h>
#include "tokens.h"

typedef struct {
  uint32_t cursor;
  const char *content;
  char current_char;
  size_t content_len;
  uint32_t line;
  uint32_t line_start;
} Lexer;

typedef struct
{
  token_t type;
  char* val;
} Token;

Lexer* init_lexer(const char* content, long content_size);
void advance_lexer(Lexer *lexer);
char peek(Lexer* lexer);
void skip_by_lexer(Lexer* lexer, int offset);
char* get_string_literal(Lexer* lexer);
Token* get_next_token(Lexer* lexer);
Token* create_token(token_t token_type, char* value);


#endif