#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils/list.h"
#include "utils/cstring.h"



cstring from_file(cstring path) {
  cstring builder = cstring_init(1);
  cstring buffer = cstring_init(1024);
  FILE * pFile;
  pFile = fopen (path , "r");
  if ( fgets (buffer , 1024 , pFile) != NULL ) {
    builder = cstring_write(builder, buffer);
  }
  fclose (pFile);

  printf("%s\n", builder);
}

int authomata_to_grammar() {

}

int grammar_to_authomata() {
  from_file("hola.gr");
}

int main(int len, char ** args) {
  if (len == 2) {
    char * input = args[0];
    if (strstr(input,".gr") != -1) {
      grammar_to_authomata();
    }
    if (strstr(input,".dot") != -1) {
      authomata_to_grammar();
    }
  }
}