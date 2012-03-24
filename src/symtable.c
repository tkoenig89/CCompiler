#include "symtable.h"
#include <stdio.h>
#include <stdlib.h>

struct symInt 		*symIntTable;
struct symFunc 	*symFuncTable;
struct symFunc 	*currFunc;

void init_table ()
{
	currFunc=NULL;
	printf("Hello World from symbol table.\n");
}

struct symInt *putInt (char const *name, int isArray, int val)
{
	struct symInt *ptr;
	//set name
	ptr = (struct symInt *) malloc (sizeof (struct symInt));
	ptr->name = (char *) malloc (strlen (name) + 1);
	strcpy (ptr->name,name);
	//set value
	ptr->value.var = val; 
	//set isArray
	ptr->isArray = isArray; 
	//set scope; NULL == global
	ptr->scope = currFunc; 
	
	ptr->next = (struct symInt *)symIntTable;
	symIntTable = ptr;
	printf("Integer ->%s<- was put in the table, value:%d\n", ptr->name, ptr->value.var);
	return ptr;
}

struct symInt *tempInt (char const *name)
{
	struct symInt *ptr;
	//set name
	ptr = (struct symInt *) malloc (sizeof (struct symInt));
	ptr->name = (char *) malloc (strlen (name) + 1);
	strcpy (ptr->name,name);
	
	ptr->isArray = 0; 
	ptr->scope = NULL; 
	
	return ptr;
}

struct symInt *getIntG (char const * name)
{
	struct symInt *ptr;
	for (ptr = symIntTable; ptr != (struct symInt *) 0;ptr = (struct symInt *)ptr->next) {
		if ((strcmp (ptr->name,name) == 0) && (ptr->scope == NULL)) {
			return ptr;
		}
	}
	return 0;
}

struct symInt *getIntL (char const * name)
{
	struct symInt *ptr;
	for (ptr = symIntTable; ptr != (struct symInt *) 0;ptr = (struct symInt *)ptr->next) {
		if ((strcmp (ptr->name,name) == 0) && (ptr->scope == currFunc) ) {
			return ptr;
		}
	}
	return 0;
}

struct symInt *getInt (char const * name)
{
	if(!existsIntL(name)) {
		if(existsIntG(name)) {
			return getIntG(name);			
		} else {
			return NULL;
		}
	} else {
		return getIntL(name);
	}
}

int existsIntG (char const * name)
{
	struct symInt *ptr;
	for (ptr = symIntTable; ptr != (struct symInt *) 0;ptr = (struct symInt *)ptr->next) {
		if ((strcmp (ptr->name,name) == 0) && (ptr->scope == NULL)) {
			return 1;
		}
	}
	return 0;
}

int existsIntL (char const * name)
{
	struct symInt *ptr;
	for (ptr = symIntTable; ptr != (struct symInt *) 0;ptr = (struct symInt *)ptr->next) {
		if ((strcmp (ptr->name,name) == 0) && (ptr->scope == currFunc) ) {
			return 1;
		}
	}
	return 0;
}

//Prioritasie Local over Global
int existsInt (char const * name)
{
	if(!existsIntL(name)) {
		return existsIntG(name);
	} else {
		return existsIntL(name);
	}
}

void deleteInt (char const * name)
{
	struct symInt *ptr;
	struct symInt *ptr_back;
	for (ptr = symIntTable; ptr != (struct symInt *) 0;ptr = (struct symInt *)ptr->next) {
		if ((strcmp (ptr->name,name) == 0) && ((ptr->scope == currFunc) || (ptr->scope == NULL))) {
			if(ptr == symIntTable) {
				symIntTable = ptr->next;
			} else {
				ptr_back->next = ptr->next;
			}
			printf("%s was deleted as a var\n", ptr->name);
			free(ptr);
		}
		ptr_back = ptr;
	};
}

struct symFunc *putFunc (char const *name, int isVoid)
{
	struct symFunc *ptr;
	//set name
	ptr = (struct symFunc *) malloc (sizeof (struct symFunc));
	ptr->name = (char *) malloc (strlen (name) + 1);
	strcpy (ptr->name,name);
	//set isVoid
	ptr->isVoid = isVoid; 
	//set paramcount=0
	ptr->params.paramCount = 0; 
	
	ptr->next = (struct symFunc *)symFuncTable;
	symFuncTable = ptr;
	printf("Function ->%s<- was put in the table\n", ptr->name);
	
	currFunc = ptr;
	
	return ptr;
}

struct symFunc *getFunc (char const *name)
{
	struct symFunc *ptr;
	for (ptr = symFuncTable; ptr != (struct symFunc *) 0;ptr = (struct symFunc *)ptr->next) {
		if (strcmp (ptr->name,name) == 0) {
			return ptr;
		}
	}	
	return 0;
}

int existsFunc (char const *name) 
{
	struct symFunc *ptr;
	for (ptr = symFuncTable; ptr != (struct symFunc *) 0;ptr = (struct symFunc *)ptr->next) {
		if (strcmp (ptr->name,name) == 0) {
			return 1;
		}
	}	
	return 0;
}

void addParam (char const *funcname, struct symInt *sInt)
{
	struct symFunc *ptr = getFunc (funcname);
	ptr->params.paramCount += 1;
	
	struct symInt *parambuffer = (struct symInt*) realloc (ptr->params.params, ptr->params.paramCount * sizeof(struct symInt));
	
	ptr->params.params = parambuffer;
	ptr->params.params[ptr->params.paramCount-1] = sInt;
}

void funcEnd()
{
	currFunc = NULL;
}