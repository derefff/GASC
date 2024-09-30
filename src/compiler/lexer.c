#include <stdint.h>
#include <stdlib.h>
#include "tokens.h"
#include "lexer.h"

typedef struct
{
  token_t type;
  char* val;
} Token;

Lexer* init_lexer(const char *content, long content_len)
{
  Lexer *lexer = malloc(sizeof(Lexer));
  lexer->cursor = 0;
  lexer->content = content;
  lexer->content_len = content_len;
  return lexer;
}

void advance_lexer(Lexer *lexer)
{
  if(lexer->content_len < lexer->cursor) lexer->cursor++;
}

void skip_whitespaces(Lexer* lexer, char current_char)
{
  if(current_char == ' ' || current_char == '\t') advance_lexer(lexer);
}

Token* create_token(token_t token_type, char* value)
{
  Token *token = malloc(sizeof(Token));
  token->type = token_type;
  token->val = value;
  return token;
}

char peek(Lexer* lexer)
{
  return lexer->current_char;
}

//TODO: peek_by(lex, offset)