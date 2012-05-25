#ifndef _AUTOMATHA_H_
#define _AUTOMATHA_H_

typedef struct automatha * automatha;

#include "../utils/cstring.h"
#include "../utils/list.h"
#include "grammar.h"
#include <stdio.h>



/**
 * Allocates the automatha
 */
automatha automatha_init();

/**
 * Adds a terminal element to the automatha
 */
void automatha_add_node(automatha a, int final, cstring name, cstring label);

/**
 * Adds a transition to the autmatha
 */
void automatha_add_transition(automatha a, cstring from, cstring to, cstring token);


void automatha_print(automatha a, FILE * file);


grammar automatha_to_grammar(automatha a);

void fix_productions(grammar g);

#endif
