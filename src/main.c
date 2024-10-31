#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<assert.h>

#include "compiler/tokens.h"
#include "compiler/lexer.h"

int main(int argc, char **argv )
{
  FILE *file;
  if(argc > 1){
    file = fopen(argv[1], "r");
    if(!file)
    {
      fprintf(stderr,"File doesn't exist!\n");
      return 1;
    }
  }
  else
  {
    fprintf(stderr,"No argument was passed!\n");
    return 2;
  }

  fseek(file,0, SEEK_END);
  long file_size = ftell(file);
  rewind(file);

  //size_t fread ( void * ptr, size_t size, size_t count, FILE * stream );
  char* file_content;
  file_content = (char*) malloc (sizeof(char)*file_size);
  fread(file_content, 1, file_size, file);
  file_content[file_size] = '\0';

  printf("filesize: %ld \n", file_size);
 // for(long i = 0; i < file_size; i++) printf("%c", file_content[i]);

  Lexer* lex = init_lexer(file_content, file_size);

  Token* program_tokens = malloc(sizeof(Token));
  Token* token = malloc(sizeof(Token));

  while((token = get_next_token(lex))->type != TOKEN_EOF)
  {
    free(token);
  }
  printf("token_EOF");

  free(token);
  fclose(file);
  return 0;
}
