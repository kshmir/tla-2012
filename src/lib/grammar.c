
#include <stdlib.h>
#include <stdio.h>
#include "../utils/cstring.h"
#include "grammar.h"

struct grammar {
  list vt;    // Terminals
  list vn;    // Non terminals
  cstring s;  // Start token
  list p;     // Productions
};

grammar grammar_init() {
    grammar g = malloc(sizeof(struct grammar));
    
    g->vt = list_init();
    g->vn = list_init();
    g->s = cstring_init(0);
    g->p = list_init();
    
    
    return g;
}

void grammar_add_terminal(grammar g, cstring token) {
    list_add(g->vt, cstring_copy(token));
}

void grammar_add_non_terminal(grammar g, cstring token) {
    list_add(g->vn, cstring_copy(token));
}

void grammar_set_start_token(grammar g, cstring token) {
	g->s = cstring_copy(token);
}

void grammar_add_production(grammar g, production production) {
	list_add(g->p, production);
}
