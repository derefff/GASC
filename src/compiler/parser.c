#include "parser.h"
#include "AST.h"
#include "lexer.h"
#include "tokens.h"
#include <stdio.h>

Parser* init_parser(Lexer* lexer)
{
  Parser* parser = (Parser*)malloc(sizeof(Parser));
  parser->lexer = lexer;
  parser->current_token = get_next_token(parser->lexer);
  return parser;
}

void advance_parser(Parser* parser)
{
  parser->current_token = get_next_token(parser->lexer);
}

/*
ASTNode* parse_program(Parser* parser);
ASTNode* parse_statement(Parser* parser);
ASTNode* parse_expression(Parser* parser);
ASTNode* parse_term(Parser* parser);
void expect_token(Parser* parser, token_t token_type);
*/

void consume_token(Parser* parser, token_t expected_token_type)
{
  if (parser->current_token->type != expected_token_type)
  {
    printf("Error: didn't expect excalty that type of token");
  }
  advance_parser(parser);
}

bool match_token(Parser* parser, token_t token_type)
{
  return parser->current_token->type == token_type;
}

ASTNode* parse_statement(Parser* parser)
{
  switch(parser->current_token->type)
  {
    case TOKEN_KEYWORD_CONST:
    break;

    case TOKEN_KEYWORD_DO:
    break;

    case TOKEN_KEYWORD_FOR:
    break;

    case TOKEN_KEYWORD_FUNCTION:
    break;

    case TOKEN_KEYWORD_IF:
    break;

    case TOKEN_KEYWORD_VAR:
    break;

    case TOKEN_KEYWORD_WHILE:
    break;

    default:
      return parse_expression(parser);
    break;

  }
}

/*
<literal> ::= <number_literal>
              | <string_literal>
              | <boolean_literal>
              | null
*/
ASTNode* parse_literal(Parser* parser)
{
  ASTNode* literal = (ASTNode*)malloc(sizeof(ASTNode));
  switch(parser->current_token->type)
  {
    case TOKEN_FLOAT:
      literal = create_ast_node(AST_LITERAL, parser->current_token);
      advance_parser(parser);
      return literal;
    break;

    case TOKEN_INTEAGER:
      literal = create_ast_node(AST_LITERAL, parser->current_token);
      advance_parser(parser);
      return literal;
    break;

    case TOKEN_BOOL_TRUE:
      literal = create_ast_node(AST_LITERAL, parser->current_token);
      advance_parser(parser);
      return literal;
    break;

    case TOKEN_BOOL_FALSE:
      literal = create_ast_node(AST_LITERAL, parser->current_token);
      advance_parser(parser);
      return literal;
    break;

    case TOKEN_STRING_LITERAL:
      literal = create_ast_node(AST_LITERAL, parser->current_token);
      advance_parser(parser);
      return literal;
    break;

    case TOKEN_NULL:
      literal = create_ast_node(AST_LITERAL, parser->current_token);
      advance_parser(parser);
      return literal;
    break;
  }
}

/*
<expression> ::= <literal>
               | <identifier>
               | <function_call>
               | <binary_expression>
               | <assignment>

<expression> ::= <term> ( ("+" | "-") <term> )*
<term>       ::= <factor> ( ("*" | "/") <factor> )*
<factor>     ::= <literal> | <identifier> | "(" <expression> ")"
*/
ASTNode* parse_expression(Parser* parser)
{
  return parse_term(parser);
}

ASTNode* parse_term(Parser* parser)
{
  if(parser->current_token->type == TOKEN_STRING_LITERAL)
  {
    ASTNode* literal_node = create_ast_node(AST_LITERAL, parser->current_token);
    advance_parser(parser);
    return literal_node;
  }
}

ASTNode* parse_program(Parser* parser)
{
  ASTNode* program = create_ast_node(AST_PROGRAM, NULL);

  while (parser->current_token->type != TOKEN_EOF)
  {
    // TODO: create parse_statement function
    ASTNode* statement = parse_statement(parser);
    add_child_to_node(program, statement);
  }

  return program;
}