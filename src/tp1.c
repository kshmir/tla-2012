#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils/list.h"
#include "utils/cstring.h"
#include "tp1.h"

void run(int len, char ** args) {
  if (len == 2) {
    char * input = args[1];
    int ready_to_read = 0;

    if (strstr(input,".gr") != NULL) {
      FILE * pFile = fopen (input , "r");
      stdin = pFile;
      ready_to_read = GRAMMAR;
      start_adts(ready_to_read);
    }
    if (strstr(input,".dot") != NULL) {
      FILE * pFile = fopen (input , "rx3");
      stdin = pFile;
      ready_to_read = AUTOMATHA;
      start_adts(ready_to_read);
    }
    
    if (ready_to_read) {
      yylex();
      // run();
    }
  }
}

void start_adts(int mode) {
  switch(mode) {
    case GRAMMAR:
      parsingGrammar = 1;
      break;
    case AUTOMATHA:
      parsingAutomatha = 1;
      break;
  }
}
