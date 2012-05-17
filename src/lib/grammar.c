#include "grammar.h"

struct grammar {
  list vt;    // Terminals
  list vn;    // Non terminals
  cstring s;  // Start token
  list p;     // Productions
};

grammar grammar_init() {
  // Start a grammar and return the adt
}

void grammar_add_terminal(grammar g, cstring token) {

}

void grammar_add_non_terminal(grammar g, cstring token) {

}

void grammar_set_start_token(grammar g, cstring token) {

}

void grammar_add_production(grammar g, production production) {
  
}