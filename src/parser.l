%{
#include "tp1.h"
#include "lib/automatha.h"
#include "lib/grammar.h"

int parsingGrammar = 0;
int state = 0; 
int parsingAutomatha = 0;

%}

%%

(\n|\/\/.*\n)         {}

([a-zA-Z0-9]+)[ ]?=   { if (parsingGrammar) { state++; printf("GRAMMAR!\n"); } else { REJECT; } } 

(\{)?[A-Z][ ]*->[ ]*([A-Za-z\\]+\|)*[A-Za-z]+[ ]*(,)?       { 
                        if (parsingGrammar && state == G_PRODUCTIONS) { 
                          printf("grammarProduction!"); 
                        } else { REJECT; }
                      }

\{[ ]*                {
                        if (parsingGrammar && state == G_TERMS) {  
                          printf("parsingGrammarTerminals!\n"); ; 
                        } else { REJECT; }
                      }

[ ]*\([ ]*\{[ ]*      { 
                        if (parsingGrammar && state == G_NON_TERMS) {  
                          printf("parsingGrammarNonTerminals!\n"); ; 
                        } else { REJECT; }
                      }

[A-Z][ ]*(,)          {
                        if (parsingGrammar && state == G_START) {
                          printf("GRAMMAR START!\n"); state++;
                        } else if (parsingGrammar && state == G_NON_TERMS) { 
                          printf("NT"); 
                        } else { REJECT; }
                      }

[A-Za-z](,)?          { 
                        if (parsingGrammar && state == G_NON_TERMS) { 
                          printf("NT"); 
                        } else if (parsingGrammar && state == G_TERMS) { 
                          printf("T "); 
                        } else { REJECT; }
                      }

\}[ ]*,[ ]*           { 
                        if (parsingGrammar && state == G_NON_TERMS) { 
                          printf("\nDONE PNT!\n"); state++;
                        } else if (parsingGrammar && state == G_TERMS) { 
                          printf("\nDONE PT!\n"); state++;
                        } else if (parsingGrammar && state == G_PRODUCTIONS) { 
                          printf("DONE Prods!\n");  state++;
                        } else { REJECT; }
                      }

[ ]*\}[ ]*\)          {;
                        if (parsingGrammar && state == G_PRODUCTIONS) { 
                          printf("\t\nDONE WITH GRAMMAR!\n");
                        }  else { REJECT; }
                      }    
digraph[ ]*\{         { 
                        state++; printf("AUTHOMATA!!!\n");
                      }

rankdir.*;            {}

node[ ]*\[shape=      { if (parsingAutomatha) { state = A_STATE_SHAPE_START; } }
circle                { if (parsingAutomatha && state == A_STATE_SHAPE_START) { printf("GOT NORMAL STATE\n"); state++; } else { REJECT;} }
doublecircle          { if (parsingAutomatha && state == A_STATE_SHAPE_START) { printf("GOT END STATE\n"); state++; } else { REJECT;} }
\][ ]*                { if (parsingAutomatha && state == A_STATE_SHAPE_END) { state = A_STATE_NAME_START; } else { REJECT; } }
[A-Za-z0-9]+[ ]*      { if (parsingAutomatha && state == A_STATE_NAME_START) { printf("\n", yytext); state = A_STATE_LABEL_START; } else { REJECT; } }
\[label=\"            { if (parsingAutomatha && state == A_STATE_LABEL_START) { state = A_STATE_LABEL_END; } 
                   else if (parsingAutomatha && state == A_TRANS_MATCH) { state = A_TRANS_LABEL_START; } else { REJECT; } }
[A-Za-z0-9]+          { if (parsingAutomatha && state == A_STATE_LABEL_END) { state = A_STATE_END; } 
                   else if (parsingAutomatha && state == A_TRANS_LABEL_START) { state = A_TRANS_LABEL_END; } else { REJECT; } }
\"][ ]*;              { if (parsingAutomatha && state == A_STATE_END) { state = 0; }
                   else if (parsingAutomatha && state == A_TRANS_LABEL_END) { state = 0; } else { REJECT; } }

\}                    { if (parsingAutomatha) { } }

[A-Za-z0-9]+[ ]*->[A-Za-z0-9]+[ ]*[ ] { if (parsingAutomatha && state == 0) { state = A_TRANS_MATCH; } else { REJECT; } }

%%



int main(int len, char ** args) {
  run(len, args);
}