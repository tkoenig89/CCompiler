#include "symtable.h"
#include <stdio.h>
#include <stdlib.h>

struct symInt 		*symIntTable;
struct symFunc 	*symFuncTable;

void init_table ()
{
	printf("Hello World from symbol table.\n");
}

struct symInt *putInt (char const *name, int isArray, int val, void *scope)
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
	ptr->scope = scope; 
	
	ptr->next = (struct symInt *)symIntTable;
	symIntTable = ptr;
	printf("Integer ->%s<- was put in the table, value:%d\n", ptr->name, ptr->value.var);
	return ptr;
}

struct symInt *getInt (char const * name, void *scope)
{
	struct symInt *ptr;
	for (ptr = symIntTable; ptr != (struct symInt *) 0;ptr = (struct symInt *)ptr->next) {
		if (strcmp (ptr->name,name) == 0) {
			return ptr;
		}
	}	
	return 0;
}

int existsInt (char const * name, void *scope)
{
	struct symInt *ptr;
	for (ptr = symIntTable; ptr != (struct symInt *) 0;ptr = (struct symInt *)ptr->next) {
		if ((strcmp (ptr->name,name) == 0) && (ptr->scope == scope)) {
			return 1;
		}
	}	
	return 0;
}

void deleteInt (char const * name, void *scope)
{
	struct symInt *ptr;
	struct symInt *ptr_back;
	for (ptr = symIntTable; ptr != (struct symInt *) 0;ptr = (struct symInt *)ptr->next) {
		if ((strcmp (ptr->name,name) == 0) && (ptr->scope == scope)) {
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
	ptr->params.params[ptr->params.paramCount-1] = (*sInt);
}