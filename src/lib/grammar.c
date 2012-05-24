
#include <stdlib.h>
#include <stdio.h>
#include "../utils/list.h"
#include "../utils/map.h"
#include "../utils/cstring.h"
#include "grammar.h"

struct grammar {
  list vt;    // Terminals
  list vn;    // Non terminals
  cstring s;  // Start token
  map  p;     // Productions, ordered by their key
};

struct production {
	cstring start;
	list	tokens; // List of lists containing all the tokens of a production
};

list grammar_get_terminals(grammar g) {
	return g->vt;
}

list grammar_get_non_terminals(grammar g) {
	return g->vn;
}

cstring grammar_get_start(grammar g) {
	return g->s;
}

map grammar_get_productions(grammar g) {
	return g->p;
}


grammar grammar_init() {
    grammar g = malloc(sizeof(struct grammar));

    g->vt = list_init();
    g->vn = list_init();
    g->s = NULL;
    g->p = map_init(cstring_comparer, NULL);


    return g;
}

void grammar_add_terminal(grammar g, cstring token) {
	printf(".%s.\n", token);
    list_add(g->vt, cstring_copy(token));
}

void grammar_add_non_terminal(grammar g, cstring token) {
	printf(".%s.\n", token);
    list_add(g->vn, cstring_copy(token));
}

void grammar_set_start_token(grammar g, cstring token) {
	printf(".%s.\n", token);
	g->s = cstring_copy(token);
}

void grammar_add_production(grammar g, production production) {
	map_set(g->p, production->start, production);
}

void grammar_print(grammar g, FILE * file) {
	fprintf(file, "G1 = (");
}


production production_init() {
	production p = malloc(sizeof(struct production));
	p->start = NULL;
	p->tokens = list_init();

	return p;
}

void production_set_start(production p, cstring start) {
	p->start = cstring_copy(start);
}

void production_add_token(production p, list token) {
	list_add(p->tokens, token);
}

production production_from_string(cstring string) {
	production p = production_init();
	cstring to_parse = cstring_trim(string);


	// Parto al medio el A->X|Y|Z
	list parse = cstring_split_list(to_parse, "->");

	// "A"
	cstring start = cstring_trim(list_get(parse, 0));

	production_set_start(p, start);

	// "X|Y|Z"
	cstring productions_raw = list_get(parse, 1);

	// Lista de {X, Y, Z}
	list productions = cstring_split_list(productions_raw, "|");

	// Crea una lista con todos los tokens de X, Y ... Z
	// Y los agrega a la producción.
	foreach(cstring, prod, productions) {
		production_add_token(p, cstring_trim(prod));
	}

	return p;
}

list production_get_tokens(production p) {
	return p->tokens;
}

cstring production_get_start(production p) {
	return p->start;
}
