#ifndef PARSER_H
#define PARSER_H

#include "AST.h"
#include "lexer.h"
#include "tokens.h"

typedef struct
{
  Lexer* lexer;
  Token* current_token;
} Parser;

Parser* init_parser(Lexer* lexer);
ASTNode* parse_program(Parser* parser);
ASTNode* parse_statement(Parser* parser);
ASTNode* parse_expression(Parser* parser);
ASTNode* parse_term(Parser* parser);
void advance_parser(Parser* parser);
void expect_token(Parser* parser, token_t token_type);
void free_ast(ASTNode* node);

#endif