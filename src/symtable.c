#include "symtable.h"
#include <stdio.h>

symrec *sym_table;

void init_table ()
{
	printf("Hello World from symbol table.\n");
}

symrec *putsym (char const *sym_name, int sym_val, int sym_type)
{
	symrec *ptr;
	ptr = (symrec *) malloc (sizeof (symrec));
	ptr->name = (char *) malloc (strlen (sym_name) + 1);
	strcpy (ptr->name,sym_name);
	ptr->type = sym_type;
	ptr->value.var = sym_val; /* Set value to 0 even if fctn. */
	ptr->next = (struct symrec *)sym_table;
	sym_table = ptr;
	printf("Symbol ->%s<- was put in the table\n", sym_name);
	return ptr;
}

symrec *getsym (char const *sym_name)
{
	symrec *ptr;
	for (ptr = sym_table; ptr != (symrec *) 0;ptr = (symrec *)ptr->next) {
		if (strcmp (ptr->name,sym_name) == 0) {
			return ptr;		
		}
	}	
	return 0;
}

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
