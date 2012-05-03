#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils/list.h"
#include "utils/cstring.h"
// #include "utils/grammar.h"


int grammar_to_authomata() {
  list l = list_init();

  cstring cadena = cstring_copy("hola");

  list_add(l, cadena);

  char * b = list_get(l, 0);

  printf("%s\n", b);

}

int main(int len, char ** args) {
  if (len == 2) {
    char * input = args[0];
    if (strstr(input,".gr") != -1) {
      grammar_to_authomata();
    }
  }
}