#ifndef LEXER_H
#define LEXER_H

#include<stdint.h>
#include<stdlib.h>
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

void advance_lexer(Lexer *lexer);
Token* create_token(token_t token_type, char* value);
char* get_identifier(Lexer* lexer);
Token* get_next_token(Lexer* lexer);
char* get_string_literal(Lexer* lexer);
Lexer* init_lexer(const char* content, long content_size);
char peek(Lexer* lexer);
char peek_by(Lexer* lexer, int offset);
void skip_by_lexer(Lexer* lexer, int offset);


#endif