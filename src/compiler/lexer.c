#include "lexer.h"
#include "tokens.h"
#include <ctype.h>
#include "stdbool.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG_CURSOR_CHAR(lexer) printf("cursor:%d \t char:\t %c\t ascii_form: %d \n", (lexer)->cursor, (lexer)->current_char, (lexer)->current_char)

void advance_lexer(Lexer* lexer)
{
  if (lexer->content_len > lexer->cursor)
  {
    lexer->cursor++;
    lexer->current_char = lexer->content[lexer->cursor];
  }
}

Lexer* init_lexer(const char* content, long content_len)
{
  Lexer* lexer = malloc(sizeof(Lexer));
  lexer->cursor = 0;
  lexer->content = content;
  lexer->content_len = content_len;
  return lexer;
}

Token* create_token(token_t token_type, char* value)
{
  Token* token = malloc(sizeof(Token));
  token->type = token_type;

  if(token_type == TOKEN_INTEAGER)
  {
    token->val_inteager = atoi(value);
  }
  else if(token_type == TOKEN_FLOAT)
  {
    token->val_float = strtod(value, NULL);
  }
  else
  {
    token->val = value;
  }
  return token;
}

int is_numeric_end(char c) {
    return c == ' ' || c == '\n' || c == '\t' ||
           c == ',' || c == ';' || c == ':' ||
           c == '+' || c == '-' || c == '*' || c == '/' || c == '%' ||
           c == '(' || c == ')' || c == '[' || c == ']' ||
           c == '=' || c == '&' || c == '|' || c == '<' || c == '>' ||
           c == '\'' || c == '\"' || c == '\\' ||
           c == '\0';
}


// handling whitespaces and new line symbol '\n'
void skip_whitespaces(Lexer* lexer)
{
  while (lexer->current_char == ' ' || lexer->current_char == '\t' || lexer->current_char == '\n')
  {
    if (lexer->current_char == ' ' || lexer->current_char == '\t')
      advance_lexer(lexer);
    if (lexer->current_char == '\n')
      advance_lexer(lexer);
  }
}

// look at current_character + 1, don't move cursor
char peek(Lexer* lexer) { return lexer->content[lexer->cursor + 1]; }

char peek_by(Lexer* lexer, int offset) { return lexer->content[lexer->cursor + offset]; }

void skip_by_lexer(Lexer* lexer, int offset)
{
  if (lexer->content_len < lexer->cursor + offset)
    lexer->cursor += offset;
}

char* get_comment_multi(Lexer* lexer)
{
  int comment_length = 0;
  int buffer = 40;
  char* comment_text = malloc(sizeof(char) * buffer);

  while (lexer->current_char != '*' && peek(lexer) != '/')
  {
    if (comment_length >= buffer)
    {
      buffer *= 2;
      comment_text = realloc(comment_text, buffer * sizeof(char));
    }

    comment_text[comment_length] = lexer->current_char;
    comment_length++;
    advance_lexer(lexer);
  }
  skip_by_lexer(lexer, comment_length + 2); // the "*/" the end

  return comment_text;
}

char* get_comment_single(Lexer* lexer)
{
  int comment_length = 0;
  int buffer = 40;
  char* comment_text = malloc(sizeof(char) * buffer);

  while (lexer->current_char != '\n')
  {
    if (comment_length >= buffer)
    {
      buffer *= 2;
      comment_text = realloc(comment_text, buffer * sizeof(char));
    }

    comment_text[comment_length] = lexer->current_char;
    comment_length++;
    advance_lexer(lexer);
  }
  skip_by_lexer(lexer, comment_length + 1); // the \n the end

  return comment_text;
}

char* get_identifier(Lexer* lexer)
{

  int identifier_name_length = 1;
  char* identifier_name = malloc(sizeof(char) * identifier_name_length);

  // while(lexer->current_char != ' ' && lexer->current_char != '\0' &&
  // lexer->current_char != '\t' || lexer->current_char != '\n')
  while (isalnum(lexer->current_char) || lexer->current_char == '_')
  {
    if (lexer->cursor % identifier_name_length == 0)
    {
      // printf("cursor: %d\t ident_len: %d\t mod: %d\n", lexer->cursor,
      // identifier_name_length,lexer->cursor % identifier_name_length );
      identifier_name = realloc(identifier_name, identifier_name_length * sizeof(char));
    }
    identifier_name[identifier_name_length - 1] = lexer->current_char;
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
  char* string_literal = malloc(sizeof(char) * literal_size);

  while (lexer->current_char != '"')
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

  skip_whitespaces(lexer);

  if (lexer->cursor >= lexer->content_len)
  {
    return create_token(TOKEN_EOF, "\0");
  }

  if (isalpha(lexer->current_char) && !isdigit(lexer->current_char))
  {
    // TODO: create more tokens
    char* current_identfier = get_identifier(lexer);

    if (strcmp(current_identfier, "const") == 0)
    {
      return create_token(TOKEN_KEYWORD_CONST, "");
    }
    else if (strcmp(current_identfier, "do") == 0)
    {
      return create_token(TOKEN_KEYWORD_DO, "");
    }
    else if (strcmp(current_identfier, "else") == 0)
    {
      return create_token(TOKEN_KEYWORD_ELSE, "");
    }
    else if (strcmp(current_identfier, "for") == 0)
    {
      return create_token(TOKEN_KEYWORD_FOR, "");
    }
    else if (strcmp(current_identfier, "function") == 0)
    {
      return create_token(TOKEN_KEYWORD_FUNCTION, "");
    }
    else if (strcmp(current_identfier, "if") == 0)
    {
      return create_token(TOKEN_KEYWORD_IF, "");
    }
    else if (strcmp(current_identfier, "return") == 0)
    {
      return create_token(TOKEN_KEYWORD_RETURN, "");
    }
    else if (strcmp(current_identfier, "var") == 0)
    {
      return create_token(TOKEN_KEYWORD_VAR, "");
    }
    else if (strcmp(current_identfier, "void") == 0)
    {
      return create_token(TOKEN_KEYWORD_VOID, "");
    }
    else if (strcmp(current_identfier, "while") == 0)
    {
      return create_token(TOKEN_KEYWORD_WHILE, "");
    }
    else
    {
      // printf("identifier %s \n", curr_identfier);
      return create_token(TOKEN_ID, "");
    }
  }

  if(isdigit(lexer->current_char))
  {
    int literal_index = 1;
    int buffer_size = 4;
    bool is_float = false;
    char* numeric_literal = malloc(sizeof(char)*10);

    while(!is_numeric_end(lexer->current_char) && !isalpha(lexer->current_char))
    {
      //DEBUG_CURSOR_CHAR(lexer);
      if (literal_index >= buffer_size - 1) {
          buffer_size *= 2;
          numeric_literal = realloc(numeric_literal, buffer_size * sizeof(char));
      }

      if(lexer->current_char != '.')
      {
        numeric_literal[literal_index] = lexer->current_char;
      }
      else
      {
        if(!is_float)
        {
          is_float = true;
          numeric_literal[literal_index] = lexer->current_char;
        }
      }
      advance_lexer(lexer);
      literal_index++;
    }


    if(is_float) return create_token(TOKEN_FLOAT, numeric_literal);
    return create_token(TOKEN_INTEAGER, numeric_literal);

  }

  switch (lexer->current_char)
  {
  case '"': // TODO 'lorem ipsum' would be also a string literal and ... char I guess
    advance_lexer(lexer);
    temp_token = create_token(TOKEN_STRING_LITERAL, get_string_literal(lexer));
    return temp_token;
    break;

  case '+':
    if (peek(lexer) == '+' && peek_by(lexer, 2) == '+')
    {
      temp_token = create_token(TOKEN_ADD, "");
      advance_lexer(lexer);
    }
    else if (peek(lexer) == '+' && peek_by(lexer, 2) != '+')
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
    if (peek(lexer) == '-' && peek_by(lexer, 2) == '-')
    {
      temp_token = create_token(TOKEN_SUBTRACT, "");
      advance_lexer(lexer);
    }
    else if (peek(lexer) == '-' && peek_by(lexer, 2) != '-')
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
    if (peek(lexer) != '/' && peek(lexer) != '*')
    {
      temp_token = create_token(TOKEN_DIVIDE, "");
      advance_lexer(lexer);
      return temp_token;
    }
    else if (peek(lexer) == '/')
    {
      advance_lexer(lexer);
      advance_lexer(lexer);
      temp_token = create_token(TOKEN_COMMENT_SINGLE, get_comment_single(lexer));
      return temp_token;
    }
    else if (peek(lexer) == '*')
    {
      advance_lexer(lexer);
      advance_lexer(lexer);
      temp_token = create_token(TOKEN_COMMENT_MULTI, get_comment_multi(lexer));
      return temp_token;
    }
    break;

  case '=':
    if (peek(lexer) != '=')
    {
      temp_token = create_token(TOKEN_ASSIGN, "");
      advance_lexer(lexer);
    }

    if (peek(lexer) == '=')
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
    temp_token = create_token(TOKEN_INVALID, "Invalid Token at " + lexer->cursor);
    advance_lexer(lexer);
    return temp_token;
    break;
  }
}