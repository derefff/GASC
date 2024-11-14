#include "parser.h"
#include "AST.h"
#include "lexer.h"
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