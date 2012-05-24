#ifndef _AUTOMATHA_H_
#define _AUTOMATHA_H_

#include "../utils/cstring.h"
#include "../utils/list.h"
#include <stdio.h>

typedef struct automatha * automatha;

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



#endif
