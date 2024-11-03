#include <ctype.h>
#include "lexer.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "tokens.h"

#define DEBUG_CURSOR_CHAR(lexer) \
    printf("cursor:%d \t char:\t %c\t ascii_form: %d \n", (lexer)->cursor, (lexer)->current_char, (lexer)->current_char)

void advance_lexer(Lexer *lexer)
{
  if(lexer->content_len > lexer->cursor)
  {
    lexer->cursor++;
    lexer->current_char = lexer->content[lexer->cursor];
  }
}

Lexer* init_lexer(const char *content, long content_len)
{
  Lexer *lexer = malloc(sizeof(Lexer));
  lexer->cursor = 0;
  lexer->content = content;
  lexer->content_len = content_len;
  return lexer;
}

Token* create_token(token_t token_type, char* value)
{
  Token *token = malloc(sizeof(Token));
  token->type = token_type;
  token->val = value;
  return token;
}

void skip_whitespaces(Lexer* lexer, char current_char)
{
  if(current_char == ' ' || current_char == '\t') advance_lexer(lexer);
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

char* get_identifier(Lexer* lexer)
{

  int identifier_name_length = 1;
  char* identifier_name = malloc(sizeof(char) * identifier_name_length);

  //while(lexer->current_char != ' ' && lexer->current_char != '\0' && lexer->current_char != '\t' || lexer->current_char != '\n')
  while(isalnum(lexer->current_char) || lexer->current_char == '_')
  {
    if(lexer->cursor % identifier_name_length == 0)
    {
     // printf("cursor: %d\t ident_len: %d\t mod: %d\n", lexer->cursor, identifier_name_length,lexer->cursor % identifier_name_length );
      identifier_name = realloc(identifier_name, identifier_name_length * sizeof(char));
    }
    identifier_name[identifier_name_length-1] = lexer->current_char;
    identifier_name_length++;
    // DEBUG_CURSOR_CHAR(lexer);
    advance_lexer(lexer);
  }
  skip_by_lexer(lexer, identifier_name_length);

  return identifier_name;
}

char* get_string_literal(Lexer* lexer)
{
  char c = lexer->current_char;
  int literal_size = 1;
  char* string_literal = malloc(sizeof(char)*literal_size);

  while(lexer->current_char != '"')
  {
    // printf("\t literal_string_current_char: %c \n", lexer->current_char);
    string_literal = realloc(string_literal, literal_size * sizeof(char));
    string_literal[literal_size] = lexer->current_char;
    literal_size++;
    advance_lexer(lexer);
  }
  advance_lexer(lexer);

  return string_literal;
}

Token* get_next_token(Lexer* lexer)
{
  lexer->current_char = lexer->content[lexer->cursor];
  Token* temp_token = (Token*)malloc(sizeof(Token));

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


  if(isalpha(lexer->current_char) && !isdigit(lexer->current_char))
  {
    // printf("potential identifier %s \n", get_identifier(lexer));

    return temp_token = create_token(TOKEN_ID, "");

  }

  switch(lexer->current_char)
  {
    case '"': //TODO 'lorem ipsum' would be also a string literal and ... char I guess
      advance_lexer(lexer);
      temp_token = create_token(TOKEN_STRING_LITERAL, get_string_literal(lexer));
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