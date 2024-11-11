#include "parser.h"
#include "lexer.h"

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
