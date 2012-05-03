/*
 *  stack.c
 *  so-2011
 *
 *  Created by Cristian Pereyra on 05/08/11.
 *  Copyright 2011 My Own. All rights reserved.
 *
 */

#include "stack.h"


// Data structure
struct stack {
	void *  data;
	int		size;
	int		index;
};

// Inits the stack
stack stack_init(size_t start_size) {
	if (!start_size)
		return NULL;
	stack pila = (stack) malloc(sizeof(struct stack));
	pila->size = start_size;
	pila->index = 0;
	pila->data = (void *) malloc(start_size * sizeof(void *));
	
	return pila;
}


// Expands the stack if needed
// It makes it the double of size.
static void stack_expand(stack pila) {
	int i = 0;
	
	char * space = (char *) malloc(pila->size * 2 * sizeof(void *));
	
	for (i = 0; i < pila->size * sizeof(void *); i++) {
		space[i] = ((char*)pila->data)[i];
	}
	
	free(pila->data);
	pila->data = (void *)space;
	pila->size *= 2;
}

// Tells the amount of data stored in the stack
int stack_count(stack pila) {
	return pila->index;
}

// Tells wether the stack is empty or not
int stack_is_empty(stack pila) {
	return !stack_count(pila);
}

// Pushes a pointer to the stack
int stack_push(stack pila, void * object) {
	if (pila->index == pila->size) {
		stack_expand(pila);
	}
	((int*)pila->data)[pila->index] = (int) object;
	pila->index++;
	return 1;
}

// Pops the pointer from the stack
void * stack_pop(stack pila) {
	
	if (pila->index == 0)
		return NULL;
	pila->index--;
	return (void *)((int*)pila->data)[pila->index];
}

// Frees up the stack
stack stack_free(stack pila){
	free(pila->data);
	free(pila);
}	

