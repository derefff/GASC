#ifndef PARSER_H
#define PARSER_H

#include "AST.h"
#include "lexer.h"
#include "tokens.h"
#include <stdbool.h>

typedef struct
{
  Lexer* lexer;
  Token* current_token;
} Parser;

void advance_parser(Parser* parser);
void consume_token(Parser* parser, token_t expected_token_type);
Parser* init_parser(Lexer* lexer);
bool match_token(Parser* parser, token_t type);
ASTNode* parse_assignment(Parser* parser);
ASTNode* parse_factor(Parser* parser);
ASTNode* parse_literal(Parser* parser);
ASTNode* parse_program(Parser* parser);
ASTNode* parse_return_statement(Parser* parser);
ASTNode* parse_statement(Parser* parser);
ASTNode* parse_expression(Parser* parser);
ASTNode* parse_term(Parser* parser);


#endif