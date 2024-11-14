#ifndef AST_H
#define AST_H

#include "lexer.h"
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

typedef struct {

} ASTNode;

#endif