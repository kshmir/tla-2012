%option noyywrap

%{
#include "tp1.h"
#include "lib/automatha.h"
#include "lib/grammar.h"

int parsingGrammar = 0;
int state = 0;
int parsingAutomatha = 0;

int     automatha_state_isfinal = 0;
cstring automatha_state_name;
cstring automatha_state_label;

cstring automatha_trans_relation;
cstring automatha_trans_label;

automatha _a;
grammar _g;

%}

%%

(\n|\/\/.*\n)         {}

([a-zA-Z0-9]+)[ ]?=   { if (parsingGrammar) { state++; } else { REJECT; } } 

(\{)?[A-Z\\][ ]*->[ ]*([ ]*[A-Za-z\\]+[ ]*\|)*[ ]*[A-Za-z\\]+[ ]*(,)?       { 
	
                        if (parsingGrammar && state == G_PRODUCTIONS) {
                        	cstring nokey = cstring_replace(yytext, "{", "");
                        	cstring nocomma = cstring_replace(nokey, ",", "");
                        	cstring trimmed = cstring_trim(nocomma);
                        	production p = production_from_string(trimmed);
                        	
                        	grammar_add_production(_g, p);
                       
                        } else { REJECT; }
                      }

\{[ ]*                {
                        if (parsingGrammar && state == G_TERMS) {   
                        } else { REJECT; }
                      }

[ ]*\([ ]*\{[ ]*      { 
                        if (parsingGrammar && state == G_NON_TERMS) {  
                           
                        } else { REJECT; }
                      }

[A-Z][ ]*(,)          {
                        if (parsingGrammar && state == G_START) {
                        	cstring nocomma = cstring_replace(yytext, ",", "");
							cstring trimmed = cstring_trim(nocomma);
							grammar_set_start_token(_g, trimmed);
							state++;
                        } else if (parsingGrammar && state == G_NON_TERMS) {
                        	cstring nocomma = cstring_replace(yytext, ",", "");
							cstring trimmed = cstring_trim(nocomma);
							grammar_add_non_terminal(_g, trimmed);
                        } else { REJECT; }
                      }

[A-Za-z](,)?          { 
                        if (parsingGrammar && state == G_NON_TERMS) {
                        	cstring nocomma = cstring_replace(yytext, ",", "");
                        	cstring trimmed = cstring_trim(nocomma);
                        	grammar_add_non_terminal(_g, trimmed); 
                        } else if (parsingGrammar && state == G_TERMS) { 
                        	cstring nocomma = cstring_replace(yytext, ",", "");
							cstring trimmed = cstring_trim(nocomma);
							grammar_add_terminal(_g, trimmed); 
                        } else { REJECT; }
                      }

\}[ ]*,[ ]*           { 
                        if (parsingGrammar && state == G_NON_TERMS) { 
                          state++;
                        } else if (parsingGrammar && state == G_TERMS) { 
                          state++;
                        } else if (parsingGrammar && state == G_PRODUCTIONS) { 
                          state++;
                        } else { REJECT; }
                      }

[ ]*\}[ ]*\)          {;
                        if (parsingGrammar && state == G_PRODUCTIONS) { 
                        
                        }  else { REJECT; }
                      }    
digraph[ ]*\{         { 
                        state++; 
                      }

rankdir.*;            {}

node[ ]*\[shape=      { if (parsingAutomatha) { state = A_STATE_SHAPE_START; } }
circle                { 
						if (parsingAutomatha && state == A_STATE_SHAPE_START) { 
							automatha_state_isfinal = 0; state++; 
						} else { REJECT; } 

					  }
doublecircle          { 
						if (parsingAutomatha && state == A_STATE_SHAPE_START) { 
							automatha_state_isfinal = 1; state++; 
						} else { REJECT; } 
					  }
\][ ]*                { if (parsingAutomatha && state == A_STATE_SHAPE_END) { state = A_STATE_NAME_START; } else { REJECT; } }

[A-Za-z0-9]+[ ]*      { 
						if (parsingAutomatha && state == A_STATE_NAME_START) 
						{ 
							automatha_state_name = cstring_trim(yytext); 
							state = A_STATE_LABEL_START; 
						} else { REJECT; } 
					  }
\[label=\"            { if (parsingAutomatha && state == A_STATE_LABEL_START) { state = A_STATE_LABEL_END; } 
                   else if (parsingAutomatha && state == A_TRANS_MATCH) { state = A_TRANS_LABEL_START; } else { REJECT; } }
[A-Za-z0-9/\\]+        { 
						if (parsingAutomatha && state == A_STATE_LABEL_END) 
						{ 
							automatha_state_label = cstring_copy(yytext);
							automatha_add_node(_a, automatha_state_isfinal, automatha_state_name, automatha_state_label);
							state = A_STATE_END; 							
						} 
						else if (parsingAutomatha && state == A_TRANS_LABEL_START) { 
							automatha_trans_label = cstring_copy(yytext);
							
							list tokens = cstring_split_list(automatha_trans_relation, "->");
	
							automatha_add_transition(_a, list_get(tokens, 0), list_get(tokens, 1), automatha_trans_label);

							list_free(tokens);
							
							state = A_TRANS_LABEL_END; 
						} else { REJECT; } 
					}
\"][ ]*;              { if (parsingAutomatha && state == A_STATE_END) { state = 0; }
                   else if (parsingAutomatha && state == A_TRANS_LABEL_END) { state = 0; } else { REJECT; } }

\}                    {  }

[A-Za-z0-9]+[ ]*->[A-Za-z0-9]+[ ]*[ ] { if (parsingAutomatha && state == 0) { 
							automatha_trans_relation = cstring_copy(yytext);
							
							state = A_TRANS_MATCH; } else { REJECT; } }

%%



int main(int len, char ** args) {
  run(len, args);
}