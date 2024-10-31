#include <stdint.h>
#include <stdlib.h>
#include "tokens.h"
#include "lexer.h"
#include <stdio.h>

#define DEBUG_CURSOR_CHAR(lexer) \
    printf("cursor:%d \t char:\t %c\n", (lexer)->cursor, (lexer)->current_char)

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
  if(lexer->content_len > lexer->cursor)
  {
    lexer->cursor++;
    lexer->current_char = lexer->content[lexer->cursor];
  }
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

// look at current_character + 1, don't move cursor
char peek(Lexer* lexer)
{
  return lexer->current_char;
}

void skip_by_lexer(Lexer* lexer, int offset)
{
  if(lexer->content_len < lexer->cursor+offset) lexer->cursor+=offset;
}

char* get_string_literal(Lexer* lexer)
{
  char c = lexer->current_char;
  int literal_size = 1;
  char* string_literal = malloc(sizeof(char));

  while(lexer->current_char != '"')
  {
    string_literal = realloc(string_literal, literal_size * sizeof(char));
    string_literal[literal_size] = lexer->current_char;
    literal_size++;
  }
  return string_literal;
}

Token* get_next_token(Lexer* lexer)
{
  lexer->current_char = lexer->content[lexer->cursor];

  //FIXME: redundant: with skip_whitespaces()
  //TODO: take care of '\n'' char
  while(lexer->current_char == ' ' || lexer->current_char == '\t')
  {
    skip_whitespaces(lexer, peek(lexer));
  }

  if (lexer->cursor >= lexer->content_len) {
      return create_token(TOKEN_EOF, "\0");
  }

  Token* temp_token = (Token*)malloc(sizeof(Token));
  DEBUG_CURSOR_CHAR(lexer);

  switch(peek(lexer))
  {
    case '"': //TODO 'lorem ipsum' would be also a string literal and ... char I guess
      //FIXME: use get_string_literal() here
      advance_lexer(lexer);
      return create_token(TOKEN_STRING_LITERAL, "...string value");
    break;

    default:
      temp_token =  create_token(TOKEN_INVALID, "Invalid Token at"+lexer->cursor);
      advance_lexer(lexer);
      return temp_token;
    break;
  }
}
//TODO: peek_by(lex, offset)