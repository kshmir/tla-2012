%{
  #define TUPLES      0
  #define NON_TERMS   1
  #define TERMS       2
  #define START       3
  #define PRODUCTIONS 4

  int parsingGrammar = 0;

  int state = 0; // 0: tuples, 1: non terms, 2: terms, 3: start, 4: productions
  int parsingGrammarTuple = 0;
  int parsingGrammarNonTerminals = 0;
  int parsingGrammarTerminals = 0;
  int parsingGrammarStart = 0;
  int parsingGrammarProductions = 0;
%}

%%
([a-zA-Z0-9]+)[ ]?=   { parsingGrammar = 1; state++; printf("GRAMMAR!\n"); } 

[A-Z][ ]*->[ ]*([A-Za-z\\]+\|)*[A-Za-z]+[ ]*(,)?       { 
                        if (parsingGrammar && state == PRODUCTIONS) { 
                          printf("grammarProduction!"); 
                        } 
                      }

\{[ ]*                {
                        if (parsingGrammar && state == TERMS) {  
                          printf("parsingGrammarTerminals!\n"); parsingGrammarTerminals = 1; 
                        } 
                      }

[ ]*\([ ]*\{[ ]*      { 
                        if (parsingGrammar && state == NON_TERMS) {  
                          printf("parsingGrammarNonTerminals!\n"); parsingGrammarNonTerminals = 1; 
                        } 
                      }

[A-Z][ ]*(,)          {
                        if (parsingGrammar && state == START) {
                          printf("GRAMMAR START!\n"); state++;
                        }
                        if (parsingGrammar && parsingGrammarNonTerminals) { 
                          printf("NT"); 
                        } 
                      }

[A-Za-z](,)?          { 
                        if (parsingGrammar && parsingGrammarNonTerminals) { 
                          printf("NT"); 
                        } 
                        if (parsingGrammar && parsingGrammarTerminals) { 
                          printf("T "); 
                        } 
                      }

\}[ ]*,[ ]*           { 
                        if (parsingGrammar && parsingGrammarNonTerminals) { 
                          printf("\nDONE PNT!\n"); parsingGrammarNonTerminals = 0; state++;
                        } 
                        if (parsingGrammar && parsingGrammarTerminals) { 
                          printf("\nDONE PT!\n"); parsingGrammarTerminals = 0; state++;
                        }
                        if (parsingGrammar && parsingGrammarProductions) { 
                          printf("DONE Prods!\n"); parsingGrammarProductions = 0; state++;
                        }
                      }

[ ]*\}[ ]*\)            {
                        if (parsingGrammar && state == PRODUCTIONS) { 
                          printf("\t\nDONE WITH GRAMMAR!\n");
                        } 
                      }          
%%
int main ()
{
  yylex ();
  return 0;
}