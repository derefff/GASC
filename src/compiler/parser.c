#include "AST.h"
#include "lexer.h"
#include "parser.h"


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

void expect_token(Parser *parser, token_t token_type)
{
  if(parser->current_token->type != token_type)
  {
    printf("Error: didn't expect excalty that type of token");
  }
}