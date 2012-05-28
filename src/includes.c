/**
 *  SISTEMAS OPERATIVOS - ITBA - 2011  
 *	ALUMNOS:                         
 *		MARSEILLAN 
 *		PEREYRA
 *		VIDELA
 * -----------------------------------
 *
 * @file includes.c
 *
 * @brief Provides functions shared across all the programs.
 *
 * @author Agustin Marseillan
 * @author Maximo Videla
 * @author Cristian Pereyra
 *
 */

#include "includes.h"

#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


void * pointer_cloner(void * ptr) {
	return ptr;
}

int pointer_comparer(void * int1, void * int2) {
	return (int*)int1 - (int*)int2;
}

int int_comparer(void * int1, void * int2) {
	
	if ((int)int2 == 0)
		return 1;
	
	if ((int)int1 == 0)
		return -1;
	return *(int*)int1 - *(int*)int2;
}

int double_comparer(void * double1, void * double2) {
	if (*((double*)double1) < *((double*)double2)) {
		return -1;
	}
	else if (*((double*)double1) > *((double*)double2)) {
		return 1;
	}
	else {
		return 0;
	}
}

int cstring_ds_comparer(void * s1, void * s2) {
	return strcmp((char*)s1, *(char**)s2);
}

int cstring_comparer(void * s1, void * s2) {
	return strcmp((char*)s1, (char*)s2);
}


void int_printer(void * int1) {
	printf("%d",*((int*)int1));
}
void double_printer(void * double1){
	printf("%g",*((double*)double1));
}

void cstring_printer(void * s1) {
	printf("%s",(char*)s1);
}

void * int_cloner(void * int1) {
	int * nuevo = (int *) malloc(sizeof(int));
	*nuevo = *(int*)int1;
	return nuevo;
}
void * double_cloner(void * double1){
	double * nuevo = (double *) malloc(sizeof(double));
	*nuevo = *(int*)double1;
	return nuevo;
}

void * cstring_cloner(void * s1) {
	char * _s1 = * (char **) s1;
	int len = strlen(_s1);
	char ** nuevo = (char **)malloc(sizeof(char*) * (len+1));
	*nuevo = strcpy(*nuevo, _s1);
	return nuevo;
}

