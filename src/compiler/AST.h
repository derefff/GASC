#ifndef AST_H
#define AST_H

#include "lexer.h"
#include "tokens.h"
#include<stdlib.h>

typedef enum {
  AST_PROGRAM,
  AST_VARIABLE_DECLARATION,
  AST_FUNCTION_DECLARATION,
  AST_ASSIGNMENT,
  AST_EXPRESSION,
  AST_IF_STATEMENT,
  AST_WHILE_STATEMENT,
  AST_RETURN_STATEMENT,
  AST_BINARY_OP,
  AST_LITERAL,
  AST_IDENTIFIER
} ASTNodeType;

typedef struct ASTNode {
  ASTNodeType type;
  struct ASTNode** children;
  int child_count;
  Token* token;
} ASTNode;

ASTNode* create_ast_node(ASTNodeType type, Token* token);
void add_child_to_node(ASTNode* parent, ASTNode* child);
void free_ast(ASTNode* node);

#endif