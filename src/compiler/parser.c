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
  switch (parser->current_token->type)
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
  switch (parser->current_token->type)
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

//<factor>     ::= <literal> | <identifier> | "(" <expression> ")"
ASTNode* parse_factor(Parser* parser)
{
  if (parser->current_token->type == TOKEN_FLOAT || parser->current_token->type == TOKEN_INTEAGER || parser->current_token->type == TOKEN_STRING_LITERAL ||
      parser->current_token->type == TOKEN_BOOL_TRUE || parser->current_token->type == TOKEN_BOOL_FALSE)
  {
    parse_literal(parser);
  }

  if (parser->current_token->type == TOKEN_ID)
  {
    ASTNode* ID_node = create_ast_node(AST_IDENTIFIER, parser->current_token);
    consume_token(parser, TOKEN_ID);
    return ID_node;
  }
  if (parser->current_token->type == TOKEN_OPEN_PARANTHESIS)
  {
    consume_token(parser, TOKEN_OPEN_PARANTHESIS);
    ASTNode* expression_node = parse_expression(parser);
    consume_token(parser, TOKEN_CLOSE_PARANTHESIS);
    return expression_node;
  }
}

//<term>       ::= <factor> ( ("*" | "/") <factor> )*
ASTNode* parse_term(Parser* parser)
{
  ASTNode* node = parse_factor(parser);

  while (parser->current_token->type == TOKEN_MULTIPLY || parser->current_token->type == TOKEN_DIVIDE)
  {
    Token* op_token = parser->current_token;
    consume_token(parser, op_token->type);

    ASTNode* op_node = create_ast_node(AST_BINARY_OP, op_token);
    ASTNode* right_node = parse_factor(parser);
    add_child_to_node(op_node, node);       // left node
    add_child_to_node(op_node, right_node); // right_node

    node = op_node;
  }

  return node;
}
// TODO: add other <binary_oparators>, <function_call>
//<expression> ::= <term> ( ("+" | "-") <term> )*
ASTNode* parse_expression(Parser* parser)
{
  ASTNode* node = parse_term(parser);

  while (parser->current_token->type == TOKEN_MULTIPLY || parser->current_token->type == TOKEN_DIVIDE)
  {
    Token* op_token = parser->current_token;
    consume_token(parser, op_token->type);

    ASTNode* op_node = create_ast_node(AST_BINARY_OP, op_token);
    ASTNode* right_node = parse_factor(parser);
    add_child_to_node(op_node, node);       // left node
    add_child_to_node(op_node, right_node); // right_node

    node = op_node;
  }
  return node;
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

// <return_statement> ::= "return" <expression>? ";"
ASTNode* parse_return_statement(Parser* parser)
{
  consume_token(parser, TOKEN_KEYWORD_RETURN);

  ASTNode* return_node = create_ast_node(AST_RETURN_STATEMENT, parser->current_token);

  if (parser->current_token->type != TOKEN_SEMICOLON)
  {
    ASTNode* exp_node = parse_expression(parser);
    add_child_to_node(return_node, exp_node);
  }
  consume_token(parser, TOKEN_SEMICOLON);

  return return_node;
}

// <assignment> ::= <identifier> "=" <expression>
ASTNode* parse_assignment(Parser* parser)
{
  ASTNode* assignment_node = create_ast_node(AST_ASSIGNMENT, parser->current_token);

  // NOTE: sometime in the future make sure to peek for TOKEN_ID + TOKEN_ASSIGN
  if (match_token(parser, TOKEN_ID))
  {
    ASTNode* id_node = create_ast_node(AST_IDENTIFIER, parser->current_token);
    add_child_to_node(assignment_node, id_node);
    consume_token(parser, TOKEN_ID);
    consume_token(parser, TOKEN_ASSIGN);

    ASTNode* exp_node = parse_expression(parser);
    add_child_to_node(assignment_node, exp_node);
    consume_token(parser, TOKEN_SEMICOLON);

    return assignment_node;
  }
}