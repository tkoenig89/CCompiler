#include "symtable.h"
#include <stdio.h>

//symrec *sym_table;
struct symInt *symIntTable;

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
	printf("Symbol ->%s<- was put in the table, value:%d\n", ptr->name, ptr->value.var);
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
			free(ptr);
		}
		ptr_back = ptr;
	};
}