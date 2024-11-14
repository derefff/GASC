#include "AST.h"

ASTNode* create_ast_node(ASTNodeType type, Token* token)
{
  ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));

  node->type = type;
  node->token = token;
  node->children = NULL;
  node->child_count = 0;

  return node;
}

void add_child_to_node(ASTNode* parent, ASTNode* child)
{
  parent->children = realloc(parent->children, sizeof(ASTNode*) * (parent->child_count + 1));
  parent->children[parent->child_count + 1] = child;
  parent->child_count++;
}

void free_ast(ASTNode* node)
{
  for (int i = 0; i < node->child_count; i++)
  {
    free_ast(node->children[i]);
  }
  free(node->children);
  free(node);
}
