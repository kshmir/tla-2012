#ifndef _GRAMMAR_H_
#define _GRAMMAR_H_

#include "../utils/cstring.h"
#include "../utils/list.h"
#include "production.h"

typedef struct grammar * grammar;

// Allocates the grammar
grammar grammar_init();

// Adds a terminal element to the grammar
void grammar_add_terminal(grammar g, cstring token);

// Adds a non terminal element to the grammar
void grammar_add_non_terminal(grammar g, cstring token);


// Sets the start token of the grammar
void grammar_set_start_token(grammar g, cstring token);

// Adds a production to the grammar
void grammar_add_production(grammar g, production production);

#endif