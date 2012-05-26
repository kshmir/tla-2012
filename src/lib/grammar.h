#ifndef _GRAMMAR_H_
#define _GRAMMAR_H_

typedef struct production * production;
typedef struct grammar * grammar;

#include "automatha.h"
#include "../utils/map.h"
#include "../utils/cstring.h"
#include "../utils/list.h"






list grammar_get_terminals(grammar g);

list grammar_get_non_terminals(grammar g);

cstring grammar_get_start(grammar g);

map grammar_get_productions(grammar g);

/**
 * Allocates the grammar
 */
grammar grammar_init();

/**
 * Adds a terminal element to the grammar
 */
void grammar_add_terminal(grammar g, cstring token);

/**
 * Adds a non terminal element to the grammar
 */
void grammar_add_non_terminal(grammar g, cstring token);

/**
 * Sets the start token of the grammar
 */
void grammar_set_start_token(grammar g, cstring token);

/**
 * Adds a production to the grammar
 */
void grammar_add_production(grammar g, production production);


void grammar_print_info(grammar g);

/**
 * Prints a grammar
 */
void grammar_print(grammar g, FILE * file);

production production_init();

void production_set_start(production p, cstring start);

/**
 * Tokens should be a list of elements which belong to the grammar.
 * Like a->b->A (represents the production A->abA when A is the token A of the production)
 */
void production_add_token(production p, cstring token);

production production_from_string(cstring string);

list production_get_tokens(production p);

cstring production_get_start(production p);

automatha grammar_to_automatha(grammar g);

#endif
