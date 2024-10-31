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
  return lexer->content[lexer->cursor+1];
}

char peek_by(Lexer* lexer, int offset)
{
  return lexer->content[lexer->cursor+offset];
}

void skip_by_lexer(Lexer* lexer, int offset)
{
  if(lexer->content_len < lexer->cursor+offset) lexer->cursor+=offset;
}

char* get_string_literal(Lexer* lexer)
{
  char c = lexer->current_char;
  int literal_size = 1;
  char* string_literal = malloc(sizeof(char)*literal_size);

  while(lexer->current_char != '"')
  {
    // printf("\t literal_string_current_char: %c \n", lexer->current_char);
    advance_lexer(lexer);
    string_literal = realloc(string_literal, literal_size * sizeof(char));
    string_literal[literal_size] = lexer->current_char;
    literal_size++;
  }
  advance_lexer(lexer);

  return string_literal;
}

Token* get_next_token(Lexer* lexer)
{
  lexer->current_char = lexer->content[lexer->cursor];

  //FIXME: redundant: with skip_whitespaces()
  //TODO: take care of '\n'' char
  while(lexer->current_char == ' ' || lexer->current_char == '\t')
  {
    skip_whitespaces(lexer, lexer->current_char);
  }

  if(lexer->current_char == '\n')
  {
    advance_lexer(lexer);
    //next line
  }

  if (lexer->cursor >= lexer->content_len) {
      return create_token(TOKEN_EOF, "\0");
  }

  Token* temp_token = (Token*)malloc(sizeof(Token));

  switch(lexer->current_char)
  {
    case '"': //TODO 'lorem ipsum' would be also a string literal and ... char I guess
      advance_lexer(lexer);
      char* temp_str = get_string_literal(lexer);
      temp_token = create_token(TOKEN_STRING_LITERAL, temp_str);
      return temp_token;
    break;

    case '+':
      if(peek(lexer) == '+' && peek_by(lexer, 2) == '+' )
      {
        temp_token = create_token(TOKEN_ADD, "");
        advance_lexer(lexer);
      }
      else if(peek(lexer) == '+' && peek_by(lexer, 2) != '+' )
      {
        temp_token = create_token(TOKEN_INCREMENT, "");
        advance_lexer(lexer);
        advance_lexer(lexer);
      }
      else
      {
        temp_token = create_token(TOKEN_ADD, "");
        advance_lexer(lexer);
      }

      return temp_token;
    break;

    case '-':
      if(peek(lexer) == '-' && peek_by(lexer, 2) == '-' )
      {
        temp_token = create_token(TOKEN_SUBTRACT, "");
        advance_lexer(lexer);
      }
      else if(peek(lexer) == '-' && peek_by(lexer, 2) != '-' )
      {
        temp_token = create_token(TOKEN_DECREMENT, "");
        advance_lexer(lexer);
        advance_lexer(lexer);
      }
      else
      {
        temp_token = create_token(TOKEN_SUBTRACT, "");
        advance_lexer(lexer);
      }
    break;

    case '*':
      temp_token = create_token(TOKEN_MULTIPLY, "");
      advance_lexer(lexer);
      return temp_token;
    break;

    case '/':
      temp_token = create_token(TOKEN_DIVIDE, "");
      advance_lexer(lexer);
      return temp_token;
    break;

    case '=':
      if(peek(lexer) != '=')
      {
        temp_token = create_token(TOKEN_ASSIGN, "");
        advance_lexer(lexer);
      }

      if(peek(lexer) == '=')
      {
        temp_token = create_token(TOKEN_EQ, "");
        advance_lexer(lexer);
        advance_lexer(lexer);
      }

      return temp_token;
    break;

    case '(':
      temp_token = create_token(TOKEN_OPEN_PARANTHESIS, "");
      advance_lexer(lexer);
      return temp_token;
    break;

    case ')':
      temp_token = create_token(TOKEN_CLOSE_PARANTHESIS, "");
      advance_lexer(lexer);
      return temp_token;
    break;

    case '{':
      temp_token = create_token(TOKEN_OPEN_BRACE, "");
      advance_lexer(lexer);
      return temp_token;
    break;

    case '}':
      temp_token = create_token(TOKEN_CLOSE_BRACE, "");
      advance_lexer(lexer);
      return temp_token;
    break;

    case '[':
      temp_token = create_token(TOKEN_OPEN_BRACKET, "");
      advance_lexer(lexer);
      return temp_token;
    break;

    case ']':
      temp_token = create_token(TOKEN_CLOSE_BRACKET, "");
      advance_lexer(lexer);
      return temp_token;
    break;

    case ';':
      temp_token = create_token(TOKEN_SEMICOLON, "");
      advance_lexer(lexer);
      return temp_token;
    break;

    case ':':
      temp_token = create_token(TOKEN_COLON, "");
      advance_lexer(lexer);
      return temp_token;
    break;

    case '.':
      temp_token = create_token(TOKEN_PERIOD, "");
      advance_lexer(lexer);
      return temp_token;
    break;

    case ',':
      temp_token = create_token(TOKEN_COMMA, "");
      advance_lexer(lexer);
      return temp_token;
    break;


    default:
      DEBUG_CURSOR_CHAR(lexer);
      temp_token =  create_token(TOKEN_INVALID, "Invalid Token at "+lexer->cursor);
      advance_lexer(lexer);
      return temp_token;
    break;
  }
}