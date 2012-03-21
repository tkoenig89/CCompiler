#include "symtable.h"
#include <stdio.h>

//symrec *sym_table;
struct symInt *symIntTable;

void init_table ()
{
	printf("Hello World from symbol table.\n");
}
/*
symrec *putsym (char const *sym_name, int sym_val, int sym_type)
{
	symrec *ptr;
	ptr = (symrec *) malloc (sizeof (symrec));
	ptr->name = (char *) malloc (strlen (sym_name) + 1);
	strcpy (ptr->name,sym_name);
	ptr->type = sym_type;
	ptr->value.var = sym_val; // Set value to 0 even if fctn. 
	ptr->next = (struct symrec *)sym_table;
	sym_table = ptr;
	printf("Symbol ->%s<- was put in the table\n", sym_name);
	return ptr;
}

symrec *getsym (char const *sym_name, void *funcscope)
{
	symrec *ptr;
	for (ptr = sym_table; ptr != (symrec *) 0;ptr = (symrec *)ptr->next) {
		if (strcmp (ptr->name,sym_name) == 0) {
			return ptr;
		}
	}	
	return 0;
}

symrec *edit (char 

int exists_sym (char const *sym_name)
{
	symrec *ptr;
	for (ptr = sym_table; ptr != (symrec *) 0;ptr = (symrec *)ptr->next) {
		if (strcmp (ptr->name,sym_name) == 0) {
			return 1;		
		}
	}	
	return 0;
}
*/

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
		if (strcmp (ptr->name,name) == 0) {
			return 1;
		}
	}	
	return 0;
}